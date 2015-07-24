#include "air_mspi_file.h"
// This won't be needed once version 3 becomes the default for boost filesystem
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <cstdlib>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// AirMSPI uses a file caching mechanism to speed up the processing
/// by copying files to a local disk before running executables. What
/// this means in practice is that a file can either be found in its
/// original location, or possibly in the directory pointed to by
/// MSPI_TEMP_DIR environment variable. This routine handles the logic
/// of this, either returning the file name passed in or a local
/// cached version if it is available.
//-----------------------------------------------------------------------

std::string GeoCal::air_mspi_true_file_name(const std::string& Fname)
{
  std::string res = Fname;
  if(getenv("MSPI_TEMP_DIR")) {
    boost::filesystem::path p(Fname);
    std::string bname = boost::filesystem::basename(p);
    boost::filesystem::path p2(std::string(getenv("MSPI_TEMP_DIR")));
    p2 /= p.filename();
    if(boost::filesystem::exists(p2)) {
      res = p2.string();
    }
  }
  return res;
}
