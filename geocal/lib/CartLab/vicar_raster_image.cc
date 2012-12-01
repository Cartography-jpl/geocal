#include "vicar_raster_image.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Initialize the class, once vicar_file_ has been filled in.
//-----------------------------------------------------------------------

void VicarRasterImage::initialize(int Number_line_per_tile, int Number_tile)
{
  switch(vicar_file_->type()) {
  case VicarFile::VICAR_BYTE:
    initialize_t<VicarByte>(Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_HALF:
    initialize_t<VicarHalf>(Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_FULL:
    initialize_t<VicarFull>(Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_FLOAT:
    initialize_t<VicarFloat>(Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_DOUBLE:
    initialize_t<VicarDouble>(Number_line_per_tile, Number_tile);
    break;
  }
}
