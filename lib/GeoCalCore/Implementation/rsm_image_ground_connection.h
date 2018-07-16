#ifndef RSM_IMAGE_GROUND_CONNECTION_H
#define RSM_IMAGE_GROUND_CONNECTION_H
#include "rsm.h"
#include "image_ground_connection.h"

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by a
  Rsm.
*******************************************************************/

class RsmImageGroundConnection : virtual public ImageGroundConnection {
public:
//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image",
			   const boost::shared_ptr<ImageMask>& Img_mask = 
			   boost::shared_ptr<ImageMask>(),
			   const boost::shared_ptr<GroundMask>& Ground_mask =
			   boost::shared_ptr<GroundMask>())
    : ImageGroundConnection(D, Img, boost::shared_ptr<RasterImageMultiBand>(),
			    Title, Img_mask, Ground_mask), 
      rsm_(R)
  { }

//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title = "Image",
			   const boost::shared_ptr<ImageMask>& Img_mask = 
			   boost::shared_ptr<ImageMask>(),
			   const boost::shared_ptr<GroundMask>& Ground_mask =
			   boost::shared_ptr<GroundMask>())
    : ImageGroundConnection(D, boost::shared_ptr<RasterImage>(), Img_mb,
			    Title, Img_mask, Ground_mask), 
      rsm_(R)
  { }

//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  RsmImageGroundConnection(const boost::shared_ptr<Rsm>& R,
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title = "Image",
			   const boost::shared_ptr<ImageMask>& Img_mask = 
			   boost::shared_ptr<ImageMask>(),
			   const boost::shared_ptr<GroundMask>& Ground_mask =
			   boost::shared_ptr<GroundMask>(),
			   bool Fit_height_offset = false)
    : ImageGroundConnection(D, Img, Img_mb, Title, Img_mask, Ground_mask), 
      rsm_(R)
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~RsmImageGroundConnection() {}
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic,
			const Dem& D) const
  { return rsm_->ground_coordinate(Ic, D);}
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic,
				  double H) const
  { return rsm_->ground_coordinate_approx_height(Ic,H); }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
					    ImageCoordinate& Res,
					    bool& Success) const;
  virtual void print(std::ostream& Os) const;
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) const;

//-----------------------------------------------------------------------
/// RSM that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Rsm>& rsm() const { return rsm_; }
protected:
  RsmImageGroundConnection() {}
private:
  boost::shared_ptr<Rsm> rsm_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}
GEOCAL_EXPORT_KEY(RsmImageGroundConnection);
#endif
