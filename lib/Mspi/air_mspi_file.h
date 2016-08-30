#ifndef AIRMSPI_FILE_H
#define AIRMSPI_FILE_H
#include <string>
// This won't be needed once version 3 becomes the default for boost filesystem
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

namespace GeoCal {

//-----------------------------------------------------------------------
/// Starting in boost 1.61, there is a lexically_normal
/// function. However we don't want to depend on that
/// new a version of boost, at least for now (2016). So we've copied
/// this function from boost 1.61, and massaged it a bit to be a free
/// function. This can go away in the future when boost 1.61 isn't
/// so new.
//-----------------------------------------------------------------------
  
  boost::filesystem::path lexically_normal(const boost::filesystem::path& p);
  std::string air_mspi_permanent_file_name(const std::string& Fname);
  std::string air_mspi_local_file_name(const std::string& Fname);
}
#endif
