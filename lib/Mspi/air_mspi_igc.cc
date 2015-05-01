#include "air_mspi_igc.h"
#include "air_mspi_time_table.h"
#include "mspi_config_file.h"
#include "mspi_camera.h"
#include "did_datum.h"
#include "simple_dem.h"
#include "usgs_dem.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void AirMspiIgc::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IpiImageGroundConnection);
}

GEOCAL_IMPLEMENT(AirMspiIgc);
#endif

//-----------------------------------------------------------------------
/// Constructor. This takes the master config file and uses it to
/// create a AirMspiIgc.
///
/// You can optionally add the base directory that file names in the
/// Master_config_file are relative to. The default is the current
/// directory. 
//-----------------------------------------------------------------------

AirMspiIgc::AirMspiIgc
(const std::string& Master_config_file,
 const std::string& Orbit_file_name,
 const std::string& L1b1_file_name,
 int Band,
 const std::string& Base_directory,
 const std::string& Title)
{
  MspiConfigFile c(Master_config_file);
  // Get camera set up
  std::string fname = c.value<std::string>("camera_model_config");
  if(fname[0] != '/')
    fname = Base_directory + "/" + fname;
  std::string extra_config = "";
  if(c.have_key("extra_camera_model_config")) {
    extra_config = c.value<std::string>("extra_camera_model_config");
    if(extra_config[0] != '/')
      extra_config = Base_directory + "/" + extra_config;
  }
  boost::shared_ptr<MspiCamera> cam(new MspiCamera(fname, extra_config));

  // Get orbit set up
  // Not sure if we still need the "static gimbal", but we don't
  // currently support this. So check, and issue an error if this is
  // requested. We can modify the code to support this if needed.
  if(c.value<bool>("use_static_gimbal"))
    throw Exception("We don't currently support static gimbals");
  blitz::Array<double, 1> gimbal_angle(3);
  gimbal_angle = cam->gimbal_epsilon(),
    cam->gimbal_psi(),
    cam->gimbal_theta();
  boost::shared_ptr<AirMspiOrbit> 
    orb(new AirMspiOrbit(Orbit_file_name, gimbal_angle));

  // Get DEM set up
  boost::shared_ptr<Dem> dem;
  double dem_resolution;
  if(c.value<std::string>("dem_type") == "usgs") {
    boost::shared_ptr<Datum> 
      datum(new DidDatum(c.value<std::string>("MSL_DATA")));
    dem.reset(new UsgsDem(c.value<std::string>("USGSDATA"), true, datum));
    dem_resolution = 10.0;
  } else {
    double h = (c.have_key("simple_dem_height") ?
		c.value<double>("simple_dem_height") : 0);
    dem.reset(new SimpleDem(h));
    dem_resolution = 10.0;
  }

  // Get the time table and L1B1 data.
  boost::shared_ptr<TimeTable> tt(new AirMspiTimeTable(L1b1_file_name));
  Time tmin = std::max(orb->min_time(), tt->min_time());
  Time tmax = std::min(orb->max_time(), tt->max_time());

  // Short term, have image empty.
  boost::shared_ptr<RasterImage> img;

  // Ready now to initialize ipi and Igc
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, Band, tmin, tmax, tt));
  initialize(ipi, dem, img, Title, dem_resolution);
}

//-----------------------------------------------------------------------
/// Constructor. This variation of the constructor directly takes the
/// information needed to construct the object, rather than reading
/// this from the master configuration file.
//-----------------------------------------------------------------------

AirMspiIgc::AirMspiIgc
(const boost::shared_ptr<Orbit>& Orb,
 const boost::shared_ptr<Camera>& Cam,
 const boost::shared_ptr<Dem>& Dem,
 const std::string& L1b1_file_name,
 int Reference_row,
 int Band,
 const std::string& Title,
 int Dem_resolution)
{

  // Short term, have image empty.
  boost::shared_ptr<RasterImage> img;
  // Need to come back with Reference_row fix
  boost::shared_ptr<TimeTable> tt(new AirMspiTimeTable(L1b1_file_name));
  Time tmin = std::max(Orb->min_time(), tt->min_time());
  Time tmax = std::min(Orb->max_time(), tt->max_time());
  boost::shared_ptr<Ipi> ipi(new Ipi(Orb, Cam, Band, tmin, tmax, tt));
  initialize(ipi, Dem, img, Title, Dem_resolution);
}

void AirMspiIgc::print(std::ostream& Os) const 
{
  OstreamPad opad(Os, "    ");
  Os << "AirMspiIgc:\n";
  IpiImageGroundConnection::print(opad);
  opad.strict_sync();
}


