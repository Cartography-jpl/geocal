#include "air_mspi_igc_collection.h"
#include "air_mspi_igc.h"
#include "mspi_config_file.h"
#include "did_datum.h"
#include "usgs_dem.h"
#include "srtm_90m_dem.h"
#include "simple_dem.h"
#include "geocal_serialize_support.h"
#include <boost/lexical_cast.hpp>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void AirMspiIgcCollection::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_BASE(AirMspiIgcCollection, IgcCollection);
  GEOCAL_GENERIC_BASE(WithParameterNested);
  GEOCAL_BASE(AirMspiIgcCollection, WithParameterNested);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(WithParameterNested)
    & GEOCAL_NVP(dem) & GEOCAL_NVP(dem_resolution)
    & GEOCAL_NVP_(camera) 
    & GEOCAL_NVP_(gimbal) 
    & GEOCAL_NVP_(orbit)
    & GEOCAL_NVP_(view_config)
    & GEOCAL_NVP(base_directory)
    & GEOCAL_NVP(swath_to_use)
    & GEOCAL_NVP_(min_l1b1_line) & GEOCAL_NVP_(max_l1b1_line);
}

GEOCAL_IMPLEMENT(AirMspiIgcCollection);
#endif

//-----------------------------------------------------------------------
/// This create a AirMspiIgcCollection by directly giving the various
/// pieces needed to construct it.
//-----------------------------------------------------------------------

AirMspiIgcCollection::AirMspiIgcCollection
(const boost::shared_ptr<Orbit>& Orb,
 const boost::shared_ptr<MspiCamera>& Cam,
 const boost::shared_ptr<MspiGimbal>& Gim,
 const boost::shared_ptr<Dem>& D,
 const std::vector<std::string>& L1b1_file_name,
 const std::string& Swath_to_use,
 int Dem_resolution,
 const std::string& Base_directory)
  :  dem(D),
     dem_resolution(Dem_resolution),
     camera_(Cam),
     gimbal_(Gim),
     orbit_(Orb),
     base_directory(Base_directory),
     swath_to_use(Swath_to_use)
{
  add_object(Cam);
  add_object(Gim);
  add_object(Orb);
  BOOST_FOREACH(const std::string& fname, L1b1_file_name) {
    MspiConfigFile vc;
    vc.add("l1b1_file", fname);
    view_config_.push_back(vc);
  }
  calc_min_max_l1b1_line();
}

//-----------------------------------------------------------------------
/// This creates a AirMspiIgcCollection by reading the given master
/// config file. Various files found in the input files can have
/// relative paths. You can specify the base directory these paths are
/// relative to, the default is the current directory.
//-----------------------------------------------------------------------

AirMspiIgcCollection::AirMspiIgcCollection
(
 const std::string& Master_config_file,
 const std::string& Orbit_file_name,
 const std::string& L1b1_table,
 const std::string& Swath_to_use,
 const std::string& Base_directory
)
  : base_directory(Base_directory),
    swath_to_use(Swath_to_use)
{
  MspiConfigFile c(Master_config_file);

  // Get camera set up
  std::string fname = c.value<std::string>("camera_model_config");
  if(fname[0] != '/')
    fname = base_directory + "/" + fname;
  std::string extra_config = "";
  if(c.have_key("extra_camera_model_config")) {
    extra_config = c.value<std::string>("extra_camera_model_config");
    if(extra_config[0] != '/')
      extra_config = base_directory + "/" + extra_config;
  }
  boost::shared_ptr<MspiCamera> mspi_camera(new MspiCamera(fname, extra_config));
  boost::shared_ptr<MspiGimbal> mspi_gimbal(new MspiGimbal(fname, extra_config));
  camera_ = mspi_camera;
  gimbal_ = mspi_gimbal;

  // Not sure if we still need the "static gimbal", but we don't
  // currently support this. So check, and issue an error if this is
  // requested. We can modify the code to support this if needed.
  if(c.value<bool>("use_static_gimbal"))
    throw Exception("We don't currently support static gimbals");
  orbit_.reset(new AirMspiOrbit(Orbit_file_name, mspi_gimbal));

  // Get DEM set up
  if(c.value<std::string>("dem_type") == "usgs") {
    boost::shared_ptr<Datum> 
      datum(new DidDatum(c.value<std::string>("MSL_DATA")));
    dem.reset(new UsgsDem(c.value<std::string>("USGSDATA"), true, datum));
    dem_resolution = 10.0;
  } else if (c.value<std::string>("dem_type") == "srtm_90m"){
	boost::shared_ptr<Datum>
	datum(new DidDatum(c.value<std::string>("MSL_DATA")));
	dem.reset(new Srtm90mDem(c.value<std::string>("USGSDATA"), false, datum));
	dem_resolution = 90.0;
  }else{
    double h = (c.have_key("simple_dem_height") ?
		c.value<double>("simple_dem_height") : 0);
    dem.reset(new SimpleDem(h));
    dem_resolution = 10.0;
  }

  replace_view_config(Master_config_file, L1b1_table);

  add_object(camera_);
  add_object(gimbal_);
  add_object(orbit_);
}

//-----------------------------------------------------------------------
/// There is various metadata needed by the airmspi programs that is
/// only available once a master config and l1b1_table file are
/// created. This is created as part of AirMspiMapInfoProcessor (a
/// python class found in the MSPI-Ground software, not here in
/// GeoCal). We need to be able to add in this metadata to an existing 
/// IgcCollection. This function does this. Because we may also have
/// direction to process only a subset of the data, this also
/// recalculates the minimum and maximum L1B1 lines to use.
//-----------------------------------------------------------------------

void AirMspiIgcCollection::replace_view_config
(const std::string& Master_config_file,
 const std::string& L1b1_table)
{
  view_config_.clear();
  MspiConfigFile c(Master_config_file);

  // Set up view information
  MspiConfigFile vconfig(L1b1_table);
  MspiConfigTable vtab(vconfig, "L1B1");
  for(int i = 0; i < vtab.number_row(); ++i) {
    MspiConfigFile vc(c);
    if(vtab.has_column("extra_config_file")) {
      std::string fname = vtab.value<std::string>(i, "extra_config_file");
      if(fname != "-") {
	if(fname[0] != '/')
	  fname = base_directory + "/" + fname;
	vc.add_file(fname);
      }
    }
    vc.add("view_number", vtab.value<std::string>(i, "view_number"));
    vc.add("view_name", vtab.value<std::string>(i, "view_name"));
    vc.add("view_time", vtab.value<std::string>(i, "view_time"));
    vc.add("l1b1_file", vtab.value<std::string>(i, "l1b1_file"));
    vc.add("resolution", vtab.value<std::string>(i, "resolution"));
    view_config_.push_back(vc);
  }
  calc_min_max_l1b1_line();
}


//-----------------------------------------------------------------------
/// Go through and fill in min and max l1b1 lines that we need to read
/// the l1b1 file for.
//-----------------------------------------------------------------------
void AirMspiIgcCollection::calc_min_max_l1b1_line()
{
  for(int i = 0; i < number_image(); ++i) {
    AirMspiTimeTable tt(l1b1_file_name(i), swath_to_use);
    view_config_[i].add("l1b1_granule_id", tt.l1b1_granule_id());
    int min_ln = tt.min_line();
    int max_ln = tt.max_line();
    // Trim time if orbit doesn't cover the full range the time table does.
    if(orbit_->min_time() > tt.min_time())
      min_ln = std::max(min_ln,
			(int) ceil(tt.image_coordinate(orbit_->min_time(), 
		       FrameCoordinate(0,0)).line) + 2);
    if(orbit_->max_time() < tt.max_time())
      max_ln = std::min(max_ln,
			(int) floor(tt.image_coordinate(orbit_->max_time(), 
		       FrameCoordinate(0,0)).line) - 2);
    if(have_config(i, "min_l1b1_line"))
      min_ln = std::max(min_ln, config_value<int>(i, "min_l1b1_line"));
    if(have_config(i, "max_l1b1_line"))
      max_ln = std::min(max_ln, config_value<int>(i, "max_l1b1_line"));
    min_l1b1_line_.push_back(min_ln);
    max_l1b1_line_.push_back(max_ln);
  }

}

//-----------------------------------------------------------------------
/// Return the AirMspiIgc for the given Image_index, doing the lazy 
/// evaluation of necessary.
//-----------------------------------------------------------------------

boost::shared_ptr<AirMspiIgc> 
AirMspiIgcCollection::air_mspi_igc(int Image_index) const
{
  range_check(Image_index, 0, number_image());
  // We create igc using lazy evaluation, so if this is empty go ahead 
  // and add enough space to hold all the data.
  if(igc.empty()) {
    boost::shared_ptr<AirMspiIgc> null_ptr;
    igc.insert(igc.end(), number_image(), null_ptr);
  }
  if(!igc[Image_index]) {
    igc[Image_index].reset(new AirMspiIgc(orbit_,
					  camera_,
					  gimbal_,
					  dem,
					  l1b1_file_name(Image_index),
					  min_l1b1_line(Image_index),
					  max_l1b1_line(Image_index),
					  swath_to_use,
					  "Image " + boost::lexical_cast<std::string>(Image_index),
					  dem_resolution));
  }
  return igc[Image_index];
}

// see base class for description.
boost::shared_ptr<IgcCollection> 
AirMspiIgcCollection::subset(const std::vector<int>& Index_set) const
{
  return boost::shared_ptr<IgcCollection>(new AirMspiIgcCollection(*this, Index_set));
}


// see base class for description.
void AirMspiIgcCollection::print(std::ostream& Os) const 
{
  OstreamPad opad(Os, "    ");  
  Os << "AirMspiIgcCollection:\n";
  Os << "  Dem:\n";
  opad << *dem;
  opad.strict_sync();
  Os << "  Dem resolution: " << dem_resolution << "\n"
     << "  Camera:\n";
  opad << *camera_;
  opad.strict_sync();
  Os << "  Gimbal:\n";
  opad << *gimbal_;
  opad.strict_sync();
  Os << "  Orbit:\n";
  opad << *orbit_;
  opad.strict_sync();
  Os << "  Base directory: " << base_directory << "\n"
     << "  Swath to use: " << swath_to_use << "\n"
     << "  Parameter:\n";
  blitz::Array<double, 1> p = parameter_subset();
  std::vector<std::string> pname = parameter_name_subset();
  for(int i = 0; i < p.rows(); ++i)
    Os << "    " << pname[i] << ": " << p(i) << "\n";
}


//-----------------------------------------------------------------------
/// Create a subsetted version of a AirMspiIgcCollection.
//-----------------------------------------------------------------------

AirMspiIgcCollection::AirMspiIgcCollection
(
 const AirMspiIgcCollection& Original,
 const std::vector<int>& Index_set
)
{
  BOOST_FOREACH(int i, Index_set)
    if(i < 0 || i >= Original.number_image()) {
      Exception e;
      e << "Value " << i << " is outside of allowed index range of 0 to "
	<< Original.number_image() - 1;
    }

  dem = Original.dem;
  dem_resolution = Original.dem_resolution;
  camera_ = Original.camera_;
  gimbal_ = Original.gimbal_;
  orbit_ = Original.orbit_;
  base_directory = Original.base_directory;
  swath_to_use = Original.swath_to_use;
  BOOST_FOREACH(int i, Index_set) {
    view_config_.push_back(MspiConfigFile(Original.view_config_[i]));
    min_l1b1_line_.push_back(Original.min_l1b1_line_[i]);
    max_l1b1_line_.push_back(Original.max_l1b1_line_[i]);
  }
}

//-----------------------------------------------------------------------
/// Go from view number (found in the l1b1 table file) to the index
/// number matching it.
//-----------------------------------------------------------------------

int AirMspiIgcCollection::view_number_to_image_index(int View_number) const
{
  for(int i = 0; i < number_image(); ++i)
    if(config_value<int>(i, "view_number") == View_number)
      return i;
  Exception e;
  e << "The view number " << View_number 
    << " is not found in the AirMspiIgcCollection";
  throw e;
}
