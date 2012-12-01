#include "vicar_dem.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Initialize the class, once vicar_file_ has been filled in.
//-----------------------------------------------------------------------

void VicarDem::initialize(int Number_line_per_tile, int Number_tile,
			  const boost::shared_ptr<Datum>& D, 
			  bool Outside_dem_is_error)
{
  switch(vicar_file_->type()) {
  case VicarFile::VICAR_BYTE:
    initialize_t<VicarByte>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_HALF:
    initialize_t<VicarHalf>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_FULL:
    initialize_t<VicarFull>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_FLOAT:
    initialize_t<VicarFloat>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_DOUBLE:
    initialize_t<VicarDouble>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  }
}
