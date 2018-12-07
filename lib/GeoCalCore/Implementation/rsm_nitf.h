#ifndef RSM_NITF_H
#define RSM_NITF_H
#include "rsm.h"

namespace GeoCal {
  void rsm_write_nitf(const std::string& Fname,
		      const boost::shared_ptr<Rsm>& R);
  boost::shared_ptr<Rsm> rsm_read_nitf(const std::string& Fname);
}
#endif
