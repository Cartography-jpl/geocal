#ifndef AIRMSPI_IGC_COLLECTION_H
#define AIRMSPI_IGC_COLLECTION_H
#include "igc_collection.h"

namespace GeoCal {
/****************************************************************//**
  This is an IgcCollection for AirMspi.
*******************************************************************/

class AirMspiIgcCollection : public virtual IgcCollection {
public:
  AirMspiIgcCollection(const std::string& Master_config_file,
		       const std::string& Orbit_file_name,
		       const std::string& L1b1_table) {}
  virtual ~AirMspiIgcCollection() {}
  virtual void print(std::ostream& Os) const;
  virtual int number_image() const { return (int) igc.size(); }
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int> Index_set) const;

  // We'll mess with parameter in a bit, for now leave this out.

  // Add this in to fill in metadata
  // void fill_in_meta(MSPI::Shared::Meta)
private:
  // We do lazy evaluation, so allow this to be changed by 
  // image_ground_connection const function.
  mutable std::vector<boost::shared_ptr<ImageGroundConnection> > igc;
};
}

#endif
