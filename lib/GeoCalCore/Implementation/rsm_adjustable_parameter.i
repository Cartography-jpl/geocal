// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_adjustable_parameter.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RsmAdjustableParameter);

namespace GeoCal {
class RsmAdjustableParameter : public GenericObject {
public:
  RsmAdjustableParameter();
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameter>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
