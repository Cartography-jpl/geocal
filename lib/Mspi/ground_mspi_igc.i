// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ground_mspi_igc.h"
%}

%base_import(ipi_image_ground_connection)

%geocal_shared_ptr(GeoCal::GroundMspiIgc);
namespace GeoCal {
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
		const boost::shared_ptr<Camera>& Cam,
		int Band);
};
}
