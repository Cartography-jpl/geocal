#include "gdal_dem.h"

using namespace GeoCal;

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
  : fname(Fname), band_id_(Band_id)
{
  switch(gdal_band_type(Fname, Band_id)) {
  case GDT_Byte:
    initialize<GByte>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize<GUInt16>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize<GInt16>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize<GUInt32>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize<GInt32>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize<float>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize<double>(Fname, D, Band_id, Number_tile, Outside_dem_is_error,
		      Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
}


