// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "raster_image_tiled_file.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image_variable)
%import "tiled_file.i"
%geocal_shared_ptr(GeoCal::RasterImageTiledFile);

namespace GeoCal {
class RasterImageTiledFile : public RasterImageVariable {
public:
  RasterImageTiledFile(const boost::shared_ptr<TiledFileBase<2> >& D);
  RasterImageTiledFile(const boost::shared_ptr<TiledFileBase<2> >& D,
		       const MapInfo& Mi);
  %python_attribute2(tile_file_base, tile_file_base_ptr, 
		     boost::shared_ptr<TiledFileBase<2> >)
  void flush() const;
  %python_attribute(number_swap, unsigned int)
  %python_attribute(number_tile, int)
  void reset_number_swap();
};

}
// List of things "import *" will include
%python_export("RasterImageTiledFile")
