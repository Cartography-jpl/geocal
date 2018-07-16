#ifndef LANDSAT7_PANCHROMATIC
#define LANDSAT7_PANCHROMATIC
#include "cart_lab_multifile.h"
namespace GeoCal {
/****************************************************************//**
  This is used to read the global Landsat 7 panchromatic band. This
  data has 15 meter resolution. The geolocation accuracy was estimated
  by Tom Logan at 1/3 pixel or so (so 5m). Not sure how accurate this
  estimate is, perhaps Tom has a better estimate.

  Note that this is very similar to Landsat7Global. The
  difference is that this was used for MSPI, where the data was
  supplied as geotiff rather that the VICAR format we use in the
  cartlab. Could probably combine these into one class with some
  extra options, but doesn't seem to be much point in doing that.
*******************************************************************/
class Landsat7Panchromatic: public GdalCartLabMultifile {
public:
  Landsat7Panchromatic(const std::string& Dir);
  virtual ~Landsat7Panchromatic() {}
  virtual void print(std::ostream& Os) const
  { Os << "Landsat7Panchromatic";}
protected:
  Landsat7Panchromatic() {}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Landsat7Panchromatic);
#endif
