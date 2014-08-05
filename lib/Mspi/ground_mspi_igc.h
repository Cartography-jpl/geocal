#ifndef GROUND_MSPI_IGC_H
#define GROUND_MSPI_IGC_H
#include "ipi_image_ground_connection.h" 
				// Definition of IpiImageGroundConnection
#include "quaternion_camera.h" 	// Definition of QuaternionCamera.

namespace GeoCal {

/****************************************************************//**
  This is an ImageGroundConnection for GroundMspi.
*******************************************************************/

class GroundMspiIgc : public IpiImageGroundConnection {
public:
  GroundMspiIgc(const Time& Start_time,
		boost::shared_ptr<GroundCoordinate>& Pos,
		double Azimuth,
		double Start_elevation_angle,
		double Rotation_rate,
		const std::vector<Time>& Time_tag,
		const std::string& Camera_config,
		int Band);
  GroundMspiIgc(const Time& Start_time,
		boost::shared_ptr<GroundCoordinate>& Pos,
		double Azimuth,
		double Start_elevation_angle,
		double Rotation_rate,
		const std::vector<Time>& Time_tag,
		const boost::shared_ptr<QuaternionCamera>& Cam,
		int Band);
  virtual ~GroundMspiIgc() {}
  DcsLookVector solar_look(int Line_number) const;
  DcsLookVector normal_look(int Line_number) const;
  DcsLookVector pixel_look(int Sample_number) const;

//-----------------------------------------------------------------------
/// Band that we are using.
//-----------------------------------------------------------------------

  int band() const {return ipi().band(); }

//-----------------------------------------------------------------------
/// Set band that we are using.
//-----------------------------------------------------------------------

  void band(int B) { ipi_ptr()->band(B); }


private:
  // This is a duplicate of what is passed to the base class. We have
  // a copy here because some things we do with this class requires a
  // QuaternionCamera (which can convert to and from DcsLookVector,
  // that a general camera can't.
  boost::shared_ptr<QuaternionCamera> mspi_cam;
};
}
#endif

