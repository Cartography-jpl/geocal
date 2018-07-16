#ifndef GDAL_DATUM_H
#define GDAL_DATUM_H
#include "gdal_raster_image.h"
#include "geocal_datum.h"

namespace GeoCal {
/****************************************************************//**
  This uses a GdalRasterImage to provide a datum. This file 
  should have the offset from the datum to reference ellipsoid
  (e.g., WGS-84).
*******************************************************************/

class GdalDatum : public Datum {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  GdalDatum(const std::string& Fname, int Band_id = 1, int
	    Number_tile = 4, int Tile_number_line = -1,
	    int Tile_number_sample = -1)
    : map_file_(new GdalRasterImage(Fname, Band_id, Number_tile, false, 
				   Tile_number_line, Tile_number_sample))
  { }

  GdalDatum(const boost::shared_ptr<GdalRasterImage>& Map_file)
    : map_file_(Map_file) 
  {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GdalDatum() {}

//-----------------------------------------------------------------------
/// Undulation, which is the distance form mean sea level to the
/// reference ellipsoid, for the given ground location. This is in
/// meters. 
//-----------------------------------------------------------------------

  virtual double undulation(const GroundCoordinate& Gc) const
  { return map_file_->interpolate(map_file_->coordinate(Gc)); }
  virtual double undulation(const Geodetic& Gc) const
  { return map_file_->interpolate(map_file_->coordinate(Gc)); }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "GDAL Datum:\n"
       << "  Map file: " << *map_file_ << "\n";
  }

  boost::shared_ptr<GdalRasterImage> map_file() const {return map_file_;}
protected:
  GdalDatum() {}
private:
  boost::shared_ptr<GdalRasterImage> map_file_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(GdalDatum);
#endif
