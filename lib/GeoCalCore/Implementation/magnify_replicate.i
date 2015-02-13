// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "magnify_replicate.h"
%}
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::MagnifyReplicate);

namespace GeoCal {
class MagnifyReplicate : public CalcRaster {
public:
  MagnifyReplicate(const boost::shared_ptr<RasterImage>& Data,
		   int Magfactor, int Number_tile = 4);
  // Note that this convoluted looking expression is necessary to 
  // get SWIG to override correctly with use of director feature in
  // the base class
  %rename(_v_number_line) number_line;
  %rename(_v_number_sample) number_sample;
  %rename(_v_number_tile_line) number_tile_line;
  %rename(_v_number_tile_sample) number_tile_sample;
  %rename(_v_has_map_info) has_map_info;
  %rename(_v_map_info) map_info;
  %rename(_v_grid_center_line_resolution) grid_center_line_resolution;
  %rename(_v_grid_center_sample_resolution) grid_center_sample_resolution;
  %rename(_v_has_rpc) has_rpc;
  %rename(_v_rpc) rpc_ptr;
  virtual int number_line();
  virtual int number_sample();
  virtual int number_tile_line();
  virtual int number_tile_sample();
  virtual bool has_map_info();
  virtual MapInfo map_info();
  virtual double grid_center_line_resolution();
  virtual double grid_center_sample_resolution();
  virtual bool has_rpc();
  virtual boost::shared_ptr<Rpc> rpc_ptr();
  %python_attribute2(raw_data, raw_data_ptr, boost::shared_ptr<RasterImage>)
  %python_attribute(magfactor, int)
  %pickle_init(1, self.raw_data, self.magfactor, self.number_tile)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
