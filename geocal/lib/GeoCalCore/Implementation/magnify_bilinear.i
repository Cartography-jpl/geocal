// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "magnify_bilinear.h"
%}

%geocal_shared_ptr(GeoCal::MagnifyBilinear);
%geocal_shared_ptr(GeoCal::MagnifyBilinearImageGroundConnection);

namespace GeoCal {
class MagnifyBilinear : public CalcRaster {
public:
  MagnifyBilinear(const boost::shared_ptr<RasterImage>& Data,
		  int Magfactor, int Number_tile = 4);
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
