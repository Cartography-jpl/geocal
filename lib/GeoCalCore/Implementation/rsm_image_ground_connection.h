#ifndef RSM_IMAGE_GROUND_CONNECTION_H
#define RSM_IMAGE_GROUND_CONNECTION_H
#include "rsm.h"
#include "image_ground_connection.h"

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by a
  Rsm.
*******************************************************************/

class RsmImageGroundConnection : virtual public ImageGroundConnection,
				 public WithParameterNested {
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
  { add_object(rsm_); }

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
  { add_object(rsm_); }

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
  { add_object(rsm_); }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~RsmImageGroundConnection() {}
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic,
			const Dem& D) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic,
				  double H) const;
  virtual void image_coordinate_extended(const GroundCoordinate& Gc,
					 ImageCoordinate& Res,
					 bool& Success) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
					    ImageCoordinate& Res,
					    bool& Success) const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { 
    return rsm_->image_coordinate_jac_parm(Gc);
  }
  
  virtual void print(std::ostream& Os) const;
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) const;

//-----------------------------------------------------------------------
/// RSM that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Rsm>& rsm() const { return rsm_; }

  virtual blitz::Array<double, 1> parameter() const
  { return WithParameterNested::parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { WithParameterNested::parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return WithParameterNested::parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { WithParameterNested::parameter_with_derivative(Parm);}
protected:
  RsmImageGroundConnection() {}
private:
  boost::shared_ptr<Rsm> rsm_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};

}
GEOCAL_EXPORT_KEY(RsmImageGroundConnection);
#endif
