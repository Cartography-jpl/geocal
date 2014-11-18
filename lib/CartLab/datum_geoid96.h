#ifndef DATUM_GEOID96_H
#define DATUM_GEOID96_H
#include "geocal_datum.h"
#include "vicar_raster_image.h"
namespace GeoCal {
/****************************************************************//**
  This class read the EGM 96 geoid data to provide a Datum.
*******************************************************************/

class DatumGeoid96 : public Datum {
public:
  DatumGeoid96(const std::string& Fname = "");
  virtual ~DatumGeoid96() {}
  const std::string& file_name() const {return fname;}
  virtual double undulation(const GroundCoordinate& Gc) const;
  virtual double undulation(const Geodetic& Gc) const;
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<VicarRasterImage> data;
  std::string fname;
};

}
#endif
