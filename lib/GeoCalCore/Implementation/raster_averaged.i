// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "raster_averaged.h"
#include "image_ground_connection.h"
%}
%base_import(image_mask)
%base_import(image_ground_connection)
%base_import(calc_raster)
%base_import(calc_raster_multi_band)
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
  %python_attribute2(high_resolution_image, high_resolution_image_ptr,
		     boost::shared_ptr<RasterImage>)
  %python_attribute(number_line_per_pixel, int)
  %python_attribute(number_sample_per_pixel, int)
  %python_attribute(ignore_zero, bool)
  %pickle_serialization();
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
  %pickle_serialization();
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
  %pickle_serialization();
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
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("RasterAveraged", "RasterAveragedMultiBand", "ImageMaskAveraged", "AveragedImageGroundConnection")
