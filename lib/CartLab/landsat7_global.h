#ifndef LANDSAT7_GLOBAL
#define LANDSAT7_GLOBAL
#include "cart_lab_multifile.h"
namespace GeoCal {
/****************************************************************//**
  This is used to read the global Landsat 7 data. The resolution
  varies by band. The The geolocation accuracy was estimated
  by Tom Logan at 1/3 pixel or so. Not sure how accurate this
  estimate is, perhaps Tom has a better estimate.

  The bands are the standard Landsat 7 bands:

   Band           Wavelength (micrometer)  Resolution (m)
   ------------  ------------------------  --------------
   Band 1 - Blue  0.45-0.52 	           30
   Band 2 - Green 0.52-0.60                30
   Band 3 - Red	  0.63-0.69                30
   Band 4 - Near Infrared (NIR)
                  0.77-0.90                30
   Band 5 - Shortwave Infrared (SWIR) 1
                  1.55-1.75                30
   Band 6 - Thermal
                  10.40-12.50              60
   Band 7 - Shortwave Infrared (SWIR) 2
                  2.09-2.35                30
   Band 8 - Panchromatic
                  .52-.90                  15

Band 6 has both a low and a high gain (Bands 61 and 62).

*******************************************************************/
class Landsat7Global: public VicarCartLabMultifile {
public:
  enum Band {BAND1, BAND2, BAND3, BAND4, BAND5, BAND61, BAND62, BAND7, BAND8};
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
