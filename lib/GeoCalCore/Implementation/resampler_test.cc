#include "unit_test_support.h"
#include "resampler.h"
#include "geocal_thread_pool.h"
#include "memory_raster_image.h"
#include "memory_raster_image_float.h"
#include "magnify_bilinear.h"
#include "ogr_coordinate.h"
#include "gdal_raster_image.h"
#include "landsat7_global.h"
#include <boost/make_shared.hpp>
#include <chrono>
#include <sstream>
#include <cmath>
using namespace GeoCal;
using namespace blitz;

class ResamplerFixture : public GlobalFixture {
public:
  ResamplerFixture()
    : nsub(2), mi(boost::make_shared<GeodeticConverter>(), 10.0, 11.0,
		  11.0, 10.0, 200, 200)
  {
    // A rotated swath, with pixels a little larger than the MapInfo
    // pixels so most output pixels see several subpixels. The
    // swath extends past the MapInfo on one side, so with
    // Exactly_match_mi some of the data falls outside.
    int nraw_line = 120, nraw_samp = 150;
    double th = 20 * M_PI / 180.0;
    x.resize(nraw_line * nsub, nraw_samp * nsub);
    y.resize(x.shape());
    for(int i = 0; i < x.rows(); ++i)
      for(int j = 0; j < x.cols(); ++j) {
	double ii = (i + 0.5) / nsub - 0.5;
	double jj = (j + 0.5) / nsub - 0.5;
	x(i,j) = 10.05 + (std::cos(th) * jj - std::sin(th) * ii) * 0.0062;
	y(i,j) = 10.95 - (std::sin(th) * jj + std::cos(th) * ii) * 0.0062;
      }
    data.resize(nraw_line, nraw_samp);
    for(int i = 0; i < data.rows(); ++i)
      for(int j = 0; j < data.cols(); ++j)
	data(i,j) = std::sin(i * 0.3) * 50 + std::cos(j * 0.2) * 30 +
	  i * 0.1 - j * 0.05;
    // Some fill data. Includes a large enough block that some output
    // pixels only see fill.
    data_fill.reference(data.copy());
    data_fill(Range(20,40), Range(30,50)) = -9999;
    for(int i = 0; i < data.rows(); i += 7)
      data_fill(i, (i * 3) % data.cols()) = -9999;
  }
  // Reference implementation, the old serial algorithm using
  // MagnifyReplicate.
  blitz::Array<double, 2> reference_resample
  (const Resampler& r, const blitz::Array<double, 2>& Data,
   double Scale_data=1.0, bool Negative_to_zero=false,
   double Fill_value = 0.0, bool Use_smallest_ic = false,
   double Fill_value_threshold = std::numeric_limits<double>::lowest())
  {
    // Replicate the data, the same as MagnifyReplicate does.
    blitz::Array<double, 2> d(Data.rows() * r.number_sub_pixel(),
			      Data.cols() * r.number_sub_pixel());
    for(int i = 0; i < d.rows(); ++i)
      for(int j = 0; j < d.cols(); ++j)
	d(i,j) = Data(i / r.number_sub_pixel(), j / r.number_sub_pixel());
    const blitz::Array<int, 3>& data_index = r.data_index();
    const MapInfo& m = r.map_info();
    blitz::Array<double, 2> res(m.number_y_pixel(), m.number_x_pixel());
    blitz::Array<int, 2> cnt(res.shape());
    res = 0.0;
    cnt = 0;
    for(int i = 0; i < data_index.rows(); ++i)
      for(int j = 0; j < data_index.cols(); ++j) {
	int ln = data_index(i,j,0);
	int smp = data_index(i,j,1);
	if(ln >= 0 && ln < cnt.rows() &&
	   smp >=0 && smp < cnt.cols()) {
	  if(d(i,j) > Fill_value_threshold) {
	    if(cnt(ln,smp) == 0)
	      res(ln,smp) = 0.0;
	    if(!Use_smallest_ic || cnt(ln,smp) == 0) {
	      res(ln,smp) += d(i,j);
	      cnt(ln,smp) += 1;
	    }
	  } else {
	    if(cnt(ln, smp) == 0) {
	      if(res(ln, smp) > Fill_value_threshold)
		res(ln, smp) = d(i,j);
	      else
		res(ln, smp) = std::max(res(ln, smp), d(i,j));
	    }
	  }
	}
      }
    res = blitz::where(cnt == 0, Fill_value, res / cnt * Scale_data);
    if(Negative_to_zero)
      res = blitz::where(res < 0, 0, res);
    return res;
  }
  // Reference implementation of init. This is the old serial code, with
  // the fixes for GeoCal 2.0: a point is missing if either X or Y is
  // NaN or <= Fill_value_threshold (instead of the old Mark_missing,
  // with both X and Y fill), and missing points are marked -9999 with
  // Exactly_match_mi as well. Returns data_index, and fills in Mi_out.
  blitz::Array<int, 3> reference_init
  (const blitz::Array<double, 2>& X_coor, const blitz::Array<double, 2>& Y_coor,
   const MapInfo& Mi, bool Exactly_match_mi, double Fill_value_threshold,
   MapInfo& Mi_out)
  {
    Range ra = Range::all();
    blitz::Array<int, 3> data_index(X_coor.rows(), X_coor.cols(), 2);
    blitz::Array<double, 2> xindex, yindex;
    Mi.coordinate_to_index(X_coor, Y_coor, xindex, yindex);
    data_index(ra,ra,0) = blitz::cast<int>(blitz::rint(yindex));
    data_index(ra,ra,1) = blitz::cast<int>(blitz::rint(xindex));
    auto missing = [&](int i, int j)
      { return !(Y_coor(i, j) > Fill_value_threshold &&
		 X_coor(i, j) > Fill_value_threshold); };
    if(Exactly_match_mi) {
      Mi_out = Mi;
      for(int i = 0; i < X_coor.rows(); ++i)
	for(int j = 0; j < X_coor.cols(); ++j)
	  if(missing(i, j))
	    data_index(i,j,ra) = -9999;
      return data_index;
    }
    bool first = true;
    int minx = 0, miny = 0, maxx = 0, maxy = 0;
    for(int i = 0; i < X_coor.rows(); ++i)
      for(int j = 0; j < X_coor.cols(); ++j)
	if(!missing(i, j)) {
	  if(first) {
	    minx = data_index(i,j,1);
	    miny = data_index(i,j,0);
	    maxx = minx;
	    maxy = miny;
	    first = false;
	  }
	  minx = std::min(data_index(i,j,1),minx);
	  maxx = std::max(data_index(i,j,1),maxx);
	  miny = std::min(data_index(i,j,0),miny);
	  maxy = std::max(data_index(i,j,0),maxy);
	}
    Mi_out = Mi.subset(minx, miny, maxx - minx + 1, maxy - miny + 1);
    data_index(ra,ra,0) -= miny;
    data_index(ra,ra,1) -= minx;
    for(int i = 0; i < X_coor.rows(); ++i)
      for(int j = 0; j < X_coor.cols(); ++j)
	if(missing(i, j))
	  data_index(i,j,ra) = -9999;
    return data_index;
  }
  // Reference implementation of the old determine_range
  void reference_determine_range
  (const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
   const MapInfo& Mi, int Num_sub_pixel,
   int& lstart, int& lend, int& sstart, int& send)
  {
    lstart = X.rows();
    lend = -1;
    sstart = X.cols();
    send = -1;
    blitz::Array<double, 2> xindex, yindex;
    Mi.coordinate_to_index(X, Y, xindex, yindex);
    for(int i = 0; i < xindex.rows(); ++i)
      for(int j = 0; j < xindex.cols(); ++j) {
	int ln = (int) std::round(yindex(i,j));
	int smp = (int) std::round(xindex(i,j));
	if(ln >= 0 && ln < Mi.number_y_pixel() &&
	   smp >=0 && smp < Mi.number_x_pixel()) {
	  lstart = std::min(lstart, i);
	  lend = std::max(lend, i);
	  sstart = std::min(sstart, j);
	  send = std::max(send, j);
	}
      }
    lstart = std::floor(double(lstart) / Num_sub_pixel) * Num_sub_pixel;
    lend = std::ceil(double(lend) / Num_sub_pixel) * Num_sub_pixel;
    lend = std::min(lend, xindex.rows());
    sstart = std::floor(double(sstart) / Num_sub_pixel) * Num_sub_pixel;
    send = std::ceil(double(send) / Num_sub_pixel) * Num_sub_pixel;
    send = std::min(send, xindex.cols());
  }
  // Check that two MapInfo are the same
  void check_map_info(const MapInfo& M1, const MapInfo& M2)
  {
    BOOST_CHECK_EQUAL(M1.number_x_pixel(), M2.number_x_pixel());
    BOOST_CHECK_EQUAL(M1.number_y_pixel(), M2.number_y_pixel());
    BOOST_CHECK_MATRIX_CLOSE_TOL(M1.transform(), M2.transform(), 1e-12);
  }
  // Coordinates with some fill values. Includes points where only one
  // of x and y is fill, which the old code treated differently in the
  // range calculation and the marking, and NaN.
  void coor_with_fill(blitz::Array<double, 2>& Xf, blitz::Array<double, 2>& Yf)
  {
    Xf.reference(x.copy());
    Yf.reference(y.copy());
    Xf(Range(10, 30), Range(0, 40)) = -9999;
    Yf(Range(10, 30), Range(0, 40)) = -9999;
    Xf(Range(100, 110), Range(200, 220)) = -9999;
    Yf(Range(150, 160), Range(100, 120)) = -9999;
    Xf(Range(200, 205), Range(10, 20)) = std::numeric_limits<double>::quiet_NaN();
    Yf(Range(200, 205), Range(10, 20)) = std::numeric_limits<double>::quiet_NaN();
    Xf(Range(210, 215), Range(10, 20)) = std::numeric_limits<double>::quiet_NaN();
  }
  // A projected CoordinateConverter. We only need the MapInfo
  // parameters for most tests, but the coordinate checks look at the
  // kind of converter.
  boost::shared_ptr<CoordinateConverter> projected_converter()
  {
    return OgrCoordinateConverter::utm_converter(11);
  }
  // Number of output pixels that have data
  int number_with_data(const blitz::Array<double, 2>& Res, double Fill_value)
  {
    return blitz::count(Res != Fill_value);
  }
  // Restore the ThreadPool after the test.
  ~ResamplerFixture()
  {
    ThreadPool::thread_pool().number_thread(ThreadPool::default_number_thread);
  }
  int nsub;
  MapInfo mi;
  blitz::Array<double, 2> x, y, data, data_fill;
};

BOOST_FIXTURE_TEST_SUITE(resampler, ResamplerFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  // Skip this test, it depends on ECOSTRESS  data. We have tested this over there.
  return;

  // Skip if we don't have landsat data available.
  // if(landsat7_dir() == "")
  //   return;
  // // This is a fairly slow test, so skip normally. We can turn this
  // // on if there are any issues
  // return;
  // std::string fname = test_data_dir() + "ECOSTRESS_L1B_GEO_80005_001_20150124T204250_0100_01.h5.expected";
  // std::string fname2 = test_data_dir() + "ECOSTRESS_L1B_RAD_80005_001_20150124T204250_0100_01.h5.expected";
  // boost::shared_ptr<GdalRasterImage> lat = boost::make_shared<GdalRasterImage>("HDF5:\"" + fname + "\"://Geolocation/latitude");
  // boost::shared_ptr<GdalRasterImage> lon = boost::make_shared<GdalRasterImage>("HDF5:\"" + fname + "\"://Geolocation/longitude");
  // boost::shared_ptr<GdalRasterImage> swir_dn = boost::make_shared<GdalRasterImage>("HDF5:\"" + fname2 + "\"://SWIR/swir_dn");
  // Landsat7Global orth(landsat7_dir(), Landsat7Global::BAND5);
  // MapInfo mi = orth.map_info().scale(2, 2);
  // Resampler r(lat, lon, mi);
  // r.resample_field("swir_res.img", swir_dn);
}

BOOST_AUTO_TEST_CASE(array_matches_reference)
{
  for(bool exact : {false, true}) {
    Resampler r(x, y, mi, nsub, exact);
    blitz::Array<double, 2> expect = reference_resample(r, data);
    blitz::Array<double, 2> res = r.resample_field(data);
    BOOST_CHECK(res.rows() == expect.rows() && res.cols() == expect.cols());
    BOOST_CHECK_MATRIX_CLOSE_TOL(res, expect, 1e-10);
    // Make sure the test actually has data, and some pixels without
    BOOST_CHECK(number_with_data(res, 0.0) > 1000);
    if(exact)
      BOOST_CHECK(number_with_data(res, 0.0) < (int) res.size());
  }
}

BOOST_AUTO_TEST_CASE(raster_matches_array)
{
  // RasterImage version should give the same results. MemoryRasterImage
  // is integer, so use integer data.
  Resampler r(x, y, mi, nsub);
  blitz::Array<double, 2> dint(blitz::floor(data));
  auto img = boost::make_shared<MemoryRasterImage>(data.rows(), data.cols());
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j)
      img->write(i, j, (int) dint(i,j));
  blitz::Array<double, 2> res_img = r.resample_field(img);
  blitz::Array<double, 2> res_arr = r.resample_field(dint);
  BOOST_CHECK_MATRIX_CLOSE_TOL(res_img, res_arr, 1e-12);
  BOOST_CHECK_MATRIX_CLOSE_TOL(res_img, reference_resample(r, dint), 1e-10);
  // empty_resample should agree between the two as well.
  BOOST_CHECK(!r.empty_resample(img));
  BOOST_CHECK(!r.empty_resample(dint));
}

BOOST_AUTO_TEST_CASE(thread_consistency)
{
  // The default number of tasks does this data in a single thread,
  // since it is small. Explicitly give the number of tasks to test the
  // threaded version.
  Resampler r(x, y, mi, nsub);
  blitz::Array<double, 2> expect = r.resample_field(data, 1.0, false, 0.0,
						    false,
						    std::numeric_limits<double>::lowest(), 1);
  for(int nthread : {1, 4, 10}) {
    ThreadPool::thread_pool().number_thread(nthread);
    for(int ntask : {-1, 1, 7, 100}) {
      blitz::Array<double, 2> res =
	r.resample_field(data_fill, 2.0, false, -1.0, false, -9000, ntask);
      blitz::Array<double, 2> expect_fill =
	reference_resample(r, data_fill, 2.0, false, -1.0, false, -9000);
      BOOST_CHECK_MATRIX_CLOSE_TOL(res, expect_fill, 1e-10);
      res.reference(r.resample_field(data, 1.0, false, 0.0, false,
				     std::numeric_limits<double>::lowest(),
				     ntask));
      BOOST_CHECK_MATRIX_CLOSE_TOL(res, expect, 1e-10);
    }
  }
}

BOOST_AUTO_TEST_CASE(use_smallest_ic)
{
  for(bool exact : {false, true}) {
    Resampler r(x, y, mi, nsub, exact);
    for(const blitz::Array<double, 2>& d : {data, data_fill}) {
      blitz::Array<double, 2> expect =
	reference_resample(r, d, 1.0, false, 0.0, true, -9000);
      for(int ntask : {1, 7, 100}) {
	blitz::Array<double, 2> res =
	  r.resample_field(d, 1.0, false, 0.0, true, -9000, ntask);
	// Use_smallest_ic picks a value, so this should be exact.
	BOOST_CHECK(blitz::all(res == expect));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(fill_values)
{
  Resampler r(x, y, mi, nsub);
  for(int ntask : {1, 7}) {
    blitz::Array<double, 2> res =
      r.resample_field(data_fill, 1.0, false, -5.0, false, -9000, ntask);
    blitz::Array<double, 2> res_nofill =
      r.resample_field(data, 1.0, false, -5.0, false, -9000, ntask);
    // The fill block means some pixels only see fill data, and get the
    // fill value even though there is data there without fill.
    int nfill_only = blitz::count(res == -5.0 && res_nofill != -5.0);
    BOOST_CHECK(nfill_only > 50);
    // No pixel gets a value from the fill data.
    BOOST_CHECK(blitz::all(res > -9000));
    BOOST_CHECK_MATRIX_CLOSE_TOL(res,
		 reference_resample(r, data_fill, 1.0, false, -5.0, false, -9000),
				 1e-10);
    // Without the threshold, the fill data is just treated as data
    blitz::Array<double, 2> res2 =
      r.resample_field(data_fill, 1.0, false, -5.0, false,
		       std::numeric_limits<double>::lowest(), ntask);
    BOOST_CHECK(blitz::any(res2 < -9000));
  }
}

BOOST_AUTO_TEST_CASE(scale_and_negative_to_zero)
{
  Resampler r(x, y, mi, nsub);
  blitz::Array<double, 2> base = r.resample_field(data, 1.0, false, 0.0);
  BOOST_CHECK(blitz::any(base < 0));
  for(int ntask : {1, 7}) {
    blitz::Array<double, 2> res =
      r.resample_field(data, 3.0, false, 0.0, false,
		       std::numeric_limits<double>::lowest(), ntask);
    BOOST_CHECK_MATRIX_CLOSE_TOL(res, base * 3.0, 1e-10);
    res.reference(r.resample_field(data, 3.0, true, 0.0, false,
				   std::numeric_limits<double>::lowest(), ntask));
    BOOST_CHECK(blitz::all(res >= 0));
    BOOST_CHECK_MATRIX_CLOSE_TOL(res, blitz::where(base < 0, 0.0, base * 3.0),
				 1e-10);
    BOOST_CHECK_MATRIX_CLOSE_TOL(res,
		 reference_resample(r, data, 3.0, true, 0.0), 1e-10);
  }
}

BOOST_AUTO_TEST_CASE(size_check)
{
  Resampler r(x, y, mi, nsub);
  blitz::Array<double, 2> small(data(Range(0, data.rows() - 2), Range::all()));
  BOOST_CHECK_THROW(r.resample_field(small), Exception);
  BOOST_CHECK_THROW(r.empty_resample(small), Exception);
  blitz::Array<double, 2> small2(data(Range::all(), Range(0, data.cols() - 2)));
  BOOST_CHECK_THROW(r.resample_field(small2), Exception);
}

BOOST_AUTO_TEST_CASE(exactly_match_mi)
{
  // With Exactly_match_mi, the data that falls outside of the MapInfo
  // is ignored. Compare with the coverage MapInfo, which should have
  // the same values where they overlap.
  Resampler r_exact(x, y, mi, nsub, true);
  Resampler r_cover(x, y, mi, nsub, false);
  BOOST_CHECK(r_cover.map_info().number_x_pixel() > 0);
  blitz::Array<double, 2> res_exact = r_exact.resample_field(data, 1.0, false,
							     -1.0, false,
     std::numeric_limits<double>::lowest(), 7);
  blitz::Array<double, 2> res_cover = r_cover.resample_field(data, 1.0, false,
							     -1.0, false,
     std::numeric_limits<double>::lowest(), 7);
  // Offset of the coverage MapInfo in the full MapInfo
  double xoff, yoff;
  mi.coordinate(*r_cover.map_info().ground_coordinate(0, 0), xoff, yoff);
  int soff = (int) std::round(xoff);
  int loff = (int) std::round(yoff);
  int ncompare = 0;
  for(int i = 0; i < res_cover.rows(); ++i)
    for(int j = 0; j < res_cover.cols(); ++j) {
      int ie = i + loff, je = j + soff;
      if(ie >= 0 && ie < res_exact.rows() && je >= 0 && je < res_exact.cols()) {
	BOOST_CHECK_CLOSE(res_exact(ie, je), res_cover(i, j), 1e-8);
	++ncompare;
      }
    }
  BOOST_CHECK(ncompare > 1000);
  // The coverage resampler sees all the data, so it has pixels the
  // exact one doesn't.
  BOOST_CHECK(number_with_data(res_cover, -1.0) >
	      number_with_data(res_exact, -1.0));
}

BOOST_AUTO_TEST_CASE(empty_resample)
{
  // MapInfo away from the data
  MapInfo mi_far(boost::make_shared<GeodeticConverter>(), 20.0, 21.0,
		 21.0, 20.0, 200, 200);
  Resampler r_far(x, y, mi_far, nsub, true);
  BOOST_CHECK(r_far.empty_resample());
  BOOST_CHECK(r_far.empty_resample(data));
  Resampler r(x, y, mi, nsub, true);
  BOOST_CHECK(!r.empty_resample());
  BOOST_CHECK(!r.empty_resample(data));
  blitz::Array<double, 2> all_fill(data.shape());
  all_fill = -9999;
  BOOST_CHECK(r.empty_resample(all_fill, -9000));
  BOOST_CHECK(!r.empty_resample(data_fill, -9000));
}

BOOST_AUTO_TEST_CASE(ctor_matches_reference)
{
  blitz::Array<double, 2> xf, yf;
  coor_with_fill(xf, yf);
  for(int nthread : {1, 4, 10}) {
    ThreadPool::thread_pool().number_thread(nthread);
    for(bool exact : {false, true})
      for(bool fill : {false, true})
	for(int ntask : {-1, 1, 7, 100}) {
	  const blitz::Array<double, 2>& xc = (fill ? xf : x);
	  const blitz::Array<double, 2>& yc = (fill ? yf : y);
	  MapInfo mi_expect;
	  blitz::Array<int, 3> expect =
	    reference_init(xc, yc, mi, exact, -1000.0, mi_expect);
	  Resampler r(xc, yc, mi, nsub, exact, -1000.0, ntask);
	  BOOST_CHECK(blitz::all(r.data_index() == expect));
	  check_map_info(r.map_info(), mi_expect);
	  // Same thing using NaN rather than a threshold.
	  blitz::Array<double, 2> xn(xc.copy()), yn(yc.copy());
	  xn = blitz::where(xn <= -1000.0, std::numeric_limits<double>::quiet_NaN(), xn);
	  yn = blitz::where(yn <= -1000.0, std::numeric_limits<double>::quiet_NaN(), yn);
	  Resampler rn(xn, yn, mi, nsub, exact,
		       std::numeric_limits<double>::lowest(), ntask);
	  BOOST_CHECK(blitz::all(rn.data_index() == expect));
	  check_map_info(rn.map_info(), mi_expect);
	}
  }
}

BOOST_AUTO_TEST_CASE(missing_points)
{
  // A point is missing if either X or Y is fill or NaN, for both
  // coverage and exact MapInfo.
  blitz::Array<double, 2> xf, yf;
  coor_with_fill(xf, yf);
  for(bool exact : {false, true})
    for(int ntask : {1, 7}) {
      Resampler r(xf, yf, mi, nsub, exact, -1000.0, ntask);
      const blitz::Array<int, 3>& di = r.data_index();
      int nmissing = 0;
      for(int i = 0; i < xf.rows(); ++i)
	for(int j = 0; j < xf.cols(); ++j) {
	  bool miss = !(xf(i,j) > -1000.0 && yf(i,j) > -1000.0);
	  if(miss) {
	    ++nmissing;
	    BOOST_CHECK_EQUAL(di(i,j,0), -9999);
	    BOOST_CHECK_EQUAL(di(i,j,1), -9999);
	  } else {
	    BOOST_CHECK(di(i,j,0) != -9999 || di(i,j,1) != -9999);
	  }
	}
      // Points where only one coordinate is fill, both fill, and NaN.
      BOOST_CHECK_EQUAL(nmissing, 21 * 41 + 11 * 21 + 11 * 21 + 6 * 11 + 6 * 11);
      // Missing points never contribute data. Put a huge value in the
      // raw pixels where all the subpixels are missing, and check it
      // never shows up.
      blitz::Array<double, 2> d(data.copy());
      int nall_missing = 0;
      for(int i = 0; i < d.rows(); ++i)
	for(int j = 0; j < d.cols(); ++j) {
	  bool all_missing = true;
	  for(int ii = i * nsub; ii < (i + 1) * nsub; ++ii)
	    for(int jj = j * nsub; jj < (j + 1) * nsub; ++jj)
	      if(xf(ii,jj) > -1000.0 && yf(ii,jj) > -1000.0)
		all_missing = false;
	  if(all_missing) {
	    d(i,j) = 1e10;
	    ++nall_missing;
	  }
	}
      BOOST_CHECK(nall_missing > 100);
      blitz::Array<double, 2> res = r.resample_field(d, 1.0, false, 0.0, false,
			      std::numeric_limits<double>::lowest(), ntask);
      BOOST_CHECK(blitz::max(res) < 1e9);
    }
}

BOOST_AUTO_TEST_CASE(no_valid_points)
{
  // All points missing throws, with both coverage and exact MapInfo.
  blitz::Array<double, 2> xf(x.shape()), yf(y.shape());
  xf = -9999;
  yf = y;
  for(bool exact : {false, true}) {
    BOOST_CHECK_THROW(Resampler(xf, yf, mi, nsub, exact, -1000.0), Exception);
    // Without a threshold, -9999 is a valid coordinate. It just
    // doesn't fall in the MapInfo.
    Resampler r(xf, yf, mi, nsub, exact);
    if(exact)
      BOOST_CHECK(r.empty_resample());
    blitz::Array<double, 2> xn(x.shape());
    xn = std::numeric_limits<double>::quiet_NaN();
    BOOST_CHECK_THROW(Resampler(xn, xn, mi, nsub, exact), Exception);
  }
  try {
    Resampler r(xf, yf, mi, nsub, false, -1000.0);
    BOOST_FAIL("Should have thrown an exception");
  } catch(const Exception& e) {
    BOOST_CHECK(std::string(e.what()).find("no valid coordinates") !=
		std::string::npos);
  }
  // Valid points that don't fall in the MapInfo are fine with
  // Exactly_match_mi, empty_resample handles that.
  MapInfo mi_far(boost::make_shared<GeodeticConverter>(), 20.0, 21.0,
		 21.0, 20.0, 200, 200);
  Resampler r(x, y, mi_far, nsub, true);
  BOOST_CHECK(r.empty_resample());
}

BOOST_AUTO_TEST_CASE(raster_ctor_matches_magnify_bilinear)
{
  // The RasterImage constructor used to use MagnifyBilinear. We now
  // do the interpolation ourselves, which should give exactly the
  // same results.
  int nl = 120, ns = 150;
  auto ximg = boost::make_shared<MemoryRasterImageFloat>(nl, ns);
  auto yimg = boost::make_shared<MemoryRasterImageFloat>(nl, ns);
  double th = 20 * M_PI / 180.0;
  for(int i = 0; i < nl; ++i)
    for(int j = 0; j < ns; ++j) {
      ximg->data()(i, j) = (10.05 + (std::cos(th) * j - std::sin(th) * i) * 0.0062);
      yimg->data()(i, j) = (10.95 - (std::sin(th) * j + std::cos(th) * i) * 0.0062);
    }
  for(int ns_sub : {1, 2, 3}) {
    MagnifyBilinear xmag(ximg, ns_sub);
    MagnifyBilinear ymag(yimg, ns_sub);
    blitz::Array<double, 2> xm = xmag.read_double(0,0, xmag.number_line(),
						  xmag.number_sample());
    blitz::Array<double, 2> ym = ymag.read_double(0,0, ymag.number_line(),
						  ymag.number_sample());
    for(bool exact : {false, true})
      for(int ntask : {1, 7}) {
	MapInfo mi_expect;
	blitz::Array<int, 3> expect =
	  reference_init(xm, ym, mi, exact,
			 std::numeric_limits<double>::lowest(), mi_expect);
	Resampler r(ximg, yimg, mi, ns_sub, exact,
		    std::numeric_limits<double>::lowest(), ntask);
	BOOST_CHECK_EQUAL(r.data_index().rows(), (nl - 1) * ns_sub);
	BOOST_CHECK_EQUAL(r.data_index().cols(), (ns - 1) * ns_sub);
	BOOST_CHECK(blitz::all(r.data_index() == expect));
	check_map_info(r.map_info(), mi_expect);
      }
  }
}

BOOST_AUTO_TEST_CASE(raster_ctor_fill)
{
  // The RasterImage constructor applies Fill_value_threshold before
  // interpolating, so exactly the subpixels that depend on a fill
  // point are missing. Uses -1e20 fill for one point, NaN for
  // another, and one point where only Y is fill.
  int nl = 30, ns = 40;
  auto ximg = boost::make_shared<MemoryRasterImageFloat>(nl, ns);
  auto yimg = boost::make_shared<MemoryRasterImageFloat>(nl, ns);
  for(int i = 0; i < nl; ++i)
    for(int j = 0; j < ns; ++j) {
      ximg->data()(i, j) = 10.05 + j * 0.0062;
      yimg->data()(i, j) = 10.95 - i * 0.0062;
    }
  std::vector<std::pair<int, int>> fill_pt = {{5, 5}, {10, 20}, {20, 30}};
  ximg->data()(5, 5) = -1e20;
  yimg->data()(5, 5) = -1e20;
  ximg->data()(10, 20) = std::numeric_limits<double>::quiet_NaN();
  yimg->data()(20, 30) = -1e20;
  int ns_sub = 3;
  // Each fill point affects (2 * ns_sub - 1)^2 subpixels, the ones
  // with a nonzero bilinear weight for that point.
  int naffect = (2 * ns_sub - 1) * (2 * ns_sub - 1);
  for(bool exact : {false, true}) {
    Resampler r(ximg, yimg, mi, ns_sub, exact, -1e10);
    const blitz::Array<int, 3>& di = r.data_index();
    int nmissing = 0;
    for(int i = 0; i < di.rows(); ++i)
      for(int j = 0; j < di.cols(); ++j) {
	bool expect_missing = false;
	for(auto& p : fill_pt) {
	  double dl = std::abs(i / (double) ns_sub - p.first);
	  double ds = std::abs(j / (double) ns_sub - p.second);
	  if(dl < 1 && ds < 1)
	    expect_missing = true;
	}
	bool missing = (di(i,j,0) == -9999 && di(i,j,1) == -9999);
	BOOST_CHECK_EQUAL(missing, expect_missing);
	if(missing)
	  ++nmissing;
      }
    BOOST_CHECK_EQUAL(nmissing, 3 * naffect);
  }
  // Without a threshold, the -1e20 points are treated as real
  // coordinates. That looks like a jump in the coordinates, which we
  // report as an error.
  try {
    Resampler r(ximg, yimg, mi, ns_sub, true);
    BOOST_FAIL("Should have thrown an exception");
  } catch(const Exception& e) {
    BOOST_CHECK(std::string(e.what()).find("Fill_value_threshold") !=
		std::string::npos);
  }
}

BOOST_AUTO_TEST_CASE(projected_negative_coordinates)
{
  // Large negative coordinates are fine for a projected MapInfo, so
  // they shouldn't be treated as fill by default. We use a meter sized
  // grid centered on 0, like an Albers grid.
  MapInfo mi_proj(projected_converter(), -500000.0, 500000.0,
		  500000.0, -500000.0, 4000, 4000);
  blitz::Array<double, 2> xp(100, 100), yp(100, 100);
  for(int i = 0; i < 100; ++i)
    for(int j = 0; j < 100; ++j) {
      xp(i,j) = -300000.0 + 125.0 + j * 250.0;
      yp(i,j) = -200000.0 - 125.0 - i * 250.0;
    }
  blitz::Array<double, 2> ones(100, 100);
  ones = 1;
  for(bool exact : {false, true}) {
    Resampler r(xp, yp, mi_proj, 1, exact);
    blitz::Array<double, 2> res = r.resample_field(ones, 1.0, false, -1.0);
    BOOST_CHECK_EQUAL((int) blitz::count(res == 1.0), 100 * 100);
  }
}

BOOST_AUTO_TEST_CASE(determine_range_matches_reference)
{
  // determine_range does small data in one thread, so use data large
  // enough to use the threads.
  int nl = 1200, ns = 1000;
  blitz::Array<double, 2> xl(nl, ns), yl(nl, ns);
  double th = 20 * M_PI / 180.0;
  for(int i = 0; i < nl; ++i)
    for(int j = 0; j < ns; ++j) {
      double ii = i / 2.0, jj = j / 2.0;
      xl(i,j) = 9.8 + (std::cos(th) * jj - std::sin(th) * ii) * 0.0062;
      yl(i,j) = 11.2 - (std::sin(th) * jj + std::cos(th) * ii) * 0.0062;
    }
  // Partial overlap, full overlap (MapInfo covers everything), and no
  // overlap.
  MapInfo mi_big(boost::make_shared<GeodeticConverter>(), 0.0, 20.0,
		 20.0, 0.0, 4000, 4000);
  MapInfo mi_far(boost::make_shared<GeodeticConverter>(), 30.0, 31.0,
		 31.0, 30.0, 200, 200);
  for(int nthread : {1, 10}) {
    ThreadPool::thread_pool().number_thread(nthread);
    for(const MapInfo& m : {mi, mi_big, mi_far})
      for(int ns_sub : {1, 2, 3}) {
	int ls, le, ss, se, ls2, le2, ss2, se2;
	Resampler::determine_range(xl, yl, m, ns_sub, ls, le, ss, se);
	reference_determine_range(xl, yl, m, ns_sub, ls2, le2, ss2, se2);
	BOOST_CHECK_EQUAL(ls, ls2);
	BOOST_CHECK_EQUAL(le, le2);
	BOOST_CHECK_EQUAL(ss, ss2);
	BOOST_CHECK_EQUAL(se, se2);
      }
  }
  // Check the cases are what we think they are
  int ls, le, ss, se;
  Resampler::determine_range(xl, yl, mi, 2, ls, le, ss, se);
  BOOST_CHECK(le > ls && se > ss && (le < nl || ss > 0 || se < ns));
  Resampler::determine_range(xl, yl, mi_big, 2, ls, le, ss, se);
  BOOST_CHECK_EQUAL(ls, 0);
  BOOST_CHECK_EQUAL(le, nl);
  BOOST_CHECK_EQUAL(ss, 0);
  BOOST_CHECK_EQUAL(se, ns);
  Resampler::determine_range(xl, yl, mi_far, 2, ls, le, ss, se);
  BOOST_CHECK(le < ls);
}

// Swath of n x n points with the given spacing in meters, centered
// on Lat0, Lon0, returning latitude and longitude in degrees. We
// build this in a local azimuthal equidistant projection, so it is a
// sensible swath even over a pole.
static void local_swath(double Lat0, double Lon0, int N, double Spacing,
			blitz::Array<double, 2>& Lat, blitz::Array<double, 2>& Lon)
{
  std::ostringstream s;
  s << "+proj=aeqd +lat_0=" << Lat0 << " +lon_0=" << Lon0
    << " +datum=WGS84 +units=m";
  auto ogr = OgrWrapper::from_proj4(s.str());
  Lat.resize(N, N);
  Lon.resize(N, N);
  for(int i = 0; i < N; ++i)
    for(int j = 0; j < N; ++j) {
      OgrCoordinate c(ogr, (j - N / 2.0 + 0.37) * Spacing,
		      -(i - N / 2.0 + 0.37) * Spacing, 0);
      Lat(i,j) = c.latitude();
      Lon(i,j) = c.longitude();
    }
}

// Convert lat/lon to the X/Y coordinates of a MapInfo.
static void to_map(const MapInfo& Mi, const blitz::Array<double, 2>& Lat,
		   const blitz::Array<double, 2>& Lon,
		   blitz::Array<double, 2>& X, blitz::Array<double, 2>& Y)
{
  X.resize(Lat.shape());
  Y.resize(Lat.shape());
  for(int i = 0; i < Lat.rows(); ++i)
    for(int j = 0; j < Lat.cols(); ++j) {
      double z;
      Mi.coordinate_converter().convert_to_coordinate(Geodetic(Lat(i,j), Lon(i,j)),
						      X(i,j), Y(i,j), z);
    }
}

// Bilinear interpolation to nsub subpixels, like
// scipy.ndimage.zoom(order=1) does.
static blitz::Array<double, 2> zoom_bilinear(const blitz::Array<double, 2>& A,
					     int Nsub)
{
  blitz::Array<double, 2> res(A.rows() * Nsub, A.cols() * Nsub);
  for(int i = 0; i < res.rows(); ++i)
    for(int j = 0; j < res.cols(); ++j) {
      double fi = i * (A.rows() - 1) / double(res.rows() - 1);
      double fj = j * (A.cols() - 1) / double(res.cols() - 1);
      int i0 = std::min((int) fi, A.rows() - 2);
      int j0 = std::min((int) fj, A.cols() - 2);
      double ti = fi - i0, tj = fj - j0;
      res(i,j) = (A(i0, j0) * (1 - tj) + A(i0, j0 + 1) * tj) * (1 - ti) +
	(A(i0 + 1, j0) * (1 - tj) + A(i0 + 1, j0 + 1) * tj) * ti;
    }
  return res;
}

// Check that F throws an Exception with Msg in the message.
template<class F> static void check_throw_msg(F f, const std::string& Msg)
{
  try {
    f();
    BOOST_ERROR("Expected an exception with \"" + Msg + "\"");
  } catch(const Exception& e) {
    if(std::string(e.what()).find(Msg) == std::string::npos)
      BOOST_ERROR("Exception didn't have \"" + Msg + "\":\n" + e.what());
  }
}

// 200 x 200 swath at 0.01 degrees, crossing the date line at column 100.
static void date_line_swath(blitz::Array<double, 2>& Lon,
			    blitz::Array<double, 2>& Lat, bool Lon360 = false)
{
  Lon.resize(200, 200);
  Lat.resize(200, 200);
  for(int i = 0; i < 200; ++i)
    for(int j = 0; j < 200; ++j) {
      double l = 179.005 + j * 0.01;
      Lon(i,j) = (Lon360 ? l : (l >= 180 ? l - 360 : l));
      Lat(i,j) = 10.005 + i * 0.01;
    }
}

BOOST_AUTO_TEST_CASE(date_line_geodetic_throws)
{
  blitz::Array<double, 2> lon, lat;
  date_line_swath(lon, lat);
  MapInfo mi_g(boost::make_shared<GeodeticConverter>(), 178.0, 13.0,
	       180.0, 9.0, 200, 400);
  std::string msg = "Use a Geodetic360 MapInfo";
  check_throw_msg([&]() { Resampler r(lon, lat, mi_g, 1); }, msg);
  check_throw_msg([&]() { Resampler r(lon, lat, mi_g, 1, true); }, msg);
  int ls, le, ss, se;
  check_throw_msg([&]()
     { Resampler::determine_range(lon, lat, mi_g, 1, ls, le, ss, se); }, msg);
  // Same through the RasterImage constructor
  auto ximg = boost::make_shared<MemoryRasterImageFloat>(200, 200);
  auto yimg = boost::make_shared<MemoryRasterImageFloat>(200, 200);
  ximg->data() = lon;
  yimg->data() = lat;
  check_throw_msg([&]() { Resampler r(ximg, yimg, mi_g, 2); }, msg);
}

BOOST_AUTO_TEST_CASE(date_line_interpolated_throws)
{
  // The realistic case, where the coordinates are interpolated
  // before we get them. The subpixels across the date line get made
  // up longitudes.
  blitz::Array<double, 2> lon, lat;
  date_line_swath(lon, lat);
  MapInfo mi_g(boost::make_shared<GeodeticConverter>(), 178.0, 13.0,
	       180.0, 9.0, 200, 400);
  for(int ns_sub : {2, 15}) {
    blitz::Array<double, 2> lonz = zoom_bilinear(lon, ns_sub);
    blitz::Array<double, 2> latz = zoom_bilinear(lat, ns_sub);
    check_throw_msg([&]() { Resampler r(lonz, latz, mi_g, ns_sub); },
		    "Use a Geodetic360 MapInfo");
    int ls, le, ss, se;
    check_throw_msg([&]()
       { Resampler::determine_range(lonz, latz, mi_g, ns_sub, ls, le, ss, se); },
		    "Use a Geodetic360 MapInfo");
  }
}

BOOST_AUTO_TEST_CASE(date_line_geodetic360_works)
{
  blitz::Array<double, 2> lon, lat;
  date_line_swath(lon, lat, true);
  // Pixel centers at the data points
  MapInfo mi_360(boost::make_shared<Geodetic360Converter>(), 179.0, 12.0,
		 181.0, 10.0, 200, 200);
  Resampler r(lon, lat, mi_360, 1);
  // The coverage MapInfo is about the size of the data, not 360 degrees
  BOOST_CHECK(r.map_info().number_x_pixel() <= 201);
  Resampler r2(lon, lat, mi_360, 1, true);
  blitz::Array<double, 2> ones(200, 200);
  ones = 1;
  blitz::Array<double, 2> res = r2.resample_field(ones, 1.0, false, -1.0);
  BOOST_CHECK_EQUAL((int) blitz::count(res == 1.0), 200 * 200);
}

BOOST_AUTO_TEST_CASE(near_date_line_no_throw)
{
  // Data up to, but not crossing, the date line is fine
  blitz::Array<double, 2> lon(200, 200), lat(200, 200);
  for(int i = 0; i < 200; ++i)
    for(int j = 0; j < 200; ++j) {
      lon(i,j) = 178.0 + j * 0.00995;
      lat(i,j) = 10.005 + i * 0.01;
    }
  MapInfo mi_g(boost::make_shared<GeodeticConverter>(), 178.0, 13.0,
	       180.0, 9.0, 200, 400);
  BOOST_CHECK_NO_THROW(Resampler(lon, lat, mi_g, 1));
  for(int ns_sub : {2, 15})
    BOOST_CHECK_NO_THROW(Resampler(zoom_bilinear(lon, ns_sub),
				   zoom_bilinear(lat, ns_sub), mi_g, ns_sub));
}

BOOST_AUTO_TEST_CASE(prime_meridian_geodetic360_throws)
{
  blitz::Array<double, 2> lon(200, 200), lat(200, 200);
  for(int i = 0; i < 200; ++i)
    for(int j = 0; j < 200; ++j) {
      double l = -0.995 + j * 0.01;
      lon(i,j) = (l < 0 ? l + 360 : l);
      lat(i,j) = 10.005 + i * 0.01;
    }
  MapInfo mi_360(boost::make_shared<Geodetic360Converter>(), 0.0, 13.0,
		 2.0, 9.0, 200, 400);
  check_throw_msg([&]() { Resampler r(lon, lat, mi_360, 1); },
		  "Use a Geodetic MapInfo");
  // Fine with Geodetic, where the longitudes are -1 to 1.
  lon = blitz::where(lon > 180, lon - 360, lon);
  MapInfo mi_g(boost::make_shared<GeodeticConverter>(), -1.0, 13.0,
	       1.0, 9.0, 200, 400);
  BOOST_CHECK_NO_THROW(Resampler(lon, lat, mi_g, 1));
}

BOOST_AUTO_TEST_CASE(date_line_ogr_geographic_throws)
{
  blitz::Array<double, 2> lon, lat;
  date_line_swath(lon, lat);
  // EPSG:4326 is geographic, so this is the same as Geodetic
  auto conv = boost::make_shared<OgrCoordinateConverter>(OgrWrapper::from_epsg(4326));
  MapInfo mi_ogr(conv, 178.0, 13.0, 180.0, 9.0, 200, 400);
  check_throw_msg([&]() { Resampler r(lon, lat, mi_ogr, 1); },
		  "Use a Geodetic360 MapInfo");
  // A projected system with the same data is fine.
  blitz::Array<double, 2> xu, yu;
  auto utm60 = OgrCoordinateConverter::utm_converter(60);
  MapInfo mi_utm60(utm60, 600000, 1300000, 900000, 1000000, 1200, 1200);
  to_map(mi_utm60, lat, lon, xu, yu);
  BOOST_CHECK_NO_THROW(Resampler(xu, yu, mi_utm60, 1));
}

BOOST_AUTO_TEST_CASE(date_line_missing_points)
{
  // Fill values next to good data aren't a crossing, as long as they
  // are marked as missing.
  blitz::Array<double, 2> lon(200, 200), lat(200, 200);
  for(int i = 0; i < 200; ++i)
    for(int j = 0; j < 200; ++j) {
      lon(i,j) = 178.0 + j * 0.00995;
      lat(i,j) = 10.005 + i * 0.01;
    }
  lon(Range(50, 60), Range(50, 60)) = -9999;
  lat(Range(50, 60), Range(50, 60)) = -9999;
  lon(Range(100, 110), Range(10, 20)) = std::numeric_limits<double>::quiet_NaN();
  MapInfo mi_g(boost::make_shared<GeodeticConverter>(), 178.0, 13.0,
	       180.0, 9.0, 200, 400);
  BOOST_CHECK_NO_THROW(Resampler(lon, lat, mi_g, 1, false, -1000.0));
  // Without the threshold, the -9999 isn't marked, and looks like a
  // jump.
  check_throw_msg([&]() { Resampler r(lon, lat, mi_g, 1); },
		  "fill values in the coordinates");
}

BOOST_AUTO_TEST_CASE(pole_geodetic_throws)
{
  blitz::Array<double, 2> lat, lon;
  for(double lat0 : {90.0, -90.0}) {
    local_swath(lat0, 0, 100, 1000, lat, lon);
    MapInfo mi_g(boost::make_shared<GeodeticConverter>(), -180, 90, 180, -90,
		 3600, 1800);
    check_throw_msg([&]() { Resampler r(lon, lat, mi_g, 1); }, "covers a pole");
    blitz::Array<double, 2> lon360(lon.copy());
    lon360 = blitz::where(lon360 < 0, lon360 + 360, lon360);
    MapInfo mi_360(boost::make_shared<Geodetic360Converter>(), 0, 90, 360, -90,
		   3600, 1800);
    check_throw_msg([&]() { Resampler r(lon360, lat, mi_360, 1); },
		    "covers a pole");
  }
  // A swath at 88N, near but not covering the pole, is fine
  local_swath(88, 0, 100, 1000, lat, lon);
  MapInfo mi_g(boost::make_shared<GeodeticConverter>(), -180, 90, 180, -90,
	       3600, 1800);
  BOOST_CHECK_NO_THROW(Resampler(lon, lat, mi_g, 1));
}

BOOST_AUTO_TEST_CASE(pole_projected_throws)
{
  // A swath over the north pole in projections that can't handle the
  // pole.
  blitz::Array<double, 2> lat, lon, x, y;
  local_swath(90, 0, 100, 1000, lat, lon);
  // Albers equal area centered on Los Angeles, as used by MAIA
  std::string albers_wkt = "PROJCS[\"unknown\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Albers_Conic_Equal_Area\"],PARAMETER[\"standard_parallel_1\",33.3],PARAMETER[\"standard_parallel_2\",37.3],PARAMETER[\"latitude_of_center\",35.3],PARAMETER[\"longitude_of_center\",-118.6],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1]]";
  auto albers = boost::make_shared<OgrCoordinateConverter>
    (boost::make_shared<OgrWrapper>(albers_wkt));
  auto merc = boost::make_shared<OgrCoordinateConverter>(OgrWrapper::from_epsg(3857));
  for(auto conv : std::vector<boost::shared_ptr<CoordinateConverter> >{albers, merc}) {
    MapInfo m(conv, -1e7, 1e7, 1e7, -1e7, 1000, 1000);
    to_map(m, lat, lon, x, y);
    check_throw_msg([&]() { Resampler r(x, y, m, 1); }, "polar projection");
    // Turning the check off works
    double sv = Resampler::max_step_ratio();
    Resampler::set_max_step_ratio(0);
    BOOST_CHECK_NO_THROW(Resampler(x, y, m, 1));
    Resampler::set_max_step_ratio(sv);
  }
}

BOOST_AUTO_TEST_CASE(pole_polar_stereographic_works)
{
  blitz::Array<double, 2> lat, lon, x, y;
  local_swath(90, 0, 100, 1000, lat, lon);
  auto ps = boost::make_shared<OgrCoordinateConverter>(OgrWrapper::from_epsg(3413));
  // 2 km grid, so each output pixel sees several data points.
  MapInfo m(ps, -120000, 120000, 120000, -120000, 120, 120);
  to_map(m, lat, lon, x, y);
  Resampler r(x, y, m, 1, true);
  blitz::Array<double, 2> ones(100, 100);
  ones = 1;
  blitz::Array<double, 2> res = r.resample_field(ones, 1.0, false, -1.0);
  // 100 x 100 km of data on a 2 km grid is about 2500 output pixels,
  // with holes where 1 km data doesn't hit every 2 km pixel.
  int ncov = blitz::count(res == 1.0);
  BOOST_CHECK(ncov > 2200 && ncov < 2800);
}

BOOST_AUTO_TEST_CASE(projection_cut_throws)
{
  // Swath at 35N straddling the Albers cut (180 degrees from the
  // central meridian of -118.6, so 61.4E). In UTM it is fine.
  blitz::Array<double, 2> lat, lon, x, y;
  local_swath(35, 61.4, 100, 1000, lat, lon);
  std::string albers_wkt = "PROJCS[\"unknown\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Albers_Conic_Equal_Area\"],PARAMETER[\"standard_parallel_1\",33.3],PARAMETER[\"standard_parallel_2\",37.3],PARAMETER[\"latitude_of_center\",35.3],PARAMETER[\"longitude_of_center\",-118.6],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1]]";
  auto albers = boost::make_shared<OgrCoordinateConverter>
    (boost::make_shared<OgrWrapper>(albers_wkt));
  MapInfo m(albers, -2e7, 2e7, 2e7, -2e7, 1000, 1000);
  to_map(m, lat, lon, x, y);
  check_throw_msg([&]() { Resampler r(x, y, m, 1); }, "discontinuity");
  int ls, le, ss, se;
  check_throw_msg([&]()
     { Resampler::determine_range(x, y, m, 1, ls, le, ss, se); },
		  "discontinuity");
  MapInfo mu(OgrCoordinateConverter::utm_converter(41), 0, 4e6, 1e6, 3e6,
	     1000, 1000);
  to_map(mu, lat, lon, x, y);
  BOOST_CHECK_NO_THROW(Resampler(x, y, mu, 1));
}

BOOST_AUTO_TEST_CASE(wrong_pole_throws)
{
  // South pole swath with north polar stereographic doesn't work, but
  // is fine with south polar stereographic.
  blitz::Array<double, 2> lat, lon, x, y;
  local_swath(-90, 0, 100, 1000, lat, lon);
  auto psn = boost::make_shared<OgrCoordinateConverter>(OgrWrapper::from_epsg(3413));
  MapInfo mn(psn, -2e7, 2e7, 2e7, -2e7, 1000, 1000);
  to_map(mn, lat, lon, x, y);
  check_throw_msg([&]() { Resampler r(x, y, mn, 1); }, "polar projection");
  auto pss = boost::make_shared<OgrCoordinateConverter>(OgrWrapper::from_epsg(3031));
  MapInfo ms(pss, -60000, 60000, 60000, -60000, 120, 120);
  to_map(ms, lat, lon, x, y);
  BOOST_CHECK_NO_THROW(Resampler(x, y, ms, 1));
}

BOOST_AUTO_TEST_CASE(no_false_positive_wide_scan)
{
  // A swath whose pixel size grows 10x across the scan, like the edge
  // of a wide scan. This shouldn't trigger the step check.
  int n = 200;
  blitz::Array<double, 2> x(n, n), y(n, n);
  for(int i = 0; i < n; ++i) {
    double xs = 0;
    for(int j = 0; j < n; ++j) {
      x(i,j) = 400000 + xs;
      xs += 250.0 * (1 + 9.0 * std::abs(j - n / 2.0) / (n / 2.0));
      y(i,j) = 3500000 - i * 250.0;
    }
  }
  MapInfo mu(projected_converter(), 0, 4e6, 1e6, 3e6, 1000, 1000);
  BOOST_CHECK_NO_THROW(Resampler(x, y, mu, 1));
}

BOOST_AUTO_TEST_CASE(timing)
{
  // Timing test, normally skipped. Turn on if you want to check the
  // speed of the threaded version.
  if(!getenv("GEOCAL_RESAMPLER_TIMING"))
    return;
  int n = 5000;
  blitz::Array<double, 2> xl(n * nsub, n * nsub), yl(xl.shape());
  double th = 20 * M_PI / 180.0;
  for(int i = 0; i < xl.rows(); ++i)
    for(int j = 0; j < xl.cols(); ++j) {
      double ii = i / (double) nsub, jj = j / (double) nsub;
      xl(i,j) = 1.3 + (std::cos(th) * jj - std::sin(th) * ii) * 0.0008;
      yl(i,j) = 9.0 - (std::sin(th) * jj + std::cos(th) * ii) * 0.0008;
    }
  MapInfo mil(boost::make_shared<GeodeticConverter>(), 0.0, 10.0, 10.0, 0.0,
	      2 * n, 2 * n);
  for(int ntask : {1, -1}) {
    auto t = std::chrono::steady_clock::now();
    Resampler r2(xl, yl, mil, nsub, false,
		 std::numeric_limits<double>::lowest(), ntask);
    std::cerr << "Resampler constructor Number_task " << ntask << ": "
	      << std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count()
	      << " s\n";
  }
  // Projected MapInfo, which uses the step check rather than the
  // longitude check.
  {
    MapInfo milp(projected_converter(), 0.0, 10.0, 10.0, 0.0, 2 * n, 2 * n);
    for(int ntask : {1, -1}) {
      auto t = std::chrono::steady_clock::now();
      Resampler r2(xl, yl, milp, nsub, false,
		   std::numeric_limits<double>::lowest(), ntask);
      std::cerr << "Resampler constructor projected Number_task " << ntask << ": "
		<< std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count()
		<< " s\n";
    }
    double sv = Resampler::max_step_ratio();
    Resampler::set_max_step_ratio(0);
    for(int ntask : {1, -1}) {
      auto t = std::chrono::steady_clock::now();
      Resampler r2(xl, yl, milp, nsub, false,
		   std::numeric_limits<double>::lowest(), ntask);
      std::cerr << "Resampler constructor projected, no step check, Number_task "
		<< ntask << ": "
		<< std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count()
		<< " s\n";
    }
    Resampler::set_max_step_ratio(sv);
  }
  for(int nthread : {1, ThreadPool::default_number_thread}) {
    ThreadPool::thread_pool().number_thread(nthread);
    int ls, le, ss, se;
    auto t = std::chrono::steady_clock::now();
    Resampler::determine_range(xl, yl, mil, nsub, ls, le, ss, se);
    std::cerr << "determine_range number_thread " << nthread << ": "
	      << std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count()
	      << " s\n";
  }
  Resampler r(xl, yl, mil, nsub);
  blitz::Array<double, 2> d(n, n);
  for(int i = 0; i < n; ++i)
    for(int j = 0; j < n; ++j)
      d(i,j) = (i * 7 + j * 13) % 1000;
  for(int ntask : {1, -1}) {
    auto t = std::chrono::steady_clock::now();
    blitz::Array<double, 2> res = r.resample_field(d, 1.0, false, 0.0, false,
			       std::numeric_limits<double>::lowest(), ntask);
    std::cerr << "resample_field Number_task " << ntask << ": "
	      << std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count()
	      << " s\n";
  }
}

// Don't actually test for serialization. The data is really pretty
// big, larger than we want to write out as XML. We could possibly
// write out binary data for some special case thing, but for now just
// assume serialization is working if we every need it.
// BOOST_AUTO_TEST_CASE(serialization)
//{
//}
BOOST_AUTO_TEST_SUITE_END()
