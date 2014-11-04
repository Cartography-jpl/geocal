// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "igc_rolling_shutter.h"
%}
%base_import(image_ground_connection)
%import "orbit.i"
%import "camera.i"
%import "dem.i"
%import "time_table.i"
%import "raster_image.i"
%import "refraction.i"
%geocal_shared_ptr(GeoCal::IgcRollingShutter);
namespace GeoCal {
class IgcRollingShutter: public ImageGroundConnection {
public:
  enum RollDirection { ROLL_LINE_DIRECTION , ROLL_SAMPLE_DIRECTION };
  IgcRollingShutter(const boost::shared_ptr<Orbit>& Orb,
		    const boost::shared_ptr<TimeTable>& Time_table,
		    const boost::shared_ptr<Camera>& Cam, 
		    const boost::shared_ptr<Dem>& D,
		    const boost::shared_ptr<RasterImage>& Img,
		    RollDirection Roll_direction = ROLL_LINE_DIRECTION,
		    const std::string Title = "");
  IgcRollingShutter(const boost::shared_ptr<Orbit>& Orb,
		    const boost::shared_ptr<TimeTable>& Time_table,
		    const boost::shared_ptr<Camera>& Cam, 
		    const boost::shared_ptr<Dem>& D,
		    const boost::shared_ptr<RasterImage>& Img,
		    RollDirection Roll_direction,
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
  %python_attribute_with_set(orbit, boost::shared_ptr<Orbit>)
  %python_attribute_with_set(time_table, boost::shared_ptr<TimeTable>)
  %python_attribute_with_set(camera,boost::shared_ptr<Camera>)
  %python_attribute_with_set(resolution, double)
  %python_attribute_with_set(refraction, boost::shared_ptr<Refraction>)
  %python_attribute_with_set(band, int)
  %python_attribute_with_set(max_height, double)
  %pickle_init(1, self.orbit, self.time_table, self.camera, self.dem,
	       self.image, self.title, self.refraction,
	       self.resolution, self.band, self.max_height)
};
}

