#include "vicar_raster_image.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Initialize the class, once vicar_file_ has been filled in.
//-----------------------------------------------------------------------

void VicarRasterImage::initialize(int Band_id, int Number_line_per_tile, 
				  int Number_tile)
{
  switch(vicar_file_->type()) {
  case VicarFile::VICAR_BYTE:
    initialize_t<VicarByte>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_HALF:
    initialize_t<VicarHalf>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_FULL:
    initialize_t<VicarFull>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_FLOAT:
    initialize_t<VicarFloat>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_DOUBLE:
    initialize_t<VicarDouble>(Band_id, Number_line_per_tile, Number_tile);
    break;
  }
}
