#ifndef MAP_INFO_IMAGE_GROUND_CONNECTION_H
#define MAP_INFO_IMAGE_GROUND_CONNECTION_H
#include "map_info.h"
#include "image_ground_connection.h"
namespace GeoCal {
/****************************************************************//**
  This class creates an ImageGroundConnection that simply maps 
  map registered image to the ground.
*******************************************************************/

class MapInfoImageGroundConnection : public ImageGroundConnection {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------
  MapInfoImageGroundConnection(const boost::shared_ptr<RasterImage>& Img,
			       const boost::shared_ptr<Dem>& D,
			       const std::string& Title = "Image")
    : ImageGroundConnection(D, Img, 
			    boost::shared_ptr<RasterImageMultiBand>(),
			    Title)
  { }
  
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------
  MapInfoImageGroundConnection(const boost::shared_ptr<RasterImageMultiBand>& 
			       Img_mb,
			       const boost::shared_ptr<Dem>& D,
			       const std::string& Title = "Image")
    : ImageGroundConnection(D, boost::shared_ptr<RasterImage>(),
			    Img_mb, Title)
  { }
  
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------
  MapInfoImageGroundConnection(const boost::shared_ptr<RasterImage>& Img,
			       const boost::shared_ptr<RasterImageMultiBand>& 
			       Img_mb,
			       const boost::shared_ptr<Dem>& D,
			       const std::string& Title = "Image")
    : ImageGroundConnection(D, Img, Img_mb, Title)
  { }
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MapInfoImageGroundConnection() {}

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    throw Exception("Look vector isn't defined for a MapInfoImageGroundConnection");
  }
  
//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image coordinate.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const
  { 
    if(image())
      return image()->ground_coordinate(Ic, D);
    return image_multi_band()->raster_image(0).ground_coordinate(Ic, D);
  }

//-----------------------------------------------------------------------
/// Return image coordinate that goes with a particular GroundCoordinate.
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const
  {
    if(image())
      return image()->coordinate(Gc);
    return image_multi_band()->raster_image(0).coordinate(Gc);
  }

  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) const
  {
    ImageCoordinate ic0 = image_coordinate(Gc);
    boost::shared_ptr<CartesianFixed> gc2 = Gc.convert_to_cf();
    blitz::Array<double, 2> res(2, 3);
    const double eps = 10;
    for(int i = 0; i < 3; ++i) {
      gc2->position[i] += eps;
      ImageCoordinate ic1 = image_coordinate(*gc2);
      res(0, i) = (ic1.line - ic0.line) / eps;
      res(1, i) = (ic1.sample - ic0.sample) / eps;
      gc2->position[i] -= eps;
    }
    return res;
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { 
    Os << "MapInfoImageGroundConnection\n";
    if(image())
      Os << "  RasterImage:\n"
	 << *image() << "\n";
    else
      Os << "  RasterImageMultiBand:\n"
	 << *image_multi_band() << "\n";
  }
};
}
#endif
