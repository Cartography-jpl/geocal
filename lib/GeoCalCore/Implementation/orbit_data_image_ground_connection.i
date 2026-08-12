// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_data_image_ground_connection.h"
%}
%base_import(image_ground_connection)
%import "orbit.i"
%import "camera.i"
%import "dem.i"
%import "raster_image.i"
%import "refraction.i"
%import "velocity_aberration.i"
%geocal_shared_ptr(GeoCal::OrbitDataImageGroundConnection);
namespace GeoCal {
class OrbitDataImageGroundConnection: public ImageGroundConnection {
public:
  OrbitDataImageGroundConnection(const boost::shared_ptr<OrbitData>& Od,
				 const boost::shared_ptr<Camera>& Cam, 
				 const boost::shared_ptr<Dem>& D,
				 const boost::shared_ptr<RasterImage>& Img,
				 const std::string Title = "",
				 const boost::shared_ptr<Refraction>&
				 Ref = boost::shared_ptr<Refraction>(),
				 const boost::shared_ptr<VelocityAberration>&
				 Vabb = boost::shared_ptr<VelocityAberration>(),
				 double Resolution=30, int Band=0, 
				 double Max_height=9000);
  OrbitDataImageGroundConnection(const boost::shared_ptr<Orbit>& Orb,
  				 const Time& Tm,
  				 const boost::shared_ptr<Camera>& Cam, 
  				 const boost::shared_ptr<Dem>& D,
  				 const boost::shared_ptr<RasterImage>& Img,
  				 const std::string Title = "",
  				 const boost::shared_ptr<Refraction>&
  				 Ref = boost::shared_ptr<Refraction>(),
				 const boost::shared_ptr<VelocityAberration>&
				 Vabb = boost::shared_ptr<VelocityAberration>(),
  				 double Resolution=30, int Band=0, 
  				 double Max_height=9000);

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic,
			const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const GroundCoordinate& Gc) const;
  %python_attribute(orbit, boost::shared_ptr<Orbit>);
  %python_attribute_with_set(orbit_data, boost::shared_ptr<OrbitData>)
  %python_attribute_with_set(camera,boost::shared_ptr<Camera>)
  %python_attribute_with_set(resolution, double)
  %python_attribute_with_set(refraction, boost::shared_ptr<Refraction>)
  %python_attribute_with_set(velocity_aberration, boost::shared_ptr<VelocityAberration>)
  %python_attribute_with_set(band, int)
  %python_attribute_with_set(max_height, double)
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("OrbitDataImageGroundConnection")
