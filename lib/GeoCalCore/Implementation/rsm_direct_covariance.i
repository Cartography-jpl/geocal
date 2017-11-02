// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_direct_covariance.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RsmDirectCovariance);

namespace GeoCal {
class RsmDirectCovariance : public GenericObject {
public:
  RsmDirectCovariance();
  std::string tre_string() const;
  static boost::shared_ptr<RsmDirectCovariance>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
