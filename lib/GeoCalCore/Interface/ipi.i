// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

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
%include "geocal_time_include.i"

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
      double Time_tolerance = 1e-6, double Max_frame_extend=1000);
  void image_coordinate(const GroundCoordinate& Gp, ImageCoordinate &OUTPUT,
			bool &OUTPUT) const;
  void image_coordinate_with_derivative(const GroundCoordinate& Gp, ImageCoordinateWithDerivative &OUTPUT,
			bool &OUTPUT) const;
  void image_coordinate_with_derivative(const GroundCoordinate& Gp, 
				const boost::array<AutoDerivative<double>, 3>&
				Gp_with_der,
				ImageCoordinateWithDerivative& OUTPUT,
				bool& OUTPUT) const;
  void image_coordinate_with_derivative_extended(const GroundCoordinate& Gp, 
				const boost::array<AutoDerivative<double>, 3>&
				Gp_with_der,
				 ImageCoordinateWithDerivative &OUTPUT,
				 bool &OUTPUT) const;
  void image_coordinate_with_derivative_extended(const GroundCoordinate& Gp, 
				 ImageCoordinateWithDerivative &OUTPUT,
				 bool &OUTPUT) const;
  void time(const GroundCoordinate& Gp, Time &OUTPUT, FrameCoordinate &OUTPUT,
	    bool &OUTPUT) const;
  void time_with_derivative(const GroundCoordinate& Gp, 
			    TimeWithDerivative& OUTPUT, 
			    FrameCoordinateWithDerivative& OUTPUT,
			    bool& OUTPUT) const;
  void time_with_derivative(const GroundCoordinate& Gp, 
			    const boost::array<AutoDerivative<double>, 3>&
			    Gp_with_der,
			    TimeWithDerivative& OUTPUT, 
			    FrameCoordinateWithDerivative& OUTPUT,
			    bool& OUTPUT) const;
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
  %python_attribute(max_frame_extend, double)
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("Ipi")
