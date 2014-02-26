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
			   boost::shared_ptr<GroundMask>(),
			   bool Fit_height_offset = false)
			   
    : ImageGroundConnection(D, Img, boost::shared_ptr<RasterImageMultiBand>(),
			    Title, Img_mask, Ground_mask), 
      rpc_(new Rpc(R)),
      fit_height_offset_(Fit_height_offset)
  { }

//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImageMultiBand>& 
			   Img_mb,
			   const std::string& Title = "Image",
			   const boost::shared_ptr<ImageMask>& Img_mask = 
			   boost::shared_ptr<ImageMask>(),
			   const boost::shared_ptr<GroundMask>& Ground_mask =
			   boost::shared_ptr<GroundMask>(),
			   bool Fit_height_offset = false)
    : ImageGroundConnection(D, boost::shared_ptr<RasterImage>(), Img_mb,
			    Title, Img_mask, Ground_mask), 
      rpc_(new Rpc(R)),
      fit_height_offset_(Fit_height_offset)
  { }

//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  RpcImageGroundConnection(const Rpc& R, const boost::shared_ptr<Dem>& D,
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
      rpc_(new Rpc(R)),
      fit_height_offset_(Fit_height_offset)
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
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic,
				  double H) const
  { return 
      boost::shared_ptr<Geodetic>
      (new Geodetic(rpc_->ground_coordinate(Ic, H)));
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
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;

//-----------------------------------------------------------------------
/// In addition to the RPC parameters marked as being fitted for, add
/// the RPC height_offset to the list of parameters. This allows for
/// errors is the height to be corrected for, e.g. WV-2 has the wrong
/// altitude. Note that in general the height_offset won't be an
/// integer. It is the convention of NITF that the height_offset is
/// always an exact integer. If you fit for this, you should either
/// not store this as a NITF, or regenerate the RPC to match this by
/// for example a call to Rpc::generate_rpc.
//-----------------------------------------------------------------------

  void fit_height_offset(bool Fit_height_offset)
  { fit_height_offset_ = Fit_height_offset; }

//-----------------------------------------------------------------------
/// If true, we are fitting for the height offset. See the note in
/// setting fit_height_offset on issues with storing the RPC in NITF
/// if you fit for height offset.
//-----------------------------------------------------------------------

  bool fit_height_offset() const { return fit_height_offset_; }
  
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
  bool fit_height_offset_;
};

}
#endif
