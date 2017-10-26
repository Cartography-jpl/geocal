#ifndef RSM_ADJUSTABLE_PARAMETER_H
#define RSM_ADJUSTABLE_PARAMETER_H
#include "printable.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This provides additional information about the Rsm, it is what is
  used to populate 
*******************************************************************/

class RsmAdjustableParameter : public Printable<RsmAdjustableParameter> {
public:
  RsmAdjustableParameter()
  {
  }
  virtual ~RsmAdjustableParameter() {}
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameter>
  read_tre_string(const std::string& Tre_in);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmAdjustableParameter);
#endif
