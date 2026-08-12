#ifndef RSM_NITF_H
#define RSM_NITF_H
#include "rsm.h"
#include "image_ground_connection.h"

namespace GeoCal {
  void rsm_write_nitf(const std::string& Fname,
		      const boost::shared_ptr<Rsm>& R);
  boost::shared_ptr<Rsm> rsm_read_nitf(const std::string& Fname, int Naif_code);
  void glas_gfm_write_nitf(const std::string& Fname,
			   const boost::shared_ptr<ImageGroundConnection>& Igc);
  boost::shared_ptr<ImageGroundConnection>
  glas_gfm_read_nitf(const std::string& Fname, int Naif_code);
}
#endif
