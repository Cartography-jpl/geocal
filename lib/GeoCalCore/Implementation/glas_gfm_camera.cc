#include "glas_gfm_camera.h"
#include "geocal_serialize_support.h"
#include "geocal_gsl_root.h"
#include "ostream_pad.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GlasGfmCamera::save(Archive& Ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void GlasGfmCamera::load(Archive& Ar, const unsigned int version)
{
  frame_to_sc_nd_ = value(frame_to_sc_);
  init_model();
}

template<class Archive>
void GlasGfmCamera::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Camera);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(Camera, WithParameter);
  GEOCAL_BASE(GlasGfmCamera, Camera);
  ar & GEOCAL_NVP_(focal_length)
    & GEOCAL_NVP2("number_line", nline_)
    & GEOCAL_NVP2("number_sample", nsamp_)
    & GEOCAL_NVP_(frame_to_sc)
    & GEOCAL_NVP_(band_type) & GEOCAL_NVP_(band_wavelength)
    & GEOCAL_NVP_(band_index) & GEOCAL_NVP_(irepband)
    & GEOCAL_NVP_(isubcat) & GEOCAL_NVP_(sample_number_first)
    & GEOCAL_NVP_(delta_sample_pair) & GEOCAL_NVP_(field_alignment)
    & GEOCAL_NVP_(field_angle_type)
    & GEOCAL_NVP_(field_angle_interpolation_type)
    & GEOCAL_NVP_(first_line_block)
    & GEOCAL_NVP_(first_sample_block)
    & GEOCAL_NVP_(delta_line_block)
    & GEOCAL_NVP_(delta_sample_block)
    & GEOCAL_NVP_(field_alignment_block);
  boost::serialization::split_member(ar, *this, version);
}


GEOCAL_IMPLEMENT(GlasGfmCamera);
#endif

// We have three different ways of modeling a camera in GlasGfm.
// The differences are just in how we go from FrameCoordinate to xp,
// yp and back. Abstract this out so we can cleanly separate the three
// models
namespace GeoCal {

class GlasGfmCameraModelImp : public Observer<Camera> {
public:
  GlasGfmCameraModelImp(GlasGfmCamera& Cam)
    : cam(Cam), cache_stale(true)
  { Cam.add_observer(*this); }
  virtual ~GlasGfmCameraModelImp() {}
  virtual void fc_to_xy(const FrameCoordinate& F, double &X, double& Y)
    const = 0;
  virtual void fc_to_xy(const FrameCoordinateWithDerivative& F,
 			AutoDerivative<double> &X,
 			AutoDerivative<double>& Y) const = 0;
  virtual void xy_to_fc(double X, double Y, FrameCoordinate& F) const = 0;
  virtual void xy_to_fc(const AutoDerivative<double>& X,
 			const AutoDerivative<double>& Y,
 			FrameCoordinateWithDerivative& F) const = 0;
  virtual void notify_update(const Camera& Observed_object)
  { cache_stale = true; }
  const GlasGfmCamera& cam;
  mutable bool cache_stale;
};

/// Implementation that use the field_alignment array.
class GlasFa : public GlasGfmCameraModelImp {
public:
  GlasFa(GlasGfmCamera& Cam) : GlasGfmCameraModelImp(Cam) {}
  virtual ~GlasFa() {}
  virtual void fc_to_xy(const FrameCoordinate& F, double &X, double& Y)
    const
  {
    fill_cache();
    int i = F.sample / dsamp;
    if(i < 0)
      i = 0;
    if(i >= fa.rows())
      i = fa.rows() - 1;
    int ibase = i * dsamp;
    X = (F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp + fa(i,0);
    // GLAS doesn't actually support line numbers being anything other
    // than 0. But it is very useful to be able to have a "extended"
    // camera, so we just give it a pitch of the average of all the
    // sample data as a "reasonable" thing.
    Y = (F.sample - ibase) * (fa(i, 3) - fa(i,1)) / dsamp + fa(i,1) -
      (F.line - 0) * x_space;
  }
  virtual void fc_to_xy(const FrameCoordinateWithDerivative& F,
			AutoDerivative<double>& X,
			AutoDerivative<double>& Y) const
  {
    fill_cache();
    int i = F.sample.value() / dsamp;
    if(i < 0)
      i = 0;
    if(i >= fa.rows())
      i = fa.rows() - 1;
    int ibase = i * dsamp;
    X = (F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp + fa(i,0);
    // GLAS doesn't actually support line numbers being anything other
    // than 0. But it is very useful to be able to have a "extended"
    // camera, so we just give it a pitch of the average of all the
    // sample data as a "reasonable" thing.
    Y = (F.sample - ibase)  * (fa(i, 3) - fa(i,1)) / dsamp + fa(i,1) -
      (F.line - 0) * x_space;
  }
  virtual void xy_to_fc(double X, double Y, FrameCoordinate& F) const
  {
    fill_cache();
    int i = index_from_x(X);
    int ibase = i * dsamp;
    F.sample = (X - fa(i,0)) / ((fa(i, 2) - fa(i,0)) / dsamp) + ibase;
    F.line = -(Y - ((F.sample - ibase)  * (fa(i, 3) - fa(i,1)) / dsamp + fa(i,1))) / x_space;
  }
  virtual void xy_to_fc(const AutoDerivative<double>& X,
			const AutoDerivative<double>& Y,
			FrameCoordinateWithDerivative& F) const
  {
    fill_cache();
    int i = index_from_x(X.value());
    int ibase = i * dsamp;
    F.sample = (X - fa(i,0)) / ((fa(i, 2) - fa(i,0)) / dsamp) + ibase;
    F.line = -(Y - ((F.sample - ibase)  * (fa(i, 3) - fa(i,1)) / dsamp + fa(i,1))) / x_space;
  }
  int index_from_x(double X) const
  {
    if(x_space > 0) {
      for(int i = 0; i < fa.rows(); ++i)
	if(X < fa(i,3))
	  return i;
      return fa.rows() - 1;
    }
    for(int i = 0; i < fa.rows(); ++i)
      if(X > fa(i,3))
	return i;
    return fa.rows() - 1;
  }
  void fill_cache() const
  {
    if(!cache_stale)
      return;
    nsamp = cam.number_sample(0);
    dsamp = cam.delta_sample_pair();
    fa.reference(cam.field_alignment());
    x_0 = fa(0,0);
    y_0 = fa(0,1);
    x_end = fa(fa.rows() - 1, 2);
    y_end = fa(fa.rows() - 1, 3);
    x_space = (x_end - x_0)/nsamp;
    cache_stale = false;
  }
  mutable double x_0, x_end, x_space, y_0, y_end;
  mutable int nsamp;
  mutable int dsamp;
  mutable blitz::Array<double, 2> fa;
};

class GfmFa;
  
class GfmFaEqu: public VFunctor {
public:
  GfmFaEqu(const GfmFa& F, double X, double Y)
    : f(F), x(X), y(Y) {}
  virtual ~GfmFaEqu() {}
  virtual blitz::Array<double, 1> operator()(const blitz::Array<double, 1>& X) const;
  const GfmFa& f;
  double x, y;
};
/// Implementation that use the field_alignment array.
class GfmFa : public GlasGfmCameraModelImp {
public:
  GfmFa(GlasGfmCamera& Cam) : GlasGfmCameraModelImp(Cam) {}
  virtual ~GfmFa() {}
  virtual void fc_to_xy(const FrameCoordinate& F, double &X, double& Y)
    const
  {
    fill_cache();
    // Right now, just have one fa plane and set of blocks
    double l = (F.line - cam.first_line_block()(0)) / cam.delta_line_block()(0);
    double s = (F.sample - cam.first_sample_block()(0)) / cam.delta_sample_block()(0);
    double x0 = l * (fa(0,0,1,0,0) - fa(0,0,0,0,0)) + fa(0,0,0,0,0);
    double x1 = l * (fa(0,0,1,1,0) - fa(0,0,0,1,0)) + fa(0,0,0,1,0);
    double y0 = l * (fa(0,0,1,0,1) - fa(0,0,0,0,1)) + fa(0,0,0,0,1);
    double y1 = l * (fa(0,0,1,1,1) - fa(0,0,0,1,1)) + fa(0,0,0,1,1);
    X = s * (x1 - x0) + x0;
    Y = s * (y1 - y0) + y0;
  }
  virtual void fc_to_xy(const FrameCoordinateWithDerivative& F,
			AutoDerivative<double>& X,
			AutoDerivative<double>& Y) const
  {
    fill_cache();
    // Right now, just have one fa plane and set of blocks
    AutoDerivative<double> l = (F.line - cam.first_line_block()(0)) / cam.delta_line_block()(0);
    AutoDerivative<double> s = (F.sample - cam.first_sample_block()(0)) / cam.delta_sample_block()(0);
    AutoDerivative<double> x0 = l * (fa(0,0,1,0,0) - fa(0,0,0,0,0)) + fa(0,0,0,0,0);
    AutoDerivative<double> x1 = l * (fa(0,0,1,1,0) - fa(0,0,0,1,0)) + fa(0,0,0,1,0);
    AutoDerivative<double> y0 = l * (fa(0,0,1,0,1) - fa(0,0,0,0,1)) + fa(0,0,0,0,1);
    AutoDerivative<double> y1 = l * (fa(0,0,1,1,1) - fa(0,0,0,1,1)) + fa(0,0,0,1,1);
    X = s * (x1 - x0) + x0;
    Y = s * (y1 - y0) + y0;
  }
  virtual void xy_to_fc(double X, double Y, FrameCoordinate& F) const
  {
    fill_cache();
    // For now, we just brute force this. We can try doing something
    // more refined if this ends up being a bottle neck.
    GfmFaEqu eq(*this, X, Y);
    blitz::Array<double, 1> initial(2);
    initial = cam.number_line(0) / 2, cam.number_sample(0) / 2;
    blitz::Array<double, 1> res = gsl_root(eq, initial, root_residual_needed);
    F.line = res(0);
    F.sample = res(1);
  }
  virtual void xy_to_fc(const AutoDerivative<double>& X,
			const AutoDerivative<double>& Y,
			FrameCoordinateWithDerivative& F) const
  {
    fill_cache();
    FrameCoordinate fc, fc_dx, fc_dy;
    xy_to_fc(X.value(), Y.value(), fc);
    xy_to_fc(X.value() + 0.1 * max_pitch, Y.value(), fc_dx);
    xy_to_fc(X.value(), Y.value() + 0.1 * max_pitch, fc_dy);
    double l_dx = (fc_dx.line - fc.line) / (0.1 * max_pitch);
    double l_dy = (fc_dy.line - fc.line) / (0.1 * max_pitch);
    double s_dx = (fc_dx.sample - fc.sample) / (0.1 * max_pitch);
    double s_dy = (fc_dy.sample - fc.sample) / (0.1 * max_pitch);
    if(X.is_constant() && Y.is_constant()) {
      F.line = fc.line;
      F.sample = fc.sample;
      return;
    }
    int nvar = std::max(X.number_variable(), Y.number_variable());
    Array<double, 1> l_grad(nvar), s_grad(nvar);
    l_grad = l_dx * X.gradient() + l_dy * Y.gradient();
    s_grad = s_dx * X.gradient() + s_dy * Y.gradient();
    F.line = AutoDerivative<double>(fc.line, l_grad);
    F.sample = AutoDerivative<double>(fc.sample, s_grad);
  }
  void fill_cache() const
  {
    if(!cache_stale)
      return;
    blitz::Range ra = blitz::Range::all();
    fa.reference(cam.field_alignment_block(0));
    // This is just a rough estimate of pitch, but it should be
    // sufficient to determine a reasonable tolerance for root
    // finding.
    double spitch = max(fabs(fa(ra,ra,ra,1,ra) - fa(ra,ra,ra,0,ra)) / cam.delta_sample_block()(0));
    double lpitch = max(fabs(fa(ra,ra,1,ra,ra) - fa(ra,ra,0,ra,ra)) / cam.delta_sample_block()(0));
    max_pitch = std::max(spitch, lpitch);
    // Want solution to 0.001 pixels.
    root_residual_needed = 1e-3 * max_pitch;
    cache_stale = false;
  }
  mutable blitz::Array<double, 5> fa;
  mutable double max_pitch;
  mutable double root_residual_needed;
};

blitz::Array<double, 1> GfmFaEqu::operator()(const blitz::Array<double, 1>& X) const
{
  FrameCoordinate fc(X(0), X(1));
  blitz::Array<double, 1> res(2);
  f.fc_to_xy(fc, res(0), res(1));
  res(0) -= x;
  res(1) -= y;
  return res;
}
  
  
}


void GlasGfmCamera::init_model()
{
  if(sensor_type() == "S")
    model_ = boost::make_shared<GlasFa>(*this);
  else if(field_angle_type() == 0)
    model_ = boost::make_shared<GfmFa>(*this);
  else
    throw Exception("Don't support GFM with calibration parameters yet");
}

GlasGfmCamera::GlasGfmCamera(int Number_line, int Number_sample)
: nline_(Number_line),
  nsamp_(Number_sample),
  field_alignment_block_(9)
{
  // These default values come from the RIP sample NITF file for Hyperion.
  focal_length_ = 1.41009182;
  frame_to_sc_ = boost::math::quaternion<double>(1,0,0,0);
  frame_to_sc_nd_ = value(frame_to_sc_);
  field_alignment_.resize(1, 4);
  field_alignment_ = -0.00765, 0, 0.00765, 0;
  band_type_ = "N";
  band_wavelength_ = 1.45;
  // Leave these as empty
  // band_index_, irepband_, isubcat_.
  focal_length_time_ = Time::parse_time("2005-04-07T00:00:00Z") +
    26649.88899999857;
  sample_number_first_ = 0;
  delta_sample_pair_ = 256;

  // Not used for sensor "S", but set a value so this isn't undefined
  field_angle_type_ = 0;
  field_angle_interpolation_type_ = 1;
  init_model();
}

FrameCoordinate GlasGfmCamera::frame_coordinate(const ScLookVector& Sl, 
						int Band) const
{
  range_check(Band, 0, number_band());

  // Just reverse of sc_look_vector.
  boost::math::quaternion<double> dcs = 
    conj(frame_to_sc()) * Sl.look_quaternion() * frame_to_sc();
  double xfp = focal_length() * (dcs.R_component_2() / dcs.R_component_4());
  double yfp = focal_length() * (dcs.R_component_3() / dcs.R_component_4());
  FrameCoordinate res;
  model_->xy_to_fc(xfp, yfp, res);
  return res;
}

FrameCoordinateWithDerivative GlasGfmCamera::frame_coordinate_with_derivative
(const ScLookVectorWithDerivative& Sl, int Band) const
{
  range_check(Band, 0, number_band());

  boost::math::quaternion<AutoDerivative<double> > dcs = 
    conj(frame_to_sc_with_derivative()) * Sl.look_quaternion() * 
    frame_to_sc_with_derivative();
  AutoDerivative<double> xfp = focal_length_with_derivative() *
    (dcs.R_component_2() / dcs.R_component_4());
  AutoDerivative<double> yfp = focal_length_with_derivative() *
    (dcs.R_component_3() / dcs.R_component_4());
  FrameCoordinateWithDerivative res;
  model_->xy_to_fc(xfp, yfp, res);
  return res;
}

ScLookVector GlasGfmCamera::sc_look_vector
(const FrameCoordinate& F, int Band) const
{
  range_check(Band, 0, number_band());
  double xfp, yfp;
  model_->fc_to_xy(F, xfp, yfp);
  boost::math::quaternion<double> lv(0, xfp, yfp, focal_length());  
  return ScLookVector(frame_to_sc() * lv * conj(frame_to_sc()));
}

ScLookVectorWithDerivative GlasGfmCamera::sc_look_vector_with_derivative
(const FrameCoordinateWithDerivative& F, int Band) const
{
  range_check(Band, 0, number_band());
  AutoDerivative<double> xfp, yfp;
  model_->fc_to_xy(F, xfp, yfp);
  boost::math::quaternion<AutoDerivative<double> >
    lv(0, xfp, yfp, focal_length_with_derivative());  
  return ScLookVectorWithDerivative(frame_to_sc_with_derivative() * lv *
				    conj(frame_to_sc_with_derivative()));
}

//-----------------------------------------------------------------------
/// Angular sensor frame offset. This is in radians, and is the order
/// "xyz".
//-----------------------------------------------------------------------

blitz::Array<double, 1> GlasGfmCamera::angoff() const
{
  blitz::Array<double, 1> res(3);
  quat_to_euler(frame_to_sc(), res(2), res(1), res(0));
  return res;
}

void GlasGfmCamera::angoff(const blitz::Array<double, 1>& V)
{
  if(V.rows() != 3)
    throw Exception("angoff must be size 3");
  frame_to_sc(quat_rot("zyx", V(2), V(1), V(0)));
  notify_update();
}
