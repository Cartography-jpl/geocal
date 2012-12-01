// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "location_to_file.h"
%}

%geocal_shared_ptr(LocationToFile);
namespace GeoCal {
class LocationToFile {
  LocationToFile() {}
  void add(int x1, int y1, int x2, int y2, const std::string& fname);
  std::string find(int x, int y) const;
  std::string print_to_string() const;
};
}
