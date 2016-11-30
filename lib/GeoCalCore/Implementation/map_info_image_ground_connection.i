// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "map_info_image_ground_connection.h"
%}
%base_import(image_ground_connection)
%import "geocal_rpc.i"
%import "dem.i"
%geocal_shared_ptr(GeoCal::MapInfoImageGroundConnection);

namespace GeoCal {
class MapInfoImageGroundConnection: public ImageGroundConnection {
public:
  MapInfoImageGroundConnection(const boost::shared_ptr<RasterImage>& Img,
			       const boost::shared_ptr<Dem>& D,
			       const std::string& Title = "Image");
  MapInfoImageGroundConnection(const boost::shared_ptr<RasterImageMultiBand>& 
			       Img_mb,
			       const boost::shared_ptr<Dem>& D,
			       const std::string& Title = "Image");
  MapInfoImageGroundConnection(const boost::shared_ptr<RasterImage>& Img,
			       const boost::shared_ptr<RasterImageMultiBand>& 
			       Img_mb,
			       const boost::shared_ptr<Dem>& D,
			       const std::string& Title = "Image");
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %pickle_init(2, self.image, self.image_multi_band, self.dem, self.title)
};
}
