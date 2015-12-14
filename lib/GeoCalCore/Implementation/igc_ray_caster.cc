#include "igc_ray_caster.h"
#include "simple_dem.h"
#include "ecr.h"
using namespace GeoCal;
using namespace blitz;

inline double sqr(double x) { return x * x; }
//-----------------------------------------------------------------------
/// Constructor. You can pass the starting line to use and the number
/// of lines to process, the default is to do the full range covered
/// by the Igc. The resolution is the desired accuracy, we use this
/// both to figure out the number of subpixels to use and the accuracy
/// that we do the intersection with the DEM. The Max_height should be
/// larger than the greatest height we will encounter in the Dem
/// belonging to the Igc.
///
/// For larger cameras, it might be more convenient to pass in start
/// sample and number of samples to process, the default is to do the
/// full camera.
//-----------------------------------------------------------------------

IgcRayCaster::IgcRayCaster
(const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Start_line,
 int Number_line,
 int Number_integration_step, 
 double Resolution,
 double Max_height,
 int Start_sample,
 int Number_sample
)
  : igc(Igc),
    start_position_(Start_line),
    npos_(Number_line > 0 ? Number_line : Igc->number_line() - Start_line),
    ind(-1),
    nintegration_step(Number_integration_step),
    start_sample_(Start_sample),
    number_sample_(Number_sample > 0 ? Number_sample : 
		   Igc->number_sample() - Start_sample),
    resolution(Resolution),
    max_height(Max_height)
{
  // Determine if we are using a forward or aftward direction
  SimpleDem d;
  boost::shared_ptr<GroundCoordinate> gc1 = 
    igc->ground_coordinate_dem(ImageCoordinate(0,0), d);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    igc->ground_coordinate_dem(ImageCoordinate(1,0), d);
  boost::shared_ptr<CartesianFixed> p1 =
    igc->cf_look_vector_pos(ImageCoordinate(0,0));
  is_forward = (distance(*p1, *gc2) > distance(*p1, *gc1));

  // Determine number of subpixel in line and sample direction. Note
  // that we are interested in the *camera* footprint here, not the
  // spacing in the acquired image. In general, these aren't the same
  // thing. For example, MISR has a pixel spacing of 275 meter, but
  // the footprint can be as much as 750 meter on the
  // ground. So we use the footprint_resolution here, not
  // resolution_meter (which gives the spacing between locations in
  // the igc raster image).
  double line_res, samp_res;
  igc->footprint_resolution(0, 0, line_res, samp_res);
  nsub_line = (int) ceil(line_res / Resolution);
  nsub_sample = (int) ceil(samp_res / Resolution);
  result_cache.resize(1, number_sample(), nsub_line, nsub_sample,
		      nintegration_step,3);
}

//-----------------------------------------------------------------------
/// Do ray casting for the next position.
//-----------------------------------------------------------------------

blitz::Array<double, 6> IgcRayCaster::next_position() 
{
  if(last_position())
    throw Exception("next_position called when we have already reached the last position");
  ++ind;
  Array<double, 7> cf_lv = 
    igc->cf_look_vector_arr(current_position(), start_sample(), 1, 
			    number_sample(),
			    nsub_line, nsub_sample, nintegration_step);
  Array<double, 4> dist(result_cache.shape()[1], result_cache.shape()[2],
			result_cache.shape()[3], result_cache.shape()[4]);
  if(ind != 0)
    for(int i1 = 0; i1 < result_cache.shape()[1]; ++i1)
      for(int i2 = 0; i2 < result_cache.shape()[2]; ++i2)
	for(int i3 = 0; i3 < result_cache.shape()[3]; ++i3)
	  for(int i4 = 0; i4 < result_cache.shape()[4]; ++i4)
	    dist(i1, i2, i3, i4) = 
	      sqrt(sqr(result_cache(0,i1,i2,i3,i4, 0) -
		       cf_lv(0,i1,i2,i3,i4,0,0)) +
		   sqr(result_cache(0,i1,i2,i3,i4, 1) -
		       cf_lv(0,i1,i2,i3,i4,0,1)) +
		   sqr(result_cache(0,i1,i2,i3,i4, 2) -
		       cf_lv(0,i1,i2,i3,i4,0,2)));
  // Do this with subline as outer loop. This allows us to speed up
  // the first position, where we can get away with doing a single
  // subline rather than a full line before starting to use our
  // initial distance optimization.
  int i2_last;
  for(int i2_ind = 0; i2_ind < result_cache.shape()[2]; ++i2_ind) {
    // We do sub-line sample in the order that we are doing the ray casting.
    int i2 = (is_forward ? i2_ind : result_cache.shape()[2] - 1 - i2_ind);
    for(int i1 = 0; i1 < result_cache.shape()[1]; ++i1)
      for(int i3 = 0; i3 < result_cache.shape()[3]; ++i3)
	for(int i4 = 0; i4 < result_cache.shape()[4]; ++i4) {
	  Ecr cf(cf_lv(0,i1,i2,i3,i4,0,0), cf_lv(0,i1,i2,i3,i4,0,1),
		 cf_lv(0,i1,i2,i3,i4,0,2));
	  CartesianFixedLookVector lv(cf_lv(0,i1,i2,i3,i4,1,0), 
				      cf_lv(0,i1,i2,i3,i4,1,1),
				      cf_lv(0,i1,i2,i3,i4,1,2));
	  boost::shared_ptr<CartesianFixed> pt;
	  if(ind != 0) {
	    double start_dist = dist(i1, i2, i3, i4);
	    if(i1-1 >= 0)
	      start_dist = std::min(start_dist, dist(i1-1, i2, i3, i4));
	    if(i1+1 < result_cache.shape()[1])
	      start_dist = std::min(start_dist, dist(i1+1, i2, i3, i4));
	    pt = igc->dem().intersect_start_length(cf, lv, resolution, 
						   start_dist);
	  } else {
	    if(i2_ind == 0)
	      // First subline, we need to start at max height
	      pt = igc->dem().intersect(cf, lv, resolution, max_height);
	    else {
	      double start_dist = dist(i1, i2_last, i3, i4);
	      if(i1-1 >= 0)
		start_dist = std::min(start_dist, dist(i1-1, i2_last, i3, i4));
	      if(i1+1 < result_cache.shape()[1])
		start_dist = std::min(start_dist, dist(i1+1, i2_last, i3, i4));
	      pt = igc->dem().intersect_start_length(cf, lv, resolution, 
						     start_dist);
	    }
	    // Store distance for use in next subline.
	    dist(i1, i2, i3, i4) =
	      sqrt(sqr(pt->position[0] - cf.position[0]) +
		   sqr(pt->position[1] - cf.position[1]) +
		   sqr(pt->position[2] - cf.position[2]));
	  }
	  result_cache(0,i1,i2,i3,i4, 0) = pt->position[0];
	  result_cache(0,i1,i2,i3,i4, 1) = pt->position[1];
	  result_cache(0,i1,i2,i3,i4, 2) = pt->position[2];
	}
    i2_last = i2;
  }
  return result_cache;
}


