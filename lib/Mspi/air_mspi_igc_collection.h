#ifndef AIRMSPI_IGC_COLLECTION_H
#define AIRMSPI_IGC_COLLECTION_H
#include "igc_collection.h"
#include "air_mspi_igc.h"
#include "mspi_config_file.h"

namespace GeoCal {
/****************************************************************//**
  This is an IgcCollection for AirMspi.
*******************************************************************/

class AirMspiIgcCollection : public virtual IgcCollection, 
			     public virtual WithParameterNested {
public:
  AirMspiIgcCollection(const std::string& Master_config_file,
		       const std::string& Orbit_file_name,
		       const std::string& L1b1_table,
		       const std::string& Swath_to_use = "660-I",
		       const std::string& Base_directory = ".");
  AirMspiIgcCollection(const boost::shared_ptr<Orbit>& Orb,
   		       const boost::shared_ptr<MspiCamera>& Cam,
   		       const boost::shared_ptr<MspiGimbal>& Gim,
   		       const boost::shared_ptr<Dem>& D,
   		       const std::vector<std::string>& L1b1_file_name,
		       const std::string& Swath_to_use = "660-I",
		       int Dem_resolution = 10,
		       const std::string& Base_directory = ".");
  virtual ~AirMspiIgcCollection() {}
  void replace_view_config(const std::string& Master_config_file,
			   const std::string& L1b1_table);
  virtual void print(std::ostream& Os) const;
  virtual int number_image() const { return (int) view_config_.size(); }
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const
  { return air_mspi_igc(Image_index); }
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;


//-----------------------------------------------------------------------
/// Return specific orbit we are using.
//-----------------------------------------------------------------------

  boost::shared_ptr<Orbit> orbit(int Index) const
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
/// Change the orbit we are using.
//-----------------------------------------------------------------------

  void set_orbit(const boost::shared_ptr<Orbit>& Orb)
  {
    igc.clear();
    orbit_ = Orb;
    clear_object();
    add_object(camera_);
    add_object(gimbal_);
    add_object(orbit_);
  }

//-----------------------------------------------------------------------
/// Change the camera we are using.
//-----------------------------------------------------------------------

  void set_camera(const boost::shared_ptr<MspiCamera>& Cam)
  {
    igc.clear();
    camera_ = Cam;
    clear_object();
    add_object(camera_);
    add_object(gimbal_);
    add_object(orbit_);
  }

//-----------------------------------------------------------------------
/// Change the gimbal we are using.
//-----------------------------------------------------------------------

  void set_gimbal(const boost::shared_ptr<MspiGimbal>& Gim)
  {
    igc.clear();
    gimbal_ = Gim;
    clear_object();
    add_object(camera_);
    add_object(gimbal_);
    add_object(orbit_);
  }

//-----------------------------------------------------------------------
/// Return specific gimbal we are using, needed for some routines that
/// depend on the details of MspiGimbal.
//-----------------------------------------------------------------------

  boost::shared_ptr<MspiGimbal> gimbal(int Index) const
  {
    return air_mspi_igc(Index)->gimbal();
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

//-----------------------------------------------------------------------
/// Return the minimum line in the l1b1 file that we have all the data
/// to process. This looks at the both the l1b1 file itself and also
/// the coverage of the navigation file. It can also be shrunk by the
/// user supplying the range in the configuration files.
//-----------------------------------------------------------------------

  int min_l1b1_line(int Index) const
  {
    range_check(Index, 0, number_image());
    return min_l1b1_line_[Index];
  }

//-----------------------------------------------------------------------
/// Return the maximum line in the l1b1 file that we have all the data
/// to process. This looks at the both the l1b1 file itself and also
/// the coverage of the navigation file. It can also be shrunk by the
/// user supplying the range in the configuration files.
//-----------------------------------------------------------------------

  int max_l1b1_line(int Index) const
  {
    range_check(Index, 0, number_image());
    return max_l1b1_line_[Index];
  }
  int view_number_to_image_index(int View_number) const;
  virtual blitz::Array<double, 1> parameter() const
  { return WithParameterNested::parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { WithParameterNested::parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return WithParameterNested::parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { WithParameterNested::parameter_with_derivative(Parm);}
private:
  boost::shared_ptr<Dem> dem;
  double dem_resolution;
  boost::shared_ptr<MspiCamera> camera_;
  boost::shared_ptr<MspiGimbal> gimbal_;
  boost::shared_ptr<Orbit> orbit_;
  std::vector<MspiConfigFile> view_config_;
  std::string base_directory;
  std::string swath_to_use;
  std::vector<int> min_l1b1_line_, max_l1b1_line_;

  // We do lazy evaluation, so allow this to be changed by 
  // image_ground_connection const function.
  mutable std::vector<boost::shared_ptr<AirMspiIgc> > igc;
  boost::shared_ptr<AirMspiIgc> air_mspi_igc(int Index) const;

  void calc_min_max_l1b1_line();

  AirMspiIgcCollection(const AirMspiIgcCollection& Original, 
		       const std::vector<int>& Index_set);

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

  AirMspiIgcCollection() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(AirMspiIgcCollection);

#endif
