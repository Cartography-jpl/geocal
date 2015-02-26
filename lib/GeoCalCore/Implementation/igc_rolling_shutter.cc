#include "igc_rolling_shutter.h"
#include "ostream_pad.h"
#include "geocal_gsl_root.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcRollingShutter::save
(Archive & ar, const unsigned int version) const
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnection);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(WithParameterNested);
  ar & GEOCAL_NVP_(orbit) & GEOCAL_NVP_(time_table)
    & GEOCAL_NVP(cam) & GEOCAL_NVP_(refraction)
    & GEOCAL_NVP_(roll_direction) & GEOCAL_NVP(res)
    & GEOCAL_NVP(b) & GEOCAL_NVP(max_h) & GEOCAL_NVP_(time_tolerance);
}

template<class Archive>
void IgcRollingShutter::load
(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnection);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(WithParameterNested);
  ar & GEOCAL_NVP_(orbit) & GEOCAL_NVP_(time_table)
    & GEOCAL_NVP(cam) & GEOCAL_NVP_(refraction)
    & GEOCAL_NVP_(roll_direction) & GEOCAL_NVP(res)
    & GEOCAL_NVP(b) & GEOCAL_NVP(max_h) & GEOCAL_NVP_(time_tolerance);
  orbit(orbit_);
}

GEOCAL_IMPLEMENT(IgcRollingShutter);
#endif

//-----------------------------------------------------------------------
/// Initialize object.
//-----------------------------------------------------------------------

void IgcRollingShutter::initialize
(const boost::shared_ptr<Orbit>& Orb,
 const boost::shared_ptr<TimeTable>& Time_table,
 const boost::shared_ptr<Camera>& Cam, 
 const boost::shared_ptr<Dem>& D,
 const boost::shared_ptr<RasterImage>& Img,
 RollDirection Roll_direction,
 const std::string Title,
 const boost::shared_ptr<Refraction>& Ref,
 double Resolution, 
 int Band, 
 double Max_height
)
{
  ImageGroundConnection::initialize(D, Img, 
				    boost::shared_ptr<RasterImageMultiBand>(),
				    Title);
  time_table_ = Time_table; 
  cam = Cam;
  refraction_ = Ref;
  roll_direction_ = Roll_direction;
  res = Resolution;
  b = Band; 
  max_h = Max_height;
  if(roll_direction_ != ROLL_LINE_DIRECTION)
    throw Exception("We currently only support roll line direction");

  // Calculate the time tolerance we'll need when calculating the
  // image coordinate. Right now, we just find the time separation
  // between the first 2 lines and take 0.01 of that, so we calculate
  // the line to about 0.01. We can change to something more
  // sophisticated if needed in the future.
  Time t1, t2;
  FrameCoordinate fignore;
  time_table_->time(ImageCoordinate(0,0), t1, fignore);
  time_table_->time(ImageCoordinate(1,0), t2, fignore);
  time_tolerance_ = (t2 - t1) * 0.01;
  // Make sure we don't get a ridiculous value due to some weirdness
  // in the time table.
  time_tolerance_ = std::max(time_tolerance_, 1e-8);
  orbit(Orb);
}

// See base class for description

void IgcRollingShutter::cf_look_vector
(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
 boost::shared_ptr<CartesianFixed>& P) const
{
    Time t;
    FrameCoordinate f;
    time_table_->time(Ic, t, f);
    boost::shared_ptr<QuaternionOrbitData> od = orbit_data(t);
    Lv = od->cf_look_vector(cam->sc_look_vector(f, b));
    P = od->position_cf();
}

// See base class for description

boost::shared_ptr<GroundCoordinate> IgcRollingShutter::ground_coordinate_dem
(const ImageCoordinate& Ic, const Dem& D) const
{
  Time t;
  FrameCoordinate f;
  time_table_->time(Ic, t, f);
  return orbit_data(t)->surface_intersect(*cam, f, D, res, b, max_h);
}

// See base class for description

boost::shared_ptr<GroundCoordinate> 
IgcRollingShutter::ground_coordinate_approx_height
(const ImageCoordinate& Ic, double H) const
{
  Time t;
  FrameCoordinate f;
  time_table_->time(Ic, t, f);
  return orbit_data(t)->reference_surface_intersect_approximate(*cam, f, b, H);
}

namespace GeoCal {
  namespace IgcRollingShutterHelper {
class IcEq: public GeoCal::DFunctorWithDerivative {
public:
  IcEq(const boost::shared_ptr<Orbit>& Orb,
       const boost::shared_ptr<QuaternionOrbitData>& Od1,
       const boost::shared_ptr<QuaternionOrbitData>& Od2,
       const boost::shared_ptr<TimeTable>& Tt,
       const boost::shared_ptr<Camera>& Cam,
       const boost::shared_ptr<CartesianFixed>& P,
       Time Tmin,
       int Band)
    : orb(Orb), od1(Od1), od2(Od2), tt(Tt), cam(Cam), p(P), band(Band), 
      tmin(Tmin)
  { }
  virtual ~IcEq() {}
  virtual double operator()(const double& Toffset) const
  {
    Time t = tmin + Toffset;
    FrameCoordinate fc(cam->frame_line_coordinate
		       (orb->sc_look_vector(t, look_vector(t)), 
			band), 0);
    ImageCoordinate ic = tt->image_coordinate(t, fc);
    return ic.line - fc.line;
  }
  virtual double df(double Toffset) const
  {
    // Right now just do a finite difference. We can revisit this if
    // needed. 
    double eps = 1e-3;
    return ((*this)(Toffset + eps) - (*this)(Toffset)) / eps;
  }
  virtual AutoDerivative<double> f_with_derivative(double Toffset) const
  {
    Time t = tmin + Toffset;
    FrameCoordinateWithDerivative fc = 
      cam->frame_coordinate_with_derivative
      (orb->sc_look_vector(t, look_vector_with_derivative(t)), 
       band);
    ImageCoordinateWithDerivative ic = 
      tt->image_coordinate_with_derivative(t, fc);
    return ic.line - fc.line;
    
  }
  ImageCoordinate image_coordinate(double Toffset) const
  {
    Time t = tmin + Toffset;
    return tt->image_coordinate(t, cam->frame_coordinate(orb->sc_look_vector(t, look_vector(t)), band));
  }
  ImageCoordinateWithDerivative image_coordinate_with_derivative
  (const AutoDerivative<double>& Toffset) const
  {
    TimeWithDerivative t = TimeWithDerivative::time_pgs(tmin.pgs() + Toffset);
    return tt->image_coordinate_with_derivative(t, cam->frame_coordinate_with_derivative(orb->sc_look_vector(t, look_vector_with_derivative(t)), band));
  }
  CartesianFixedLookVector look_vector(Time T) const
  {
    boost::array<double, 3> p1 = p->position;
    boost::array<double, 3> p2 = orb->position_cf(T)->position;
    CartesianFixedLookVector lv;
    lv.look_vector[0] = p1[0] - p2[0];
    lv.look_vector[1] = p1[1] - p2[1];
    lv.look_vector[2] = p1[2] - p2[2];
    return lv;
  }
  CartesianFixedLookVectorWithDerivative 
  look_vector_with_derivative(const TimeWithDerivative& T) const
  {
    boost::array<double, 3> p1 = p->position;
    boost::array<AutoDerivative<double>, 3> p2 = orb->position_cf_with_derivative(T);
    CartesianFixedLookVectorWithDerivative lv;
    lv.look_vector[0] = p1[0] - p2[0];
    lv.look_vector[1] = p1[1] - p2[1];
    lv.look_vector[2] = p1[2] - p2[2];
    return lv;
  }
private:
  boost::shared_ptr<Orbit> orb;
  boost::shared_ptr<QuaternionOrbitData> od1;
  boost::shared_ptr<QuaternionOrbitData> od2;
  //  boost::shared_ptr<QuaternionOrbitData> 
  // orbit_data(const Time& Tm) const
  //  { return interpolate(*od1, *od2, Tm); }
  boost::shared_ptr<OrbitData> 
  orbit_data(const Time& Tm) const
  { return orb->orbit_data(Tm); }
  boost::shared_ptr<TimeTable> tt;
  boost::shared_ptr<Camera> cam;
  boost::shared_ptr<CartesianFixed> p;
  int band;
  Time tmin;
};  
  }
}
// See base class for description

ImageCoordinate IgcRollingShutter::image_coordinate
(const GroundCoordinate& Gc) const
{
  IgcRollingShutterHelper::IcEq 
    eq(orbit_, od1, od2, time_table_, cam, Gc.convert_to_cf(), 
       time_table_->min_time(), b);
  if(eq(0) * eq(time_table_->max_time() - time_table_->min_time()) > 0)
    throw ImageGroundConnectionFailed();
  double t = gsl_root(eq, 0, time_table_->max_time() - 
		      time_table_->min_time(), time_tolerance_);
  return eq.image_coordinate(t);
}

// See base class for description

blitz::Array<double, 2> IgcRollingShutter::image_coordinate_jac_parm
(const GroundCoordinate& Gc) const
{
  IgcRollingShutterHelper::IcEq 
    eq(orbit_, od1, od2, time_table_, cam, Gc.convert_to_cf(), 
       time_table_->min_time(), b);
  if(eq(0) * eq(time_table_->max_time() - time_table_->min_time()) > 0)
    throw ImageGroundConnectionFailed();
  AutoDerivative<double> t = 
    gsl_root_with_derivative(eq, 0, time_table_->max_time() - 
			     time_table_->min_time(), time_tolerance_);
  ImageCoordinateWithDerivative ic = eq.image_coordinate_with_derivative(t);
  Array<double, 2> jac(2, t.number_variable());
  if(t.number_variable() != 0) {
    if(!ic.line.is_constant())
      jac(0, Range::all()) = ic.line.gradient();
    else
      jac(0, Range::all()) = 0;
    if(!ic.sample.is_constant())
      jac(1, Range::all()) = ic.sample.gradient();
    else
      jac(1, Range::all()) = 0;
  }
  return jac;
}

// See base class for description

blitz::Array<double, 7> IgcRollingShutter::cf_look_vector_arr
(int ln_start, int smp_start, int nline, int nsamp, int nsubpixel_line, 
 int nsubpixel_sample, int nintegration_step) const
{
  CartesianFixedLookVector lv;
  blitz::Array<double, 7>  res(nline, nsamp, nsubpixel_line, 
			       nsubpixel_sample, nintegration_step, 2, 3);
  for(int i = 0; i < nline; ++i) {
    Time t;
    FrameCoordinate f;
    time_table_->time(ImageCoordinate(i + ln_start, 0), t, f);
    std::vector<boost::shared_ptr<OrbitData> > od;
    std::vector<boost::shared_ptr<CartesianFixed> > pos;
    for(int k = 0; k < nintegration_step; ++k) {
      double tint = 0;
      if(k != 0)
	tint = cam->integration_time(b) / (nintegration_step - 1) * k;
      od.push_back(orbit_data(t + tint));
      pos.push_back(od[k]->position_cf());
    }
    for(int j = 0; j < nsamp; ++j) 
      for(int i2 = 0; i2 < nsubpixel_line; ++i2)
	for(int j2 = 0; j2 < nsubpixel_sample; ++j2) {
	  FrameCoordinate fc(i + ln_start + ((double) i2 / nsubpixel_line),
			     j + smp_start + ((double) j2 / nsubpixel_sample));
	  ScLookVector slv = cam->sc_look_vector(fc, b);
	  for(int k = 0; k < nintegration_step; ++k) {
	    lv = od[k]->cf_look_vector(slv);
	    for(int k2 = 0; k2 < 3; ++k2) {
	      res(i, j, i2, j2, k, 0, k2) = pos[k]->position[k2];
	      res(i, j, i2, j2, k, 1, k2) = lv.look_vector[k2];
	    }
	  }
	}
  }
  return res;
}

// See base class for description

void IgcRollingShutter::footprint_resolution
(int Line, int Sample, double &Line_resolution_meter, 
 double &Sample_resolution_meter) const
{
  Time t;
  FrameCoordinate f;
  time_table_->time(ImageCoordinate(Line, Sample), t, f);
  boost::shared_ptr<GroundCoordinate> gc = orbit_data(t)->
    reference_surface_intersect_approximate(*cam, f, b);
  f.line += 1;
  boost::shared_ptr<GroundCoordinate> gc1 = orbit_data(t)->
    reference_surface_intersect_approximate(*cam, f, b);
  f.line -= 1;
  f.sample += 1;
  boost::shared_ptr<GroundCoordinate> gc2 = orbit_data(t)->
    reference_surface_intersect_approximate(*cam, f, b);
  Line_resolution_meter = distance(*gc, *gc1);
  Sample_resolution_meter = distance(*gc, *gc2);
}

// See base class for description
void IgcRollingShutter::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "IgcRollingShutter" << "\n"
     << "  Title:      " << title() << "\n"
     << "  Resolution: " << resolution() << "\n"
     << "  Band:       " << band() << "\n"
     << "  Max height: " << max_height() << "\n"
     << "  Orbit: \n";
  opad << *orbit_;
  opad.strict_sync();
  Os << "  Time table:\n";
  opad << *time_table_;
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
/// Set Camera that we are using
//-----------------------------------------------------------------------

void IgcRollingShutter::camera(const boost::shared_ptr<Camera>& C) 
{ 
  cam = C; 
  clear_object();
  add_object(orbit_);
  add_object(cam);
}

//-----------------------------------------------------------------------
/// Set orbit.
//-----------------------------------------------------------------------

void IgcRollingShutter::orbit(const boost::shared_ptr<Orbit>& Orb) 
{ 
  if(orbit_)
    orbit_->remove_observer(*this);
  orbit_ = Orb;
  orbit_->add_observer(*this);
  clear_object();
  add_object(orbit_);
  add_object(cam);
  od1 = boost::dynamic_pointer_cast<QuaternionOrbitData>
    (orbit_->orbit_data(time_table_->min_time()));
  od2 = boost::dynamic_pointer_cast<QuaternionOrbitData>
    (orbit_->orbit_data(time_table_->max_time()));
  if(!od1 || !od2)
    throw Exception("This class only works with QuaternionOrbitData");
}
