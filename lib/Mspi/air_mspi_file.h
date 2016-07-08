#ifndef AIRMSPI_FILE_H
#define AIRMSPI_FILE_H
#include <string>

namespace GeoCal {
  std::string air_mspi_permanent_file_name(const std::string& Fname);
  std::string air_mspi_local_file_name(const std::string& Fname);
}
#endif
