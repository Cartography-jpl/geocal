#ifndef GEOCAL_TEMP_FILE_H
#define GEOCAL_TEMP_FILE_H
#include "generic_object.h"
#include <string>
#include <boost/filesystem.hpp>

namespace GeoCal {
/****************************************************************//**
  Simple class for creating a temporary file. The file gets
  automatically removed when this class goes out of scope.
*******************************************************************/
  
class GeoCalTempFile : public GenericObject {
public:  
  GeoCalTempFile()
  {
    temp_f = boost::filesystem::temp_directory_path();
    temp_f /= boost::filesystem::unique_path();
  }
  virtual ~GeoCalTempFile()
  {
    boost::filesystem::remove(temp_f);
  }
  const std::string& temp_fname() const {return temp_f.string();}
private:
  boost::filesystem::path temp_f;
};
}
#endif
