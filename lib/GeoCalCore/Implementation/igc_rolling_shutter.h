#ifndef IGC_ROLLING_SHUTTER_H
#define IGC_ROLLING_SHUTTER_H
#include "image_ground_connection.h"
#include "orbit.h"
#include "time_table.h"
#include "refraction.h"
#include "camera.h"

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by
  OrbitData and a Camera. This is similar to
  OrbitDataImageGroundConnection, however this is intended for use
  with a rolling shutter camera. 

  A rolling shutter is common with CMOS sensors. Rather than 
  recording an image at a single instance, the image is recorded by
  scanning over the camera rapidly in either the line or sample
  direction.

  If the camera is moving while the image is taken (e.g., it is on an
  aircraft), then significant differences can occur between modeling
  this as close a frame camera vs. modeling the rolling shutter.

  We currently only support the line roll direction, although we 
  have some interface support for sample roll direction (just in case 
  we need to expand this in the future).
*******************************************************************/

class IgcRollingShutter : public ImageGroundConnection, 
			  public WithParameterNested {
public:
  enum RollDirection { ROLL_LINE_DIRECTION , ROLL_SAMPLE_DIRECTION };

//-----------------------------------------------------------------------
/// Constructor that takes a Orbit and a Time_table. If the roll
/// direction is in the line direction we assume that all the samples
/// for a particular line are acquired at the same time, and if the
/// roll direction is in the sample direction we assume that all the
/// lines for a particular sample are acquired at the same time.
///
/// You can optionally include a approximate refraction
/// correction, the default is not to.
//-----------------------------------------------------------------------

  IgcRollingShutter(const boost::shared_ptr<Orbit>& Orb,
		    const boost::shared_ptr<TimeTable>& Time_table,
		    const boost::shared_ptr<Camera>& Cam, 
		    const boost::shared_ptr<Dem>& D,
		    const boost::shared_ptr<RasterImage>& Img,
		    RollDirection Roll_direction = ROLL_LINE_DIRECTION,
		    const std::string Title = "",
		    const boost::shared_ptr<Refraction>&
		    Ref = boost::shared_ptr<Refraction>(),
		    double Resolution=30, int Band=0, 
		    double Max_height=9000)
  {
    initialize(Orb, Time_table, Cam, D, Img, Roll_direction, Title, Ref, 
	       Resolution, Band, Max_height);
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~IgcRollingShutter() {}

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  virtual blitz::Array<double, 7> 
  cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp,
		     int nsubpixel_line = 1, 
		     int nsubpixel_sample = 1,
		     int nintegration_step = 1) const;
  virtual void footprint_resolution(int Line, int Sample, 
				    double &Line_resolution_meter, 
				    double &Sample_resolution_meter);
  virtual bool has_time() const { return true; }
  virtual Time pixel_time(const ImageCoordinate& Ic) const
  { Time t; FrameCoordinate fc; time_table_->time(Ic, t, fc);
    return t; }
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Orbit that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Orbit>& orbit() const 
  { return orbit_; }

//-----------------------------------------------------------------------
/// Set orbit.
//-----------------------------------------------------------------------

  void orbit(const boost::shared_ptr<Orbit>& Orb) 
  { orbit_ = Orb; }

//-----------------------------------------------------------------------
/// Time table that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<TimeTable>& time_table() const 
  { return time_table_; }


//-----------------------------------------------------------------------
/// Set time table.
//-----------------------------------------------------------------------

  void time_table(const boost::shared_ptr<TimeTable>& Tt) 
  { time_table_ = Tt; }

//-----------------------------------------------------------------------
/// Camera that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Camera>& camera() const {return cam; }

//-----------------------------------------------------------------------
/// Set Camera that we are using
//-----------------------------------------------------------------------

  void camera(const boost::shared_ptr<Camera>& C) { cam = C; }

//-----------------------------------------------------------------------
/// Resolution in meters that we examine Dem out. This affects how
/// long ground_coordinate takes to figure out. It should be about the
/// resolution of the Dem
//-----------------------------------------------------------------------

  double resolution() const { return res; }

//-----------------------------------------------------------------------
/// Set resolution in meters that we examine Dem out. This affects how
/// long ground_coordinate takes to figure out. It should be about the
/// resolution of the Dem
//-----------------------------------------------------------------------

  void resolution(double R) { res = R; }

//-----------------------------------------------------------------------
/// Camera band we are using.
//-----------------------------------------------------------------------

  int band() const { return b; }

//-----------------------------------------------------------------------
/// Set camera band we are using.
//-----------------------------------------------------------------------

  void band(int B) { b = B; }

//-----------------------------------------------------------------------
/// Maximum height that we expect to see in the Dem.
//-----------------------------------------------------------------------

  double max_height() const {return max_h;}

//-----------------------------------------------------------------------
/// Set Maximum height that we expect to see in the Dem.
//-----------------------------------------------------------------------

  void max_height(double Max_h) { max_h = Max_h;}

//-----------------------------------------------------------------------
/// Refraction object we are using. May be null if we aren't including
/// refraction. 
//-----------------------------------------------------------------------

  boost::shared_ptr<Refraction> refraction() const {return refraction_;}

//-----------------------------------------------------------------------
/// Set refraction object we are using. May be null if we aren't including
/// refraction. 
//-----------------------------------------------------------------------

  void refraction(const boost::shared_ptr<Refraction>& Ref) 
  {refraction_ = Ref;}

//-----------------------------------------------------------------------
/// Roll direction that rolling shutter moves.
//-----------------------------------------------------------------------

  RollDirection roll_direction() const { return roll_direction_; }

  virtual blitz::Array<double, 1> parameter() const
  { return WithParameterNested::parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { WithParameterNested::parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return WithParameterNested::parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { WithParameterNested::parameter_with_derivative(Parm);}
protected:

//-----------------------------------------------------------------------
/// Default constructor. Derived classes should make sure to call init.
//-----------------------------------------------------------------------

  IgcRollingShutter() {}

  void initialize(const boost::shared_ptr<Orbit>& Orb,
	    const boost::shared_ptr<TimeTable>& Time_table,
	    const boost::shared_ptr<Camera>& Cam, 
	    const boost::shared_ptr<Dem>& D,
	    const boost::shared_ptr<RasterImage>& Img,
	    RollDirection Roll_direction = ROLL_LINE_DIRECTION,
	    const std::string Title = "",
	    const boost::shared_ptr<Refraction>&
	    Ref = boost::shared_ptr<Refraction>(),
	    double Resolution=30, int Band=0, 
	    double Max_height=9000);
private:
  boost::shared_ptr<Orbit> orbit_;
  boost::shared_ptr<TimeTable> time_table_;
  boost::shared_ptr<Camera> cam;
  boost::shared_ptr<Refraction> refraction_;
  RollDirection roll_direction_;
  double res;
  int b;
  double max_h;
  double time_tolerance_;
};
}
#endif

