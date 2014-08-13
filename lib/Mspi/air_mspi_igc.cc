#include "air_mspi_igc.h"
#include "mspi_config_file.h"
#include "mspi_camera.h"
#include "usgs_dem.h"
#include "simple_dem.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. This takes the master config file and uses it to
/// create a AirMspiIgc.
///
/// You can optionally add the base directory that file names in the
/// Master_config_file are relative to. The default is the current
/// directory. 
///
/// \todo Do we want to allow the camera to be passed in, so we can
/// share one in the IgcCollection?
//-----------------------------------------------------------------------

AirMspiIgc::AirMspiIgc(const std::string& Master_config_file,
		       const std::string& Orbit_file_name,
		       const std::string& Base_directory)
: bdir(Base_directory),
  mconfig(Master_config_file)
{
  MspiConfigFile c(Master_config_file);
  // Get camera set up
  boost::shared_ptr<MspiCamera> cam
    (new MspiCamera(Base_directory + "/" + 
		    c.value<std::string>("camera_model_config")));

  // Get orbit set up

  // Not sure if we still need the "static gimbal", but we don't
  // currently support this. So check, and issue an error if this is
  // requested. We can modify the code to support this if needed.
  if(c.value<bool>("use_static_gimbal"))
    throw Exception("We don't currently support static gimbals");
  MspiConfigFile cam_config(cam->file_name());
  blitz::Array<double, 1> gimbal_angle(3);
  gimbal_angle = cam_config.value<double>("gimbal_epsilon"),
    cam_config.value<double>("gimbal_psi"),
    cam_config.value<double>("gimbal_theta");
  boost::shared_ptr<AirMspiOrbit> 
    orb(new AirMspiOrbit(Orbit_file_name, gimbal_angle));

  // Get DEM set up
  boost::shared_ptr<Dem> dem;
  if(c.value<std::string>("dem_type") == "usgs") {
    // Note that Mike used the DID datum. We'll use the DatumGeoid96
    // here, but we can change that if needed.
    dem.reset(new UsgsDem(c.value<std::string>("USGSDATA")));
  } else {
    double h = (c.have_key("simple_dem_height") ?
		c.value<double>("simple_dem_height") : 0);
    dem.reset(new SimpleDem(h));
  }

  // Ready now to initialize ipi and Igc
  //  boost::shared_ptr<Ipi> ipi(orb, cam, band, tmin, tmax, tt);
  // initialize(ipi, dem, img, title, resolution);
}

void AirMspiIgc::print(std::ostream& Os) const 
{
  Os << "AirMspiIgc:\n"
     << "  Master config file: " << master_config_file() << "\n"
     << "  Orbit file name:    " << orbit_file_name() << "\n"
     << "  Base directory:     " << base_directory() << "\n";
}
