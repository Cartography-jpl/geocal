// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ipi_image_ground_connection.h"
%}
%base_import(image_ground_connection)
%import "ipi.i"
%import "dem.i"
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::IpiImageGroundConnection);
namespace GeoCal {
class IpiImageGroundConnection: public ImageGroundConnection {
public:
  IpiImageGroundConnection(const boost::shared_ptr<Ipi>& I, 
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image",
			   double Resolution = 30, 
			   double Max_height = 9000);
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute(ipi, boost::shared_ptr<Ipi>)
  %python_attribute(resolution, double)
  %python_attribute(maximum_height, double)
  %pickle_serialization()
protected:
  IpiImageGroundConnection();
  void initialize(const boost::shared_ptr<Ipi>& I, 
		  const boost::shared_ptr<Dem>& D,
		  const boost::shared_ptr<RasterImage>& Img,
		  const std::string& Title = "Image",
		  double Resolution = 30, 
		  double Max_height = 9000);
};
}

// List of things "import *" will include
%python_export("IpiImageGroundConnection")
