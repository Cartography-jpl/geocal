#ifndef QUATERNION_CAMERA_H
#define QUATERNION_CAMERA_H
#include "printable.h"
#include "camera.h"
#include "geocal_exception.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple frame camera. This doesn't account for any lens
  distortion, we model this as a pinhole camera. The camera has 
  an orientation to the spacecraft frame by a given quaternion.

  There are 2 conventions used for the frame coordinates. The
  convention used by other Cameras we have implemented is that the
  line direction is +x, and the sample direction is +y.

  However, another convention is that line goes in the +y direction
  and sample goes in +x direction. This is what was used in the
  VICAR procedure sc2rpc. Note that this is more than just a
  rotation from the other convention, it is both a rotation and a
  reflection (so the chirality is different).

  In addition to either x or y direction for line, we can have
  increasing line go in the positive direction or negative direction.
  Likewise for sample.

  We support both conventions, depending on the setting of the
  frame_convention.
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
      principal_point_(Principal_point),
      frame_to_sc_(Frame_to_sc_q),
      frame_convention_(Frame_convention),
      line_direction_(Line_direction),
      sample_direction_(Sample_direction)
  { }

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

  double focal_length() const {return focal_length_;}


//-----------------------------------------------------------------------
/// Set focal length, in mm.
//-----------------------------------------------------------------------

  void focal_length(double V) { focal_length_ = V; }

//-----------------------------------------------------------------------
/// Principal point of camera
//-----------------------------------------------------------------------

  const FrameCoordinate& principal_point() const {return principal_point_;}


//-----------------------------------------------------------------------
/// Set principal point of camera
//-----------------------------------------------------------------------

  void principal_point(const FrameCoordinate& Fc) {principal_point_ = Fc;}

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double line_pitch() const {return line_pitch_; }


//-----------------------------------------------------------------------
/// Set CCD pitch, in mm
//-----------------------------------------------------------------------

  void line_pitch(double Lp) {line_pitch_ = Lp; }

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double sample_pitch() const {return sample_pitch_;}

//-----------------------------------------------------------------------
/// Set CCD pitch, in mm
//-----------------------------------------------------------------------

  void sample_pitch(double Sp) {sample_pitch_ = Sp;}

//-----------------------------------------------------------------------
/// Frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  const boost::math::quaternion<double>& frame_to_sc() const
  {return frame_to_sc_;}

//-----------------------------------------------------------------------
/// Set frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  void frame_to_sc(const boost::math::quaternion<double>& frame_to_sc_q) 
  { frame_to_sc_ = frame_to_sc_q; }

//-----------------------------------------------------------------------
/// Frame convention, indicates if Line is in X or Y direction in
/// frame coordinates.
//-----------------------------------------------------------------------

  FrameConvention frame_convention() const { return frame_convention_; }

//-----------------------------------------------------------------------
/// Set frame convention.
//-----------------------------------------------------------------------

  void frame_convention(FrameConvention Frame_convention)
  { frame_convention_ = Frame_convention; }

//-----------------------------------------------------------------------
/// Line direction, indicates if increasing line is in positive or
/// negative direction.
//-----------------------------------------------------------------------

  FrameDirection line_direction() const { return line_direction_; }

//-----------------------------------------------------------------------
/// Set line direction.
//-----------------------------------------------------------------------

  void line_direction(FrameDirection Line_direction)
  { line_direction_ = Line_direction; }

//-----------------------------------------------------------------------
/// Sample direction, indicates if increasing sample is in positive or
/// negative direction.
//-----------------------------------------------------------------------

  FrameDirection sample_direction() const { return sample_direction_; }

//-----------------------------------------------------------------------
/// Set sample direction.
//-----------------------------------------------------------------------

  void sample_direction(FrameDirection Sample_direction)
  { sample_direction_ = Sample_direction; }

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;

  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
protected:
//-----------------------------------------------------------------------
// Constructor for use by derived classes. Derived classes should make
// sure to fill in all the values
//-----------------------------------------------------------------------
  QuaternionCamera() {}
  double focal_length_;		// Focal length, in mm.
  int nband_;			// Number of bands in camera.
  int nline_;			// Number of lines in camera.
  int nsamp_;			// Number of samples in camera.
  double line_pitch_;		// CCD pitch, in mm
  double sample_pitch_;		// CCD pitch, in mm
  FrameCoordinate principal_point_;
				// Principal point
  boost::math::quaternion<double> frame_to_sc_;
  FrameConvention frame_convention_;
                                // Indicates if X or Y is the line
                                // direction.
  FrameDirection line_direction_, sample_direction_;
				// Indicate if increasing line or
				// sample goes in positive or negative 
				// direction.
private:
  int line_dir() const 
  {return (line_direction_ == INCREASE_IS_POSITIVE ? 1 : -1);}
  int samp_dir() const 
  {return (sample_direction_ == INCREASE_IS_POSITIVE ? 1 : -1);}
};
}
#endif

