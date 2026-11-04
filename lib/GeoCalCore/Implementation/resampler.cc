#include "resampler.h"
#include "geocal_serialize_support.h"
#include "geodetic.h"
#include "vicar_raster_image.h"
#include "geocal_thread_pool.h"
#include <boost/make_shared.hpp>
#include <cmath>
#include <limits>
#include <vector>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Resampler::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Resampler);
  ar & GEOCAL_NVP(mi)
    & GEOCAL_NVP(nsub)
    & GEOCAL_NVP_(data_index);
}

GEOCAL_IMPLEMENT(Resampler);
#endif

//-------------------------------------------------------------------------
/// Constructor. This takes the latitude (Y) and longitude (X) fields as
/// RasterImage (we could have taken the L1B_GEO file name, but taking
/// RasterImage seems a little more general). We take the MapInfo that
/// we will resample to (you can get that from something like
/// mi = Landsat7Global("/raid22",Landsat7Global.BAND5).map_info.scale(2,2)
/// in python).
///
/// For a geodetic map, this is longitude (X) and latitude (Y). For
/// other map projections, this is just the general X and Y
/// coordinates. Note that these should already be converted to the
/// coordinate system of the map info.
///
/// We make sure the mapinfo covers the latitude/longitude range
///
/// We also pass in the number of subpixels to calculate, so for
/// example to work with 60 m landsat like map projection you'd want
/// this to be 2.
///
/// By default, we only use Mi to determine the pixel resolution, and
/// we make sure the output covers the full latitude/longitude
/// range. You can optionally specify that we exactly match the passed
/// in Mi, regardless of the actually coverage of the lat/lon. This is
/// useful if we are producing output files to compare against some
/// existing file.
///
/// A point is missing if either X or Y is <= Mark_missing, or is
/// NaN. Missing points aren't used. We throw an exception if all the
/// points are missing. Note that it is fine for none of the points to
/// fall in Mi with Exactly_match_mi, you can check for that with
/// empty_resample().
///
/// Note that we interpolate the coordinates with bilinear
/// interpolation, as MagnifyBilinear does. A subpixel next to a
/// missing point is interpolated with the fill value, so it is
/// probably missing as well, as long as the fill value is very negative
/// (e.g., -1e20).
///
/// See resample_field for a description of Number_task.
//-------------------------------------------------------------------------

Resampler::Resampler
(const boost::shared_ptr<RasterImage>& X_coor,
 const boost::shared_ptr<RasterImage>& Y_coor,
 const MapInfo& Mi, int Num_sub_pixel, bool Exactly_match_mi,
 double Mark_missing, int Number_task)
  : nsub(Num_sub_pixel)
{
  range_min_check(Num_sub_pixel, 1);
  // We interpolate the coordinates as we go, rather than creating
  // the full size interpolated arrays. This gives exactly the same
  // values as MagnifyBilinear (which we used to use): subpixel i is
  // at raw position i / nsub, so the interpolated arrays are
  // (number_line - 1) * nsub x (number_sample - 1) * nsub, and we
  // never need to extrapolate.
  blitz::Array<double, 2> x = X_coor->read_double(0, 0, X_coor->number_line(),
						  X_coor->number_sample());
  blitz::Array<double, 2> y = Y_coor->read_double(0, 0, Y_coor->number_line(),
						  Y_coor->number_sample());
  if(x.rows() != y.rows() || x.cols() != y.cols())
    throw Exception("X_coor and Y_coor need to be the same size");
  int nrow = std::max(x.rows() - 1, 0) * nsub;
  int ncol = std::max(x.cols() - 1, 0) * nsub;
  std::vector<double> frac(nsub);
  for(int k = 0; k < nsub; ++k)
    frac[k] = 1.0 / nsub * k;
  auto coor = [&x, &y, &frac, this](int i, int j, double& X, double& Y)
  {
    int iy = i / nsub, ix = j / nsub;
    double ln = frac[i % nsub], smp = frac[j % nsub];
    // Same order of operations as MagnifyBilinear, so we get
    // identical results.
    X = x(iy,ix) * (1 - ln) * (1 - smp) + x(iy, ix + 1) * (1 - ln) * smp
      + x(iy + 1, ix) * ln * (1 - smp) + x(iy + 1, ix + 1) * ln * smp;
    Y = y(iy,ix) * (1 - ln) * (1 - smp) + y(iy, ix + 1) * (1 - ln) * smp
      + y(iy + 1, ix) * ln * (1 - smp) + y(iy + 1, ix + 1) * ln * smp;
  };
  init(nrow, ncol, coor, Mi, Exactly_match_mi, Mark_missing, Number_task);
}

//-------------------------------------------------------------------------
/// Alternative constructor where we get the lat/lon from something
/// other than a file. The data should already be interpolated (e.g.,
/// in python do scipy.ndimage.interpolation.zoom(t,Num_sub_pixel,order=2)
///
/// Missing points are handled as in the other constructor, see that
/// for details. See resample_field for a description of Number_task.
//-------------------------------------------------------------------------

Resampler::Resampler
(const blitz::Array<double, 2>& X_coor_interpolated,
 const blitz::Array<double, 2>& Y_coor_interpolated,
 const MapInfo& Mi, int Num_sub_pixel, bool Exactly_match_mi,
 double Mark_missing, int Number_task)
  : nsub(Num_sub_pixel)
{
  range_min_check(Num_sub_pixel, 1);
  if(X_coor_interpolated.rows() != Y_coor_interpolated.rows() ||
     X_coor_interpolated.cols() != Y_coor_interpolated.cols())
    throw Exception("X_coor_interpolated and Y_coor_interpolated need to be the same size");
  auto coor = [&X_coor_interpolated, &Y_coor_interpolated]
    (int i, int j, double& X, double& Y)
  {
    X = X_coor_interpolated(i, j);
    Y = Y_coor_interpolated(i, j);
  };
  init(X_coor_interpolated.rows(), X_coor_interpolated.cols(), coor, Mi,
       Exactly_match_mi, Mark_missing, Number_task);
}

//-------------------------------------------------------------------------
/// Number of tasks to use for the given number of subpixels. A
/// Number_task < 0 is the default: 4 x the number of threads, or 1 if
/// the data is smaller than min_parallel_size.
//-------------------------------------------------------------------------

int Resampler::default_number_task(long Size, int Number_task)
{
  if(ThreadPool::thread_pool().number_thread() == 1)
    return 1;
  if(Number_task >= 0)
    return std::max(Number_task, 1);
  if(Size < min_parallel_size)
    return 1;
  return 4 * ThreadPool::thread_pool().number_thread();
}

//-------------------------------------------------------------------------
/// Fill in data_index_ and mi. Coor(i, j, X, Y) returns the X and Y
/// coordinate of subpixel i, j, for i in [0, Number_row) and j in
/// [0, Number_col). This gets called from several threads at once, so it
/// needs to be thread safe.
///
/// Note that we only use the inline MapInfo::coordinate_to_index in the
/// threads, which is just arithmetic on the MapInfo parameters. We
/// don't use the CoordinateConverter, which isn't thread safe in
/// general (e.g., OgrCoordinateConverter).
//-------------------------------------------------------------------------

template<class F> void Resampler::init
(int Number_row, int Number_col, F Coor,
 const MapInfo& Mi, bool Exactly_match_mi, double Mark_missing,
 int Number_task)
{
  data_index_.resize(Number_row, Number_col, 2);
  int ntask = default_number_task((long) Number_row * Number_col, Number_task);
  // Min and max index of the points that aren't fill, for each task.
  struct Range {
    bool found = false;
    int minx = 0, miny = 0, maxx = 0, maxy = 0;
  };
  std::vector<Range> trange(std::max(std::min(ntask, Number_row), 1));
  // With a coverage MapInfo, missing points are marked with this in
  // the first pass, and changed to -9999 in the second pass. We can't
  // use -9999 in the first pass, since a real index could happen to be
  // -9999 and we then shift the indexes in the second pass. A real
  // index can't be INT_MIN, we don't get anywhere close to that.
  const int fill_flag = std::numeric_limits<int>::min();
  const int missing_value = (Exactly_match_mi ? -9999 : fill_flag);
  parallel_row_blocks(Number_row, ntask,
    [&](int k, int istart, int iend)
    {
      Range r;
      for(int i = istart; i < iend; ++i)
	for(int j = 0; j < Number_col; ++j) {
	  double x, y, xindex, yindex;
	  Coor(i, j, x, y);
	  // Missing if either coordinate is fill (or NaN).
	  if(!(x > Mark_missing && y > Mark_missing)) {
	    data_index_(i,j,0) = missing_value;
	    data_index_(i,j,1) = missing_value;
	    continue;
	  }
	  Mi.coordinate_to_index(x, y, xindex, yindex);
	  // This is the same as blitz::cast<int>(blitz::rint(..)) that
	  // we used to use.
	  int ln = (int) std::rint(yindex);
	  int smp = (int) std::rint(xindex);
	  data_index_(i,j,0) = ln;
	  data_index_(i,j,1) = smp;
	  if(!r.found) {
	    r.found = true;
	    r.minx = r.maxx = smp;
	    r.miny = r.maxy = ln;
	  }
	  r.minx = std::min(smp, r.minx);
	  r.maxx = std::max(smp, r.maxx);
	  r.miny = std::min(ln, r.miny);
	  r.maxy = std::max(ln, r.maxy);
	}
      trange[k] = r;
    });
  // Determine min and max xindex and yindex of the points that aren't
  // fill.
  Range r;
  for(const auto& t : trange)
    if(t.found) {
      if(!r.found)
	r = t;
      r.minx = std::min(t.minx, r.minx);
      r.maxx = std::max(t.maxx, r.maxx);
      r.miny = std::min(t.miny, r.miny);
      r.maxy = std::max(t.maxy, r.maxy);
    }
  if(!r.found) {
    Exception e;
    e << "Resampler: there are no valid coordinates. All "
      << Number_row << " x " << Number_col
      << " points are fill (<= Mark_missing = " << Mark_missing
      << ") or NaN.";
    throw e;
  }
  if(Exactly_match_mi) {
    mi = Mi;
    return;
  }
  mi = Mi.subset(r.minx, r.miny, r.maxx - r.minx + 1, r.maxy - r.miny + 1);
  // Shift to the subsetted MapInfo, and set all the fill points to
  // -9999 so we don't use the data.
  parallel_row_blocks(Number_row, ntask,
    [&](int, int istart, int iend)
    {
      for(int i = istart; i < iend; ++i)
	for(int j = 0; j < Number_col; ++j) {
	  if(data_index_(i,j,0) == fill_flag) {
	    data_index_(i,j,0) = -9999;
	    data_index_(i,j,1) = -9999;
	  } else {
	    data_index_(i,j,0) -= r.miny;
	    data_index_(i,j,1) -= r.minx;
	  }
	}
    });
}

//-------------------------------------------------------------------------
/// Had an issue with python pool hanging. Not sure of the exact
/// reason, but we tracked this down to some resource not being
/// released. As a simple workaround, have a clear function that
/// empties data_index. You wouldn't normally call this, but we do in
/// python to work around this python multiprocessing pool issue.
///
/// ** Note this ended up being a red herring, and we don't actually use
/// this anymore. However leave this place in case we end up needing
/// come back to this **
//-------------------------------------------------------------------------

void Resampler::clear()
{
  blitz::Array<int, 3> empty;
  data_index_.reference(empty);
  std::cerr << "Cleared data\n";
}

//-------------------------------------------------------------------------
/// Determine range of X and Y coordinate arrays that actually touch
/// the given MapInfo. Used when we are producing the tiled product
/// where lots of the data isn't even used and can be skipped before
/// we even start.
//-------------------------------------------------------------------------

void Resampler::determine_range(const blitz::Array<double, 2>& X_coor_interpolated,
				const blitz::Array<double, 2>& Y_coor_interpolated,
				const MapInfo& Mi, int Num_sub_pixel,
				int& lstart, int& lend, int& sstart, int& send)
{
  if(X_coor_interpolated.rows() != Y_coor_interpolated.rows() ||
     X_coor_interpolated.cols() != Y_coor_interpolated.cols())
    throw Exception("X_coor_interpolated and Y_coor_interpolated need to be the same size");
  int nrow = X_coor_interpolated.rows();
  int ncol = X_coor_interpolated.cols();
  int ntask = default_number_task((long) nrow * ncol, -1);
  struct Range {
    int lstart, lend, sstart, send;
  };
  std::vector<Range> trange(std::max(std::min(ntask, nrow), 1),
			    Range{nrow, -1, ncol, -1});
  parallel_row_blocks(nrow, ntask,
    [&](int k, int istart, int iend)
    {
      Range r{nrow, -1, ncol, -1};
      for(int i = istart; i < iend; ++i)
	for(int j = 0; j < ncol; ++j) {
	  double xindex, yindex;
	  Mi.coordinate_to_index(X_coor_interpolated(i,j),
				 Y_coor_interpolated(i,j), xindex, yindex);
	  int ln = (int) std::round(yindex);
	  int smp = (int) std::round(xindex);
	  if(ln >= 0 && ln < Mi.number_y_pixel() &&
	     smp >=0 && smp < Mi.number_x_pixel()) {
	    r.lstart = std::min(r.lstart, i);
	    r.lend = std::max(r.lend, i);
	    r.sstart = std::min(r.sstart, j);
	    r.send = std::max(r.send, j);
	  }
	}
      trange[k] = r;
    });
  lstart = nrow;
  lend = -1;
  sstart = ncol;
  send = -1;
  for(const auto& r : trange) {
    lstart = std::min(lstart, r.lstart);
    lend = std::max(lend, r.lend);
    sstart = std::min(sstart, r.sstart);
    send = std::max(send, r.send);
  }
  // Tweak to make divisible by Num_sub_pixel
  lstart = std::floor(double(lstart) / Num_sub_pixel) * Num_sub_pixel;
  lend = std::ceil(double(lend) / Num_sub_pixel) * Num_sub_pixel;
  lend = std::min(lend, nrow);
  sstart = std::floor(double(sstart) / Num_sub_pixel) * Num_sub_pixel;
  send = std::ceil(double(send) / Num_sub_pixel) * Num_sub_pixel;
  send = std::min(send, ncol);
}

//-------------------------------------------------------------------------
/// Check to see if we will have any data fall with the MapInfo.
//-------------------------------------------------------------------------

bool Resampler::empty_resample() const
{
  for(int i = 0; i < data_index_.rows(); ++i)
    for(int j = 0; j < data_index_.cols(); ++j) {
      int ln, smp;
      ln = data_index_(i,j,0);
      smp = data_index_(i,j,1);
      if(ln >= 0 && ln < mi.number_y_pixel() &&
	 smp >=0 && smp < mi.number_x_pixel())
	return false;
    }
  return true;
}

//-------------------------------------------------------------------------
/// Check to see if we will have any data fall with the
/// MapInfo. Checks a field also to exclude fill data
///
/// For some data, we use a fill value. You can pass the threshold to
/// check if we have fill data or not (e.g., -9999.0). The default is
/// no fill data.
//-------------------------------------------------------------------------

bool Resampler::empty_resample(const boost::shared_ptr<RasterImage>& Data, double Fill_value_threshold) const
{
  return empty_resample(Data->read_double(0, 0, Data->number_line(),
					  Data->number_sample()),
			Fill_value_threshold);
}

//-------------------------------------------------------------------------
/// Check to see if we will have any data fall with the
/// MapInfo. Checks a field also to exclude fill data.
///
/// This is the same as the RasterImage version, but takes the data
/// directly. As with resample_field, the data is at the original
/// resolution (i.e., not magnified by number_sub_pixel()).
//-------------------------------------------------------------------------

bool Resampler::empty_resample(const blitz::Array<double, 2>& Data,
			       double Fill_value_threshold) const
{
  check_data_size(Data.rows(), Data.cols());
  for(int i = 0; i < data_index_.rows(); ++i)
    for(int j = 0; j < data_index_.cols(); ++j) {
      int ln, smp;
      ln = data_index_(i,j,0);
      smp = data_index_(i,j,1);
      if(ln >= 0 && ln < mi.number_y_pixel() &&
	 smp >=0 && smp < mi.number_x_pixel() &&
	 Data(i / nsub, j / nsub) > Fill_value_threshold)
	return false;
    }
  return true;
}

//-------------------------------------------------------------------------
/// Check that data passed to resample_field or empty_resample is large
/// enough. The data is at the original resolution, so magnified by
/// nsub it should cover data_index.
//-------------------------------------------------------------------------

void Resampler::check_data_size(int Number_line, int Number_sample) const
{
  if((long) Number_line * nsub < data_index_.rows() ||
     (long) Number_sample * nsub < data_index_.cols()) {
    Exception e;
    e << "data_index should be larger than magnified data\n"
      << "d:          " << (long) Number_line * nsub << " x "
      << (long) Number_sample * nsub << "\n"
      << "data_index: " << data_index_.rows() << " x " << data_index_.cols()
      << "\n";
    throw e;
  }
}

//-------------------------------------------------------------------------
/// Resample the given data and return an array of values.
///
/// We can scale the data by the given factor, optionally clip
/// negative values to 0, and specify the fill_value to use for data
/// that we don't see.
///
/// For some data, averaging the values that hit a grid cell doesn't
/// actually make sense (e.g., for view angles). You can instead
/// specify Use_smallest_ic which uses the value for the smallest
/// line/sample that sees a particular grid cell.
///
/// For some data, we use a fill value. You can pass the threshold to
/// check if we have fill data or not (e.g., -9999.0). The default is
/// no fill data.
///
/// This reads all the data from the RasterImage, and then calls the
/// version of resample_field that takes a blitz::Array. See that
/// function for a description of Number_task.
//-------------------------------------------------------------------------

blitz::Array<double, 2> Resampler::resample_field
(const boost::shared_ptr<RasterImage>& Data,
 double Scale_data, bool Negative_to_zero, double Fill_value,
 bool Use_smallest_ic, double Fill_value_threshold, int Number_task) const
{
  return resample_field(Data->read_double(0, 0, Data->number_line(),
					  Data->number_sample()),
			Scale_data, Negative_to_zero, Fill_value,
			Use_smallest_ic, Fill_value_threshold, Number_task);
}

//-------------------------------------------------------------------------
/// Resample the given data and return an array of values.
///
/// This is the same as the RasterImage version, but takes the data
/// directly. The data is at the original resolution (i.e., not
/// magnified by number_sub_pixel()). We replicate each pixel to
/// number_sub_pixel() x number_sub_pixel() subpixels. This is
/// particularly important to get the fill values correct.
///
/// The work is divided up into Number_task tasks, run by the
/// ThreadPool (so ThreadPool::number_thread() controls the number of
/// threads used). The default of -1 uses 4 x number_thread() tasks,
/// and does small data in a single thread. Number_task = 1 does
/// everything in this thread.
///
/// The result doesn't depend on the number of tasks, except for
/// floating point round off in the averages (about 1e-15 relative).
/// The result with Use_smallest_ic doesn't depend on the number of
/// tasks at all.
///
/// This uses the global ThreadPool, so it can't be called from inside
/// a ThreadPool task, or by two threads at the same time.
//-------------------------------------------------------------------------

blitz::Array<double, 2> Resampler::resample_field
(const blitz::Array<double, 2>& Data,
 double Scale_data, bool Negative_to_zero, double Fill_value,
 bool Use_smallest_ic, double Fill_value_threshold, int Number_task) const
{
  check_data_size(Data.rows(), Data.cols());
  int nline_out = mi.number_y_pixel();
  int nsamp_out = mi.number_x_pixel();
  int nrow = data_index_.rows();
  int ncol = data_index_.cols();
  blitz::Array<double, 2> res(nline_out, nsamp_out);
  // Final value for an output pixel, given the sum and count.
  auto final_value = [&](double sum, int count) -> double
  {
    double v = (count == 0 ? Fill_value : sum / count * Scale_data);
    if(Negative_to_zero && v < 0)
      v = 0;
    return v;
  };
  int ntask = default_number_task((long) nrow * ncol, Number_task);

  // Divide data_index into tiles. We want a reasonable number of
  // tiles for each task, but not so small that the bounding boxes
  // overlap a lot.
  std::vector<ResampleTile> tile;
  if(ntask > 1 && nrow > 0 && ncol > 0) {
    int tsize = (int) std::ceil(std::sqrt((double) nrow * ncol / ntask));
    tsize = std::max(std::min(tsize, 1024), 16);
    for(int i = 0; i < nrow; i += tsize)
      for(int j = 0; j < ncol; j += tsize) {
	ResampleTile t;
	t.istart = i;
	t.iend = std::min(i + tsize, nrow);
	t.jstart = j;
	t.jend = std::min(j + tsize, ncol);
	tile.push_back(t);
      }
    // Find the bounding box of each tile.
    parallel_row_blocks((int) tile.size(), ntask,
      [&](int, int kstart, int kend)
      {
	for(int k = kstart; k < kend; ++k) {
	  ResampleTile& t = tile[k];
	  int lmin = nline_out, lmax = -1, smin = nsamp_out, smax = -1;
	  for(int i = t.istart; i < t.iend; ++i)
	    for(int j = t.jstart; j < t.jend; ++j) {
	      int ln = data_index_(i,j,0);
	      int smp = data_index_(i,j,1);
	      if(ln >= 0 && ln < nline_out && smp >=0 && smp < nsamp_out) {
		lmin = std::min(lmin, ln);
		lmax = std::max(lmax, ln);
		smin = std::min(smin, smp);
		smax = std::max(smax, smp);
	      }
	    }
	  if(lmax >= 0) {
	    t.lstart = lmin;
	    t.nline = lmax - lmin + 1;
	    t.sstart = smin;
	    t.nsamp = smax - smin + 1;
	  }
	}
      });
    // If the bounding boxes are much larger than the output (e.g., a
    // strongly curved swath), the memory isn't worth it. Just do this
    // in one thread.
    double total = 0;
    for(const auto& t : tile)
      total += (double) t.nline * t.nsamp;
    if(total > 2.0 * nline_out * nsamp_out)
      ntask = 1;
  }

  if(ntask <= 1) {
    blitz::Array<double, 2> sum(res.shape());
    blitz::Array<int, 2> cnt(res.shape());
    sum = 0;
    cnt = 0;
    for(int i = 0; i < nrow; ++i)
      for(int j = 0; j < ncol; ++j) {
	int ln = data_index_(i,j,0);
	int smp = data_index_(i,j,1);
	if(ln >= 0 && ln < nline_out && smp >=0 && smp < nsamp_out) {
	  double d = Data(i / nsub, j / nsub);
	  // Add data, unless it is fill or we already have data there
	  // and Use_smallest_ic is true
	  if(d > Fill_value_threshold &&
	     (!Use_smallest_ic || cnt(ln,smp) == 0)) {
	    sum(ln,smp) += d;
	    cnt(ln,smp) += 1;
	  }
	}
      }
    for(int i = 0; i < res.rows(); ++i)
      for(int j = 0; j < res.cols(); ++j)
	res(i,j) = final_value(sum(i,j), cnt(i,j));
    return res;
  }

  // Accumulate the data for each tile.
  parallel_row_blocks((int) tile.size(), ntask,
    [&](int, int kstart, int kend)
    {
      for(int k = kstart; k < kend; ++k) {
	ResampleTile& t = tile[k];
	t.sum.assign((size_t) t.nline * t.nsamp, 0.0);
	t.cnt.assign(t.sum.size(), 0);
	if(Use_smallest_ic)
	  t.first.assign(t.sum.size(), std::numeric_limits<long>::max());
	if(t.nline == 0)
	  continue;
	for(int i = t.istart; i < t.iend; ++i)
	  for(int j = t.jstart; j < t.jend; ++j) {
	    int ln = data_index_(i,j,0);
	    int smp = data_index_(i,j,1);
	    if(ln >= 0 && ln < nline_out && smp >=0 && smp < nsamp_out) {
	      double d = Data(i / nsub, j / nsub);
	      if(d > Fill_value_threshold) {
		size_t q = (size_t) (ln - t.lstart) * t.nsamp + (smp - t.sstart);
		// We go through the tile in order, so the first value
		// we see has the smallest position in the tile.
		if(!Use_smallest_ic) {
		  t.sum[q] += d;
		  t.cnt[q] += 1;
		} else if(t.cnt[q] == 0) {
		  t.sum[q] = d;
		  t.cnt[q] = 1;
		  t.first[q] = (long) i * ncol + j;
		}
	      }
	    }
	  }
      }
    });

  // Merge the tiles, dividing the output into blocks of lines. We go
  // through the tiles in the same order for every output pixel, so
  // the result doesn't depend on how the work is divided up.
  parallel_row_blocks(nline_out, ntask,
    [&](int, int lstart, int lend)
    {
      int nl = lend - lstart;
      std::vector<double> sum((size_t) nl * nsamp_out, 0.0);
      std::vector<int> cnt(sum.size(), 0);
      std::vector<long> first;
      if(Use_smallest_ic)
	first.assign(sum.size(), std::numeric_limits<long>::max());
      for(const auto& t : tile) {
	int l1 = std::max(lstart, t.lstart);
	int l2 = std::min(lend, t.lstart + t.nline);
	for(int l = l1; l < l2; ++l)
	  for(int s = 0; s < t.nsamp; ++s) {
	    size_t qt = (size_t) (l - t.lstart) * t.nsamp + s;
	    if(t.cnt[qt] == 0)
	      continue;
	    size_t q = (size_t) (l - lstart) * nsamp_out + t.sstart + s;
	    if(!Use_smallest_ic) {
	      sum[q] += t.sum[qt];
	      cnt[q] += t.cnt[qt];
	    } else if(t.first[qt] < first[q]) {
	      first[q] = t.first[qt];
	      sum[q] = t.sum[qt];
	      cnt[q] = 1;
	    }
	  }
      }
      for(int l = lstart; l < lend; ++l)
	for(int s = 0; s < nsamp_out; ++s) {
	  size_t q = (size_t) (l - lstart) * nsamp_out + s;
	  res(l, s) = final_value(sum[q], cnt[q]);
	}
    });
  return res;
}

//-------------------------------------------------------------------------
/// Resample the given data, and write out to a VICAR file with the
/// given name.
///
/// You can optionally scale the output data, and specify the file
/// output type to write. This is useful if you want to view float
/// data in xvd, which works much better with scaled int.
///
/// You can optionally map all negative values to zero, useful to view
/// data without large negative fill values (e.g., -9999).
///
/// For some data, we use a fill value. You can pass the threshold to
/// check if we have fill data or not (e.g., -9999.0). The default is
/// no fill data.
//-------------------------------------------------------------------------

void Resampler::resample_field
(const std::string& Fname,
 const boost::shared_ptr<RasterImage>& Data,
 double Scale_data, const std::string& File_type, bool Negative_to_zero,
 double Fill_value, double Fill_value_threshold) const
{
  blitz::Array<double, 2> res(resample_field(Data, Scale_data, Negative_to_zero,
					     Fill_value, false,
					     Fill_value_threshold));
  VicarRasterImage f(Fname, mi, File_type);
  f.write(0,0,res);
}


//-------------------------------------------------------------------------
/// Various fields from the map_info. This is just all in a function
/// because this is much faster to do in C++ vs. looping in python.
//-------------------------------------------------------------------------

void Resampler::map_values
(const Dem& d,
 blitz::Array<double, 2>& Lat, blitz::Array<double, 2>& Lon,
 blitz::Array<double, 2>& Height) const
{
  Lat.resize(mi.number_y_pixel(), mi.number_x_pixel());
  Lon.resize(Lat.shape());
  Height.resize(Lat.shape());
  for(int i = 0; i < Lat.rows(); ++i)
    for(int j = 0; j < Lat.cols(); ++j) {
      boost::shared_ptr<GroundCoordinate> gp = mi.ground_coordinate(j, i, d);
      gp->lat_lon_height(Lat(i,j), Lon(i,j), Height(i,j));
    }
}

