#ifndef AIRMSPI_IGC_COLLECTION_H
#define AIRMSPI_IGC_COLLECTION_H
#include "igc_collection.h"
#include "air_mspi_igc.h"
#include "mspi_config_file.h"

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
  virtual int number_image() const { return (int) view_config_.size(); }
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;


//-----------------------------------------------------------------------
/// Return specific orbit we are using, needed for some routines that
/// depend on the details of AirMspiOrbit.
//-----------------------------------------------------------------------

  boost::shared_ptr<AirMspiOrbit> orbit(int Index) const
  {
    return air_mspi_igc(Index)->orbit();
  }

//-----------------------------------------------------------------------
/// Return specific camera we are using, needed for some routines that
/// depend on the details of MspiCamera.
//-----------------------------------------------------------------------

  boost::shared_ptr<MspiCamera> camera(int Index) const
  {
    return air_mspi_igc(Index)->camera();
  }

//-----------------------------------------------------------------------
/// Return specific time table we are using, needed for some routines that
/// depend on the details of the time table..
//-----------------------------------------------------------------------

  boost::shared_ptr<TimeTable> time_table(int Index) const
  {
    return air_mspi_igc(Index)->time_table();
  }

//-----------------------------------------------------------------------
/// Return number of bands.
//-----------------------------------------------------------------------

  int number_band(int Index) const {return camera(Index)->number_band(); }

//-----------------------------------------------------------------------
/// Return band number.
//-----------------------------------------------------------------------

  int band(int Index) const {return air_mspi_igc(Index)->band();}

//-----------------------------------------------------------------------
/// Set band that we are using.
//-----------------------------------------------------------------------

  void band(int Index, int B) { air_mspi_igc(Index)->band(B); }


//-----------------------------------------------------------------------
/// Do we have keyword in configuration value given view number?
//-----------------------------------------------------------------------

  bool have_config(int Index, const std::string& Keyword) const
  {
    range_check(Index, 0, number_image());
    return view_config_[Index].have_key(Keyword);
  }

//-----------------------------------------------------------------------
/// Configuration value for given view number.
//-----------------------------------------------------------------------

  template<class T> T config_value(int Index, const std::string& Keyword) const
  {
    range_check(Index, 0, number_image());
    return view_config_[Index].value<T>(Keyword);
  }

//-----------------------------------------------------------------------
/// Return L1B1 file name.
//-----------------------------------------------------------------------

  std::string l1b1_file_name(int Index) const
  {
    range_check(Index, 0, number_image()); 
    return file_name(config_value<std::string>(Index, "l1b1_file"));
  }

  // We'll mess with parameter in a bit, for now leave this out.
private:
  // We do lazy evaluation, so allow this to be changed by 
  // image_ground_connection const function.
  mutable std::vector<boost::shared_ptr<ImageGroundConnection> > igc;
  boost::shared_ptr<Dem> dem;
  double dem_resolution;
  std::vector<MspiConfigFile> view_config_;
  std::vector<int> min_l1b1_line_, max_l1b1_line_;
  std::string base_directory;
  AirMspiIgcCollection(const AirMspiIgcCollection& Original, 
		       const std::vector<int>& Index_set);
  int reference_row(const std::string& Instrument_config_file_name) const;

//-----------------------------------------------------------------------
/// Get a file name, possibly adding the base_directory if it is a
/// relative path.
//-----------------------------------------------------------------------

  std::string file_name(const std::string& F) const
  {
    std::string res = F;
    if(res[0] != '/')
      res = base_directory + "/" + res;
    return res;
  }

  // Temp stuff, we'll remove this in a bit.
  std::string config_filename_, orbit_filename_;
  // Probably temporary
  boost::shared_ptr<AirMspiIgc> air_mspi_igc(int Index) const
  {
    return boost::dynamic_pointer_cast<AirMspiIgc>(image_ground_connection(Index));
  }

  AirMspiIgcCollection() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(AirMspiIgcCollection);

#endif
