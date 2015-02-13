// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "raster_averaged.h"
%}
%base_import(calc_raster)
%base_import(calc_raster_multi_band)
%base_import(image_mask)
%base_import(image_ground_connection)
%geocal_shared_ptr(GeoCal::RasterAveraged);
%geocal_shared_ptr(GeoCal::RasterAveragedMultiBand);
%geocal_shared_ptr(GeoCal::ImageMaskAveraged);
%geocal_shared_ptr(GeoCal::AveragedImageGroundConnection);

namespace GeoCal {
class RasterAveraged : public CalcRaster {
public:
  RasterAveraged(const boost::shared_ptr<RasterImage>& Data,
		 int Number_line_per_pixel, 
		 int Number_sample_per_pixel,
		 bool Ignore_zero = false);
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
  %python_attribute2(high_resolution_image, high_resolution_image_ptr,
		     boost::shared_ptr<RasterImage>)
  %python_attribute(number_line_per_pixel, int)
  %python_attribute(number_sample_per_pixel, int)
  %python_attribute(ignore_zero, bool)
  %pickle_init(1, self.high_resolution_image,
	       self.number_line_per_pixel, self.number_sample_per_pixel,
	       self.ignore_zero)
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

class RasterAveragedMultiBand : public CalcRasterMultiBand {
public:
  RasterAveragedMultiBand(const boost::shared_ptr<RasterImageMultiBand>& Data,
		 int Number_line_per_pixel, 
		 int Number_sample_per_pixel,
		 bool Ignore_zero = false);
  %python_attribute2(high_resolution_image, high_resolution_image_ptr,
		     boost::shared_ptr<RasterImageMultiBand>)
  %python_attribute(number_line_per_pixel, int)
  %python_attribute(number_sample_per_pixel, int)
  %python_attribute(ignore_zero, bool)
  %pickle_init(1, self.high_resolution_image,
	       self.number_line_per_pixel, self.number_sample_per_pixel,
	       self.ignore_zero)
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

class ImageMaskAveraged : public ImageMask {
public:
  ImageMaskAveraged(const boost::shared_ptr<ImageMask>& Data,
		    int Number_line_per_pixel,
		    int Number_sample_per_pixel);
  %python_attribute2(high_resolution_image_mask, high_resolution_image_mask_ptr,
		     boost::shared_ptr<ImageMask>)
  %python_attribute(number_line_per_pixel, int)
  %python_attribute(number_sample_per_pixel, int)
  virtual bool mask(int Line, int Sample) const;
  %pickle_init(1, self.high_resolution_image_mask,
	       self.number_line_per_pixel, 
	       self.number_sample_per_pixel)
};

class AveragedImageGroundConnection: public ImageGroundConnection {
public:
  AveragedImageGroundConnection
  (const boost::shared_ptr<ImageGroundConnection> Igc,
   int Number_line_per_pixel, 
   int Number_sample_per_pixel,
   bool In_memory = false,
   bool Ignore_zero = false);
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute(original_image_ground_connection, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(number_line_per_pixel, int)
  %python_attribute(number_sample_per_pixel, int)
  %python_attribute(ignore_zero, bool)
  %python_attribute(in_memory, bool)
  %pickle_init(1, self.original_image_ground_connection, 
	       self.number_line_per_pixel, 
	       self.number_sample_per_pixel,
	       self.in_memory, self.ignore_zero);
};
}
