// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%base_import(geocal_exception)
%import "dem.i"
%import "raster_image.i"
%import "raster_image_multi_band.i"
%import "ground_coordinate.i"
%import "image_coordinate.i"
%import "image_mask.i"
%import "ground_mask.i"
%import "ecr.i"
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
class ImageGroundConnection: public GenericObject {
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
  virtual boost::shared_ptr<GroundCoordinate> 
    ground_coordinate(const ImageCoordinate& Ic) const;
  virtual boost::shared_ptr<GroundCoordinate> 
    ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const = 0;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const = 0;
  virtual blitz::Array<double, 2> image_coordinate_jac_ecr(const Ecr& Gc) 
    const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  MapInfo cover(const MapInfo& Mi, int boundary = 0) const;
  %python_attribute_with_set(image, boost::shared_ptr<RasterImage>)
  %python_attribute_with_set(image_multi_band, 
			     boost::shared_ptr<RasterImageMultiBand>)
  %python_attribute_with_set(image_mask, boost::shared_ptr<ImageMask>)
  %python_attribute_with_set(ground_mask, boost::shared_ptr<GroundMask>)
  %python_attribute(number_line, virtual int)
  %python_attribute(number_sample, virtual int)
  %python_attribute(number_band, virtual int)
  %python_attribute_with_set(title, std::string)
  std::string print_to_string() const;
  %python_attribute_with_set(parameter, blitz::Array<double, 1>)
  %python_attribute(parameter_name, virtual std::vector<std::string>)
  virtual double resolution_meter(const ImageCoordinate& Ic) const;
  virtual double resolution_meter() const;
  %python_attribute2(dem, dem_ptr, boost::shared_ptr<Dem>)
  void dem(const boost::shared_ptr<Dem>& D);
%pythoncode {
@dem.setter
def dem(self, value):  
  self.__dem(value)

def cf_look_vector(self, ic):
  return (self.cf_look_vector_lv(ic), self.cf_look_vector_pos(ic))

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
  OffsetImageGroundConnection(const boost::shared_ptr<ImageGroundConnection>& Ig_original, double Line_offset, double Sample_offset);
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute(original_image_ground_connection, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(line_offset, double)
  %python_attribute(sample_offset, double)
  %pickle_init(1, self.original_image_ground_connection, self.line_offset, 
	       self.sample_offset)
};

class ImageGroundConnectionCopy: public ImageGroundConnection
{
public:
  ImageGroundConnectionCopy
  (const boost::shared_ptr<ImageGroundConnection>& Igc);
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  const boost::shared_ptr<ImageGroundConnection>& igc_original() const;
};

}

