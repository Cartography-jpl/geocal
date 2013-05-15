// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%{
#include "image_ground_connection.h"
#include "ipi_image_ground_connection.h"
#include "magnify_bilinear.h"
#include "rpc_image_ground_connection.h"
#include "orbit_data_image_ground_connection.h"
#include "map_info.h"
%}

// In geocal_rpc.i
//%geocal_shared_ptr(GeoCal::ImageGroundConnection);
%geocal_shared_ptr(GeoCal::OffsetImageGroundConnection);

%shared_ptr_dynamic_list(GeoCal::ImageGroundConnection,
			 GeoCal::OffsetImageGroundConnection,
			 GeoCal::MapInfoImageGroundConnection,
			 GeoCal::MagnifyBilinearImageGroundConnection,
			 GeoCal::RpcImageGroundConnection,
			 GeoCal::IpiImageGroundConnection,
			 GeoCal::OrbitDataImageGroundConnection);

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
class ImageGroundConnection {
protected:
  ImageGroundConnection(const boost::shared_ptr<Dem> d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const std::string& Title);
  ImageGroundConnection();
public:
  virtual ~ImageGroundConnection();
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT2,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const = 0;
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
  %python_attribute(image, boost::shared_ptr<RasterImage>)
  %python_attribute(number_line, virtual int)
  %python_attribute(number_sample, virtual int)
  %python_attribute_with_set(title, std::string)
  std::string print_to_string() const;
  %python_attribute_with_set(parameter, blitz::Array<double, 1>)
  %python_attribute(parameter_name, std::vector<std::string>)
  virtual double resolution_meter(const ImageCoordinate& Ic) const;
  virtual double resolution_meter() const;
  %python_attribute2(dem, dem_ptr, boost::shared_ptr<Dem>)
  void dem(const boost::shared_ptr<Dem>& D);
%pythoncode {
@dem.setter
def dem(self, value):  
  self.__dem(value)

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
  OffsetImageGroundConnection(const boost::shared_ptr<ImageGroundConnection>& Ig_original, double Line_offset, double Sample_offset, int Number_line, int Number_sample);
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual void 
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  %python_attribute(original_image_ground_connection, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(line_offset, double)
  %python_attribute(sample_offset, double)
  %pickle_init(1, self.original_image_ground_connection, self.line_offset, 
	       self.sample_offset, self.number_line, self.number_sample)
};

}

