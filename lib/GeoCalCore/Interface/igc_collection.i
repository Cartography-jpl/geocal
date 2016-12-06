// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_collection.h"
%}
%base_import(generic_object)
%base_import(with_parameter)
%import "dem.i"
%import "raster_image.i"
%import "raster_image_multi_band.i"
%import "ground_coordinate.i"
%import "image_coordinate.i"
%import "image_mask.i"
%import "ground_mask.i"
%import "image_ground_connection.i"
%geocal_shared_ptr(GeoCal::IgcCollection);

namespace GeoCal {

// Allow this class to be derived from in Python.
%feature("director") IgcCollection;

// Note, a class that is derived from in python needs to declare every
// virtual function that can be called on it, even if all that happens
// is the base class to a director is called. This is because this
// class is used to create the SwigDirector class, and this class
// needs each of the member functions to direct things properly. It is
// *not* necessary to add these function to the underlying C++, only
// that you declare them here.
//
// For this particular class, this isn't an issue since this is the
// base class. But I'll keep this note here in case we are cutting
// and pasting to make another class a director

// Make parameter a python property
#ifdef SWIGPYTHON
%rename("__ground_coordinate") IgcCollection::ground_coordinate;
#endif
class IgcCollection: public WithParameter {
public:
  virtual ~IgcCollection() {}
  %python_attribute(number_image, virtual int);
  virtual blitz::Array<double, 1> 
  collinearity_residual(int Image_index,
			const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  virtual blitz::Array<double, 2> 
  collinearity_residual_jacobian(int Image_index,
				 const GroundCoordinate& Gc,
				 const ImageCoordinate& Ic_actual) const;
  virtual boost::shared_ptr<GroundCoordinate> 
    ground_coordinate(int Image_index, const ImageCoordinate& Ic) const;
  virtual boost::shared_ptr<GroundCoordinate> 
    ground_coordinate_dem(int Image_index, const ImageCoordinate& Ic, 
			  const Dem& D) const;
  virtual boost::shared_ptr<Dem> dem(int Image_index) const;
  virtual ImageCoordinate image_coordinate(int Image_index,
					   const GroundCoordinate& Gc) 
    const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(int Image_index, const CartesianFixed& Gc) 
    const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm_fd(int Image_index, const CartesianFixed& Gc,
			       const blitz::Array<double, 1> Pstep) const;
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(int Image_index,
					  const CartesianFixed& Gc) const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_cf_fd(int Image_index, const CartesianFixed& Gc,
			     double Step_size) 
    const;
  virtual std::string title(int Image_index) const;
  virtual boost::shared_ptr<RasterImage> image(int Image_index) const;
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const = 0;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const = 0;
  std::string print_to_string() const;

  %python_attribute_with_set_virtual(parameter, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name, virtual std::vector<std::string>);
  %python_attribute_with_set_virtual(parameter_subset, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative_subset, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name_subset, virtual std::vector<std::string>);
  %python_attribute(parameter_mask, virtual blitz::Array<bool, 1>);

%pythoncode {
def ground_coordinate(self, image_index, ic, dem = None):
  '''Return ground coordinate for the given image coordinate. You can specify
   a dem to use, or we use the dem associated with the class.'''
  if(dem):
    return self.ground_coordinate_dem(image_index, ic, dem)
  else:
    return self.__ground_coordinate(image_index, ic)
}

};

}

