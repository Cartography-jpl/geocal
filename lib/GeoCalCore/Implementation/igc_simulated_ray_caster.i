// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_simulated_ray_caster.h"
%}
%base_import(calc_raster)
%import "image_ground_connection.i"
%import "raster_image.i"
%import "image_coordinate.i"
%import "map_info.i"
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::IgcSimulatedRayCaster);
namespace GeoCal {
class IgcSimulatedRayCaster : public CalcRaster {
public:
  IgcSimulatedRayCaster(const boost::shared_ptr<ImageGroundConnection>& Igc,
	      const boost::shared_ptr<RasterImage>& Map_projected_image, 
              int Number_integration_step = 1,
              double Raycast_resolution = -1,
              double Max_height = 10e3,
              int Number_tile_line = -1,
	      int Number_tile_sample = -1,
	      double Fill_value = 0.0
	      );
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

  %python_attribute(igc, boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(number_integration_step, int)
  %python_attribute(fill_value, double)
  %python_attribute(max_height, double)
  %python_attribute(raycast_resolution, double)
  %python_attribute(map_projected_image, boost::shared_ptr<RasterImage>)
  %pickle_serialization()
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
// List of things "import *" will include
%python_export("IgcSimulatedRayCaster")
