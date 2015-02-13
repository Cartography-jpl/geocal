// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "igc_map_projected.h"
%}
%base_import(calc_raster)
%base_import(calc_raster_multi_band)
%import "image_ground_connection.i"
%import "raster_image.i"
%import "map_info.i"
%geocal_shared_ptr(GeoCal::IgcMapProjected);
%geocal_shared_ptr(GeoCal::IgcMapProjectedMultiBand);
namespace GeoCal {
class IgcMapProjected : public CalcRaster {
public:
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  int Grid_spacing = 1,
		  int Avg_fact = -1,
		  bool Read_into_memory = true,
		  int Number_tile_line = -1,
		  int Number_tile_sample = -1,
		  double Fill_value = 0.0);
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

  %python_attribute(igc_original, boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(avg_factor, int)
  %python_attribute(grid_spacing, int)
  %python_attribute(fill_value, double)
  %python_attribute(read_into_memory, bool)
  %pickle_init(2, self.map_info, self.igc_original, self.grid_spacing,
	       self.avg_factor, self.read_into_memory, self.number_tile_line,
	       self.number_tile_sample, self.fill_value)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

class IgcMapProjectedMultiBand : public CalcRasterMultiBand {
public:
  IgcMapProjectedMultiBand(const MapInfo& Mi, 
			   const boost::shared_ptr<ImageGroundConnection>& Igc,
			   int Grid_spacing = 1,
			   int Avg_fact = -1,
			   bool Read_into_memory = true,
			   int Number_tile_line = -1,
			   int Number_tile_sample = -1,
			   double Fill_value = 0.0);
  %python_attribute(igc_original, boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(avg_factor, int)
  %python_attribute(grid_spacing, int)
  %python_attribute(fill_value, double)
  %python_attribute(read_into_memory, bool)
  %pickle_init(2, self.raster_image(0).map_info, 
	       self.igc_original, self.grid_spacing,
	       self.avg_factor, self.read_into_memory, 
	       self.raster_image(0).number_tile_line,
	       self.raster_image(0).number_tile_sample,
	       self.fill_value)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};
}
