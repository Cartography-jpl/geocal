#ifndef MSP_SUPPORT_H
#define MSP_SUPPORT_H
#include "ecr.h"
#include "image_coordinate.h"
#include <string>

namespace GeoCal {
/****************************************************************//**
  Exception thrown if MSP isn't available.
*******************************************************************/

class MspNotAvailableException : public Exception {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  MspNotAvailableException() : Exception("Geocal library wasn't configured to use MSP library during installation, so can't call MSP functions.")
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MspNotAvailableException() throw() {}
};

bool have_msp_supported();
Ecr msp_terrain_point(const std::string& Fname, const ImageCoordinate& Ic);
void msp_print_plugin_list();
void msp_register_plugin(const std::string& Plugin_name);  
}
#endif

