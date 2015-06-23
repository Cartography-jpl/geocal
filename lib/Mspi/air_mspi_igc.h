#ifndef AIRMSPI_IGC_H
#define AIRMSPI_IGC_H
#include "ipi_image_ground_connection.h" 
				// Definition of
				// IpiImageGroundConnection
#include "air_mspi_orbit.h"	// Definition of AirMspiOrbit.
#include "air_mspi_time_table.h" // Definition of AirMspiTimeTable.
#include "mspi_camera.h"	// Definition of MspiCamera.

namespace GeoCal {
/****************************************************************//**
  This is an ImageGroundConnection for AirMspi.
*******************************************************************/
class AirMspiIgc : public virtual IpiImageGroundConnection,
		   public virtual WithParameterNested {
public:
  AirMspiIgc(const std::string& Master_config_file,
	     const std::string& Orbit_file_name,
	     const std::string& L1b1_file_name,
	     const std::string& Swath_to_use = "660-I",
	     const std::string& Base_directory = ".",
	     const std::string& Title = "Image",
	     int Tile_number_line = -1,
	     int Tile_number_sample = -1, 
	     unsigned int Number_tile = 4);
  AirMspiIgc(const boost::shared_ptr<Orbit>& Orb,
	     const boost::shared_ptr<MspiCamera>& Cam,
	     const boost::shared_ptr<Dem>& Dem,
	     const std::string& L1b1_file_name,
	     const std::string& Swath_to_use = "660-I",
	     const std::string& Title = "Image",
	     int Dem_resolution = 10,
	     int Tile_number_line = -1,
	     int Tile_number_sample = -1, 
	     unsigned int Number_tile = 4);

//-----------------------------------------------------------------------
/// Return band number.
//-----------------------------------------------------------------------

  int band() const {return ipi().band();}
  void band(int B);

//-----------------------------------------------------------------------
/// Camera we are using.
//-----------------------------------------------------------------------

  boost::shared_ptr<MspiCamera> camera() const
  { return boost::dynamic_pointer_cast<MspiCamera>(ipi().camera_ptr()); }

//-----------------------------------------------------------------------
/// Orbit we are using.
//-----------------------------------------------------------------------

  boost::shared_ptr<AirMspiOrbit> orbit() const
  {
    return boost::dynamic_pointer_cast<AirMspiOrbit>(ipi().orbit_ptr());
  }

//-----------------------------------------------------------------------
/// TimeTable we are using.
//-----------------------------------------------------------------------

  boost::shared_ptr<AirMspiTimeTable> time_table() const
  {
    return boost::dynamic_pointer_cast<AirMspiTimeTable>(ipi().time_table_ptr());
  }

//-----------------------------------------------------------------------
/// The name of the l1b1 file
//-----------------------------------------------------------------------

  // We need to find out where this comes from. Possibly we will stash
  // the name somewhere.
  std::string l1b1_file_name() const
  { return "fake_name"; }

  virtual void print(std::ostream& Os) const;
  virtual blitz::Array<double, 1> parameter() const
  { return WithParameterNested::parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { WithParameterNested::parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return WithParameterNested::parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { WithParameterNested::parameter_with_derivative(Parm);}
private:
  AirMspiIgc() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(AirMspiIgc);
#endif

