// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "location_to_file.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::LocationToFile);
namespace GeoCal {
class LocationToFile: public GenericObject {
  LocationToFile() {}
  void add(int x1, int y1, int x2, int y2, const std::string& fname);
  std::string find(int x, int y) const;
  std::vector<std::string> find_region(int x, int y, int num_x_pixel,
				       int num_y_pixel) const;
  std::string print_to_string() const;
};
}
