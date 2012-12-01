// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "gdal_datum.h"
%}


%geocal_shared_ptr(GdalDatum);
namespace GeoCal {
class GdalDatum : public Datum {
public:
  GdalDatum(const std::string& Fname, int Band_id = 1, int
	    Number_tile = 4, int Tile_number_line = -1,
	    int Tile_number_sample = -1);
  GdalDatum(const boost::shared_ptr<GdalRasterImage>& Map_file);
  virtual double undulation(const GroundCoordinate& Gc) const;
  %python_attribute(map_file, boost::shared_ptr<GdalRasterImage>)
  %pickle_init(self.map_file)
};

}
