// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "orbit_data_image_ground_connection.h"
%}

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
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic,
			const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual int number_line() const;
  virtual int number_sample() const;
  virtual boost::shared_ptr<RasterImage> image() const;
  virtual std::string title() const;
  %python_attribute2(orbit_data, orbit_data_ptr, boost::shared_ptr<OrbitData>)
  %python_attribute2(camera, camera_ptr, boost::shared_ptr<Camera>)
  %python_attribute(resolution, double)
  %python_attribute(refraction, boost::shared_ptr<Refraction>)
  %python_attribute(band, int)
  %python_attribute(max_height, double)
  %pickle_init(1, self.orbit_data, self.camera, self.dem,
	       self.image, self.title, self.refraction,
	       self.resolution, self.band, self.max_height)
};
}

