#ifndef RSM_ID_H
#define RSM_ID_H
#include "rsm_base.h"

namespace GeoCal {
/****************************************************************//**
  This provides additional information about the Rsm, it is what is
  used to populate 
*******************************************************************/

class RsmId : public Printable<RsmId> {
public:
  RsmId(const boost::shared_ptr<RsmBase>& Base,
	const boost::shared_ptr<CoordinateConverter> &Cconv)
  {
  }
  virtual ~RsmId() {}
  std::string tre_string() const;
  static boost::shared_ptr<RsmId>
  read_tre_string(const std::string& Tre_in);
private:
  RsmId() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmId);
#endif
