// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "geometric_model.h"
%}
%base_import(generic_object)
%base_import(observer)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::GeometricModel);
%geocal_shared_ptr(GeoCal::GeometricTiePoints);
%geocal_shared_ptr(GeoCal::Observer<GeoCal::GeometricTiePoints>);
%geocal_shared_ptr(GeoCal::Observable<GeoCal::GeometricTiePoints>);

namespace GeoCal {
%template(ObservableGeometricTiePoints) GeoCal::Observable<GeoCal::GeometricTiePoints>;
%template(ObserverGeometricTiePoints) GeoCal::Observer<GeoCal::GeometricTiePoints>;

class GeometricTiePoints;
  
%nodefaultctor GeometricModel;

class GeometricModel: public Observer<GeometricTiePoints> {
public:
  virtual ImageCoordinate original_image_coordinate
  (const ImageCoordinate& Resampled_ic) const;
  virtual ImageCoordinate resampled_image_coordinate
  (const ImageCoordinate& Resampled_ic) const;
  std::string print_to_string() const;
  %pickle_serialization()
};

class GeometricTiePoints : public Observable<GeometricTiePoints> {
public:
  GeometricTiePoints();
  void add_point(const ImageCoordinate& Resampled_ic,
		 const ImageCoordinate& Original_ic);
  void start_replacing();
  void remove_point(int Index);
  virtual void notify_update();
  virtual void add_observer(Observer<GeometricTiePoints>& Obs);
  virtual void remove_observer(Observer<GeometricTiePoints>& Obs);
  %python_attribute(x, blitz::Array<double, 2>)
  %python_attribute(y, blitz::Array<double, 2>)
  %python_attribute(number_point, int);
  std::string print_to_string() const;
  %pickle_serialization()
};
}

