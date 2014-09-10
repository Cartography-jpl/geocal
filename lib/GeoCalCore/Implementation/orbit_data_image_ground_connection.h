#ifndef ORBIT_DATA_IMAGE_GROUND_CONNECTION_H
#define ORBIT_DATA_IMAGE_GROUND_CONNECTION_H
#include "image_ground_connection.h"
#include "orbit.h"
#include "refraction.h"
#include "ostream_pad.h"

namespace GeoCal {
/****************************************************************//**
  This is a ImageGroundConnection where the connection is made by
  OrbitData and a Camera.
*******************************************************************/

class OrbitDataImageGroundConnection : public ImageGroundConnection {
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
				 double Resolution=30, int Band=0, 
				 double Max_height=9000)
    : ImageGroundConnection(D, Img, boost::shared_ptr<RasterImageMultiBand>(),
			    Title), od(Od), cam(Cam),
      refraction_(Ref),
      res(Resolution), b(Band), max_h(Max_height) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OrbitDataImageGroundConnection() {}
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
    boost::shared_ptr<GroundCoordinate> gc_uncorr = 
      od->surface_intersect(*cam, FrameCoordinate(Ic.line, Ic.sample),
			    D, res, b, max_h);
    if(!refraction_)
      return gc_uncorr;
    boost::shared_ptr<GroundCoordinate> gc_corr =
      refraction_->refraction_apply(*od->position_cf(), *gc_uncorr);
    boost::shared_ptr<CartesianFixed> gc_corr_cf = gc_corr->convert_to_cf();
    CartesianFixedLookVector lv
      (gc_corr_cf->position[0] - od->position_cf()->position[0],
       gc_corr_cf->position[1] - od->position_cf()->position[1],
       gc_corr_cf->position[2] - od->position_cf()->position[2]);
    return D.intersect(*od->position_cf(), lv, res, max_h);
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
  { 
    boost::shared_ptr<GroundCoordinate> gc_uncorr = 
      od->reference_surface_intersect_approximate
      (*cam, FrameCoordinate(Ic.line, Ic.sample), b, H);
    if(!refraction_)
      return gc_uncorr;
    boost::shared_ptr<GroundCoordinate> gc_corr =
      refraction_->refraction_apply(*od->position_cf(), *gc_uncorr);
    boost::shared_ptr<CartesianFixed> gc_corr_cf = gc_corr->convert_to_cf();
    CartesianFixedLookVector lv
      (gc_corr_cf->position[0] - od->position_cf()->position[0],
       gc_corr_cf->position[1] - od->position_cf()->position[1],
       gc_corr_cf->position[2] - od->position_cf()->position[2]);
    return od->position_cf()->reference_surface_intersect_approximate(lv, H);
  }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const 
  { 
    FrameCoordinate fc;
    if(refraction_) {
      boost::shared_ptr<GroundCoordinate> gc_uncorr =
	refraction_->refraction_reverse(*od->position_cf(), Gc);
      fc = od->frame_coordinate(*gc_uncorr, *cam, b);
    } else
      fc = od->frame_coordinate(Gc, *cam, b);
    return ImageCoordinate(fc.line, fc.sample);
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
    opad << *image();
    opad.strict_sync();
    Os << "  Refraction\n";
    if(!refraction())
      opad << "No refraction model included\n";
    else
      opad << *refraction();
    opad.strict_sync();
  }

//-----------------------------------------------------------------------
/// Orbit data that we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<OrbitData>& orbit_data() const { return od; }


//-----------------------------------------------------------------------
/// Set orbit data that we are using
//-----------------------------------------------------------------------

  void orbit_data(const boost::shared_ptr<OrbitData>& Od) { od = Od; }

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
private:
  boost::shared_ptr<OrbitData> od;
  boost::shared_ptr<Camera> cam;
  boost::shared_ptr<Refraction> refraction_;
  double res;
  int b;
  double max_h;
};
}
#endif
