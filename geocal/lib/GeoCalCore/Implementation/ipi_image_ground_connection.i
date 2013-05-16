// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ipi_image_ground_connection.h"
%}

%geocal_shared_ptr(GeoCal::IpiImageGroundConnection);
namespace GeoCal {
class IpiImageGroundConnection: public ImageGroundConnection {
public:
  IpiImageGroundConnection(const boost::shared_ptr<Ipi>& I, 
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image",
			   double Resolution = 30, int Band = 0,
			   double Max_height = 9000);
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute2(ipi, ipi_ptr, boost::shared_ptr<Ipi>)
  %python_attribute(band, int)
  %python_attribute(resolution, double)
  %python_attribute(maximum_height, double)
  %pickle_init(1, self.ipi, self.dem, self.image, self.title,
	       self.resolution, self.band, self.maximum_height)
};
}

