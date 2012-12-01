// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "map_info_image_ground_connection.h"
%}

%geocal_shared_ptr(MapInfoImageGroundConnection);

namespace GeoCal {
class MapInfoImageGroundConnection: public ImageGroundConnection {
public:
  MapInfoImageGroundConnection(const boost::shared_ptr<RasterImage>& Img,
			       const boost::shared_ptr<Dem>& D,
			       const std::string& Title = "Image");
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %pickle_init(self.image, self.dem, self.title)
};
}
