#ifndef LANDSAT7_PANCHROMATIC
#define LANDSAT7_PANCHROMATIC
#include "cart_lab_multifile.h"
namespace GeoCal {
/****************************************************************//**
  This is used to read the global Landsat 7 panchromatic band. This
  data has 15 meter resolution.
*******************************************************************/
class Landsat7Panchromatic: public GdalCartLabMultifile {
public:
  Landsat7Panchromatic(const std::string& Dir);
  virtual void print(std::ostream& Os) const
  { Os << "Landsat7Panchromatic";}
private:
  Landsat7Panchromatic() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Landsat7Panchromatic);
#endif
