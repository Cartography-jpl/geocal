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
*******************************************************************/
class QuaternionCamera : public Camera {
public:
//-----------------------------------------------------------------------
/// Create a QuaternionCamera. The orientation of the camera to the
/// spacecraft to given by the quaternion that takes frame coordinates
/// to spacecraft coordinates. The size of the camera and the line
/// pitch, sample pitch, and focal length are given. By convention,
/// these are given in mm. Finally the Principal_point (coordinates at
/// center) are given.
///
/// Note that by convention the quaternion has line going in the +y
/// direction and sample in the +x direction. This is different
/// convention that we have used in other cameras, but matches the use
/// for the initial use we have for this camera.
//-----------------------------------------------------------------------

  QuaternionCamera(boost::math::quaternion<double> frame_to_sc_q, 
		   double Number_line, double Number_sample,
		   double Line_pitch, double Sample_pitch,
		   double Focal_length, 
		   const FrameCoordinate& Principal_point,
		   double Line_scale = 1.0,
		   double Sample_scale = 1.0)
    : focal_length_(Focal_length),
      nline_(Number_line),
      nsamp_(Number_sample),
      line_pitch_(Line_pitch),
      sample_pitch_(Sample_pitch),
      line_scale_(Line_scale),
      sample_scale_(Sample_scale),
      pp_(Principal_point),
      frame_to_sc_(frame_to_sc_q)
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
/// Principal point of camera
//-----------------------------------------------------------------------

  const FrameCoordinate& principal_point() const {return pp_;}

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double line_pitch() const {return line_pitch_; }

//-----------------------------------------------------------------------
/// CCD pitch, in mm
//-----------------------------------------------------------------------

  double sample_pitch() const {return sample_pitch_;}

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
};
}
#endif

