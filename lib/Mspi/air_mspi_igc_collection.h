#ifndef AIRMSPI_IGC_COLLECTION_H
#define AIRMSPI_IGC_COLLECTION_H
#include "igc_collection.h"

namespace GeoCal {
/****************************************************************//**
  This is an IgcCollection for AirMspi.
*******************************************************************/

class AirMspiIgcCollection : public virtual IgcCollection {
public:
  AirMspiIgcCollection() {}
  virtual ~AirMspiIgcCollection() {}
  virtual void print(std::ostream& Os) const;
  virtual int number_image() const { return (int) igc.size(); }
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int> Index_set) const;

  // We'll mess with parameter in a bit, for now leave this out.
private:
  // We evaluate this lazy, so allow this to be changed by 
  // image_ground_connection.
  mutable std::vector<boost::shared_ptr<ImageGroundConnection> > igc;
};
}

#endif
