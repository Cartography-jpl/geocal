#include "air_mspi_igc_collection.h"
#include "mspi_config_file.h"
#include "did_datum.h"
#include "usgs_dem.h"
#include "simple_dem.h"
#ifdef HAVE_MSPI_SHARED
#include "File/L1B1File/src/l1b1_reader.h"
#endif

using namespace GeoCal;


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
  l1b2_hdf_chunk_size_x_ = c.value<int>("l1b2_hdf_chunk_size_x");
  l1b2_hdf_chunk_size_y_ = c.value<int>("l1b2_hdf_chunk_size_y");

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
  view_number_ = vtab.value_column<int>("view_number");
  view_name_ = vtab.value_column<std::string>("view_name");
  view_time_ = vtab.value_column<std::string>("view_time");
  view_resolution_ = vtab.value_column<double>("resolution");
  l1b1_file_name_ = vtab.value_column<std::string>("l1b1_file");

  // Go through and fill in some data that we need to read the l1b1
  // file for.
  std::string fname = c.value<std::string>("instrument_info_config");
  if(fname[0] != '/')
    fname = base_directory + "/" + fname;
#ifdef HAVE_MSPI_SHARED
  int ref_row = reference_row(fname);
  for(int i = 0; i < number_image(); ++i) {
    MSPI::Shared::L1B1Reader l1b1(l1b1_file_name(i));
    l1b1_granule_id_.push_back(l1b1.granule_id());
    MspiConfigFile view_config(Master_config_file);
    if(vtab.has_column("extra_config_file")) {
      std::string fname = vtab.value<std::string>(i, "extra_config_file");
      if(fname != "-") {
	if(fname[0] != '/')
	  fname = base_directory + "/" + fname;
	view_config.add_file(fname);
      }
    }
    int min_ln = 0;
    int max_ln = l1b1.number_frame(ref_row) - 1;
    if(view_config.have_key("min_l1b1_line"))
      min_ln = std::max(min_ln, view_config.value<int>("min_l1b1_line"));
    if(view_config.have_key("max_l1b1_line"))
      max_ln = std::min(max_ln, view_config.value<int>("max_l1b1_line"));
    min_l1b1_line_.push_back(min_ln);
    max_l1b1_line_.push_back(max_ln);
    target_type_.push_back(view_config.value<std::string>("target_type"));
    geolocation_stage_.push_back(view_config.value<std::string>("geolocation_stage"));
  }
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Return L1B1 file name.
//-----------------------------------------------------------------------

std::string AirMspiIgcCollection::l1b1_file_name(int Index) const
{ range_check(Index, 0, number_image()); 
  std::string fname = l1b1_file_name_[Index];
  if(fname[0] != '/')
    fname = base_directory + "/" + fname;
  return fname;
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
    // Fill this in.
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
/// This is a duplicate of code found in AirMpsiTimeTable. We should
/// fix this at some point, but for now we'll just copy the code here.
//
/// Determine the reference row to use for the time table. This comes
/// from the 660nm I band. 
/// Note that the band number used in the instrument config file is
/// *not* the same as the band number used in the camera. Instead this
/// is a spectral band number.
//-----------------------------------------------------------------------

int AirMspiIgcCollection::reference_row
(const std::string& Instrument_config_file_name) const
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
