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

  We support both conventions, depending on the setting of the
  frame_convention.

  Note that in addition to the line and sample pitch, we supply a 
  line and sample scale. You could achieve exactly the same behavior
  either changing the scale, or just directly changing pitch. The
  scale is supplied as a convenience, it is a more natural thing to
  vary if you are calibrating the camera. If you wish, you can 
  leave the scale fixed at 1.0 and ignore it if you don't want to use
  it. 
*******************************************************************/
class QuaternionCamera : public Camera {
public:
  enum FrameConvention { LINE_IS_X, LINE_IS_Y};

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
		   double Line_scale = 1.0,
		   double Sample_scale = 1.0,
		   FrameConvention Frame_convention = LINE_IS_X)
    : focal_length_(Focal_length),
      nline_(Number_line),
      nsamp_(Number_sample),
      line_pitch_(Line_pitch),
      sample_pitch_(Sample_pitch),
      line_scale_(Line_scale),
      sample_scale_(Sample_scale),
      pp_(Principal_point),
      frame_to_sc_(Frame_to_sc_q),
      frame_convention_(Frame_convention)
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~QuaternionCamera() {}

//-----------------------------------------------------------------------
/// Number of bands in camera. We don't treat the bands separately (at
/// least for now), so this is set to 1.
//-----------------------------------------------------------------------

  virtual int number_band() const { return 1; }

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const
  { range_check(Band, 0, 1); return nline_; }

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const
  { range_check(Band, 0, 1); return nsamp_; }

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

  const FrameCoordinate& principal_point() const {return pp_;}


//-----------------------------------------------------------------------
/// Set principal point of camera
//-----------------------------------------------------------------------

  void principal_point(const FrameCoordinate& Fc) {pp_ = Fc;}

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
/// Scaling of line.
//-----------------------------------------------------------------------

  double line_scale() const { return line_scale_;}
  void line_scale(double Line_scale) { line_scale_ = Line_scale; }

//-----------------------------------------------------------------------
/// Scaling of sample.
//-----------------------------------------------------------------------

  double sample_scale() const { return sample_scale_;}
  void sample_scale(double Sample_scale) { sample_scale_ = Sample_scale; }

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
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;

  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
private:
  double focal_length_;		// Focal length, in mm.
  int nline_;			// Number of lines in camera.
  int nsamp_;			// Number of samples in camera.
  double line_pitch_;		// CCD pitch, in mm
  double sample_pitch_;		// CCD pitch, in mm
  double line_scale_, sample_scale_;
                                // Scaling of line and sample.
  FrameCoordinate pp_;		// Principal point
  boost::math::quaternion<double> frame_to_sc_;
  FrameConvention frame_convention_;
                                // Indicates if X or Y is the line direction.
};
}
#endif

