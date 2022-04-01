#ifndef IGC_RAY_CASTER_H
#define IGC_RAY_CASTER_H
#include "ray_caster.h"
#include "image_ground_connection.h"
#include <boost/utility.hpp>

namespace GeoCal {
/****************************************************************//**
   This class implements a RayCaster by using a
   ImageGroundConnection. 

   We get a significant speed increase by having a good initial guess
   at the starting length of each ray when finding the intersection
   with the surface.

   For a pushbroom camera, we can get a pretty good guess by making
   use of the fact that the rays form the same camera line/sample are
   nearly parallel from on orbit position to the next. For a forward
   pointing camera, the rays from time t + epsilon will be above the
   rays from time t. This means that we can get a good guess at the
   length of the intersection of the ray at time t + epsilon by using
   the distance from the camera position at time t + epsilon & the
   intersection at time t.

   We can improve the guess a bit by using not just one ray. For the
   subpixel (sub_l, sub_s) of camera pixel (cam_l, cam_s), we get the
   guess at the ray length by:

     ray_length = min(distance for pos(t + eps) from intersection at
         (cam_l, cam_s - 1, sub_l, sub_s, t)
         (cam_l, cam_s    , sub_l, sub_s, t)
         (cam_l, cam_s + 1, sub_l, sub_s, t)
         (cam_l, cam_s    , sub_l, sub_s, t + eps)

   There are a few complications:
    1. What do we do for the first line of data, when data from time t
       has not been calculated.
    2. What do we do at the egdes of the camera, when cam_s - 1 or
       cam_s + 1 has not been calculated.

   For (1) we use the length of the ray that intersectes the WGS84 + 
   Max_height where Max_height is greater than any height we
   encounter in the dem. This is guaranteed to give a ray length that
   will be above the surface.

   For(2), we just use the neighbors that are available.

   For aftward cameras, we just progress through the orbit in the
   opposite order, starting with the last orbit position and working
   backwards. 

   We assume that this holds for the ImageGroundConnection, which
   might not be a push broom camera. We may need to generalize this
   class at some point, but for now this is the assumption made.

   We determine if we are forward or aftward by looking at the first
   pixel of the first 2 lines. We do a intersection with the reference
   ellipsoid, and if the point for line 1 is farther away from the
   position and line 0 then we have a forward looking camera,
   otherwise we have an aftward camera.

   Note that this assumes that we continue pointing in the same
   direction. This is *not* the case for something like AirMSPI
   running in sweep mode (where we step through a number of camera
   angles), but for now we just assume that whatever the first
   pointing is that we use is the pointing that will be used for all
   lines. 

   Note that we could relax the assumption that we are always going
   forward or aftward in the future by doing a calculation like we do
   for the initial determination of forward/aftward for each line. We
   could then break the position up into segments one direction of the
   other, going one way through forward and the other through
   aftward. But we have not implemented this yet.

   We break the pixels up into the number of subpixels needed to cover
   the desired Resolution of the results. This determination is done
   for the first line, and we assume it holds for the full
   orbit. Again, we could relax this if desired and calculate this for
   every line. But we don't do that right now.
*******************************************************************/
class IgcRayCaster : public RayCaster, boost::noncopyable {
// We can't copy this because of the result_cache. We could create a
// copy constructor if this becomes an issue.
public:
  IgcRayCaster(const boost::shared_ptr<ImageGroundConnection>& Igc,
	       int Start_line = 0,
	       int Number_line = -1,
	       int Number_integration_step = 2, double Resolution = 100,
	       double Max_height = 10e3,
	       int Start_sample = 0,
	       int Number_sample = -1,
	       bool Include_path_distance=false);
  virtual ~IgcRayCaster() {}
  virtual int start_position() const { return start_position_;}
  virtual int number_position() const { return npos_;}
  virtual bool last_position() const { return ind == npos_ - 1; }
  virtual int current_position() const 
  { if(is_forward)
      return start_position_ + ind;
    else
      return start_position_ + npos_ - 1 - ind;
  }
  virtual blitz::Array<double, 6> next_position();
  virtual void print(std::ostream& Os) const
  {
    Os << "IgcRayCaster\n";
  }
  int start_sample() const {return start_sample_;}
  int number_sample() const {return number_sample_;}
  virtual int shape(int I) const
  { range_check(I, 0, 6); return result_cache.extent(I); }
  
//-----------------------------------------------------------------------
/// Number of subpixels in the line direction we calculate.
//-----------------------------------------------------------------------
  int number_sub_line() const
  { return nsub_line; }

//-----------------------------------------------------------------------
/// Set the number of subpixels in the line direction we calculate.
//-----------------------------------------------------------------------
  void number_sub_line(int v)
  {
    nsub_line = v;
    result_cache.resize(1, number_sample(), nsub_line, nsub_sample,
			nintegration_step,(include_path_distance_ ? 4 : 3));
  }
    
//-----------------------------------------------------------------------
/// Number of subpixels in the sample direction we calculate.
//-----------------------------------------------------------------------
  int number_sub_sample() const
  { return nsub_sample; }

//-----------------------------------------------------------------------
/// Set the number of subpixels in the sample direction we calculate.
//-----------------------------------------------------------------------
  void number_sub_sample(int v)
  {
    nsub_sample = v;
    result_cache.resize(1, number_sample(), nsub_line, nsub_sample,
			nintegration_step,(include_path_distance_ ? 4 : 3));
  }
    
//-----------------------------------------------------------------------
/// Number of integration steps we use.
//-----------------------------------------------------------------------
  int number_integration_step() const
  { return nintegration_step; }

//-----------------------------------------------------------------------
/// Set the number of integration steps we use.
//-----------------------------------------------------------------------
  void number_integration_step(int v)
  {
    nintegration_step = v;
    result_cache.resize(1, number_sample(), nsub_line, nsub_sample,
			nintegration_step,(include_path_distance_ ? 4 : 3));
  }

//-----------------------------------------------------------------------
/// True if we include the path distance in the calcuation
//-----------------------------------------------------------------------
  bool include_path_distance() const
  { return include_path_distance_; }

//-----------------------------------------------------------------------
/// Set the value of the include_path_distance flag.
//-----------------------------------------------------------------------
  void include_path_distance(bool v)
  {
    include_path_distance_ = v;
    result_cache.resize(1, number_sample(), nsub_line, nsub_sample,
			nintegration_step,(include_path_distance_ ? 4 : 3));
  }
  
protected:
  IgcRayCaster() {}
private:
  boost::shared_ptr<ImageGroundConnection> igc;
  int start_position_, npos_, ind, nintegration_step, nsub_line, nsub_sample,
	  start_sample_, number_sample_;
  bool is_forward;
  bool include_path_distance_;
  double resolution, max_height;
  // Results from the last call to next_position. We save this both to 
  // prevent allocating/freeing at every position, and for use in
  // determining the starting point for the next position.
  blitz::Array<double, 6> result_cache;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcRayCaster);
#endif
