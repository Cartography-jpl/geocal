// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include <std_vector.i>
%include "geocal_common.i"

%{
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%base_import(with_parameter)
%base_import(geocal_exception)
%import "dem.i"
%import "raster_image.i"
%import "raster_image_multi_band.i"
%import "ground_coordinate.i"
%import "image_coordinate.i"
%import "image_mask.i"
%import "ground_mask.i"
%geocal_shared_ptr(GeoCal::ImageGroundConnection);
%geocal_shared_ptr(GeoCal::OffsetImageGroundConnection);
%geocal_shared_ptr(GeoCal::ImageGroundConnectionFailed);
%geocal_shared_ptr(GeoCal::ImageGroundConnectionCopy);

namespace GeoCal {

class ImageGroundConnectionFailed : public Exception {
public:
   ImageGroundConnectionFailed(const std::string& W);
};

// Allow this class to be derived from in Python.
%feature("director") ImageGroundConnection;

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
%rename("__dem") ImageGroundConnection::dem;
%rename("__ground_coordinate") ImageGroundConnection::ground_coordinate;
#endif
class ImageGroundConnection: public WithParameter {
protected:
  ImageGroundConnection(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title);
  ImageGroundConnection(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title,
			const boost::shared_ptr<ImageMask>& Img_mask);
  ImageGroundConnection(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title,
			const boost::shared_ptr<ImageMask>& Img_mask,
			const boost::shared_ptr<GroundMask>& Ground_mask);
  virtual std::string desc() const;
  void initialize(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title);
  void initialize(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title,
			const boost::shared_ptr<ImageMask>& Img_mask);
  void initialize(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title,
			const boost::shared_ptr<ImageMask>& Img_mask,
			const boost::shared_ptr<GroundMask>& Ground_mask);
  ImageGroundConnection();
public:
  virtual ~ImageGroundConnection();
  // Having multiple return values seriously confuses SWIG director
  // stuff. Just leave this function out, so SWIG doesn't know about
  // this at all. We'll create a python equivalent 
  //virtual void
  //cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
  //boost::shared_ptr<CartesianFixed>& P) const;
  virtual boost::shared_ptr<CartesianFixedLookVector>
  cf_look_vector_lv(const ImageCoordinate& Ic) const;
  virtual boost::shared_ptr<CartesianFixed>
  cf_look_vector_pos(const ImageCoordinate& Ic) const;
  virtual blitz::Array<double, 1> 
  collinearity_residual(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  virtual blitz::Array<double, 2> 
  collinearity_residual_jacobian(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  void compare_igc(const ImageGroundConnection& Igc, int Number_line_spacing,
		   int Number_sample_spacing, double Height,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT)
    const;
  virtual boost::shared_ptr<GroundCoordinate> 
    ground_coordinate(const ImageCoordinate& Ic) const;
  virtual boost::shared_ptr<GroundCoordinate> 
    ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const = 0;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, 
				  double H) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const = 0;
  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
 					    ImageCoordinate& OUTPUT2,
 					    bool& OUTPUT) const;
  virtual void image_coordinate_extended(const GroundCoordinate& Gc,
					 ImageCoordinate& OUTPUT2,
					 bool& OUTPUT) const;
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) 
    const;
  virtual blitz::Array<double, 2> image_coordinate_jac_cf_fd(const CartesianFixed& Gc, double Step_size) const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  virtual blitz::Array<double, 2>
  image_coordinate_jac_parm_fd(const GroundCoordinate& Gc,
			       const blitz::Array<double, 1>& Eps) const;
  MapInfo cover(const MapInfo& Mi, int boundary = 0) const;
  %pickle_serialization();
  %python_attribute(naif_code, int);
  %python_attribute(cartesian_fixed, boost::shared_ptr<CartesianFixed>);
  %python_attribute_with_set(image, boost::shared_ptr<RasterImage>)
  %python_attribute_with_set(image_multi_band, 
			     boost::shared_ptr<RasterImageMultiBand>)
  %python_attribute_with_set(image_mask, boost::shared_ptr<ImageMask>)
  %python_attribute_with_set(ground_mask, boost::shared_ptr<GroundMask>)
  %python_attribute(number_line, virtual int)
  %python_attribute(number_sample, virtual int)
  %python_attribute(number_band, virtual int)
  %python_attribute_with_set(title, std::string)
  %python_attribute(has_time, bool)
  virtual Time pixel_time(const ImageCoordinate& Ic) const;
  std::string print_to_string() const;
  virtual double resolution_meter(const ImageCoordinate& Ic) const;
  virtual double resolution_meter() const;

  %python_attribute_with_set_virtual(parameter, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name, virtual std::vector<std::string>);
  %python_attribute_with_set_virtual(parameter_subset, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative_subset, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name_subset, virtual std::vector<std::string>);
  %python_attribute(parameter_mask, virtual blitz::Array<bool, 1>);

  // Having multiple return values seriously confuses SWIG director
  // stuff. Just leave this function out, so SWIG doesn't know about
  // this at all. We'll create a python equivalent 
  // virtual void footprint_resolution(int Line, int Sample, 
  //			    double &OUTPUT, double &OUTPUT);
  virtual double footprint_resolution_line(int Line, int Sample) const;
  virtual double footprint_resolution_sample(int Line, int Sample) const;
  virtual blitz::Array<double, 7> 
  cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp,
		     int nsubpixel_line = 1, 
		     int nsubpixel_sample = 1,
		     int nintegration_step = 1) const;
  %python_attribute2(dem, dem_ptr, boost::shared_ptr<Dem>)
  void dem(const boost::shared_ptr<Dem>& D);
%pythoncode {
@dem.setter
def dem(self, value):  
  self.__dem(value)

def cf_look_vector(self, ic):
  return (self.cf_look_vector_lv(ic), self.cf_look_vector_pos(ic))

def footprint_resolution(self, line, sample):
  return (self.footprint_resolution_line(line, sample), 
	  self.footprint_resolution_sample(line, sample))

def ground_coordinate(self, ic, dem = None):
  '''Return ground coordinate for the given image coordinate. You can specify
   a dem to use, or we use the dem associated with the class.'''
  if(dem):
    return self.ground_coordinate_dem(ic, dem)
  else:
    return self.__ground_coordinate(ic)
}

};

class OffsetImageGroundConnection: public ImageGroundConnection {
public:
  OffsetImageGroundConnection
  (const boost::shared_ptr<ImageGroundConnection>& Ig_original,
   double Line_offset, double Sample_offset);
  OffsetImageGroundConnection
  (const boost::shared_ptr<ImageGroundConnection>& Ig_original, 
   int Lstart, int Sstart, int Number_line, int Number_sample);
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute(original_image_ground_connection, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(line_offset, double)
  %python_attribute(sample_offset, double)
  %pickle_serialization();
};

class ImageGroundConnectionCopy: public ImageGroundConnection
{
public:
  ImageGroundConnectionCopy
  (const boost::shared_ptr<ImageGroundConnection>& Igc);
  ImageGroundConnectionCopy
  (const boost::shared_ptr<ImageGroundConnection>& Igc,
   const boost::shared_ptr<Dem>& d, 
   const boost::shared_ptr<RasterImage>& Img, 
   const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
   const std::string& Title,
   const boost::shared_ptr<ImageMask>& Img_mask,
   const boost::shared_ptr<GroundMask>& Ground_mask);
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute(igc_original, boost::shared_ptr<ImageGroundConnection>)
  %pickle_serialization();
};

}

%template(Vector_ImageGroundConnection) std::vector<boost::shared_ptr<GeoCal::ImageGroundConnection> >;

// Extra code for handling boost serialization/python pickle of
// director classes
%geocal_director_serialization(image_ground_connection, ImageGroundConnection)

// List of things "import *" will include
%python_export("ImageGroundConnection", "Vector_ImageGroundConnection", "ImageGroundConnectionFailed", "OffsetImageGroundConnection", "ImageGroundConnectionCopy")
