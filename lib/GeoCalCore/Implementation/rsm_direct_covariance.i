// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_direct_covariance.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RsmDirectCovariance);

namespace GeoCal {
%nodefaultctor RsmDirectCovariance;
class RsmDirectCovariance : public GenericObject {
public:
  // RsmDirectCovariance(const std::string& Image_identifier="",
  // 		      const std::string& Rsm_support_data_edition="fake-1",
  // 		      const std::string& Triangulation_id="");
  std::string base_tre_string() const;
  void base_read_tre_string(std::istream& In);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_support_data_edition, std::string);
  %python_attribute_with_set(triangulation_id, std::string);
  %python_attribute_with_set(naif_code, int);
  virtual std::string print_to_string() const;
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("RsmDirectCovariance")
