#ifndef IGC_ROLLING_SHUTTER_H
#define IGC_ROLLING_SHUTTER_H
#include "image_ground_connection.h"
#include "orbit.h"
#include "time_table.h"
#include "refraction.h"
#include "camera.h"

namespace GeoCal {
  namespace IgcRollingShutterHelper {
    class IcEq;			// Internally used class. We need to
				// declare this because we need to
				// make it a friend class.
    // Helper class that does a fast interpolation of position. See
    // interpolation of position in orbit.cc, this just caches some
    // of the intermediate values to we can rapidly calculate position
    // at different times.
    class PositionInterpolate {
    public:
      PositionInterpolate() {}
      PositionInterpolate(const boost::array<double, 3>& P1,
			  const boost::array<double, 3>& V1,
			  const boost::array<double, 3>& P2,
			  const boost::array<double, 3>& V2,
			  Time Tmin,
			  double Tspace)
	: tmin(Tmin), tspace(Tspace)
      {
	for(int i = 0; i < 3; ++i) {
	  c0[i] = P1[i];
	  c1[i] = V1[i] * tspace;
	  c2[i] = (P2[i] - P1[i]) * 3 - (V2[i] + V1[i] * 2) * tspace;
	  c3[i] = (P2[i] - P1[i]) * (-2) + (V2[i] + V1[i]) * tspace;
	}
      }
      void position(Time T, boost::array<double, 3>& Pres) const
      {
	double t = (T - tmin) / tspace;
	for(int i = 0; i < 3; ++i)
	  Pres[i] = c0[i] + (c1[i] + (c2[i] + c3[i] * t) * t) * t;
      }
      void velocity(Time T, boost::array<double, 3>& Vres) const
      {
	double t = (T - tmin) / tspace;
	for(int i = 0; i < 3; ++i)
	  Vres[i] = (c1[i] + (c2[i] * 2 + c3[i] * (3 * t)) * t) / tspace;
      }
    private:
      boost::array<double, 3> c0, c1, c2, c3;
      Time tmin;
      double tspace;
    };
    // Helper class that does a fast interpolation of quaternion rotations
    class QuaternionInterpolate {
    public:
      QuaternionInterpolate() {}
      QuaternionInterpolate(const boost::math::quaternion<double>& Q1,
			    const boost::math::quaternion<double>& Q2,
			    const Time& Tmin,
			    double Tspace)
	: q1(Q1), q2(Q2), tmin(Tmin), tspace(Tspace) {}
      boost::math::quaternion<double> operator()(const Time& Tm) const
      { return interpolate_quaternion_rotation(q1, q2, Tm - tmin, tspace); }
    private:
      boost::math::quaternion<double> q1, q2;
      Time tmin;
      double tspace;
    };
  }

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

  Note that this class assumes that the orbit data varies smoothly 
  over the time that the rolling shutter operates. We speed up the
  class by taking the orbit data at the start and end of the rolling
  shutter and interpolating. If this is *not* true of the orbit data,
  then there will be significant errors in the calculations done by
  this class.
*******************************************************************/

class IgcRollingShutter : public virtual ImageGroundConnection, 
			  public virtual WithParameterNested,
			  public Observer<Orbit> {
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

  virtual void notify_update(const Orbit& Orb);

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
					    ImageCoordinate& Res,
					    bool& Success) const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  virtual blitz::Array<double, 1> 
  collinearity_residual(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  virtual blitz::Array<double, 2> 
  collinearity_residual_jacobian(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  virtual blitz::Array<double, 7> 
  cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp,
		     int nsubpixel_line = 1, 
		     int nsubpixel_sample = 1,
		     int nintegration_step = 1) const;
  virtual void footprint_resolution(int Line, int Sample, 
				    double &Line_resolution_meter, 
				    double &Sample_resolution_meter) const;
  virtual bool has_time() const { return true; }
  virtual Time pixel_time(const ImageCoordinate& Ic) const
  { Time t; FrameCoordinate fc; time_table_->time(Ic, t, fc);
    return t; }
  virtual void print(std::ostream& Os) const;

  virtual int number_line() const { return cam->number_line(0); }
  virtual int number_sample() const { return cam->number_sample(0); }
  virtual int number_band() const { return cam->number_band(); }

//-----------------------------------------------------------------------
/// Orbit that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Orbit>& orbit() const 
  { return orbit_; }
  void orbit(const boost::shared_ptr<Orbit>& Orb);

//-----------------------------------------------------------------------
/// Time table that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<TimeTable>& time_table() const 
  { return time_table_; }


//-----------------------------------------------------------------------
/// Set time table.
//-----------------------------------------------------------------------

  void time_table(const boost::shared_ptr<TimeTable>& Tt) 
  { time_table_ = Tt; notify_update(*orbit_); }

//-----------------------------------------------------------------------
/// Camera that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Camera>& camera() const {return cam; }
  void camera(const boost::shared_ptr<Camera>& C);

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
  friend class IgcRollingShutterHelper::IcEq;
  boost::shared_ptr<Orbit> orbit_;
  boost::shared_ptr<QuaternionOrbitData> od1;
  boost::shared_ptr<QuaternionOrbitData> od2;
  IgcRollingShutterHelper::PositionInterpolate pinterp;
  IgcRollingShutterHelper::QuaternionInterpolate qinterp;
  void position_cf(const Time& Tm, boost::array<double, 3>& Pres) const
  { pinterp.position(Tm, Pres); }
  boost::shared_ptr<CartesianFixed> position_cf(const Time& Tm) const
  {
    boost::array<double, 3> p;
    position_cf(Tm, p);
    return od1->position_cf()->create(p);
  }
  boost::math::quaternion<double> velocity_cf(const Time& Tm) const
  {
    boost::array<double, 3> v;
    pinterp.velocity(Tm, v);
    return boost::math::quaternion<double>(0, v[0], v[1], v[2]);
  }
  boost::math::quaternion<double> sc_to_cf(const Time& Tm) const
  {
    return qinterp(Tm);
  }
  boost::shared_ptr<QuaternionOrbitData> 
  orbit_data(const Time& Tm) const
  { return interpolate(*od1, *od2, Tm); }
  boost::shared_ptr<QuaternionOrbitData> 
  orbit_data(const TimeWithDerivative& Tm) const
  { return interpolate(*od1, *od2, Tm); }
  boost::shared_ptr<TimeTable> time_table_;
  boost::shared_ptr<Camera> cam;
  boost::shared_ptr<Refraction> refraction_;
  RollDirection roll_direction_;
  double res;
  int b;
  double max_h;
  double time_tolerance_;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}

GEOCAL_EXPORT_KEY(IgcRollingShutter);
#endif

