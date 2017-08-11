#include "rpc_image.h"
#include "null_deleter.h"
#include "memory_raster_image.h"
#include "sub_raster_image.h"
#include "raster_averaged.h"
#include "simple_dem.h"
#include <cmath>
#include <boost/foreach.hpp>
#include <boost/timer.hpp>
#include <fstream>
#include "image_point_display.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. We either average the raw data by the given factor,
/// or we take the ratio of the Mi resolution and the Rpc resolution.
//-----------------------------------------------------------------------

RpcImage::RpcImage(const boost::shared_ptr<RasterImage>& Raw_img, const Rpc& R,
		   const boost::shared_ptr<Dem>& D,
		   const MapInfo& Mi, int Avg_fact)
: RasterImageVariable(Mi),
  dem_(D), raw_img_(Raw_img), line_avg_fact_(9), sample_avg_fact_(9)
{
  rpc_.reset(new Rpc(R));
  if(Avg_fact > 0) {
    line_avg_fact_ = Avg_fact;
    sample_avg_fact_ = Avg_fact;
  } else {
    line_avg_fact_ = 
      (int) round(Mi.resolution_meter() / R.resolution_meter(*D));
    sample_avg_fact_ = line_avg_fact_;
  }

//-----------------------------------------------------------------------
// Now, average the data.
//-----------------------------------------------------------------------

  if(line_avg_fact_ ==1 &&
     sample_avg_fact_ ==1) {
    img_avg_ = raw_img_;
    // The number of tile lines and sample is approximate. But this
    // should roughly constrain data to stay in a few tiles.
    number_tile_line_ = raw_img_->number_tile_line();
    number_tile_sample_ = raw_img_->number_tile_sample();
  } else {
    img_avg_.reset(new MemoryRasterImage(RasterAveraged(raw_img_, 
							line_avg_fact_,
							sample_avg_fact_)));
//     img_avg_.reset(new RasterAveraged(raw_img_, 
// 				      line_avg_fact_,
// 				      sample_avg_fact_));
    number_tile_line_ = number_line();
    number_tile_sample_ = number_sample();
  }
}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample. This is like
/// operator(), but this returns a double rather than an int.
//-----------------------------------------------------------------------

double RpcImage::value(int Line, int Sample) const
{
  range_check(Line, 0, number_line());
  range_check(Sample, 0, number_sample());
  boost::shared_ptr<GroundCoordinate> gc = 
    ground_coordinate(ImageCoordinate(Line, Sample), dem());
  ImageCoordinate ic(rpc().image_coordinate(*gc));
  ic.line /= line_avg_fact_;
  ic.sample /= sample_avg_fact_;
  if(ic.line < 0 || ic.line > img_avg_->number_line() - 1 ||
     ic.sample < 0 || ic.sample > img_avg_->number_sample() - 1)
    return 0;
  return img_avg_->interpolate(ic.line, ic.sample);
}

//-----------------------------------------------------------------------
/// Variation of value that takes a height above the reference surface
/// rather than using the Dem associated with this class.
//-----------------------------------------------------------------------

double RpcImage::value(int Line, int Sample, double Height) const
{
  range_check(Line, 0, number_line());
  range_check(Sample, 0, number_sample());
  SimpleDem dem_h(Height);
  boost::shared_ptr<GroundCoordinate> gc = 
    ground_coordinate(ImageCoordinate(Line, Sample), dem_h);
  ImageCoordinate ic(rpc().image_coordinate(*gc));
  ic.line /= line_avg_fact_;
  ic.sample /= sample_avg_fact_;
  if(ic.line < 0 || ic.line > img_avg_->number_line() - 1 ||
     ic.sample < 0 || ic.sample > img_avg_->number_sample() - 1)
    return 0;
  return img_avg_->interpolate(ic.line, ic.sample);
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

void RpcImage::read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
{
  range_check(Lstart, 0, number_line() - Number_line + 1);
  range_check(Sstart, 0, number_sample() - Number_sample + 1);
  blitz::Array<double, 2> lat(Number_line, Number_sample);
  blitz::Array<double, 2> lon(lat.shape());
  blitz::Array<double, 2> height(lat.shape());
  for(int i = 0; i < Number_line; ++i)
    for(int j = 0; j < Number_sample; ++j) {
      boost::shared_ptr<GroundCoordinate> gc = 
	ground_coordinate(ImageCoordinate(i + Lstart, j + Sstart), dem());
      lat(i, j) = gc->latitude();
      lon(i, j) = gc->longitude();
      height(i, j) = gc->height_reference_surface();
    }
  blitz::Array<double, 3> ic = rpc_->image_coordinate(lat, lon, height);
  for(int i = 0; i < Number_line; ++i)
    for(int j = 0; j < Number_sample; ++j, ++Res) {
      double l = ic(0, i, j) / line_avg_fact_;
      double s = ic(1, i, j) / sample_avg_fact_;
      if(l > 0 && l < img_avg_->number_line() - 1 &&
	 s > 0 && s < img_avg_->number_sample() - 1)
	*Res = (int) img_avg_->interpolate(l, s);
      else
	*Res = 0;
    }
}

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

void RpcImage::unchecked_write(int Line, int Sample, int Val) 
{
  throw Exception("write not implemented");
}
void RpcImage::unchecked_write(int Line, int Sample, double Val) 
{
  throw Exception("write not implemented");
}

//-----------------------------------------------------------------------
/// This sets up ref_img. We subset the passed in Ref_img to match the
/// same area and the New_img. Typically the Ref_img is a bit larger.
//-----------------------------------------------------------------------

void RpcImage::set_ref_img(const RasterImage& Ref_img, 
			   const RasterImage& New_img)
{
  ImageCoordinate ref_ulc = Ref_img.coordinate(
     *New_img.ground_coordinate(ImageCoordinate(0, 0)));
  ImageCoordinate ref_lrc = Ref_img.coordinate(
     *New_img.ground_coordinate(ImageCoordinate(New_img.number_line(), 
					    New_img.number_sample())));
  int sl = std::max((int) floor(ref_ulc.line), 0);
  int ss = std::max((int) floor(ref_ulc.sample), 0);
  int el = std::min((int) ceil(ref_lrc.line), Ref_img.number_line());
  int es = std::min((int) ceil(ref_lrc.sample), 
		    Ref_img.number_sample());
  ref_img.reset(new MemoryRasterImage(
	      SubRasterImage(const_pointer(Ref_img),
			     sl, ss, el - sl, es - ss)));
}

//-----------------------------------------------------------------------
/// This collects interest points to do matching on.
///
/// We use the FeatureDetector Fd to determine where to match, the
/// areas we look for a feature is in each grid square of
/// Feature_grid_space x Feature_grid_space. We use the GroundMask to
/// exclude areas that we should not do matching in.
///
/// We collect the interest points from ref_img.
//-----------------------------------------------------------------------

void RpcImage::interest_point(const FeatureDetector& Fd, const GroundMask& M,
	            int Feature_grid_space)
{
  std::cerr << "Starting Interest Points. Elapsed time: " << tmr.elapsed() << " s.\n";
  ip_grid = Fd.interest_point_grid(*ref_img, M,
		(int) ceil(ref_img->number_line() / Feature_grid_space),
		(int) ceil(ref_img->number_sample() / Feature_grid_space));
  std::cerr << "Interest point done. Elapsed time: " << tmr.elapsed() << " s.\n";
}

//-----------------------------------------------------------------------
/// This does image matching between a reference and a new image. The
/// images should be at near the same resolution. 
///
/// This fills in gref, gnew, ic, dist, lsigma, ssigma, failed_gp,
/// and im_diagnostic. We return the
/// average distance in meters between gref and gnew (i.e., a measure
/// of how far New_img was from matching Ref_img).
//-----------------------------------------------------------------------

double RpcImage::match(const RasterImage& New_img,
		       const ImageMatcher& Im)
{
//-----------------------------------------------------------------------
/// Clear out old results.
//-----------------------------------------------------------------------

  gref.clear(); gnew.clear(); ic.clear(); lsigma.clear(); ssigma.clear();
  dist.clear(); failed_gp.clear(); im_diagnostic.clear(); 

//-----------------------------------------------------------------------
/// Do matching.
//-----------------------------------------------------------------------

  std::cerr << "Starting Matching\n";
  double dist_ave = 0.0;
  BOOST_FOREACH(ImageCoordinate& ip, ip_grid) {
    boost::shared_ptr<GroundCoordinate> gr = 
      ref_img->ground_coordinate(ip, dem());
    ImageCoordinate inew = New_img.coordinate(*gr);
    ImageCoordinate ires;
    double line_sigma, sample_sigma;
    bool success = false;
    int diagnostic = 0;
    if(inew.line >= 0 &&
       inew.line < New_img.number_line() &&
       inew.sample >= 0 &&
       inew.sample < New_img.number_sample())
      Im.match(*ref_img, New_img, ip, inew, ires, line_sigma, 
	       sample_sigma, success, &diagnostic);
    if(success) {
      boost::shared_ptr<GroundCoordinate> gn =
	New_img.ground_coordinate(ires, dem());
      ImageCoordinate inew = rpc().image_coordinate(*gn);
      ic.push_back(inew);
      lsigma.push_back(line_avg_fact_ * line_sigma);
      ssigma.push_back(sample_avg_fact_ * sample_sigma);
      double d = distance(*gn, *gr);
      dist_ave += distance(*gn, *gr);
      dist.push_back(d);
      gref.push_back(gr);
      gnew.push_back(gn);
      im_success_diagnostic.push_back(diagnostic);
    } else {
      failed_gp.push_back(gr);
      im_diagnostic.push_back(diagnostic);
    }
  }
  dist_ave /= ic.size();
  std::cerr << "Matching done. Elapsed time: " << tmr.elapsed() << " s.\n";
  return dist_ave;
}

//-----------------------------------------------------------------------
/// This does image matching with a Ref_img, and adjusts the Rpc
/// coefficients to give a better match between this map projected
/// imagery and the reference imagery.
///
/// This is done in two step. First we do a coarse fit to get the
/// map projected roughly right, and then we do a finer adjustment to
/// give a better fit.
///
/// The first step is the coarse fit. Since an Rpc is usually based on
/// navigation data only, without and ground control points, it can
/// give relatively large errors. We do a coarse shift to the Rpc to
/// reduce the errors enough that we can do a finer fit against the
/// Ref_img.
///
/// We first averages the image and the Ref_img. We average so that we
/// are coarse enough that the given image matching search window size
/// (given by Match_search) will contain errors up to Max_diff meters.
/// For example, with a 1000 m Max_diff, a search window of
/// Match_search, and 5 m Ref_img, we would average by a factor of 14
/// (5 m x 14 factor x 15 search window = 1050 m error, allowing us to
/// find 1000 m differences).
///
/// We use the FeatureDetector Fd to determine where to match, the
/// areas we look for a feature is in each grid square of
/// Feature_grid_space x Feature_grid_space. We then match at these
/// features between the reference and map projected images. We use
/// the supplied GroundMask to mask out areas that we should not do matching
/// on (e.g., water, clouds). The default GroundMask doesn't mask any areas.
///
/// We update the line_offset and sample_offset only of rpc() to
/// reduce the difference between the map projected image and the
/// Ref_img. This functions does *not* fit the other parameters given
/// by rpc().fit_line_numerator and rpc().fit_sample_numerator.
///
/// We repeat the coarse fit until the remaining differences between
/// the map projected data and the Ref_img fall within  Match_search
/// at full resolution.
///
/// We follow this with a fine fit. We again use the FeatureDetector
/// Fd to determine where to match, the areas we look for a feature is
/// in each grid square of Feature_grid_space x Feature_grid_space. We
/// then match at these features between the reference and map
/// projected images.
///
/// We fit for the Rpc parameters given by rpc().fit_line_numerator
/// and rpc().fit_sample_numerator.
///
/// This fine matching step is repeated until the change in average
/// difference from one step to the next is less than a tolerance (5%
/// change).
//-----------------------------------------------------------------------

void RpcImage::fit(const RasterImage& Ref_img,
		   double Max_diff,
		   const GroundMask& M,
		   const FeatureDetector& Fd, 
		   const ImageMatcher& Coarse_im,
		   const ImageMatcher& Fine_im,
		   double Match_search,
		   int Feature_grid_space,
		   double Blunder_threshold,
		   double Chisq_threshold,
		   bool Blunder_detect
		   )
{
  tmr.restart();
  // Keep original MapInfo information. We'll update this as we go
  // along, and then put it back when done.
  boost::shared_ptr<MapInfo> map_info_original(new MapInfo(map_info()));

  // Do coarse fit

  double ref_img_res = std::min(Ref_img.grid_center_line_resolution(),
				Ref_img.grid_center_sample_resolution());
  double dist = Max_diff;
  int max_iter = 10;
  for(int i = 0; i < max_iter && dist > Match_search * ref_img_res; ++i) {
    std::cerr << "Starting iteration " << i + 1 << " of coarse fit. Elapsed time: " << tmr.elapsed() << " s.\n";
    map_info_bounding_update();
    dist = fit_coarse(Ref_img, Fd, M, Coarse_im, dist, Match_search, 
		      Feature_grid_space);
  }

  // Now, do a more full fit.

  double dist_last = 1e10;
  dist = 1e8;
  set_ref_img(Ref_img, *this);
  interest_point(Fd, M, Feature_grid_space);
  for(int i = 0; i < max_iter && dist_last / dist > 1.05; ++i) {
    std::cerr << "Starting iteration " << i + 1 << " of fine fit. Elapsed time: " << tmr.elapsed() << " s.\n";
    dist_last = dist;
    map_info_bounding_update();
    dist = fit_fine(Fine_im, Blunder_threshold, Chisq_threshold, 
		    Blunder_detect);
  }
  std::cerr << "Done with RpcImage::fit. Elapsed time: " << tmr.elapsed() 
	    << " s.\n";
  ImagePointDisplay distimg("dist.img", "hfa", map_info(), GDT_UInt16);
  ImagePointDisplay lineimg("line.img", "hfa", map_info(), GDT_UInt16);
  ImagePointDisplay sampimg("samp.img", "hfa", map_info(), GDT_UInt16);
  std::ofstream ds("dump.txt");
  ds << "# Reference line sample New line sample Diff line Diff Sample Distance(m) Diagnostic\n"
     << "# ------------- ------- -------- ------ --------- ----------- ----------- ----------\n";
  for(std::vector<boost::shared_ptr<GroundCoordinate> >::size_type i = 0; 
      i < reference_image_ground_point().size(); ++i)
    if(blunder().count(i) ==0) {
      ImageCoordinate icref(Ref_img.coordinate(*reference_image_ground_point()[i]));
      ImageCoordinate icnew(Ref_img.coordinate(*new_image_ground_point()[i]));
      int cindex = (int) floor(distance_new_and_reference()[i] / 2.0) + 1;
      if(cindex > 10)
	cindex = 10;
      distimg.image_point(*new_image_ground_point()[i], cindex);
      cindex = (int) floor((icnew.line - icref.line + 10) / 4.0) + 1;
      if(cindex > 10)
	cindex = 10;
      if(cindex < 0)
	cindex = 0;
      lineimg.image_point(*new_image_ground_point()[i], cindex);
      cindex = (int) floor((icnew.sample - icref.sample + 10) / 4.0) + 1;
      if(cindex > 10)
	cindex = 10;
      if(cindex < 0)
	cindex = 0;
      sampimg.image_point(*new_image_ground_point()[i], cindex);
      
      ds << icref.line << " " << icref.sample << " " 
	 << icnew.line << " " << icnew.sample << " "
	 << icnew.line - icref.line << " "
	 << icnew.sample - icref.sample << " "
	 << distance_new_and_reference()[i] << " "
	 << image_match_succesful_diagnostic()[i]
	 << "\n";
    }
  map_info_ = map_info_original;
  number_line_ = map_info_->number_y_pixel();
  number_sample_ = map_info_->number_x_pixel();
}

//-----------------------------------------------------------------------
/// Since an Rpc is usually based on navigation data only, without and 
/// ground control points, it can give relatively large errors. This
/// function does a coarse shift to the Rpc to reduce the errors
/// enough that we can do a finer fit against the Ref_img.
///
/// This function first averages the image and the Ref_img. We average
/// so that we are coarse enough that the given image matching search
/// window size (given by Match_search) will contain errors up to
/// Max_diff meters. For example, with a 1000 m Max_diff, a search
/// window of Match_search, and 5 m Ref_img, we would average by a
/// factor of 14 (5 m x 14 factor x 15 search window = 1050 m error,
/// allowing us to find 1000 m differences).
///
/// We use the FeatureDetector Fd to determine where to match, the
/// areas we look for a feature is in each grid square of
/// Feature_grid_space x Feature_grid_space. We then match at these
/// features between the reference and map projected images.
///
/// We update the line_offset and sample_offset only of rpc() to
/// reduce the difference between the map projected image and the
/// Ref_img. This functions does *not* fit the other parameters given
/// by rpc().fit_line_numerator and rpc().fit_sample_numerator.
///
/// We return the average error in meters found between the Ref_img
/// and Rpc projected image before doing the Rpc update.
//-----------------------------------------------------------------------

double RpcImage::fit_coarse(const RasterImage& Ref_img,
			    const FeatureDetector& Fd,
			    const GroundMask& M,
			    const ImageMatcher& Im,
			    double Max_diff,
			    double Match_search,
			    int Feature_grid_space)
{
  double ref_img_res = std::min(Ref_img.grid_center_line_resolution(),
				Ref_img.grid_center_sample_resolution());
  int average_factor = (int) ceil(Max_diff / (ref_img_res * Match_search));
  if(average_factor < 2)
    average_factor = 2;		// No point in doing a coarse fit at
				// full resolution.
  std::cerr << "Average factor: " << average_factor << "\n";
  RasterAveraged ref_img_avg(const_pointer(Ref_img), 
			     average_factor, average_factor);
  std::cerr << "Ref average done. Elapsed time: " << tmr.elapsed() << " s.\n";
  RpcImage new_img(raw_img_, *rpc_, dem_, ref_img_avg.map_info());
  new_img.map_info_bounding_update();
  std::cerr << "New average done. Elapsed time: " << tmr.elapsed() << " s.\n";

// Scale feature grid space to the size we are working with, but
// don't let it get smaller than 10.
  const int min_fgs = 10;
  int fgs = (int) (Feature_grid_space / average_factor);
  if(fgs < min_fgs)
    fgs = min_fgs;

// Don't bother having a fgs that gives us more than 100 points. We
// are only fitting for the constant terms of the RPC, so we don't
// need large number of points like we do for the fine_fit.
  
  const int max_point = 100;
  int npoint = (int) (ceil(ref_img_avg.number_line() / fgs) *
		      ceil(ref_img_avg.number_sample() / fgs));
  if(npoint > max_point)
    fgs = (int) floor(fgs * sqrt(npoint / max_point));

  set_ref_img(ref_img_avg, new_img);
  interest_point(Fd, M, fgs);
  double dist_ave = match(new_img, Im);
  rpc().fit_coarse(gref, ic);
  std::sort(dist.begin(), dist.end());
  std::cerr << "Have " << dist.size() << " good points\n";
  std::cerr << "Average diff: " << dist_ave << " m\n";
  std::cerr << "Median diff:  " << dist[dist.size() / 2] << " m\n";
  std::cerr << "90% <= :      " << dist[dist.size() * 9 / 10] << " m\n";
  std::cerr << "Minimum diff: " << dist.front() << " m\n";
  std::cerr << "Maximum diff: " << dist.back() << " m\n";
  std::cerr << "Fit done. Elapsed time: " << tmr.elapsed() << " s.\n";
  return dist_ave;
}

//-----------------------------------------------------------------------
/// This does an improvement to the Rpc so we more closely match the
/// Ref_img.
///
/// We use the FeatureDetector Fd to determine where to match, the
/// areas we look for a feature is in each grid square of
/// Feature_grid_space x Feature_grid_space. We then match at these
/// features between the reference and map projected images.
///
/// We fit for the Rpc parameters given by rpc().fit_line_numerator
/// and rpc().fit_sample_numerator.
///
/// We return the average error in meters found between the Ref_img
/// and Rpc projected image before doing the Rpc update.
//-----------------------------------------------------------------------

double RpcImage::fit_fine(const ImageMatcher& Im,
			  double Blunder_threshold,
			  double Chisq_threshold,
			  bool Blunder_detect
			  )
{
  match(*this, Im);
  rpc().fit(gref, ic, lsigma, ssigma, blunder_, Blunder_threshold, 
	    Chisq_threshold, Blunder_detect);
  std::cerr << "Removed " << blunder().size() << " blunders\n";
  std::vector<double> dist_no_blunder;
  double dist_ave = 0;
  for(int i = 0; i < (int) dist.size(); ++i)
    if(blunder().count(i) ==0) {
      dist_no_blunder.push_back(dist[i]);
      dist_ave += dist[i];
    }
  dist_ave /= dist_no_blunder.size();
  std::sort(dist_no_blunder.begin(), dist_no_blunder.end());
  std::cerr << "Have " << dist_no_blunder.size() << " good points\n";
  std::cerr << "Average diff: " << dist_ave << " m\n";
  std::cerr << "Median diff:  " 
	    << dist_no_blunder[dist_no_blunder.size() / 2] << " m\n";
  std::cerr << "90% <= :      " 
	    << dist_no_blunder[dist_no_blunder.size() * 9 / 10] << " m\n";
  std::cerr << "Minimum diff: " << dist_no_blunder.front() << " m\n";
  std::cerr << "Maximum diff: " << dist_no_blunder.back() << " m\n";
  std::cerr << "Rpc: " << rpc() << "\n";
  return dist_ave;
}

//-----------------------------------------------------------------------
/// Create a MapInfo that completely covers projection of the raw_image
/// to the surface using the current Rpc values. This used the same
/// CoordinateConverter, pixel_x_resolution, and pixel_y_resolution as
/// the current map_info() - we just change the corners of the MapInfo
/// to contain the data. Note that this just calculates the MapInfo, it
/// doesn't actually set the map_info() to this new value. For that,
/// use map_info_bounding_update.
//-----------------------------------------------------------------------

MapInfo RpcImage::map_info_bounding() const
{
//-----------------------------------------------------------------------
// Go around the edge of the image to find the maximum and minimum
// line and sample needed in the current MapInfo to contain the image.
//-----------------------------------------------------------------------
  double line_min = 1e20;
  double line_max = -1e20;
  double sample_min = 1e20;
  double sample_max = -1e20;
  // Top and bottom line
  for(int i = 0; i < img_avg_->number_sample(); ++i) {
    try {
      ImageCoordinate ic = coordinate(*rpc().ground_coordinate(
      ImageCoordinate(0 * line_avg_fact_, i * sample_avg_fact_),
      dem()));
      line_min = std::min(line_min, ic.line);
      line_max = std::max(line_max, ic.line);
      sample_min = std::min(sample_min, ic.sample);
      sample_max = std::max(sample_max, ic.sample);
    } catch(const ConvergenceFailure&) {
      // Ignore ConvergenceFailure problems, just skip that point.
    }
    try {
      ImageCoordinate ic = coordinate(*rpc().ground_coordinate(
       ImageCoordinate(img_avg_->number_line() * line_avg_fact_, 
		       i * sample_avg_fact_), dem()));
      line_min = std::min(line_min, ic.line);
      line_max = std::max(line_max, ic.line);
      sample_min = std::min(sample_min, ic.sample);
      sample_max = std::max(sample_max, ic.sample);
    } catch(const ConvergenceFailure&) {
      // Ignore ConvergenceFailure problems, just skip that point.
    }
  }
  // Left and right side.
  for(int i = 0; i < img_avg_->number_line(); ++i) {
    try {
      ImageCoordinate ic = coordinate(*rpc().ground_coordinate(
      ImageCoordinate(i * line_avg_fact_, 0 * sample_avg_fact_),
      dem()));
      line_min = std::min(line_min, ic.line);
      line_max = std::max(line_max, ic.line);
      sample_min = std::min(sample_min, ic.sample);
      sample_max = std::max(sample_max, ic.sample);
    } catch(const ConvergenceFailure&) {
      // Ignore ConvergenceFailure problems, just skip that point.
    }
    try {
      ImageCoordinate ic = coordinate(*rpc().ground_coordinate(
       ImageCoordinate(i * line_avg_fact_,
		       img_avg_->number_sample() * sample_avg_fact_),
       dem()));
      line_min = std::min(line_min, ic.line);
      line_max = std::max(line_max, ic.line);
      sample_min = std::min(sample_min, ic.sample);
      sample_max = std::max(sample_max, ic.sample);
    } catch(const ConvergenceFailure&) {
      // Ignore ConvergenceFailure problems, just skip that point.
    }
  }
  line_min = floor(line_min) - 0.5;
  sample_min = floor(sample_min) - 0.5;
  line_max = ceil(line_max) + 0.5;
  sample_max = ceil(sample_max) + 0.5;
  return map_info().subset((int) floor(sample_min), (int) floor(line_min),
			   (int) (ceil(sample_max) - floor(sample_min)),
			   (int) (ceil(line_max) - floor(line_min)));
}

//-----------------------------------------------------------------------
/// This calls map_info_bounding(), and then sets the map_info() for
/// this object to the results of that call. 
//-----------------------------------------------------------------------

void RpcImage::map_info_bounding_update()
{
  map_info_.reset(new MapInfo(map_info_bounding()));
  number_line_ = map_info_->number_y_pixel();
  number_sample_ = map_info_->number_x_pixel();
}
