#ifndef AIRMSPI_IGC_COLLECTION_H
#define AIRMSPI_IGC_COLLECTION_H
#include "igc_collection.h"

namespace GeoCal {
/****************************************************************//**
  This is an IgcCollection for AirMspi.
*******************************************************************/

class AirMspiIgcCollection : public virtual IgcCollection {
public:
  AirMspiIgcCollection(const std::string& Master_config_file,
		       const std::string& Orbit_file_name,
		       const std::string& L1b1_table,
		       const std::string& Base_directory = ".");
  virtual ~AirMspiIgcCollection() {}
  virtual void print(std::ostream& Os) const;
  virtual int number_image() const { return (int) view_number_.size(); }
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;

//-----------------------------------------------------------------------
/// Return view number.
//-----------------------------------------------------------------------

  int view_number(int Index) const
  { range_check(Index, 0, number_image()); 
    return view_number_[Index];
  }

//-----------------------------------------------------------------------
/// Return view name.
//-----------------------------------------------------------------------

  std::string view_name(int Index) const
  { range_check(Index, 0, number_image()); 
    return view_name_[Index];
  }

//-----------------------------------------------------------------------
/// Return view name.
//-----------------------------------------------------------------------

  std::string view_time(int Index) const
  { range_check(Index, 0, number_image()); 
    return view_time_[Index];
  }

  std::string l1b1_file_name(int Index) const;

//-----------------------------------------------------------------------
/// Return l1b1 granule id.
//-----------------------------------------------------------------------

  std::string l1b1_granule_id(int Index) const
  { range_check(Index, 0, number_image()); 
    return l1b1_granule_id_[Index];
  }

//-----------------------------------------------------------------------
/// Return geolocation stage.
//-----------------------------------------------------------------------

  std::string geolocation_stage(int Index) const
  { range_check(Index, 0, number_image()); 
    return geolocation_stage_[Index];
  }

//-----------------------------------------------------------------------
/// Return target type.
//-----------------------------------------------------------------------

  std::string target_type(int Index) const
  { range_check(Index, 0, number_image()); 
    return target_type_[Index];
  }

//-----------------------------------------------------------------------
/// Return min L1B1 line.
//-----------------------------------------------------------------------

  int min_l1b1_line(int Index) const
  { range_check(Index, 0, number_image()); 
    return min_l1b1_line_[Index];
  }

//-----------------------------------------------------------------------
/// Return max L1B1 line.
//-----------------------------------------------------------------------

  int max_l1b1_line(int Index) const
  { range_check(Index, 0, number_image()); 
    return max_l1b1_line_[Index];
  }

//-----------------------------------------------------------------------
/// Return view resolution. Note this is *metadata* passed in as a 
/// configuration, *not* the actual resolution of the l1b1 data on the
/// ground (use ImageGroundConnection resolution_meter or
/// footprint_resolution for that).
//-----------------------------------------------------------------------

  double view_resolution(int Index) const
  { range_check(Index, 0, number_image()); 
    return view_resolution_[Index];
  }

//-----------------------------------------------------------------------
/// Return maximum view resolution.
//-----------------------------------------------------------------------

  double max_view_resolution() const
  {
    return *(std::max_element(view_resolution_.begin(), 
			      view_resolution_.end()));
  }

//-----------------------------------------------------------------------
/// Return HDF chunk size that we've been requested to use for the L1B2
//-----------------------------------------------------------------------

  int l1b2_hdf_chunk_size_x() const { return l1b2_hdf_chunk_size_x_ ;}
  int l1b2_hdf_chunk_size_y() const { return l1b2_hdf_chunk_size_y_ ;}

  // We'll mess with parameter in a bit, for now leave this out.
private:
  // We do lazy evaluation, so allow this to be changed by 
  // image_ground_connection const function.
  mutable std::vector<boost::shared_ptr<ImageGroundConnection> > igc;
  boost::shared_ptr<Dem> dem;
  double dem_resolution;
  std::vector<int> view_number_;
  std::vector<std::string> view_name_, view_time_, l1b1_file_name_,
    l1b1_granule_id_, target_type_, geolocation_stage_;
  std::vector<double> view_resolution_;
  std::vector<int> min_l1b1_line_, max_l1b1_line_;
  std::string base_directory;
  int l1b2_hdf_chunk_size_x_;
  int l1b2_hdf_chunk_size_y_;
  AirMspiIgcCollection(const AirMspiIgcCollection& Original, 
		       const std::vector<int>& Index_set);
  int reference_row(const std::string& Instrument_config_file_name) const;

  // Temp stuff, we'll remove this in a bit.
  std::string config_filename_, orbit_filename_;
};
}

#endif
