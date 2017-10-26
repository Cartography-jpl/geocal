#ifndef RSM_INDIRECT_COVARIANCE_H
#define RSM_INDIRECT_COVARIANCE_H
#include "printable.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This provides additional information about the Rsm, it is what is
  used to populate 
*******************************************************************/

class RsmIndirectCovariance : public Printable<RsmIndirectCovariance> {
public:
  RsmIndirectCovariance()
  {
  }
  virtual ~RsmIndirectCovariance() {}
  std::string tre_string() const;
  static boost::shared_ptr<RsmIndirectCovariance>
  read_tre_string(const std::string& Tre_in);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmIndirectCovariance);
#endif
