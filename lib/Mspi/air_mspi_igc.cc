#include "air_mspi_igc.h"
#include "mspi_config_file.h"
#include "mspi_camera.h"
#include "did_datum.h"
#include "simple_dem.h"
#ifdef HAVE_VICAR_RTL
#include "usgs_dem.h"
#endif
#ifdef HAVE_MSPI_SHARED
#include "File/L1B1File/src/l1b1_reader.h"
#endif

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
		       const std::string& L1b1_file_name,
		       int Band,
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
  double dem_resolution;
  if(c.value<std::string>("dem_type") == "usgs") {
#ifdef HAVE_VICAR_RTL
    // We could probably relax the requirement on VICAR_RTL if needed,
    // but for now require this
    boost::shared_ptr<Datum> 
      datum(new DidDatum(c.value<std::string>("MSL_DATA")));
    dem.reset(new UsgsDem(c.value<std::string>("USGSDATA"), true, datum));
    dem_resolution = 10.0;
#else
    throw Exception("UsgsDem wasn't included in the build");
#endif
  } else {
    double h = (c.have_key("simple_dem_height") ?
		c.value<double>("simple_dem_height") : 0);
    dem.reset(new SimpleDem(h));
    dem_resolution = 10.0;
  }

  // Get the time table and L1B1 data.
  boost::shared_ptr<MeasuredTimeTable> tt;
#ifdef HAVE_MSPI_SHARED
  MSPI::Shared::L1B1Reader l1b1(L1b1_file_name);
  int refrow = reference_row(Base_directory + "/" + 
			     c.value<std::string>("instrument_info_config"));
  Time tepoch = Time::parse_time(l1b1.epoch());
  std::vector<double> toffset = 
    l1b1.read_time(refrow, 0, l1b1.number_frame(refrow));
  std::vector<Time> tm;
  BOOST_FOREACH(double toff, toffset)
    tm.push_back(tepoch + toff);
  tt.reset(new MeasuredTimeTable(tm));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
  Time tmin = std::max(orb->min_time(), tt->min_time());
  Time tmax = std::min(orb->max_time(), tt->max_time());

  // Short term, have image empty.
  boost::shared_ptr<RasterImage> img;
  std::string title = "Image";

  // Ready now to initialize ipi and Igc
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, Band, tmin, tmax, tt));
  initialize(ipi, dem, img, title, dem_resolution);
}

void AirMspiIgc::print(std::ostream& Os) const 
{
  Os << "AirMspiIgc:\n"
     << "  Master config file: " << master_config_file() << "\n"
     << "  Orbit file name:    " << orbit_file_name() << "\n"
     << "  Base directory:     " << base_directory() << "\n";
}


//-----------------------------------------------------------------------
/// Determine the reference row to use for the time table. This comes
/// from the 660nm I band. 
/// Note that the band number used in the instrument config file is
/// *not* the same as the band number used in the camera. Instead this
/// is a spectral band number.
//-----------------------------------------------------------------------

int AirMspiIgc::reference_row(const std::string& Instrument_config_file_name) 
  const
{
  // Determine mapping from instrument_band, row_type to row_number
  MspiConfigFile iconfig(Instrument_config_file_name);
  std::vector<int> rn = iconfig.value<std::vector<int> >("row_numbers");
  std::vector<std::string> rt = 
    iconfig.value<std::vector<std::string> >("row_types");
  // Note that this is a spectral band, not the camera band.
  std::vector<int> rb = iconfig.value<std::vector<int> >("band");
  std::map<int, std::map<std::string, int> > inst_to_row;
  for(int i = 0 ; i < (int) rn.size(); ++i)
    inst_to_row[rb[i]][rt[i]]=rn[i];
  return inst_to_row[6]["I"];
}
