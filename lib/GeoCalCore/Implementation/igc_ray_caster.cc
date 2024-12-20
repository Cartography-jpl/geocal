#include "igc_ray_caster.h"
#include "simple_dem.h"
#include "planet_coordinate.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcRayCaster::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void IgcRayCaster::load(Archive & ar, const unsigned int version)
{
  result_cache.resize(number_line_framelet(),
		      number_sample(), nsub_line, nsub_sample,
		      nintegration_step,(include_path_distance_ ? 4 : 3));
}

template<class Archive>
void IgcRayCaster::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RayCaster)
    & GEOCAL_NVP(igc)
    & GEOCAL_NVP_(number_line_framelet)
    & GEOCAL_NVP_(start_position) & GEOCAL_NVP_(npos) & GEOCAL_NVP(ind)
    & GEOCAL_NVP(nintegration_step) & GEOCAL_NVP(nsub_line)
    & GEOCAL_NVP(nsub_sample) & GEOCAL_NVP_(start_sample)
    & GEOCAL_NVP_(number_sample) & GEOCAL_NVP(is_forward)
    & GEOCAL_NVP_(include_path_distance) & GEOCAL_NVP(resolution)
    & GEOCAL_NVP(max_height);
  boost::serialization::split_member(ar, *this, version);
}

GEOCAL_IMPLEMENT(IgcRayCaster);
#endif

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
/// We calculate the number of subpixels to use in the line and sample
/// direction from the Resolution argument. This sometimes leaves
/// holes, in particular if we are working with data where the pixel
/// size varies on the ground. You can reset these values after the
/// fact if desired - just set nsub_line and nsub_sample
///
/// Note that in some cases you may want to control the exactly number
/// of subpixels. The easiest way to do this is to just first call
/// this constructor and let it figure out what it thinks the number
/// of subpixels should be, and then manually changing this (e.g.,
/// call number_sub_line and number_sub_sample to set this).
///
/// For larger images, it might be more convenient to pass in start
/// sample and number of samples to process, the default is to do the
/// full camera.
///
/// If you have Include_path_distance set to true, then we return an
/// extra entry in the result array that is the path distance.
//-----------------------------------------------------------------------

IgcRayCaster::IgcRayCaster
(const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Number_line_framelet,
 int Start_line,
 int Number_line,
 int Number_integration_step, 
 double Resolution,
 double Max_height,
 int Start_sample,
 int Number_sample,
 bool Include_path_distance
)
: RayCaster(Igc->cartesian_fixed()),
  igc(Igc),
  start_position_(Start_line),
  npos_(Number_line > 0 ? Number_line : Igc->number_line() - Start_line),
  ind(-Number_line_framelet),
  nintegration_step(Number_integration_step),
  start_sample_(Start_sample),
  number_sample_(Number_sample > 0 ? Number_sample : 
		 Igc->number_sample() - Start_sample),
  number_line_framelet_(Number_line_framelet),
  include_path_distance_(Include_path_distance),
  resolution(Resolution),
  max_height(Max_height)
{
  // We only work with integral numbers of framelets. Can perhaps
  // relax this at some point, but for now use this as a constraint
  if(start_position_ % number_line_framelet_ != 0)
    throw Exception("start_position needs to be an even multiple of number_line_framelet");
  if(npos_ % number_line_framelet_ != 0)
    throw Exception("npos needs to be an even multiple of number_line_framelet");
  // Determine if we are using a forward or aftward direction
  boost::shared_ptr<Dem> d;
  if(igc->naif_code() == Ecr::EARTH_NAIF_CODE)
    d = boost::make_shared<SimpleDem>();
  else
    d = boost::make_shared<PlanetSimpleDem>(0, igc->naif_code());
  boost::shared_ptr<GroundCoordinate> gc1 = 
    igc->ground_coordinate_dem(ImageCoordinate(0,0), *d);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    igc->ground_coordinate_dem(ImageCoordinate(1,0), *d);
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
  // We shrink the resolution a small amount. Empirically we end up
  // with holes in the data in some cases if we use the resolution
  nsub_line = (int) ceil(line_res / (0.7 * Resolution));
  nsub_sample = (int) ceil(samp_res / (0.7 * Resolution));
  result_cache.resize(number_line_framelet(), number_sample(),
		      nsub_line, nsub_sample,
		      nintegration_step,(include_path_distance_ ? 4 : 3));
}

//-----------------------------------------------------------------------
/// Do ray casting for the next position.
//-----------------------------------------------------------------------

blitz::Array<double, 6> IgcRayCaster::next_position() 
{
  if(last_position())
    throw Exception("next_position called when we have already reached the last position");
  ind += number_line_framelet();
  Array<double, 7> cf_lv = 
    igc->cf_look_vector_arr(current_position(), start_sample(),
			    number_line_framelet(), 
			    number_sample(),
			    nsub_line, nsub_sample, nintegration_step);
  Array<double, 5> dist(result_cache.shape()[0],
			result_cache.shape()[1], result_cache.shape()[2],
			result_cache.shape()[3], result_cache.shape()[4]);
  if(ind != 0)
    for(int i0 = 0; i0 < result_cache.shape()[0]; ++i0)
      for(int i1 = 0; i1 < result_cache.shape()[1]; ++i1)
	for(int i2 = 0; i2 < result_cache.shape()[2]; ++i2)
	  for(int i3 = 0; i3 < result_cache.shape()[3]; ++i3)
	    for(int i4 = 0; i4 < result_cache.shape()[4]; ++i4)
	      dist(i0, i1, i2, i3, i4) =
		sqrt(sqr(result_cache(i0,i1,i2,i3,i4, 0) -
			 cf_lv(i0,i1,i2,i3,i4,0,0)) +
		     sqr(result_cache(i0,i1,i2,i3,i4, 1) -
			 cf_lv(i0,i1,i2,i3,i4,0,1)) +
		     sqr(result_cache(i0,i1,i2,i3,i4, 2) -
			 cf_lv(i0,i1,i2,i3,i4,0,2)));
  // Do this with subline as outer loop. This allows us to speed up
  // the first position, where we can get away with doing a single
  // subline rather than a full line before starting to use our
  // initial distance optimization.
  int i2_last=-1;
  for(int i2_ind = 0; i2_ind < result_cache.shape()[2]; ++i2_ind) {
    // We do sub-line sample in the order that we are doing the ray casting.
    int i2 = (is_forward ? i2_ind : result_cache.shape()[2] - 1 - i2_ind);
    for(int i0 = 0; i0 < result_cache.shape()[0]; ++i0)
      for(int i1 = 0; i1 < result_cache.shape()[1]; ++i1)
	for(int i3 = 0; i3 < result_cache.shape()[3]; ++i3)
	  for(int i4 = 0; i4 < result_cache.shape()[4]; ++i4) {
	    boost::array<double, 3> pv;
	    pv[0] = cf_lv(i0,i1,i2,i3,i4,0,0);
	    pv[1] = cf_lv(i0,i1,i2,i3,i4,0,1);
	    pv[2] = cf_lv(i0,i1,i2,i3,i4,0,2);
	    boost::shared_ptr<CartesianFixed> cf_p = cf->create(pv);
	    CartesianFixedLookVector lv(cf_lv(i0,i1,i2,i3,i4,1,0), 
					cf_lv(i0,i1,i2,i3,i4,1,1),
					cf_lv(i0,i1,i2,i3,i4,1,2));
	  boost::shared_ptr<CartesianFixed> pt;
	  if(ind != 0) {
	    double start_dist = dist(i0, i1, i2, i3, i4);
	    if(i1-1 >= 0)
	      start_dist = std::min(start_dist, dist(i0, i1-1, i2, i3, i4));
	    if(i1+1 < result_cache.shape()[1])
	      start_dist = std::min(start_dist, dist(i0, i1+1, i2, i3, i4));
	    pt = igc->dem().intersect_start_length(*cf_p, lv, resolution, 
						   start_dist);
	  } else {
	    if(i2_ind == 0)
	      // First subline, we need to start at max height
	      pt = igc->dem().intersect(*cf_p, lv, resolution, max_height);
	    else {
	      double start_dist = dist(i0, i1, i2_last, i3, i4);
	      if(i1-1 >= 0)
		start_dist = std::min(start_dist, dist(i0, i1-1, i2_last, i3, i4));
	      if(i1+1 < result_cache.shape()[1])
		start_dist = std::min(start_dist, dist(i0, i1+1, i2_last, i3, i4));
	      pt = igc->dem().intersect_start_length(*cf_p, lv, resolution, 
						     start_dist);
	    }
	    // Store distance for use in next subline.
	    dist(i0, i1, i2, i3, i4) =
	      sqrt(sqr(pt->position[0] - cf_p->position[0]) +
		   sqr(pt->position[1] - cf_p->position[1]) +
		   sqr(pt->position[2] - cf_p->position[2]));
	  }
	  result_cache(i0,i1,i2,i3,i4, 0) = pt->position[0];
	  result_cache(i0,i1,i2,i3,i4, 1) = pt->position[1];
	  result_cache(i0,i1,i2,i3,i4, 2) = pt->position[2];
	  if(include_path_distance_)
	    result_cache(i0,i1,i2,i3,i4, 3) = dist(i0, i1, i2, i3, i4);
	}
    i2_last = i2;
  }
  return result_cache;
}


