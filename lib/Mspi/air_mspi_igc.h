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
class AirMspiIgc : public virtual IpiImageGroundConnection {
public:
  AirMspiIgc(const std::string& Master_config_file,
	     const std::string& Orbit_file_name,
	     const std::string& L1b1_file_name,
	     int Band,
	     const std::string& Base_directory = ".",
	     const std::string& Title = "Image");
  AirMspiIgc(const boost::shared_ptr<Orbit>& Orb,
	     const boost::shared_ptr<Camera>& Cam,
	     const boost::shared_ptr<Dem>& Dem,
	     const std::string& L1b1_file_name,
	     const std::string& Instrument_config_file,
	     int Band,
	     const std::string& Title = "Image",
	     int Dem_resolution = 10);

//-----------------------------------------------------------------------
/// Return band number.
//-----------------------------------------------------------------------

  int band() const {return ipi().band();}

//-----------------------------------------------------------------------
/// Set band that we are using.
//-----------------------------------------------------------------------

  void band(int B) { ipi_ptr()->band(B); }

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
private:
  AirMspiIgc() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(AirMspiIgc);
#endif

