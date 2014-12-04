#ifndef QUATERNION_CAMERA_H
#define QUATERNION_CAMERA_H
#include "printable.h"
#include "camera.h"
#include "geocal_exception.h"
#include "geocal_quaternion.h"
#include "geocal_autoderivative_quaternion.h"
#include <vector>
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  A lot of cameras follow the model of "rotate into the detector
  space, do a nonlinear correction". This class supports this by
  handling the rotation of a ScLookVector and conversion to
  FrameCoordinate (and vice versa). This base class doesn't model any
  nonlinear corrections - we just model a pinhole. But derived classes
  can override the dcs_to_focal_plane and focal_plane_to_dcs functions
  to put in whatever functionality is desired.

  There are 2 conventions used for the frame coordinates. The
  convention used by some cameras we have implemented is that the
  line direction is +x, and the sample direction is +y.

  However, another convention is that line goes in the +y direction
  and sample goes in +x direction. This is what was used in the
  VICAR procedure sc2rpc. Note that this is more than just a
  rotation from the other convention, it is both a rotation and a
  reflection (so the chirality is different).

  We support both conventions, depending on the setting of the
  frame_convention.

  In addition to either x or y direction for line, we can have
  increasing line go in the positive direction or negative direction.
  Likewise for sample.
*******************************************************************/
class QuaternionCamera : public Camera {
public:
  enum FrameConvention { LINE_IS_X, LINE_IS_Y};
  enum FrameDirection { INCREASE_IS_POSITIVE, INCREASE_IS_NEGATIVE};

//-----------------------------------------------------------------------
/// Create a QuaternionCamera. The orientation of the camera to the
/// spacecraft to given by the quaternion that takes frame coordinates
/// to spacecraft coordinates. The size of the camera and the line
/// pitch, sample pitch, and focal length are given. By convention,
/// these are given in mm. Finally the Principal_point (coordinates at
/// center) are given.
//-----------------------------------------------------------------------

  QuaternionCamera(boost::math::quaternion<double> Frame_to_sc_q, 
		   double Number_line, double Number_sample,
		   double Line_pitch, double Sample_pitch,
		   double Focal_length, 
		   const FrameCoordinate& Principal_point,
		   FrameConvention Frame_convention = LINE_IS_X,
		   FrameDirection Line_direction = INCREASE_IS_POSITIVE,
		   FrameDirection Sample_direction = INCREASE_IS_POSITIVE)
    : focal_length_(Focal_length),
      nband_(1),
      nline_(Number_line),
      nsamp_(Number_sample),
      line_pitch_(Line_pitch),
      sample_pitch_(Sample_pitch),
      frame_to_sc_(to_autoderivative(Frame_to_sc_q)),
      frame_convention_(Frame_convention),
      line_direction_(Line_direction),
      sample_direction_(Sample_direction)
  { 
    principal_point_.push_back(Principal_point);
    parameter_mask_.resize(6 + 2 * number_band());
    parameter_mask_ = true;
  }

  QuaternionCamera(boost::math::quaternion<double> Frame_to_sc_q, 
		   double Number_line, double Number_sample,
		   double Line_pitch, double Sample_pitch,
		   double Focal_length, 
		   const FrameCoordinate& Principal_point,
		   FrameConvention Frame_convention,
		   FrameDirection Line_direction,
		   FrameDirection Sample_direction,
		   const blitz::Array<bool, 1>& Parameter_mask)
    : focal_length_(Focal_length),
      nband_(1),
      nline_(Number_line),
      nsamp_(Number_sample),
      line_pitch_(Line_pitch),
      sample_pitch_(Sample_pitch),
      frame_to_sc_(to_autoderivative(Frame_to_sc_q)),
      frame_convention_(Frame_convention),
      line_direction_(Line_direction),
      sample_direction_(Sample_direction),
      parameter_mask_(Parameter_mask.copy())
  { 
    principal_point_.push_back(Principal_point);
    if(parameter_mask_.rows() < 6 + 2 * number_band()) {
      parameter_mask_.resize(6 + 2 * number_band());
      parameter_mask_ = true;
    }
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~QuaternionCamera() {}

//-----------------------------------------------------------------------
/// Number of bands in camera. We don't treat the bands separately (at
/// least for now), so this is set to 1.
//-----------------------------------------------------------------------

  virtual int number_band() const { return nband_; }

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const
  { range_check(Band, 0, number_band()); return nline_; }

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const
  { range_check(Band, 0, number_band()); return nsamp_; }

//-----------------------------------------------------------------------
/// Focal length, in mm.
//-----------------------------------------------------------------------

  double focal_length() const {return focal_length_.value();}


//-----------------------------------------------------------------------
/// Set focal length, in mm.
//-----------------------------------------------------------------------

  void focal_length(double V) { focal_length_ = V; notify_update(); }

//-----------------------------------------------------------------------
/// Focal length, in mm.
//-----------------------------------------------------------------------

  const AutoDerivative<double>& focal_length_with_derivative() const 
  {return focal_length_;}


//-----------------------------------------------------------------------
/// Set focal length, in mm.
//-----------------------------------------------------------------------

  void focal_length_with_derivative(const AutoDerivative<double>& V) 
  { focal_length_ = V; notify_update(); }

//-----------------------------------------------------------------------
/// Principal point of camera for band B.
//-----------------------------------------------------------------------

  FrameCoordinate principal_point(int B) const 
  {range_check(B, 0, number_band()); return principal_point_[B].value();}


//-----------------------------------------------------------------------
/// Set principal point of camera
//-----------------------------------------------------------------------

  void principal_point(int B, const FrameCoordinate& Fc) 
  {range_check(B, 0, number_band());principal_point_[B] = Fc; notify_update(); }


//-----------------------------------------------------------------------
/// Principal point of camera for band B.
//-----------------------------------------------------------------------

  const FrameCoordinateWithDerivative&
  principal_point_with_derivative(int B) const 
  {range_check(B, 0, number_band()); return principal_point_[B];}


//-----------------------------------------------------------------------
/// Set principal point of camera
//-----------------------------------------------------------------------

  void principal_point_with_derivative(int B, 
				       const FrameCoordinateWithDerivative& Fc) 
  {range_check(B, 0, number_band());principal_point_[B] = Fc; notify_update(); }

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double line_pitch() const {return line_pitch_.value(); }


//-----------------------------------------------------------------------
/// Set CCD pitch, in mm
//-----------------------------------------------------------------------

  void line_pitch(double Lp) {line_pitch_ = Lp; notify_update(); }


//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  const AutoDerivative<double>& line_pitch_with_derivative() const 
  {return line_pitch_; }


//-----------------------------------------------------------------------
/// Set CCD pitch, in mm
//-----------------------------------------------------------------------

  void line_pitch_with_derivative(const AutoDerivative<double>& Lp) 
  {line_pitch_ = Lp; notify_update(); }

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double sample_pitch() const {return sample_pitch_.value();}

//-----------------------------------------------------------------------
/// Set CCD pitch, in mm
//-----------------------------------------------------------------------

  void sample_pitch(double Sp) {sample_pitch_ = Sp; notify_update(); }

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  AutoDerivative<double> sample_pitch_with_derivative() const 
  {return sample_pitch_;}

//-----------------------------------------------------------------------
/// Set CCD pitch, in mm
//-----------------------------------------------------------------------

  void sample_pitch_with_derivative(const AutoDerivative<double>& Sp) 
  {sample_pitch_ = Sp; notify_update(); }

//-----------------------------------------------------------------------
/// Return the equivalent yaw, pitch, roll angles for the
/// frame_to_sc. These are in radians.
//-----------------------------------------------------------------------

  blitz::Array<double, 1> ypr() const
  {
    blitz::Array<double, 1> res(3);
    quat_to_ypr(frame_to_sc(), res(0), res(1), res(2));
    return res;
  }

//-----------------------------------------------------------------------
/// Return the equivalent yaw, pitch, roll angles for the
/// frame_to_sc. These are in radians.
//-----------------------------------------------------------------------

  blitz::Array<AutoDerivative<double>, 1> ypr_with_derivative() const
  {
    blitz::Array<AutoDerivative<double>, 1> res(3);
    quat_to_ypr(frame_to_sc_with_derivative(), res(0), res(1), res(2));
    return res;
  }

//-----------------------------------------------------------------------
/// Update the frame_to_sc using the given yaw, pitch, roll angles in
/// radians.
//-----------------------------------------------------------------------

  void ypr(const blitz::Array<double, 1>& Ypr)
  {
    if(Ypr.rows() != 3)
      throw Exception("Ypr must be size 3");
    frame_to_sc(quat_rot("xyz", Ypr(1), Ypr(2), Ypr(0)));
    notify_update();
  }

//-----------------------------------------------------------------------
/// Update the frame_to_sc using the given yaw, pitch, roll angles in
/// radians.
//-----------------------------------------------------------------------

  void ypr_with_derivative(const blitz::Array<AutoDerivative<double>, 1>& Ypr)
  {
    if(Ypr.rows() != 3)
      throw Exception("Ypr must be size 3");
    frame_to_sc_with_derivative(quat_rot("xyz", Ypr(1), Ypr(2), Ypr(0)));
    notify_update();
  }

//-----------------------------------------------------------------------
/// Return the equivalent Euler angles epsilon, beta, delta for the
/// frame_to_sc. These are in radians.
//-----------------------------------------------------------------------

  blitz::Array<double, 1> euler() const
  {
    blitz::Array<double, 1> res(3);
    quat_to_euler(frame_to_sc(), res(0), res(1), res(2));
    return res;
  }

//-----------------------------------------------------------------------
/// Return the equivalent Euler angles epsilon, beta, delta for the
/// frame_to_sc. These are in radians.
//-----------------------------------------------------------------------

  blitz::Array<AutoDerivative<double>, 1> euler_with_derivative() const
  {
    blitz::Array<AutoDerivative<double>, 1> res(3);
    quat_to_euler(frame_to_sc_with_derivative(), res(0), res(1), res(2));
    return res;
  }

//-----------------------------------------------------------------------
/// Update the frame_to_sc using the given Euler angles epsilon, beta,
/// data in radians.
//-----------------------------------------------------------------------

  void euler(const blitz::Array<double, 1>& Euler)
  {
    if(Euler.rows() != 3)
      throw Exception("Ypr must be size 3");
    frame_to_sc(quat_rot("zyx", Euler(0), Euler(1), Euler(2)));
    notify_update();
  }

//-----------------------------------------------------------------------
/// Update the frame_to_sc using the given Euler angles epsilon, beta,
/// data in radians.
//-----------------------------------------------------------------------

  void euler_with_derivative(const blitz::Array<AutoDerivative<double>, 1>& Euler)
  {
    if(Euler.rows() != 3)
      throw Exception("Ypr must be size 3");
    frame_to_sc_with_derivative(quat_rot("zyx", Euler(0), Euler(1), Euler(2)));
    notify_update();
  }

  void euler_with_derivative(const ArrayAd<double, 1>& Euler)
  {
    if(Euler.rows() != 3)
      throw Exception("Ypr must be size 3");
    frame_to_sc_with_derivative(quat_rot("zyx", Euler(0), Euler(1), Euler(2)));
    notify_update();
  }

//-----------------------------------------------------------------------
/// Frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  boost::math::quaternion<double> frame_to_sc() const
  {return value(frame_to_sc_);}

//-----------------------------------------------------------------------
/// Frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  boost::math::quaternion<AutoDerivative<double> > 
  frame_to_sc_with_derivative() const
  {return frame_to_sc_;}

//-----------------------------------------------------------------------
/// Set frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  void frame_to_sc(const boost::math::quaternion<double>& frame_to_sc_q) 
  { frame_to_sc_ = to_autoderivative(frame_to_sc_q); notify_update(); }


//-----------------------------------------------------------------------
/// Set frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  void frame_to_sc_with_derivative(const boost::math::quaternion<AutoDerivative<double> >& frame_to_sc_q) 
  { frame_to_sc_ = frame_to_sc_q; notify_update(); }

//-----------------------------------------------------------------------
/// Frame convention, indicates if Line is in X or Y direction in
/// frame coordinates.
//-----------------------------------------------------------------------

  FrameConvention frame_convention() const { return frame_convention_; }

//-----------------------------------------------------------------------
/// Set frame convention.
//-----------------------------------------------------------------------

  void frame_convention(FrameConvention Frame_convention)
  { frame_convention_ = Frame_convention; notify_update(); }

//-----------------------------------------------------------------------
/// Line direction, indicates if increasing line is in positive or
/// negative direction.
//-----------------------------------------------------------------------

  FrameDirection line_direction() const { return line_direction_; }

//-----------------------------------------------------------------------
/// Set line direction.
//-----------------------------------------------------------------------

  void line_direction(FrameDirection Line_direction)
  { line_direction_ = Line_direction; notify_update(); }

//-----------------------------------------------------------------------
/// Sample direction, indicates if increasing sample is in positive or
/// negative direction.
//-----------------------------------------------------------------------

  FrameDirection sample_direction() const { return sample_direction_; }

//-----------------------------------------------------------------------
/// Set sample direction.
//-----------------------------------------------------------------------

  void sample_direction(FrameDirection Sample_direction)
  { sample_direction_ = Sample_direction; notify_update(); }

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const ScLookVectorWithDerivative& Sl, 
				   int Band) const;

//-----------------------------------------------------------------------
/// Convert Spacecraft look vector to the look vector in Detector
/// Coordinate System.
//-----------------------------------------------------------------------

  virtual DcsLookVector dcs_look_vector(const ScLookVector& Sl)
    const 
  { return DcsLookVector(conj(frame_to_sc()) * Sl.look_quaternion() *
			 frame_to_sc());
  }
  virtual DcsLookVectorWithDerivative 
  dcs_look_vector(const ScLookVectorWithDerivative& Sl)
    const 
  { return DcsLookVectorWithDerivative(conj(frame_to_sc_with_derivative()) * 
			 Sl.look_quaternion() *
			 frame_to_sc_with_derivative());
  }

  virtual DcsLookVector dcs_look_vector(const FrameCoordinate& F, int Band) 
    const;
  virtual DcsLookVectorWithDerivative 
  dcs_look_vector(const FrameCoordinateWithDerivative& F, int Band) const;
  using Camera::sc_look_vector;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual ScLookVectorWithDerivative 
  sc_look_vector_with_derivative(const FrameCoordinateWithDerivative& F, 
				 int Band) const;
  virtual ScLookVector sc_look_vector(const DcsLookVector& Dlv) const
  { return ScLookVector(frame_to_sc() * Dlv.look_quaternion() * 
			conj(frame_to_sc()));} 
  virtual void print(std::ostream& Os) const;
  virtual blitz::Array<double, 1> parameter() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative
  (const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;

//-----------------------------------------------------------------------
/// Return the parameter subset mask, where "true" means include the
/// parameter and "false" means don't.
//-----------------------------------------------------------------------

  virtual blitz::Array<bool, 1> parameter_mask() const 
  { return parameter_mask_; }

//-----------------------------------------------------------------------
/// Indicate if we fit for camera euler epsilon.
//-----------------------------------------------------------------------

  bool fit_epsilon() const { return parameter_mask_(0); }
  void fit_epsilon(bool V) {parameter_mask_(0) = V;}

//-----------------------------------------------------------------------
/// Indicate if we fit for camera euler beta.
//-----------------------------------------------------------------------

  bool fit_beta() const { return parameter_mask_(1); }
  void fit_beta(bool V) {parameter_mask_(1) = V;}

//-----------------------------------------------------------------------
/// Indicate if we fit for camera euler delta.
//-----------------------------------------------------------------------

  bool fit_delta() const { return parameter_mask_(2); }
  void fit_delta(bool V) {parameter_mask_(2) = V;}

//-----------------------------------------------------------------------
/// Indicate if we fit for camera line pitch.
//-----------------------------------------------------------------------

  bool fit_line_pitch() const { return parameter_mask_(3); }
  void fit_line_pitch(bool V) {parameter_mask_(3) = V;}

//-----------------------------------------------------------------------
/// Indicate if we fit for camera sample pitch.
//-----------------------------------------------------------------------

  bool fit_sample_pitch() const { return parameter_mask_(4); }
  void fit_sample_pitch(bool V) {parameter_mask_(4) = V;}

//-----------------------------------------------------------------------
/// Indicate if we fit for camera focal length.
//-----------------------------------------------------------------------

  bool fit_focal_length() const { return parameter_mask_(5); }
  void fit_focal_length(bool V) {parameter_mask_(5) = V;}

//-----------------------------------------------------------------------
/// Indicate if we fit for camera principal point line.
//-----------------------------------------------------------------------

  bool fit_principal_point_line(int Band = 0) const 
  { range_check(Band,0, number_band());
    return parameter_mask_(6 + 2 * Band); }
  void fit_principal_point_line(bool V, int Band = 0)
  {range_check(Band,0, number_band());
    parameter_mask_(6 + 2 * Band) = V;}

//-----------------------------------------------------------------------
/// Indicate if we fit for camera principal point sample.
//-----------------------------------------------------------------------

  bool fit_principal_point_sample(int Band = 0) const 
  { range_check(Band,0, number_band());
    return parameter_mask_(7 + 2 * Band); }
  void fit_principal_point_sample(bool V, int Band = 0)
  {range_check(Band,0, number_band());
    parameter_mask_(7 + 2 * Band) = V;}
protected:
//-----------------------------------------------------------------------
// Constructor for use by derived classes. Derived classes should make
// sure to fill in all the values
//-----------------------------------------------------------------------
  QuaternionCamera() {}
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<double>& Dcs,
				  double& Xfp, double& Yfp) const;
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<AutoDerivative<double> >& Dcs,
				  AutoDerivative<double>& Xfp, 
				  AutoDerivative<double>& Yfp) const;
  virtual boost::math::quaternion<double> 
  focal_plane_to_dcs(int Band, double Xfp, double Yfp) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  focal_plane_to_dcs(int Band, const AutoDerivative<double>& Xfp, 
		     const AutoDerivative<double>& Yfp) const;
  AutoDerivative<double> focal_length_;	
				// Focal length, in mm.
  int nband_;			// Number of bands in camera.
  int nline_;			// Number of lines in camera.
  int nsamp_;			// Number of samples in camera.
  AutoDerivative<double> line_pitch_;	// CCD pitch, in mm
  AutoDerivative<double> sample_pitch_;	// CCD pitch, in mm
  std::vector<FrameCoordinateWithDerivative> principal_point_;
				// Principal point, indexed by band.
  boost::math::quaternion<AutoDerivative<double> > frame_to_sc_;
  FrameConvention frame_convention_;
                                // Indicates if X or Y is the line
                                // direction.
  FrameDirection line_direction_, sample_direction_;
				// Indicate if increasing line or
				// sample goes in positive or negative 
				// direction.
  blitz::Array<bool, 1> parameter_mask_;
				// Mask of parameters we are fitting for.
  int line_dir() const 
  {return (line_direction_ == INCREASE_IS_POSITIVE ? 1 : -1);}
  int samp_dir() const 
  {return (sample_direction_ == INCREASE_IS_POSITIVE ? 1 : -1);}

  virtual void notify_update()
  {
    notify_update_do(*this);
  }
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    using boost::serialization::make_nvp;
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Camera);
    ar & GEOCAL_NVP_(focal_length)
      & make_nvp("number_band", nband_)
      & make_nvp("number_line", nline_)
      & make_nvp("number_sample", nsamp_)
      & GEOCAL_NVP_(line_pitch)
      & GEOCAL_NVP_(sample_pitch)
      & GEOCAL_NVP_(principal_point)
      & GEOCAL_NVP_(frame_to_sc)
      & GEOCAL_NVP_(frame_convention)
      & GEOCAL_NVP_(line_direction)
      & GEOCAL_NVP_(sample_direction)
      & GEOCAL_NVP_(parameter_mask);
  }
#endif
};
}
#endif

