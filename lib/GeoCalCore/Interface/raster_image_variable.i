// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "raster_image_variable.h"
%}
%base_import(raster_image)

%geocal_shared_ptr(GeoCal::RasterImageVariable);

namespace GeoCal {
class RasterImageVariable : public RasterImage {
protected:
  RasterImageVariable();
  RasterImageVariable(int Nline, int Nsamp);
  RasterImageVariable(int Nline, int Nsamp, int Ntileln, Ntilesmp);
  RasterImageVariable(const MapInfo& Mi);
  RasterImageVariable(const RasterImage& Img);
  int number_line_, number_sample_, number_tile_line_, number_tile_sample_;
  boost::shared_ptr<MapInfo> map_info_;
  void remove_rpc();
};
}
// List of things "import *" will include
%python_export("RasterImageVariable")
