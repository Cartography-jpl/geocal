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
    P = position_cf(t);
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

blitz::Array<double, 1> 
IgcRollingShutter::collinearity_residual
(const GroundCoordinate& Gc,
 const ImageCoordinate& Ic_actual) const
{
  // Instead of the difference in ImageCoordinates returned by the
  // Ipi, use the difference in the FrameCoordinate for the time
  // associated with Ic_actual. This is faster to calculate, and the
  // Jacobian is much better behaved.
  Time t;
  FrameCoordinate fc_actual;
  time_table_->time(Ic_actual, t, fc_actual);
  FrameCoordinate fc_predict = 
    orbit_data(t)->frame_coordinate(Gc, *cam, b);
  Array<double, 1> res(2);
  res(0) = fc_predict.line - fc_actual.line;
  res(1) = fc_predict.sample - fc_actual.sample;
  return res;
}

blitz::Array<double, 2> 
IgcRollingShutter::collinearity_residual_jacobian
(const GroundCoordinate& Gc,
 const ImageCoordinate& Ic_actual) const
{
  TimeWithDerivative t;
  FrameCoordinateWithDerivative fc_actual;
  ImageCoordinateWithDerivative ica(Ic_actual.line, Ic_actual.sample);
  time_table_->time_with_derivative(ica, t, fc_actual);
  FrameCoordinateWithDerivative fc_predict = 
    orbit_data(t)->frame_coordinate_with_derivative(Gc, *cam, b);
  Array<double, 2> res(2, fc_predict.line.number_variable() + 3);
  res(0, Range(0, res.cols() - 4)) = 
    (fc_predict.line - fc_actual.line).gradient();
  res(1, Range(0, res.cols() - 4)) = 
    (fc_predict.sample - fc_actual.sample).gradient();

  // Part of jacobian for cf coordinates.
  boost::shared_ptr<OrbitData> od = orbit_data(t.value());
  boost::shared_ptr<CartesianFixed> p1 = od->position_cf();
  boost::shared_ptr<CartesianFixed> p2 = Gc.convert_to_cf();
  CartesianFixedLookVectorWithDerivative lv;
  for(int i = 0; i < 3; ++i) {
    AutoDerivative<double> p(p2->position[i], i, 3);
    lv.look_vector[i] = p - p1->position[i];
  }
  ScLookVectorWithDerivative sl = od->sc_look_vector(lv);
  boost::shared_ptr<Camera> c = cam;
  ArrayAd<double, 1> poriginal = c->parameter_with_derivative();
  c->parameter_with_derivative(c->parameter());
  FrameCoordinateWithDerivative fc_gc =
    cam->frame_coordinate_with_derivative(sl, b);
  c->parameter_with_derivative(poriginal);
  res(0, Range(res.cols() - 3, toEnd)) = fc_gc.line.gradient();
  res(1, Range(res.cols() - 3, toEnd)) = fc_gc.sample.gradient();
  return res;
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
  IcEq(const IgcRollingShutter& Igc,
       const boost::shared_ptr<TimeTable>& Tt,
       const boost::shared_ptr<Camera>& Cam,
       const boost::shared_ptr<CartesianFixed>& P,
       Time Tmin,
       int Band)
    : igc(Igc), tt(Tt), cam(Cam), p(P), band(Band), 
      tmin(Tmin)
  { }
  virtual ~IcEq() {}
  virtual double operator()(const double& Toffset) const
  {
    Time t = tmin + Toffset;
    FrameCoordinate fc(cam->frame_line_coordinate
		       (sc_look_vector(t), 
			band), 0);
    ImageCoordinate ic = tt->image_coordinate(t, fc);
    return ic.line - fc.line;
  }
  virtual double df(double Toffset) const
  {
    // Right now just do a finite difference. We can revisit this if
    // needed. 
    double eps = 1e-3;
    // This might go past the edge of the time table, so add handling
    // to switch where eps is applied if needed.
    if(Toffset + eps < tt->max_time() - tmin)
      return ((*this)(Toffset + eps) - (*this)(Toffset)) / eps;
    else
      return ((*this)(Toffset) - (*this)(Toffset - eps)) / eps;
  }
  virtual AutoDerivative<double> f_with_derivative(double Toffset) const
  {
    Time t = tmin + Toffset;
    FrameCoordinateWithDerivative fc = 
      cam->frame_coordinate_with_derivative
      (sc_look_vector_with_derivative(t), band);
    ImageCoordinateWithDerivative ic = 
      tt->image_coordinate_with_derivative(t, fc);
    return ic.line - fc.line;
    
  }
  ImageCoordinate image_coordinate(double Toffset) const
  {
    Time t = tmin + Toffset;
    return tt->image_coordinate(t, cam->frame_coordinate(sc_look_vector(t), band));
  }
  ImageCoordinateWithDerivative image_coordinate_with_derivative
  (const AutoDerivative<double>& Toffset) const
  {
    TimeWithDerivative t = TimeWithDerivative::time_pgs(tmin.pgs() + Toffset);
    return tt->image_coordinate_with_derivative(t, cam->frame_coordinate_with_derivative(sc_look_vector_with_derivative(t), band));
  }
  ScLookVector sc_look_vector(Time T) const
  {
    CartesianFixedLookVector cf = look_vector(T);
    ScLookVector res;
    double k = cf.length() / Constant::speed_of_light;
    boost::math::quaternion<double> sc =
      conj(igc.sc_to_cf(T)) * (cf.look_quaternion() + k * igc.velocity_cf(T)) * 
      igc.sc_to_cf(T);
    res.look_quaternion(sc);
    return res;
  }
  ScLookVectorWithDerivative sc_look_vector_with_derivative
  (const TimeWithDerivative& T) const
  {
    return igc.orbit_->sc_look_vector(T, look_vector_with_derivative(T));
  }
  CartesianFixedLookVector look_vector(Time T) const
  {
    boost::array<double, 3> p1 = p->position;
    boost::array<double, 3> p2;
    igc.position_cf(T, p2);
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
    boost::array<AutoDerivative<double>, 3> p2 = igc.orbit_->position_cf_with_derivative(T);
    CartesianFixedLookVectorWithDerivative lv;
    lv.look_vector[0] = p1[0] - p2[0];
    lv.look_vector[1] = p1[1] - p2[1];
    lv.look_vector[2] = p1[2] - p2[2];
    return lv;
  }
private:
  const IgcRollingShutter& igc;
  boost::shared_ptr<QuaternionOrbitData> 
  orbit_data(const Time& Tm) const
  { return igc.orbit_data(Tm); }
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
  bool success;
  ImageCoordinate res;
  image_coordinate_with_status(Gc, res, success);
  if(!success)
    throw ImageGroundConnectionFailed();
  return res;
}

// See base class for description

void IgcRollingShutter::image_coordinate_with_status
(const GroundCoordinate& Gc,
 ImageCoordinate& Res,
 bool& Success) const
{
  IgcRollingShutterHelper::IcEq 
    eq(*this, time_table_, cam, Gc.convert_to_cf(), 
       time_table_->min_time(), b);
  if(eq(0) * eq(time_table_->max_time() - time_table_->min_time()) > 0) {
    Success = false;
    return;
  }
  double t = gsl_root(eq, 0, time_table_->max_time() - 
		      time_table_->min_time(), time_tolerance_);
  Res = eq.image_coordinate(t);
  Success = true;
}

// See base class for description

blitz::Array<double, 2> IgcRollingShutter::image_coordinate_jac_parm
(const GroundCoordinate& Gc) const
{
  IgcRollingShutterHelper::IcEq 
    eq(*this, time_table_, cam, Gc.convert_to_cf(), 
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
      pos.push_back(position_cf(t + tint));
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
  notify_update(*orbit_);
}

// Called when orbit has changed.
void IgcRollingShutter::notify_update(const Orbit& Orb)
{
  od1 = boost::dynamic_pointer_cast<QuaternionOrbitData>
    (orbit_->orbit_data(TimeWithDerivative(time_table_->min_time())));
  od2 = boost::dynamic_pointer_cast<QuaternionOrbitData>
    (orbit_->orbit_data(TimeWithDerivative(time_table_->max_time())));
  if(!od1 || !od2)
    throw Exception("This class only works with QuaternionOrbitData");
  pinterp = IgcRollingShutterHelper::PositionInterpolate
    (od1->position_cf()->position,
     od1->velocity_cf(),
     od2->position_cf()->position,
     od2->velocity_cf(),
     od1->time(),
     od2->time() - od1->time());
  qinterp = IgcRollingShutterHelper::QuaternionInterpolate
    (od1->sc_to_cf(),
     od2->sc_to_cf(),
     od1->time(),
     od2->time() - od1->time());
}

