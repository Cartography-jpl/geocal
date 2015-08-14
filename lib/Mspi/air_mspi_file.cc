#include "air_mspi_file.h"
// This won't be needed once version 3 becomes the default for boost filesystem
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <cstdlib>

using namespace GeoCal;
// Define to print out diagnostic messages to make sure we are reading
// cached data where we expect to.
//#define AIR_MSPI_DIAGNOSTIC

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
  bool changed = false;
  if(getenv("MSPI_TEMP_DIR")) {
    boost::filesystem::path p(Fname);
    std::string bname = boost::filesystem::basename(p);
    std::string t = getenv("MSPI_TEMP_DIR");
    std::vector<std::string> sv;
    boost::split(sv, t, boost::is_any_of(":"), boost::token_compress_on);
    BOOST_FOREACH(const std::string& s, sv) {
      boost::filesystem::path p2(s);
      p2 /= p.filename();
      if(boost::filesystem::exists(p2)) {
	res = p2.string();
	changed = true;
	break;
      }
    }
  }
#ifdef  AIR_MSPI_DIAGNOSTIC
  if(getenv("MSPI_TEMP_DIR")) {
    std::cout << "MSPI_TEMP_DIR: " << std::string(getenv("MSPI_TEMP_DIR")) << "\n";
  } else {
    std::cout << "MSPI_TEMP_DIR is not defined\n";
  }
  if(changed)
    std::cout << "Mapped file " << Fname << " to " << res << "\n";
  else
    std::cout << "Did not map file " << Fname << " to a cached version.\n";
#endif
  return res;
}
