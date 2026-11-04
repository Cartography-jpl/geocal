#include "unit_test_support.h"
#include "resampler.h"
#include "geocal_thread_pool.h"
#include "memory_raster_image.h"
#include "memory_raster_image_float.h"
#include "magnify_bilinear.h"
#include "gdal_raster_image.h"
#include "landsat7_global.h"
#include <boost/make_shared.hpp>
#include <chrono>
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
  // fill (or NaN), and missing points are marked -9999 with
  // Exactly_match_mi as well. Returns data_index, and fills in Mi_out.
  blitz::Array<int, 3> reference_init
  (const blitz::Array<double, 2>& X_coor, const blitz::Array<double, 2>& Y_coor,
   const MapInfo& Mi, bool Exactly_match_mi, double Mark_missing,
   MapInfo& Mi_out)
  {
    Range ra = Range::all();
    blitz::Array<int, 3> data_index(X_coor.rows(), X_coor.cols(), 2);
    blitz::Array<double, 2> xindex, yindex;
    Mi.coordinate_to_index(X_coor, Y_coor, xindex, yindex);
    data_index(ra,ra,0) = blitz::cast<int>(blitz::rint(yindex));
    data_index(ra,ra,1) = blitz::cast<int>(blitz::rint(xindex));
    auto missing = [&](int i, int j)
      { return !(Y_coor(i, j) > Mark_missing && X_coor(i, j) > Mark_missing); };
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
    BOOST_CHECK_THROW(Resampler(xf, yf, mi, nsub, exact), Exception);
    blitz::Array<double, 2> xn(x.shape());
    xn = std::numeric_limits<double>::quiet_NaN();
    BOOST_CHECK_THROW(Resampler(xn, xn, mi, nsub, exact), Exception);
  }
  try {
    Resampler r(xf, yf, mi, nsub);
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
  // A fill area
  for(int i = 10; i < 20; ++i)
    for(int j = 30; j < 50; ++j) {
      ximg->data()(i, j) = -1e20;
      yimg->data()(i, j) = -1e20;
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
	  reference_init(xm, ym, mi, exact, -1000.0, mi_expect);
	Resampler r(ximg, yimg, mi, ns_sub, exact, -1000.0, ntask);
	BOOST_CHECK_EQUAL(r.data_index().rows(), (nl - 1) * ns_sub);
	BOOST_CHECK_EQUAL(r.data_index().cols(), (ns - 1) * ns_sub);
	BOOST_CHECK(blitz::all(r.data_index() == expect));
	check_map_info(r.map_info(), mi_expect);
      }
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
    Resampler r2(xl, yl, mil, nsub, false, -1000.0, ntask);
    std::cerr << "Resampler constructor Number_task " << ntask << ": "
	      << std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count()
	      << " s\n";
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
