#ifndef GROUND_MSPI_IGC_H
#define GROUND_MSPI_IGC_H
#include "ipi_image_ground_connection.h" 
				// Definition of IpiImageGroundConnection

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
  virtual ~GroundMspiIgc() {}
};
}
#endif

