#include "resampler.h"
#include "geocal_serialize_support.h"
#include "magnify_bilinear.h"
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
//-------------------------------------------------------------------------

Resampler::Resampler
(const boost::shared_ptr<RasterImage>& X_coor,
 const boost::shared_ptr<RasterImage>& Y_coor,
 const MapInfo& Mi, int Num_sub_pixel, bool Exactly_match_mi,
 double Mark_missing)
  : nsub(Num_sub_pixel)
{
  MagnifyBilinear xmag(X_coor, Num_sub_pixel);
  MagnifyBilinear ymag(Y_coor, Num_sub_pixel);
  blitz::Array<double, 2> x = xmag.read_double(0,0, xmag.number_line(),
					       xmag.number_sample());
  blitz::Array<double, 2> y = ymag.read_double(0,0, ymag.number_line(),
					       ymag.number_sample());
  init(x, y, Mi, Exactly_match_mi, Mark_missing);
}

//-------------------------------------------------------------------------
/// Alternative constructor where we get the lat/lon from something
/// other than a file. The data should already be interpolated (e.g.,
/// in python do scipy.ndimage.interpolation.zoom(t,Num_sub_pixel,order=2)
//-------------------------------------------------------------------------

Resampler::Resampler
(const blitz::Array<double, 2>& X_coor_interpolated,
 const blitz::Array<double, 2>& Y_coor_interpolated,
 const MapInfo& Mi, int Num_sub_pixel, bool Exactly_match_mi,
 double Mark_missing)
  : nsub(Num_sub_pixel)
{
  init(X_coor_interpolated, Y_coor_interpolated, Mi, Exactly_match_mi, Mark_missing);
}

void Resampler::init(const blitz::Array<double, 2>& X_coor,
		     const blitz::Array<double, 2>& Y_coor,
		     const MapInfo& Mi, bool Exactly_match_mi,
		     double Mark_missing)
{
  blitz::Range ra = blitz::Range::all();

  data_index_.resize(X_coor.rows(), X_coor.cols(), 2);
  blitz::Array<double, 2> xindex, yindex;
  Mi.coordinate_to_index(X_coor, Y_coor, xindex, yindex);
  data_index_(ra,ra,0) = blitz::cast<int>(blitz::rint(yindex));
  data_index_(ra,ra,1) = blitz::cast<int>(blitz::rint(xindex));
  if(Exactly_match_mi) {
    mi = Mi;
    return;
  }
  // Determine min and max xindex and yindex, but exclude all points
  // with lat/lon that are fill values.
  bool first = true;
  int minx = 0, miny = 0, maxx = 0, maxy = 0;
  for(int i = 0; i < X_coor.rows(); ++i)
    for(int j = 0; j < X_coor.cols(); ++j)
      if(Y_coor(i, j) > Mark_missing && X_coor(i, j) > Mark_missing) {
	if(first) {
	  minx = data_index_(i,j,1);
	  miny = data_index_(i,j,0);
	  maxx = minx;
	  maxy = miny;
	  first = false;
	}
	minx = std::min(data_index_(i,j,1),minx);
	maxx = std::max(data_index_(i,j,1),maxx);
	miny = std::min(data_index_(i,j,0),miny);
	maxy = std::max(data_index_(i,j,0),maxy);
      }
  mi = Mi.subset(minx, miny, maxx - minx + 1, maxy - miny + 1);
  data_index_(ra,ra,0) -= miny;
  data_index_(ra,ra,1) -= minx;
  // Make sure all the lat/lon fill values have data_index out of
  // range so we don't use the data.
  for(int i = 0; i < X_coor.rows(); ++i)
    for(int j = 0; j < X_coor.cols(); ++j)
      if(Y_coor(i, j) <= Mark_missing && X_coor(i, j) <= Mark_missing)
	data_index_(i,j,ra) = -9999;
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
  lstart = X_coor_interpolated.rows();
  lend = -1;
  sstart = X_coor_interpolated.cols();
  send = -1;
  blitz::Array<double, 2> xindex, yindex;
  Mi.coordinate_to_index(X_coor_interpolated, Y_coor_interpolated, xindex, yindex);
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
  // Tweak to make divisible by Num_sub_pixel
  lstart = std::floor(double(lstart) / Num_sub_pixel) * Num_sub_pixel;
  lend = std::ceil(double(lend) / Num_sub_pixel) * Num_sub_pixel;
  lend = std::min(lend, xindex.rows());
  sstart = std::floor(double(sstart) / Num_sub_pixel) * Num_sub_pixel;
  send = std::ceil(double(send) / Num_sub_pixel) * Num_sub_pixel;
  send = std::min(send, xindex.cols());
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
  int ntask = Number_task;
  if(ntask < 0)
    ntask = ((long) nrow * ncol < min_parallel_size ? 1 :
	     4 * ThreadPool::thread_pool().number_thread());
  if(ThreadPool::thread_pool().number_thread() == 1)
    ntask = 1;
  
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

