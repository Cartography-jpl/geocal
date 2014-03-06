#ifndef IPI_IMAGE_GROUND_CONNECTION_H
#define IPI_IMAGE_GROUND_CONNECTION_H
#include "image_ground_connection.h"
#include "ipi.h"

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by a
  Ipi.
*******************************************************************/

class IpiImageGroundConnection : public ImageGroundConnection {
public:
//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  IpiImageGroundConnection(const boost::shared_ptr<Ipi>& I, 
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image",
			   double Resolution = 30, int Band = 0,
			   double Max_height = 9000) 
    : ImageGroundConnection(D, Img, boost::shared_ptr<RasterImageMultiBand>(),
			    Title), ipi_(I), res(Resolution), 
      max_h(Max_height), b(Band) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~IpiImageGroundConnection() {}
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    Time t;
    FrameCoordinate f;
    ipi_->time_table().time(Ic, t, f);
    Lv = ipi_->orbit().cf_look_vector(t, ipi_->camera().sc_look_vector(f, b));
    P = ipi_->orbit().position_cf(t);
  }
  
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic,
			const Dem& D) const
  { 
    Time t;
    FrameCoordinate f;
    ipi_->time_table().time(Ic, t, f);
    return ipi_->orbit().orbit_data(t)->
      surface_intersect(ipi_->camera(), f, D, res, b, max_h);
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic,
				  double H) const
  { 
    Time t;
    FrameCoordinate f;
    ipi_->time_table().time(Ic, t, f);
    return ipi_->orbit().orbit_data(t)->
      reference_surface_intersect_approximate(ipi_->camera(), f, b, H);
  }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const 
  { 
    ImageCoordinate res;
    bool success;
    ipi_->image_coordinate_extended(Gc, res, success);
    if(!success)
      throw ImageGroundConnectionFailed();
    return res; 
  }
  virtual void print(std::ostream& Os) const \
  {
    Os << "IpiImageGroundConnection" << "\n"
       << "  Ipi: \n"
       << *ipi_ << "\n";
  }

//-----------------------------------------------------------------------
/// IPI that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Ipi>& ipi_ptr() const { return ipi_; }

  const Ipi& ipi() const { return *ipi_; }

//-----------------------------------------------------------------------
/// Band we are working with.
//-----------------------------------------------------------------------
  
  int band() const {return b;}

//-----------------------------------------------------------------------
/// Resolution we step through Dem at, in meters.
//-----------------------------------------------------------------------

  double resolution() const {return res;}

//-----------------------------------------------------------------------
/// Maximum height we look through Dem at.
//-----------------------------------------------------------------------

  double maximum_height() const {return max_h;}
  virtual int number_line() const { return ipi_->time_table().max_line(); }
  virtual int number_sample() const 
  { return ipi_->camera().number_sample(b); }
private:
  boost::shared_ptr<Ipi> ipi_;
  double res, max_h;
  int b;
};

}
#endif