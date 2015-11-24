#include "ground_mspi_igc.h"
#include "ground_mspi_orbit.h"
#include "mspi_camera.h"
#include "time_table.h"
#include "simple_dem.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GroundMspiIgc::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IpiImageGroundConnection)
    & GEOCAL_NVP(mspi_cam);
}

GEOCAL_IMPLEMENT(GroundMspiIgc);
#endif

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
  mspi_cam.reset(new MspiCamera(Camera_config));
  // I don't think we actually want IPI here, but go ahead and
  // set that up now and we can come back to fix this later.
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, mspi_cam, Band, tt->min_time(),
				     tt->max_time(), tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> dummy_img;
  initialize(ipi, dem, dummy_img);
}

GroundMspiIgc::GroundMspiIgc
(const Time& Start_time,
 boost::shared_ptr<GroundCoordinate>& Pos,
 double Azimuth,
 double Start_elevation_angle,
 double Rotation_rate,
 const std::vector<boost::shared_ptr<Time> >& Time_tag,
 const std::string& Camera_config, int Band)
{
  boost::shared_ptr<Orbit> orb(new GroundMspiOrbit(Start_time, Pos, Azimuth, 
		  Start_elevation_angle, Rotation_rate));
  boost::shared_ptr<TimeTable> tt(new MeasuredTimeTable(Time_tag));
  mspi_cam.reset(new MspiCamera(Camera_config));
  // I don't think we actually want IPI here, but go ahead and
  // set that up now and we can come back to fix this later.
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, mspi_cam, Band, tt->min_time(),
				     tt->max_time(), tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> dummy_img;
  initialize(ipi, dem, dummy_img);
}

GroundMspiIgc::GroundMspiIgc
(const Time& Start_time,
 boost::shared_ptr<GroundCoordinate>& Pos,
 double Azimuth,
 double Start_elevation_angle,
 double Rotation_rate,
 const std::vector<Time>& Time_tag,
 const boost::shared_ptr<QuaternionCamera>& Cam, int Band)
  : mspi_cam(Cam)
{
  boost::shared_ptr<Orbit> orb(new GroundMspiOrbit(Start_time, Pos, Azimuth, 
		  Start_elevation_angle, Rotation_rate));
  boost::shared_ptr<TimeTable> tt(new MeasuredTimeTable(Time_tag));
  // I don't think we actually want IPI here, but go ahead and
  // set that up now and we can come back to fix this later.
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, Cam, Band, tt->min_time(),
				     tt->max_time(), tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> dummy_img;
  initialize(ipi, dem, dummy_img);
}

GroundMspiIgc::GroundMspiIgc
(const Time& Start_time,
 boost::shared_ptr<GroundCoordinate>& Pos,
 double Azimuth,
 double Start_elevation_angle,
 double Rotation_rate,
 const std::vector<boost::shared_ptr<Time> >& Time_tag,
 const boost::shared_ptr<QuaternionCamera>& Cam, int Band)
  : mspi_cam(Cam)
{
  boost::shared_ptr<Orbit> orb(new GroundMspiOrbit(Start_time, Pos, Azimuth, 
		  Start_elevation_angle, Rotation_rate));
  boost::shared_ptr<TimeTable> tt(new MeasuredTimeTable(Time_tag));
  // I don't think we actually want IPI here, but go ahead and
  // set that up now and we can come back to fix this later.
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, Cam, Band, tt->min_time(),
				     tt->max_time(), tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> dummy_img;
  initialize(ipi, dem, dummy_img);
}

//-----------------------------------------------------------------------
/// Return solar look vector to detector coordinates.
//-----------------------------------------------------------------------

DcsLookVector GroundMspiIgc::solar_look(int Line_number) const
{
  Time t = pixel_time(ImageCoordinate(Line_number, 0));
  CartesianFixedLookVector lv = 
    CartesianFixedLookVector::solar_look_vector(t);
  return mspi_cam->dcs_look_vector(ipi().orbit().sc_look_vector(t, lv));
}

//-----------------------------------------------------------------------
/// Return surface normal look vector to detector coordinates.
//-----------------------------------------------------------------------

DcsLookVector GroundMspiIgc::normal_look(int Line_number) const
{
  Time t = pixel_time(ImageCoordinate(Line_number, 0));
  CartesianFixedLookVector lv = 
    LnLookVector(0,0,1).to_cf(*ipi().orbit().position_cf(t));
  return mspi_cam->dcs_look_vector(ipi().orbit().sc_look_vector(t, lv));
}

//-----------------------------------------------------------------------
/// Return pixel look vector to detector coordinates.
//-----------------------------------------------------------------------

DcsLookVector GroundMspiIgc::pixel_look(int Sample_number) const
{
  return mspi_cam->dcs_look_vector(FrameCoordinate(0, Sample_number), 
				   ipi().band());
}

