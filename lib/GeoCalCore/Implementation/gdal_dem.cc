#include "gdal_dem.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GdalDem::save(Archive & ar, const unsigned int version) const
{
  unsigned int ntile = number_tile();
  int tile_number_line = number_line_per_tile();
  int tile_number_sample = number_sample_per_tile();
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP_(band_id)
    & GEOCAL_NVP2("number_tile", ntile)
    & GEOCAL_NVP(tile_number_line)
    & GEOCAL_NVP(tile_number_sample);
}

template<class Archive>
void GdalDem::load(Archive & ar, const unsigned int version)
{
  unsigned int ntile;
  int tile_number_line;
  int tile_number_sample;
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP_(band_id)
    & GEOCAL_NVP2("number_tile", ntile)
    & GEOCAL_NVP(tile_number_line)
    & GEOCAL_NVP(tile_number_sample);
  initialize(fname, datum_, band_id_, outside_dem_is_error_, ntile, tile_number_line,
	     tile_number_sample);
}

GEOCAL_IMPLEMENT(GdalDem);
#endif

//-----------------------------------------------------------------------
/// Constructor that reads an existing file to get the Dem data. The
/// default Datum used if none is supplied is DidDatum, which is relative
/// to WGS-84. If Outside_dem_is_error is false, we return a height of 0
/// outside of the range of the data, otherwise an exception is thrown
/// when an attempt is made to retrieve an height outside of the
/// range.
///
/// You can optionally pass the desired tile size. If you don't pass
/// this in, we use the blocking size of the underlying dataset.
//-----------------------------------------------------------------------

GdalDem::GdalDem(const std::string& Fname, const boost::shared_ptr<Datum>& D,
		 int Band_id, bool Outside_dem_is_error, 
		 int Number_tile, int Tile_number_line, int Tile_number_sample)
{
  initialize(Fname, D, Band_id, Outside_dem_is_error, Number_tile, Tile_number_line,
	     Tile_number_sample);
}

//-----------------------------------------------------------------------
/// Initialize data.
//-----------------------------------------------------------------------

void GdalDem::initialize(const std::string& Fname, const boost::shared_ptr<Datum>& D,
			 int Band_id, bool Outside_dem_is_error, 
			 int Number_tile, int Tile_number_line, int Tile_number_sample)
{
  fname = Fname;
  band_id_ = Band_id;
  switch(gdal_band_type(Fname, Band_id)) {
  case GDT_Byte:
    initialize_type<GByte>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize_type<GUInt16>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize_type<GInt16>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize_type<GUInt32>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize_type<GInt32>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize_type<float>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize_type<double>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
}


