// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ipi.h"
%}
%base_import(generic_object)
%import "orbit.i"
%import "camera.i"
%import "time_table.i"
%import "ground_coordinate.i"
%import "image_coordinate.i"
%import "frame_coordinate.i"
%import "geocal_time.i"

%geocal_shared_ptr(GeoCal::Ipi);

namespace GeoCal {

class Ipi : public GenericObject {
public:
  Ipi(const boost::shared_ptr<Orbit>& Orb, const 
      boost::shared_ptr<Camera>& Cam,
      int Band,
      Time Tmin, Time Tmax, 
      const boost::shared_ptr<TimeTable>& Tt = boost::shared_ptr<TimeTable>(),
      double Local_time_window_size = 5.0,
      double Root_min_separation = 30.0, 
      double Time_tolerance = 40.8e-3 * 1.0 / 16);
  void image_coordinate(const GroundCoordinate& Gp, ImageCoordinate &OUTPUT,
			bool &OUTPUT) const;
  void image_coordinate_extended(const GroundCoordinate& Gp, 
				 ImageCoordinate &OUTPUT,
				 bool &OUTPUT) const;
  void time(const GroundCoordinate& Gp, Time &OUTPUT, FrameCoordinate &OUTPUT,
	    bool &OUTPUT) const;
  std::vector<boost::shared_ptr<GroundCoordinate> > footprint(const Dem& D) 
    const;
  %python_attribute(resolution_meter, double)
  std::string print_to_string() const;
  %python_attribute2_with_set(orbit, orbit_ptr, boost::shared_ptr<Orbit>)
  %python_attribute2_with_set(camera, camera_ptr, boost::shared_ptr<Camera>)
  %python_attribute2(time_table, time_table_ptr, boost::shared_ptr<TimeTable>)
  %python_attribute_with_set(band, int)
  %python_attribute(min_time, Time)
  %python_attribute(max_time, Time)
  %python_attribute(local_time_window_size, double)
  %python_attribute(root_min_separation, double)
  %python_attribute(time_tolerance, double)
  %pickle_init(1, self.orbit, self.camera, self.band, self.min_time,
	       self.max_time, self.time_table, self.local_time_window_size,
	       self.root_min_separation, self.time_tolerance)
};
}
