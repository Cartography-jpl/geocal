// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "raster_averaged.h"
%}
%base_import(raster_image)
%base_import(image_ground_connection)
%geocal_shared_ptr(GeoCal::RasterAveraged);
%geocal_shared_ptr(GeoCal::AveragedImageGroundConnection);

namespace GeoCal {
class RasterAveraged : public RasterImage {
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
  virtual int operator()(int Line, int Sample) const;
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;
  virtual void write(int Line, int Sample, int Val);
  %pickle_init(1, self.high_resolution_image,
	       self.number_line_per_pixel, self.number_sample_per_pixel,
	       self.ignore_zero)
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
