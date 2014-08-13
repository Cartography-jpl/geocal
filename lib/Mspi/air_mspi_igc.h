#ifndef AIRMSPI_IGC_H
#define AIRMSPI_IGC_H
#include "ipi_image_ground_connection.h" 
				// Definition of
				// IpiImageGroundConnection
#include "air_mspi_orbit.h"	// Definition of AirMspiOrbit.

namespace GeoCal {
/****************************************************************//**
  This is an ImageGroundConnection for AirMspi.
*******************************************************************/
class AirMspiIgc : public IpiImageGroundConnection {
public:
  AirMspiIgc(const std::string& Master_config_file,
	     const std::string& Orbit_file_name,
	     const std::string& L1b1_file_name,
	     int Band,
	     const std::string& Base_directory = ".");

//-----------------------------------------------------------------------
/// Return band number.
//-----------------------------------------------------------------------

  int band() const {return ipi().band();}

//-----------------------------------------------------------------------
/// The base directory that various files in the master config file
/// are relative to.
//-----------------------------------------------------------------------

  const std::string& base_directory() const {return bdir; }

//-----------------------------------------------------------------------
/// The name of the master config file used to create this object.
//-----------------------------------------------------------------------
  
  const std::string& master_config_file() const { return mconfig; }

//-----------------------------------------------------------------------
/// Orbit we are using.
//-----------------------------------------------------------------------

  boost::shared_ptr<AirMspiOrbit> orbit() const
  {
    return boost::dynamic_pointer_cast<AirMspiOrbit>(ipi().orbit_ptr());
  }

//-----------------------------------------------------------------------
/// The name of the orbit file
//-----------------------------------------------------------------------

  std::string orbit_file_name() const
  { return orbit()->file_name(); }

//-----------------------------------------------------------------------
/// The name of the l1b1 file
//-----------------------------------------------------------------------

  // We need to find out where this comes from. Possibly we will stash
  // the name somewhere.
  std::string l1b1_file_name() const
  { return "fake_name"; }

  virtual void print(std::ostream& Os) const;
private:
  std::string bdir, mconfig;

  int reference_row(const std::string& Instrument_config_file_name) const;
};
}
#endif

