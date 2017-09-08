// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_image_ground_connection.h"
%}
%base_import(image_ground_connection)
%import "rsm.i"
%import "dem.i"
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::RsmImageGroundConnection);
namespace GeoCal {
class RsmImageGroundConnection: public ImageGroundConnection {
public:
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image");
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask
			   );
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask,
			   const boost::shared_ptr<GroundMask>& Ground_mask
			   );
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title = "Image");
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask
			   );
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask,
			   const boost::shared_ptr<GroundMask>& Ground_mask);
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title = "Image");
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask
			   );
  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title,
			   const boost::shared_ptr<ImageMask>& Img_mask,
			   const boost::shared_ptr<GroundMask>& Ground_mask);
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& OUTPUT,
		 boost::shared_ptr<CartesianFixed>& OUTPUT) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  %python_attribute(rsm, boost::shared_ptr<Rsm>)
  %pickle_serialization();
};
}

