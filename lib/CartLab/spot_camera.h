#ifndef SPOT_CAMERA_H
#define SPOT_CAMERA_H
#include "camera.h"
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class models the SPOT camera. The camera parameters are given
  by an DIMAP file. This file is an XML file format. You can find 
  documentation for this format at
  http://www.spot.com/web/SICORP/452-sicorp-the-dimap-format.php.

  We don't actually read this file format. XML is a bit of a pain to
  deal with in C++, and is much easier to process in Ruby. So we set
  up an interface that just takes the PSI_X and PSI_Y values for each
  pixel. The Afids class Dimap processes the XML file and can use that
  to create an instance of this class.

  The various angles and so forth are described in the "SPOT Geometry
  Handbook", available at
  http://www.spotimage.com/automne_modules_files/standard/public/p229_0b9c0d94a22e77aac09df2b360c73073SPOT_Geometry_Handbook.pdf
*******************************************************************/

class SpotCamera : public PushBroomCamera {
public: 
  SpotCamera(const std::vector<double>& Psi_x, 
	     const std::vector<double>& Psi_y);
  virtual ~SpotCamera() {}

//-----------------------------------------------------------------------
/// Number of bands in camera.
//-----------------------------------------------------------------------

  virtual int number_band() const { return 1; }

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const { return 1;}

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const {return (int) psi_x_.size(); }

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl,
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
  const std::vector<double> psi_x() const {return psi_x_;}
  const std::vector<double> psi_y() const {return psi_y_;}
private:
  std::vector<double> psi_x_;
  std::vector<double> psi_y_;
};
}
#endif
