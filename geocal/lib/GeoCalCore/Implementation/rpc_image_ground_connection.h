#ifndef RPC_IMAGE_GROUND_CONNECTION_H
#define RPC_IMAGE_GROUND_CONNECTION_H
#include "image_ground_connection.h"
#include "geocal_rpc.h"

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by a
  Rpc.
*******************************************************************/

class RpcImageGroundConnection : public ImageGroundConnection {
public:
//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image",
			   const boost::shared_ptr<ImageMask>& Img_mask = 
			   boost::shared_ptr<ImageMask>(),
			   const boost::shared_ptr<GroundMask>& Ground_mask =
			   boost::shared_ptr<GroundMask>())
			   
    : ImageGroundConnection(D, Img, Title, Img_mask, Ground_mask), 
      rpc_(new Rpc(R))
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~RpcImageGroundConnection() {}
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic,
			const Dem& D) const
  { return 
      boost::shared_ptr<Geodetic>
      (new Geodetic(rpc_->ground_coordinate(Ic, D)));
  }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const { return rpc_->image_coordinate(Gc); }
  virtual void print(std::ostream& Os) const \
  {
    Os << "RpcImageGroundConnection" << "\n"
       << "  Rpc: \n"
       << *rpc_ << "\n";
  }

  virtual blitz::Array<double, 1> parameter() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual blitz::Array<double, 2> image_coordinate_jac_ecr(const Ecr& Gc) const;
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { return rpc_->image_coordinate_jac_parm(Gc); }

//-----------------------------------------------------------------------
/// RPC that we are using
//-----------------------------------------------------------------------
  const Rpc& rpc() const { return *rpc_; }

//-----------------------------------------------------------------------
/// RPC that we are using
//-----------------------------------------------------------------------
  Rpc& rpc() { return *rpc_; }

  boost::shared_ptr<Rpc> rpc_ptr() const {return rpc_;}
private:
  boost::shared_ptr<Rpc> rpc_;
};

}
#endif
