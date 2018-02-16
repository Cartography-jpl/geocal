#ifndef LANDSAT7_GLOBAL
#define LANDSAT7_GLOBAL
#include "cart_lab_multifile.h"
namespace GeoCal {
/****************************************************************//**
  This is used to read the global Landsat 7 data. The resolution
  varies by band. The The geolocation accuracy was estimated
  by Tom Logan at 1/3 pixel or so. Not sure how accurate this
  estimate is, perhaps Tom has a better estimate.
*******************************************************************/
class Landsat7Global: public VicarCartLabMultifile {
public:
  enum Band {BAND62};
  Landsat7Global(const std::string& Dir, Band B);
  virtual ~Landsat7Global() {}
  virtual void print(std::ostream& Os) const;
  Band band() const { return band_;}
private:
  Band band_;
  Landsat7Global() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Landsat7Global);
#endif
