#ifndef MOLA_DEM_FILE_H
#define MOLA_DEM_FILE_H
#include "gdal_dem.h"

namespace GeoCal {
/****************************************************************//**
  This handles a Mola DEM tile. These are bit complicated, because the
  "t" or topographic files are relative to a Mars Areoid that we don't
  currently have access to a the "r" files are a radius. We may come
  back to this and just put the MarsAreoid in as Datum, but for now
  just use the "r" files and present this data as a elevation relative
  to our reference ellipsoid.
*******************************************************************/

class MolaDemFile : public GdalDem {
public:
  MolaDemFile(const std::string& Fname, bool Outside_dem_is_error = false,
	      int Number_tile = 4, int Tile_number_line = -1,
	      int Tile_number_sample = -1)
    : GdalDem(Fname, boost::shared_ptr<Datum>(new NoDatum()), 1,
	      Outside_dem_is_error, Number_tile, Tile_number_line,
	      Tile_number_sample)
  {
  }
  virtual ~MolaDemFile() {}
  virtual void print(std::ostream& Os) const 
  { 
    Os << "MOLA Dem File:\n"
       << "  File:    " << file_name() << "\n"
       << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const;
protected:
  MolaDemFile() {}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(MolaDemFile);
#endif
