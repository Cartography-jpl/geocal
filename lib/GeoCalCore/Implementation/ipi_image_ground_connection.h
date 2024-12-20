#ifndef IPI_IMAGE_GROUND_CONNECTION_H
#define IPI_IMAGE_GROUND_CONNECTION_H
#include "image_ground_connection.h"
#include "ipi.h"
#include "ostream_pad.h"
#include <boost/multi_array.hpp>

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by a
  Ipi.
*******************************************************************/

class IpiImageGroundConnection : public virtual ImageGroundConnection, 
				 public Observer<Camera> {
public:
//-----------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------

  IpiImageGroundConnection(const boost::shared_ptr<Ipi>& I, 
			   const boost::shared_ptr<Dem>& D,
			   const boost::shared_ptr<RasterImage>& Img,
			   const std::string& Title = "Image",
			   double Resolution = 30, 
			   double Max_height = 9000) 
    : ImageGroundConnection(D, Img, boost::shared_ptr<RasterImageMultiBand>(),
			    Title), ipi_(I), res(Resolution), 
      max_h(Max_height) 
  {
    ipi_->camera()->add_observer(*this);
  }


//-----------------------------------------------------------------------
/// Clear cache if camera changes.
//-----------------------------------------------------------------------

  virtual void notify_update(const Camera& Observed_object) 
  {
    sc_look_vector_cache.resize(boost::extents[0][0][0][0]);
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~IpiImageGroundConnection() {}
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    // We don't include refraction here. Not sure if this is important
    // or not, we may need to revisit this
    Time t;
    FrameCoordinate f;
    ipi_->time_table()->time(Ic, t, f);
    Lv = ipi_->orbit()->cf_look_vector(t, ipi_->camera()->sc_look_vector(f, ipi_->band()));
    P = ipi_->orbit()->position_cf(t);
  }
  virtual blitz::Array<double, 7> 
  cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp,
		     int nsubpixel_line = 1, 
		     int nsubpixel_sample = 1,
		     int nintegration_step = 1) const;
  virtual blitz::Array<double, 1> 
  collinearity_residual(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  virtual blitz::Array<double, 2> 
  collinearity_residual_jacobian(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic,
			const Dem& D) const
  {
    Time t;
    FrameCoordinate f;
    ipi_->time_table()->time(Ic, t, f);
    if(t < ipi_->orbit()->min_time() || t >= ipi_->orbit()->max_time())
      throw ImageGroundConnectionFailed();
    return ipi_->orbit()->orbit_data(t)->
      surface_intersect(*ipi_->camera(), f, D, res, ipi_->band(), max_h,
			ipi_->refraction(), ipi_->velocity_aberration());
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic,
				  double H) const
  { 
    Time t;
    FrameCoordinate f;
    ipi_->time_table()->time(Ic, t, f);
    if(t < ipi_->orbit()->min_time() || t >= ipi_->orbit()->max_time())
      throw ImageGroundConnectionFailed();
    return ipi_->orbit()->orbit_data(t)->reference_surface_intersect_approximate
      (*ipi_->camera(), f, ipi_->band(), H, ipi_->refraction(),
       ipi_->velocity_aberration());
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
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
					    ImageCoordinate& Res,
					    bool& Success) const
  {
    ipi_->image_coordinate_extended(Gc, Res, Success);
  }
  virtual void image_coordinate_extended(const GroundCoordinate& Gc,
					 ImageCoordinate& Res,
					 bool& Success) const
  {
    ipi_->image_coordinate_extended(Gc, Res, Success);
  }
  virtual void print(std::ostream& Os) const	\
  {
    OstreamPad opad(Os, "    ");
    Os << "IpiImageGroundConnection" << "\n"
       << "  Ipi: \n";
    opad << *ipi_;
    opad.strict_sync();
  }

//-----------------------------------------------------------------------
/// VelocityAberration object we are using. May be null if we are using
/// the default first order approximation. Note that this gets
/// forwarded to the Ipi object we are using - we just have this
/// interface so we don't need to treat IpiImageGroundConnection
/// differently then e.g., OrbitDataImageGroundConnection
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<VelocityAberration>& velocity_aberration() const
  {return ipi_->velocity_aberration();}
  void velocity_aberration(const boost::shared_ptr<VelocityAberration>& V)
  {ipi_->velocity_aberration(V);}

//-----------------------------------------------------------------------
/// Refraction that we are using. Note that this gets
/// forwarded to the Ipi object we are using - we just have this
/// interface so we don't need to treat IpiImageGroundConnection
/// differently then e.g., OrbitDataImageGroundConnection.
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<Refraction>& refraction() const
  {return ipi_->refraction();}
  void refraction(const boost::shared_ptr<Refraction>& Ref)
  {ipi_->refraction(Ref);}

  
//-----------------------------------------------------------------------
/// IPI that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Ipi>& ipi() const { return ipi_; }

//-----------------------------------------------------------------------
/// Synonym of ipi, for backwards compatibility. This is deprecated,
/// you should use the new ipi() name.
//-----------------------------------------------------------------------

  const boost::shared_ptr<Ipi>& ipi_ptr() const { return ipi_; }

//-----------------------------------------------------------------------
/// Resolution we step through Dem at, in meters.
//-----------------------------------------------------------------------

  double resolution() const {return res;}

  virtual void footprint_resolution(int Line, int Sample, 
				    double &Line_resolution_meter, 
				    double &Sample_resolution_meter) const;

//-----------------------------------------------------------------------
/// Maximum height we look through Dem at.
//-----------------------------------------------------------------------

  double maximum_height() const {return max_h;}
  virtual int number_line() const { return ipi_->time_table()->max_line() + 1; }
  virtual int number_sample() const 
  { return ipi_->camera()->number_sample(ipi_->band()); }

  virtual bool has_time() const {return true;}
  virtual Time pixel_time(const ImageCoordinate& Ic) const
  {
    Time res;
    FrameCoordinate fc;
    ipi_->time_table()->time(Ic, res, fc);
    return res;
  }
protected:
  IpiImageGroundConnection() {}
  void initialize(const boost::shared_ptr<Ipi>& I, 
		  const boost::shared_ptr<Dem>& D,
		  const boost::shared_ptr<RasterImage>& Img,
		  const std::string& Title = "Image",
		  double Resolution = 30, 
		  double Max_height = 9000)
  { ImageGroundConnection::initialize(D, Img, 
	      boost::shared_ptr<RasterImageMultiBand>(), Title);
    ipi_ = I;
    res = Resolution;
    max_h = Max_height;
    ipi_->camera()->add_observer(*this);
  }
private:
  boost::shared_ptr<Ipi> ipi_;
  double res, max_h;
  mutable boost::multi_array<ScLookVector, 4> sc_look_vector_cache;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(IpiImageGroundConnection);
#endif
