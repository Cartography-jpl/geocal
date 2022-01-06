#ifndef IPI_H
#define IPI_H
#include "printable.h"
#include "geocal_time.h"
#include "ground_coordinate.h"
#include "orbit.h"
#include "camera.h"
#include "time_table.h"
#include "refraction.h"
#include <iostream>

namespace GeoCal {
/****************************************************************//**
  This performs an image point intersection (IPI), finding the time or
  image coordinates that a given ground point is seen by a camera.

  This solves the collinearity equation in order to find the time and
  frame coordinates that a ground point is seen. Note however that
  the collinearity equation can give "false" solutions, because it is
  perfectly happy to find points seen by looking straight through the
  earth (since it does not account for an obstructed view). 

  These "false" solutions have nothing to do with the obscuration
  "looking being a mountain" sorts of problems. This actually is a
  different problem. When you run the IPI you can get solutions that
  are on the other side of earth, so looking at when a ground point in
  the United State is seen you can get a solution that returns a time
  for when the spacecraft is flying over China. This is *not* the "behind a
  mountain" issue, but the fact that a line of sight intersects a
  sphere twice on opposite sides of the Earth. The "false solutions"
  tries to throw these "other side of the earth" solutions out. But
  the IPI does nothing for simple obscuration of the "behind a
  mountain" sorts. The IPI *assumes* that the point it is given is not
  obscured by terrain. If you need to calculate obscuration, you
  should use a full ray caster (e.g., IgcRayCaster).

  We also get "false" solutions if the camera model diverges (i.e, we
  pass in a look vector parallel to the camera CCD plane, so the
  point is not seen by the camera). This is because the root finder
  doesn't actually look for a zero, but rather a change in the sign
  of the equation. When the camera model diverges, we go from
  negative infinity to positive infinity. We recognize these "false"
  solutions by comparing the collinearity equation results to a user
  supplied tolerance.

  We also allow a "extended" camera (i.e., frame coordinate outside of
  the range of the camera). This is useful for edge of images etc, so
  we don't have abrupt transitions. But we can get false solutions
  with really large coordinate - e.g., imagine two lines at a slight
  angle to each other than intersect at some point outside of the
  image. We pass in a "max_frame_extend" value to limit how far
  outside the camera we look for a solution. This can be a large value
  to skip this limit, or set to 0 to not allow any extension at all.

  We find all the solutions to the collinearity equation, in the range
  Tmin to Tmax. The solutions found must be seperated by a time
  larger then Root_min_separation.

  We then inspect the list of roots, and reject those that are
  "false" solutions. If 1 solution is left, we return it with the
  success flag set to true. Otherwise, we return with the success
  flag set to false.

  It is very common to call the IPI with a ground point near the last
  call. As a performance improvement, we remember the time results
  from the previous IPI calculation. We first look in a local time
  range around the previous time result. If no solution is found, we
  then look over the entire time range.
*******************************************************************/

class Ipi : public Printable<Ipi> {
public:
  Ipi(const boost::shared_ptr<Orbit>& Orb, const 
      boost::shared_ptr<Camera>& Cam,
      int Band,
      Time Tmin, Time Tmax, 
      const boost::shared_ptr<TimeTable>& Tt = boost::shared_ptr<TimeTable>(),
      const boost::shared_ptr<Refraction>&
      Ref = boost::shared_ptr<Refraction>(),
      double Local_time_window_size = 5.0,
      double Root_min_separation = 30.0, 
      double Time_tolerance = 1e-6, double Max_frame_extend=1000);
  virtual ~Ipi() {}
  void image_coordinate(const GroundCoordinate& Gp, ImageCoordinate& Ic,
			bool& Success) const;
  void image_coordinate_with_derivative(const GroundCoordinate& Gp, ImageCoordinateWithDerivative& Ic,
			bool& Success) const;
  void image_coordinate_with_derivative(const GroundCoordinate& Gp, 
				const boost::array<AutoDerivative<double>, 3>&
				Gp_with_der,
				ImageCoordinateWithDerivative& Ic,
				bool& Success) const;
  void image_coordinate_extended(const GroundCoordinate& Gp, 
				 ImageCoordinate& Ic,
				 bool& Success) const;
  void image_coordinate_with_derivative_extended(const GroundCoordinate& Gp, 
				 ImageCoordinateWithDerivative& Ic,
				 bool& Success) const;
  void image_coordinate_with_derivative_extended(const GroundCoordinate& Gp, 
				 const boost::array<AutoDerivative<double>, 3>&
				 Gp_with_der,
				 ImageCoordinateWithDerivative& Ic,
				 bool& Success) const;
  double resolution_meter() const;
  void time(const GroundCoordinate& Gp, Time& Tres, FrameCoordinate& Fres,
	    bool& Success) const;
  void time_with_derivative(const GroundCoordinate& Gp, 
			    TimeWithDerivative& Tres, 
			    FrameCoordinateWithDerivative& Fres,
			    bool& Success) const;
  void time_with_derivative(const GroundCoordinate& Gp, 
		    const boost::array<AutoDerivative<double>, 3>& Gp_with_der,
		    TimeWithDerivative& Tres, 
		    FrameCoordinateWithDerivative& Fres,
		    bool& Success) const;
  void print(std::ostream& Os) const;
  std::vector<boost::shared_ptr<GroundCoordinate> > footprint(const Dem& D) 
    const;
  
//-----------------------------------------------------------------------
/// Orbit that we are using.
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<Orbit>& orbit() const {return orb;}
  void orbit(const boost::shared_ptr<Orbit>& Orb) {orb = Orb;}

//-----------------------------------------------------------------------
/// Camera that we are using.
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<Camera>& camera() const {return cam;}
  void camera(const boost::shared_ptr<Camera>& Cam) {cam = Cam;}

//-----------------------------------------------------------------------
/// TimeTable that we are using.
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<TimeTable>& time_table() const {return tt;}
  void time_table(const boost::shared_ptr<TimeTable>& Tt) {tt = Tt;}

//-----------------------------------------------------------------------
/// Refraction that we are using.
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<Refraction>& refraction() const {return ref;}
  void refraction(const boost::shared_ptr<Refraction>& Ref) {ref = Ref;}
  
//-----------------------------------------------------------------------
/// Band that we are using.
//-----------------------------------------------------------------------

  int band() const {return band_; }

//-----------------------------------------------------------------------
/// Set band that we are using.
//-----------------------------------------------------------------------

  void band(int B) { band_ = B; }

//-----------------------------------------------------------------------
/// Minimum time that we are using.
//-----------------------------------------------------------------------

  Time min_time() const {return min_time_; }

//-----------------------------------------------------------------------
/// Maximum time that we are using.
//-----------------------------------------------------------------------

  Time max_time() const {return max_time_; }

//-----------------------------------------------------------------------
/// How large of a window to search around last solution, in seconds.
//-----------------------------------------------------------------------

  double local_time_window_size() const {return local_time_window_size_;}

//-----------------------------------------------------------------------
/// How far apart in seconds we look for solutions to the collinearity
/// equation.
//-----------------------------------------------------------------------

  double root_min_separation() const {return root_min_separation_;}

//-----------------------------------------------------------------------
/// How accurate we find the time.
//-----------------------------------------------------------------------

  double time_tolerance() const {return time_tolerance_;}

//-----------------------------------------------------------------------
/// Maximum amount we allow a Ipi solution to be outside the edged of
/// the camera. See class description for discussion of this.
//-----------------------------------------------------------------------

  double max_frame_extend() const { return max_frame_extend_; }
private:
  boost::shared_ptr<Orbit> orb;
  boost::shared_ptr<Camera> cam;
  int band_;			 ///< Camera band to use.
  boost::shared_ptr<TimeTable> tt;
  boost::shared_ptr<Refraction> ref;
  Time min_time_;		 ///< Minimum time we search around
  Time max_time_;		 ///< Maximum time we search around
  mutable Time last_time;	 ///< Time from last point we did IPI
				 ///for.
  double local_time_window_size_; ///< How large of a window to search
				 ///around last solution, in seconds
  double root_min_separation_;	 ///< How far apart in seconds we look
				 ///for solutions to the collinearity
				 ///equation.
  double time_tolerance_;	 ///< How accurate we find the time.
  double max_frame_extend_;	 // Maximum FrameCoordinate outside of
				 // the camera that we consider 'ok'.

//-----------------------------------------------------------------------
/// Initial guess at minimum time, based on last_time.
//-----------------------------------------------------------------------

  Time min_time_guess() const 
  {
    return std::max(min_time_, last_time - local_time_window_size_ / 2);
  }
      
//-----------------------------------------------------------------------
/// Initial guess at maximum time, based on last_time.
//-----------------------------------------------------------------------

  Time max_time_guess() const 
  {
    // The time_tolerance / 2 is because we usually use a range 
    // min <= x < max, but root_list uses min <= x <= max. We move off
    // a small amount from the end point so we don't run into any
    // problems with it.
    return std::min(max_time_ - time_tolerance_ / 2, 
		    last_time + local_time_window_size_ / 2);
  }

  Ipi() : max_frame_extend_(1e19) {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(Ipi);
GEOCAL_CLASS_VERSION(Ipi, 2);
#endif

