#ifndef RSM_DIRECT_COVARIANCE_H
#define RSM_DIRECT_COVARIANCE_H
#include "printable.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This provides additional information about the Rsm, it is what is
  used to populate 
*******************************************************************/

class RsmDirectCovariance : public Printable<RsmDirectCovariance> {
public:
  RsmDirectCovariance()
  {
  }
  virtual ~RsmDirectCovariance() {}
  std::string tre_string() const;
  static boost::shared_ptr<RsmDirectCovariance>
  read_tre_string(const std::string& Tre_in);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmDirectCovariance);
#endif
