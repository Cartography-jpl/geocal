// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "magnify_bilinear.h"
%}
%base_import(calc_raster)
%base_import(image_ground_connection)
%geocal_shared_ptr(GeoCal::MagnifyBilinear);
%geocal_shared_ptr(GeoCal::MagnifyBilinearImageGroundConnection);

namespace GeoCal {
class MagnifyBilinear : public CalcRaster {
public:
  MagnifyBilinear(const boost::shared_ptr<RasterImage>& Data,
		  int Magfactor, int Number_tile = 4);
  %python_attribute(underlying_data, boost::shared_ptr<RasterImage>);
  %python_attribute(magnification_factor, int);
  %pickle_init(1, self.underlying_data, self.magnification_factor);
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

class MagnifyBilinearImageGroundConnection: public ImageGroundConnection {
public:
  MagnifyBilinearImageGroundConnection(const boost::shared_ptr<ImageGroundConnection>& Ig_original, int Magfactor);
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute(original_image_ground_connection,
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(magnification_factor, int)
  %pickle_init(1, self.original_image_ground_connection, self.magnification_factor)
};

}
