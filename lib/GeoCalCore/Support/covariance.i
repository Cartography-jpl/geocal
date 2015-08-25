// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "covariance.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::Covariance);

namespace GeoCal {
class Covariance : public GenericObject {
public:
  Covariance();
  void add(double x1, double x2);
  %python_attribute(count, int)
  %python_attribute(mean1, double)
  %python_attribute(mean2, double)
  %python_attribute(covariance, double)
  std::string print_to_string() const;
  %pickle_serialization();
};
}

