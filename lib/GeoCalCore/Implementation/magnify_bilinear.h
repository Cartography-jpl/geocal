#ifndef MAGNIFY_BILINEAR_H
#define MAGNIFY_BILINEAR_H
#include "calc_raster.h"
#include "image_ground_connection.h"
namespace GeoCal {
/****************************************************************//**
  This creates a magnified image of a RasterImage. We do a bilinear
  interpolation to get the values
*******************************************************************/
class MagnifyBilinear : public CalcRaster {
public:
  MagnifyBilinear(const boost::shared_ptr<RasterImage>& Data,
		  int Magfactor, int Number_tile = 4);
  virtual ~MagnifyBilinear() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "MagnifyBilinear\n"
       << "  magfactor: " << magfactor << "\n"
       << "  underlying raster: \n" << *raw_data << "\n";
  }
  const boost::shared_ptr<RasterImage>& underlying_data() const
  { return raw_data;}
  int magnification_factor() const {return magfactor; }
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<RasterImage> raw_data;
  int magfactor;
};

/****************************************************************//**
  This create an ImageGroundConnection that is magnified the same
  way MagnifyBilinear does.
*******************************************************************/

class MagnifyBilinearImageGroundConnection : public ImageGroundConnection {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------
  MagnifyBilinearImageGroundConnection(const boost::shared_ptr<ImageGroundConnection>& Ig_original, int Magfactor)
    : ig_(Ig_original), magfactor_(Magfactor)
  { 
    dem_ = Ig_original->dem_ptr();
    // No raster image or title
  }
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MagnifyBilinearImageGroundConnection() {}

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    ImageCoordinate ic2((Ic.line + 0.5) / magfactor_ - 0.5,
			(Ic.sample + 0.5) / magfactor_ - 0.5);
    return ig_->cf_look_vector(ic2, Lv, P); 
  }

//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image coordinate.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const
  { 
    ImageCoordinate ic2((Ic.line + 0.5) / magfactor_ - 0.5,
			(Ic.sample + 0.5) / magfactor_ - 0.5);
    return ig_->ground_coordinate_dem(ic2, D); 
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
  { 
    ImageCoordinate ic2((Ic.line + 0.5) / magfactor_ - 0.5,
			(Ic.sample + 0.5) / magfactor_ - 0.5);
    return ig_->ground_coordinate_approx_height(ic2, H); 
  }
  virtual int number_line() const {return ig_->number_line() * magfactor_; }
  virtual int number_sample() const {return ig_->number_sample() * magfactor_; }

//-----------------------------------------------------------------------
/// Return image coordinate that goes with a particular GroundCoordinate.
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const
  {
    ImageCoordinate ic = ig_->image_coordinate(Gc);
    ic.line = (ic.line + 0.5) * magfactor_ - 0.5;
    ic.sample = (ic.sample + 0.5) * magfactor_ - 0.5;
    return ic;
  }

  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) const
  { 
    blitz::Array<double, 2> res = ig_->image_coordinate_jac_cf(Gc); 
    res *= magfactor_;
    return res;
  }
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { 
    blitz::Array<double, 2> res = ig_->image_coordinate_jac_parm(Gc); 
    res *= magfactor_;
    return res;
  }

  virtual blitz::Array<double, 1> parameter() const 
  { return ig_->parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { ig_->parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return ig_->parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { return ig_->parameter_with_derivative(Parm); }
  virtual std::vector<std::string> parameter_name() const
  { return ig_->parameter_name(); }
  virtual blitz::Array<bool, 1> parameter_mask() const
  { return ig_->parameter_mask(); }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { 
    Os << "MagnifyBilinearImageGroundConnection\n"
       << "  Magnification factor: " << magfactor_ << "\n"
       << "  Original ImageGroundConnection:\n"
       << *ig_ << "\n";
  }

//-----------------------------------------------------------------------
/// Underlying ImageGroundConnection.
//-----------------------------------------------------------------------

  boost::shared_ptr<ImageGroundConnection> original_image_ground_connection()
    const {return ig_;}

//-----------------------------------------------------------------------
/// Return magnification factor
//-----------------------------------------------------------------------

  int magnification_factor() const {return magfactor_;}

private:
  boost::shared_ptr<ImageGroundConnection> ig_;
  int magfactor_;
};



}
#endif
