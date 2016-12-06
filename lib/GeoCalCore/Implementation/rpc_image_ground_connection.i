// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rpc_image_ground_connection.h"
%}
%base_import(image_ground_connection)
%import "geocal_rpc.i"
%import "dem.i"
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::RpcImageGroundConnection);
namespace GeoCal {
class RpcImageGroundConnection: public ImageGroundConnection {
public:
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image");
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask
			   );
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask,
			   const boost::shared_ptr<GroundMask>& Ground_mask,
			   bool Fit_height_offset = false
			   );
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title = "Image");
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask
			   );
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask,
			   const boost::shared_ptr<GroundMask>& Ground_mask,
			   bool Fit_height_offset = false
			   );
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title = "Image");
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask
			   );
  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask,
			   const boost::shared_ptr<GroundMask>& Ground_mask,
			   bool Fit_height_offset = false
			   );
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute2(rpc, rpc_ptr, boost::shared_ptr<Rpc>)
  %python_attribute_with_set(fit_height_offset, bool)
  %pickle_serialization();
};
}

