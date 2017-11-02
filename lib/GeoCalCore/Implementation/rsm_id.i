// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_id.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%import "rsm_base.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmId);

namespace GeoCal {
class RsmId : public GenericObject {
public:
  RsmId(const boost::shared_ptr<RsmBase>& Base,
	const boost::shared_ptr<CoordinateConverter> &Cconv);
  std::string tre_string() const;
  static boost::shared_ptr<RsmId>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
