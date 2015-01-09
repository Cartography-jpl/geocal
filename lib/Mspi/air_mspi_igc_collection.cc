#include "air_mspi_igc_collection.h"
#include "air_mspi_igc.h"
#include "mspi_config_file.h"
#include "did_datum.h"
#include "usgs_dem.h"
#include "simple_dem.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void AirMspiIgcCollection::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_BASE(AirMspiIgcCollection, IgcCollection);
  // Temp, leave DEM out since we don't have ugsdem serialized yet.
  //  ar & GEOCAL_NVP(dem) & GEOCAL_NVP(dem_resolution) &
  //  GEOCAL_NVP_(view_config)
  ar & GEOCAL_NVP_(view_config)
    & GEOCAL_NVP_(reference_row)
    & GEOCAL_NVP_(min_l1b1_line) & GEOCAL_NVP_(max_l1b1_line)
    & GEOCAL_NVP(base_directory)
    & GEOCAL_NVP_(config_filename)
    & GEOCAL_NVP_(orbit_filename);
}

GEOCAL_IMPLEMENT(AirMspiIgcCollection);
#endif

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
 const std::string& Base_directory
)
  : base_directory(Base_directory)
{
  MspiConfigFile c(Master_config_file);

  // Temp
  config_filename_ = Master_config_file;
  orbit_filename_ = Orbit_file_name;

  // Get DEM set up
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
  // Go through and fill in some data that we need to read the l1b1
  // file for.
  AirMspiIgc igcscratch(config_filename_,
			orbit_filename_,
			l1b1_file_name(0), 0,
			base_directory);
  reference_row_ = igcscratch.time_table()->reference_row();
  boost::shared_ptr<AirMspiOrbit> orb = igcscratch.orbit();

  for(int i = 0; i < number_image(); ++i) {
    AirMspiTimeTable tt(l1b1_file_name(i), reference_row_);
    view_config_[i].add("l1b1_granule_id", tt.l1b1_granule_id());
    int min_ln = tt.min_line();
    int max_ln = tt.max_line();
    // Trim time if orbit doesn't cover the full range the time table does.
    if(orb->min_time() > tt.min_time())
      min_ln = std::max(min_ln,
			(int) ceil(tt.image_coordinate(orb->min_time(), 
		       FrameCoordinate(0,0)).line) + 2);
    if(orb->max_time() < tt.max_time())
      max_ln = std::min(max_ln,
			(int) floor(tt.image_coordinate(orb->max_time(), 
		       FrameCoordinate(0,0)).line) - 2);
    if(have_config(i, "min_l1b1_line"))
      min_ln = std::max(min_ln, config_value<int>(i, "min_l1b1_line"));
    if(have_config(i, "max_l1b1_line"))
      max_ln = std::min(max_ln, config_value<int>(i, "max_l1b1_line"));
    min_l1b1_line_.push_back(min_ln);
    max_l1b1_line_.push_back(max_ln);
  }

}

// see base class for description.
boost::shared_ptr<ImageGroundConnection> 
AirMspiIgcCollection::image_ground_connection(int Image_index) const
{
  range_check(Image_index, 0, number_image());
  // We create igc using lazy evaluation, so if this is empty go ahead 
  // and add enough space to hold all the data.
  if(igc.empty()) {
    boost::shared_ptr<ImageGroundConnection> null_ptr;
    igc.insert(igc.end(), number_image(), null_ptr);
  }
  if(!igc[Image_index]) {
    // Temp, we'll clean this up in a bit.
    igc[Image_index].reset(new AirMspiIgc(config_filename_,
					  orbit_filename_,
					  l1b1_file_name(Image_index), 0,
					  base_directory));
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
  Os << "AirMspiIgcCollection:\n";
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

  // Fill this in.
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
