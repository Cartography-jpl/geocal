// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_indirect_covariance.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RsmIndirectCovariance);

%feature("director") RsmIndirectCovariance;

namespace GeoCal {
%nodefaultctor RsmIndirectCovariance;
class RsmIndirectCovariance : public GenericObject {
public:
  // RsmIndirectCovariance(const std::string& Image_identifier="",
  // 		      const std::string& Rsm_support_data_edition="fake-1",
  // 		      const std::string& Triangulation_id="");
  std::string base_tre_string() const;
  virtual std::string desc() const;
  void base_read_tre_string(std::istream& In);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_support_data_edition, std::string);
  %python_attribute_with_set(triangulation_id, std::string);
  %python_attribute_with_set(naif_code, int);
  virtual std::string print_to_string() const;
  %pickle_serialization()
};
}

// This class is abstract, and it isn't clear why we originally had
// this as a directory. Skip serialization for now, we can come back
// to this if needed. Could just add a "NotImplemented" error for the
// pure virtual functions if needed, but for now we'll just set this aside.


// Extra code for handling boost serialization/python pickle of
// director classes
//%geocal_director_serialization(rsm_indirect_covariance, RsmIndirectCovariance)

// List of things "import *" will include
%python_export("RsmIndirectCovariance")
