#ifndef ORBIT_DATA_IMAGE_GROUND_CONNECTION_H
#define ORBIT_DATA_IMAGE_GROUND_CONNECTION_H
#include "image_ground_connection.h"
#include "orbit.h"
#include "refraction.h"
#include "velocity_aberration.h"
#include "simple_dem.h"
#include "ostream_pad.h"

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by
  OrbitData and a Camera.
*******************************************************************/

class OrbitDataImageGroundConnection : public virtual ImageGroundConnection,
				       public Observer<Orbit> {
public:
//-----------------------------------------------------------------------
/// Constructor. You can optionally include a approximate refraction
/// correction, the default is not to.
//-----------------------------------------------------------------------

  OrbitDataImageGroundConnection(const boost::shared_ptr<OrbitData>& Od,
				 const boost::shared_ptr<Camera>& Cam, 
				 const boost::shared_ptr<Dem>& D,
				 const boost::shared_ptr<RasterImage>& Img,
				 const std::string Title = "",
				 const boost::shared_ptr<Refraction>&
				 Ref = boost::shared_ptr<Refraction>(),
				 const boost::shared_ptr<VelocityAberration>&
				 Vabb = boost::shared_ptr<VelocityAberration>(),
				 double Resolution=30, int Band=0, 
				 double Max_height=9000)
    : ImageGroundConnection(D, Img, boost::shared_ptr<RasterImageMultiBand>(),
			    Title), od(Od), cam(Cam),
      refraction_(Ref),
      velocity_aberration_(Vabb),
      res(Resolution), b(Band), max_h(Max_height) {}

//-----------------------------------------------------------------------
/// Constructor that takes an Orbit and a time. We populate this using
/// the OrbitData from the orbit for that time. Moreover, we make this
/// class an Observer of the underlying orbit. When the orbit
/// notifies us of changes, we regenerate the orbit data. This means
/// that this class will remain in sync with changes in the underlying
/// orbit.
//-----------------------------------------------------------------------

  OrbitDataImageGroundConnection(const boost::shared_ptr<Orbit>& Orb,
  				 const Time& Tm,
  				 const boost::shared_ptr<Camera>& Cam, 
  				 const boost::shared_ptr<Dem>& D,
  				 const boost::shared_ptr<RasterImage>& Img,
  				 const std::string Title = "",
  				 const boost::shared_ptr<Refraction>&
  				 Ref = boost::shared_ptr<Refraction>(),
				 const boost::shared_ptr<VelocityAberration>&
				 Vabb = boost::shared_ptr<VelocityAberration>(),
  				 double Resolution=30, int Band=0, 
  				 double Max_height=9000)
    : ImageGroundConnection(D, Img, boost::shared_ptr<RasterImageMultiBand>(),
  			    Title), 
      orb(Orb),
      cam(Cam),      
      refraction_(Ref),
      velocity_aberration_(Vabb),
      res(Resolution), b(Band), max_h(Max_height) 
  {
    od = orb->orbit_data(TimeWithDerivative(Tm));
    orb->add_observer(*this);
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OrbitDataImageGroundConnection() {}
  virtual void notify_update(const Orbit& Orb)
  { od = Orb.orbit_data(od->time_with_derivative()); }
  virtual blitz::Array<double, 7> 
  cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp,
		     int nsubpixel_line = 1, 
		     int nsubpixel_sample = 1,
		     int nintegration_step = 1) const
  {
    throw Exception("Need to implement this.\n");
  }
  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    // We don't include refraction here. Not sure if this is important
    // or not, we may need to revisit this
    Lv = od->cf_look_vector(cam->sc_look_vector
			    (FrameCoordinate(Ic.line, Ic.sample), b));
    P = od->position_cf();
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const
  {
    return od->surface_intersect(*cam, FrameCoordinate(Ic.line, Ic.sample),
		 D, res, b, max_h, refraction_, velocity_aberration_);
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
  { 
    return od->reference_surface_intersect_approximate
      (*cam, FrameCoordinate(Ic.line, Ic.sample), b, H, refraction_,
       velocity_aberration_);
  }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const 
  {
    // TODO include velocity_aberration_
    FrameCoordinate fc;
    if(refraction_) {
      CartesianFixedLookVector lv =
	refraction_->refraction_reverse(*od->position_cf(), Gc);
      fc = cam->frame_coordinate(od->sc_look_vector(lv), b);
    } else
      fc = od->frame_coordinate(Gc, *cam, b);
    return ImageCoordinate(fc.line, fc.sample);
  }

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// parameters.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { 
    // TODO include velocity_aberration_
    FrameCoordinateWithDerivative fc;
    if(refraction_) {
      boost::shared_ptr<GroundCoordinate> gc_uncorr =
	SimpleDem().intersect(*od->position_cf(),
	      refraction_->refraction_reverse(*od->position_cf(), Gc),
	      1);
      fc = od->frame_coordinate_with_derivative(*gc_uncorr, *cam, b);
    } else
      fc = od->frame_coordinate_with_derivative(Gc, *cam, b);
    blitz::Array<double, 2> res(2, fc.line.gradient().rows());
    res(0, blitz::Range::all()) = fc.line.gradient();
    res(1, blitz::Range::all()) = fc.sample.gradient();
    return res;
  }

//-----------------------------------------------------------------------
/// This is image_coordinate, but include the derivative of this
/// with respect to the parameters of the Camera and OrbitData. Not
/// sure if we want this to be a general ImageGroundConnection
/// function, but for now we have this defined just for this class.
//-----------------------------------------------------------------------
  
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const GroundCoordinate& Gc) const 
  { 
    // TODO include velocity_aberration_
    FrameCoordinateWithDerivative fc;
    if(refraction_) {
      boost::shared_ptr<GroundCoordinate> gc_uncorr =
	SimpleDem().intersect(*od->position_cf(),
	      refraction_->refraction_reverse(*od->position_cf(), Gc),
	      1);
      fc = od->frame_coordinate_with_derivative(*gc_uncorr, *cam, b);
    } else
      fc = od->frame_coordinate_with_derivative(Gc, *cam, b);
    return ImageCoordinateWithDerivative(fc.line, fc.sample);
  }
  virtual void print(std::ostream& Os) const \
  {
    OstreamPad opad(Os, "    ");
    Os << "OrbitDataImageGroundConnection" << "\n"
       << "  Title:      " << title() << "\n"
       << "  Resolution: " << resolution() << "\n"
       << "  Band:       " << band() << "\n"
       << "  Max height: " << max_height() << "\n"
       << "  Orbit Data: \n";
    opad << *od;
    opad.strict_sync();
    Os << "  Camera: \n";
    opad << *cam;
    opad.strict_sync();
    Os << "  Dem: \n";
    opad << dem();
    opad.strict_sync();
    Os << "  Image: \n";
    if(!image())
      opad << "No image\n";
    else
      opad << *image();
    opad.strict_sync();
    Os << "  Refraction\n";
    if(!refraction())
      opad << "No refraction model included\n";
    else
      opad << *refraction();
    Os << "  VelocityAberration\n";
    if(!velocity_aberration())
      opad << "No velocity_aberration model included, using first order approximation\n";
    else
      opad << *velocity_aberration();
    opad.strict_sync();
  }

//-----------------------------------------------------------------------
/// Orbit data that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<OrbitData>& orbit_data() const { return od; }

//-----------------------------------------------------------------------
/// Orbit that we are using, may be null if we are just using a fixed
/// orbit data.
//-----------------------------------------------------------------------

  const boost::shared_ptr<Orbit>& orbit() const { return orb; }

//-----------------------------------------------------------------------
/// Set orbit data that we are using
//-----------------------------------------------------------------------

  void orbit_data(const boost::shared_ptr<OrbitData>& Od) { od = Od; }

  virtual bool has_time() const {return true;}
  virtual Time pixel_time(const ImageCoordinate& Ic) const
  {
    return od->time();
  }

//-----------------------------------------------------------------------
/// Camera that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<Camera>& camera() const {return cam; }

//-----------------------------------------------------------------------
/// Set Camera that we are using
//-----------------------------------------------------------------------

  void camera(const boost::shared_ptr<Camera>& C) { cam = C; }

//-----------------------------------------------------------------------
/// Resolution in meters that we examine Dem out. This affects how
/// long ground_coordinate takes to figure out. It should be about the
/// resolution of the Dem
//-----------------------------------------------------------------------

  double resolution() const { return res; }

//-----------------------------------------------------------------------
/// Set resolution in meters that we examine Dem out. This affects how
/// long ground_coordinate takes to figure out. It should be about the
/// resolution of the Dem
//-----------------------------------------------------------------------

  void resolution(double R) { res = R; }

//-----------------------------------------------------------------------
/// Camera band we are using.
//-----------------------------------------------------------------------

  int band() const { return b; }

//-----------------------------------------------------------------------
/// Set camera band we are using.
//-----------------------------------------------------------------------

  void band(int B) { b = B; }

//-----------------------------------------------------------------------
/// Maximum height that we expect to see in the Dem.
//-----------------------------------------------------------------------

  double max_height() const {return max_h;}

//-----------------------------------------------------------------------
/// Set Maximum height that we expect to see in the Dem.
//-----------------------------------------------------------------------

  void max_height(double Max_h) { max_h = Max_h;}

//-----------------------------------------------------------------------
/// Refraction object we are using. May be null if we aren't including
/// refraction. 
//-----------------------------------------------------------------------
  boost::shared_ptr<Refraction> refraction() const {return refraction_;}

//-----------------------------------------------------------------------
/// Set refraction object we are using. May be null if we aren't including
/// refraction. 
//-----------------------------------------------------------------------
  void refraction(const boost::shared_ptr<Refraction>& Ref) 
  {refraction_ = Ref;}

//-----------------------------------------------------------------------
/// VelocityAberration object we are using. May be null if we are using
/// the default first order approximation.
//-----------------------------------------------------------------------
  boost::shared_ptr<VelocityAberration> velocity_aberration() const
  {return velocity_aberration_;}

//-----------------------------------------------------------------------
/// Set the VelocityAberration object we are using. May be null if we are using
/// the default first order approximation.
//-----------------------------------------------------------------------
  void velocity_aberration(const boost::shared_ptr<VelocityAberration>& Vabb) 
  {velocity_aberration_ = Vabb;}

  virtual int number_line() const { return cam->number_line(band()); }
  virtual int number_sample() const { return cam->number_sample(band()); }
protected:
  OrbitDataImageGroundConnection() {}
private:
  boost::shared_ptr<Orbit> orb;
  boost::shared_ptr<OrbitData> od;
  boost::shared_ptr<Camera> cam;
  boost::shared_ptr<Refraction> refraction_;
  boost::shared_ptr<VelocityAberration> velocity_aberration_;
  double res;
  int b;
  double max_h;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitDataImageGroundConnection);
GEOCAL_CLASS_VERSION(OrbitDataImageGroundConnection, 1)
#endif
