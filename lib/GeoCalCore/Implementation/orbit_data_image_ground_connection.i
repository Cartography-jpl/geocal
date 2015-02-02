// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "orbit_data_image_ground_connection.h"
%}
%base_import(image_ground_connection)
%import "orbit.i"
%import "camera.i"
%import "dem.i"
%import "raster_image.i"
%import "refraction.i"
%geocal_shared_ptr(GeoCal::OrbitDataImageGroundConnection);
namespace GeoCal {
class OrbitDataImageGroundConnection: public ImageGroundConnection {
public:
  OrbitDataImageGroundConnection(const boost::shared_ptr<OrbitData>& Od,
				 const boost::shared_ptr<Camera>& Cam, 
				 const boost::shared_ptr<Dem>& D,
				 const boost::shared_ptr<RasterImage>& Img,
				 const std::string Title = "");
  OrbitDataImageGroundConnection(const boost::shared_ptr<OrbitData>& Od,
				 const boost::shared_ptr<Camera>& Cam, 
				 const boost::shared_ptr<Dem>& D,
				 const boost::shared_ptr<RasterImage>& Img,
				 const std::string Title,
				 const boost::shared_ptr<Refraction>& Ref,
				 double Resolution=30, int Band=0, 
				 double Max_height=9000);
  OrbitDataImageGroundConnection(Orbit& Orb, const Time& Tm,
				 const boost::shared_ptr<Camera>& Cam, 
				 const boost::shared_ptr<Dem>& D,
				 const boost::shared_ptr<RasterImage>& Img,
				 const std::string Title = "");
  OrbitDataImageGroundConnection(Orbit& Orb, const Time& Tm,
				 const boost::shared_ptr<Camera>& Cam, 
				 const boost::shared_ptr<Dem>& D,
				 const boost::shared_ptr<RasterImage>& Img,
				 const std::string Title,
				 const boost::shared_ptr<Refraction>& Ref,
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
  // Note that this convoluted looking expression is necessary to 
  // get SWIG to override correctly with use of director feature in
  // the base class
  %rename(_v_number_line) number_line;
  %rename(_v_number_sample) number_sample;
  %rename(_v_number_band) number_band;
  %rename(_v_has_time) has_time;
  virtual int number_line();
  virtual int number_sample();
  virtual int number_band();
  virtual bool has_time();
  %python_attribute_with_set(orbit_data, boost::shared_ptr<OrbitData>)
  %python_attribute_with_set(camera,boost::shared_ptr<Camera>)
  %python_attribute_with_set(resolution, double)
  %python_attribute_with_set(refraction, boost::shared_ptr<Refraction>)
  %python_attribute_with_set(band, int)
  %python_attribute_with_set(max_height, double)
  %pickle_init(1, self.orbit_data, self.camera, self.dem,
	       self.image, self.title, self.refraction,
	       self.resolution, self.band, self.max_height)
};
}

