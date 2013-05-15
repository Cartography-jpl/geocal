// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#include "raster_image_variable.h"
%}

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
