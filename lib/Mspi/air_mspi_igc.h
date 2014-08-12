#ifndef AIRMSPI_IGC_H
#define AIRMSPI_IGC_H
#include "ipi_image_ground_connection.h" 
				// Definition of IpiImageGroundConnection
namespace GeoCal {
/****************************************************************//**
  This is an ImageGroundConnection for AirMspi.
*******************************************************************/
class AirMspiIgc : public IpiImageGroundConnection {
public:
  AirMspiIgc(const std::string& Master_config_file,
	     const std::string& Base_directory = ".");

//-----------------------------------------------------------------------
/// The base directory that various files in the master config file
/// are relative to.
//-----------------------------------------------------------------------

  const std::string& base_directory() const {return bdir; }

//-----------------------------------------------------------------------
/// The name of the master config file used to create this object.
//-----------------------------------------------------------------------
  
  const std::string& master_config_file() const { return mconfig; }

  virtual void print(std::ostream& Os) const;
private:
  std::string bdir, mconfig;
};
}
#endif

