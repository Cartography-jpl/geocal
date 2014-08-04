#include "ground_mspi_igc.h"
#include "ground_mspi_orbit.h"
#include "mspi_camera.h"
#include "time_table.h"
#include "simple_dem.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. The angles are in degrees, and the rate is in degress
/// per second.
//-----------------------------------------------------------------------

GroundMspiIgc::GroundMspiIgc
(const Time& Start_time,
 boost::shared_ptr<GroundCoordinate>& Pos,
 double Azimuth,
 double Start_elevation_angle,
 double Rotation_rate,
 const std::vector<Time>& Time_tag,
 const std::string& Camera_config, int Band)
{
  boost::shared_ptr<Orbit> orb(new GroundMspiOrbit(Start_time, Pos, Azimuth, 
		  Start_elevation_angle, Rotation_rate));
  boost::shared_ptr<TimeTable> tt(new MeasuredTimeTable(Time_tag));
  boost::shared_ptr<Camera> cam(new MspiCamera(Camera_config));
  // I don't think we actually want IPI here, but go ahead and
  // set that up now and we can come back to fix this later.
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, Band, tt->min_time(),
				     tt->max_time(), tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> dummy_img;
  initialize(ipi, dem, dummy_img);
}

