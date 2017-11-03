// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_indirect_covariance.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RsmIndirectCovariance);

namespace GeoCal {
class RsmIndirectCovariance : public GenericObject {
public:
  RsmIndirectCovariance();
  std::string tre_string() const;
  static boost::shared_ptr<RsmIndirectCovariance>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
