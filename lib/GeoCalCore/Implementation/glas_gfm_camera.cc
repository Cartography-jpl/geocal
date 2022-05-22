#include "glas_gfm_camera.h"
#include "geocal_serialize_support.h"
#include "geocal_gsl_root.h"
#include "ostream_pad.h"
#include <cmath>
#include <boost/make_shared.hpp>

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
  if(version < 2) {
    parameter_mask_.resize(4);
    parameter_mask_ = true;
  }
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
  if(version > 1)
    ar & GEOCAL_NVP_(ppoff)
      & GEOCAL_NVP_(parameter_mask);
  if(version > 2)
    ar & GEOCAL_NVP_(focal_length_time);
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
    if(line_is_y) {
      X = (F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp + fa(i,0);
      // GLAS doesn't actually support line numbers being anything other
      // than 0. But it is very useful to be able to have a "extended"
      // camera, so we just give it a pitch of the average of all the
      // sample data as a "reasonable" thing.
      Y = (F.sample - ibase) * (fa(i, 3) - fa(i,1)) / dsamp + fa(i,1) -
	(F.line - 0) * p_space;
    } else {
      X = (F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp + fa(i,0) -
	(F.line - 0) * p_space;
      Y = (F.sample - ibase) * (fa(i, 3) - fa(i,1)) / dsamp + fa(i, 1);
    }
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
    if(line_is_y) {
      X = (F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp + fa(i,0);
      // GLAS doesn't actually support line numbers being anything other
      // than 0. But it is very useful to be able to have a "extended"
      // camera, so we just give it a pitch of the average of all the
      // sample data as a "reasonable" thing.
      Y = (F.sample - ibase) * (fa(i, 3) - fa(i,1)) / dsamp + fa(i,1) -
	(F.line - 0) * p_space;
    } else {
      X = (F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp + fa(i,0) -
	(F.line - 0) * p_space;
      Y = (F.sample - ibase) * (fa(i, 3) - fa(i,1)) / dsamp + fa(i, 1);
    }
  }
  virtual void xy_to_fc(double X, double Y, FrameCoordinate& F) const
  {
    fill_cache();
    if(line_is_y) {
      int i = index_from_x(X);
      int ibase = i * dsamp;
      F.sample = (X - fa(i,0)) / ((fa(i, 2) - fa(i,0)) / dsamp) + ibase;
      F.line = ((F.sample - ibase) * (fa(i, 3) - fa(i,1)) / dsamp  + fa(i,1)  - Y )/ p_space;
    } else {
      int i = index_from_y(Y);
      int ibase = i * dsamp;
      F.sample = (Y - fa(i,1)) / ((fa(i, 3) - fa(i,1)) / dsamp) + ibase;
      F.line = ((F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp  + fa(i,0)  - X)/ p_space;
    }
  }
  virtual void xy_to_fc(const AutoDerivative<double>& X,
			const AutoDerivative<double>& Y,
			FrameCoordinateWithDerivative& F) const
  {
    fill_cache();
    if(line_is_y) {
      int i = index_from_x(X.value());
      int ibase = i * dsamp;
      F.sample = (X - fa(i,0)) / ((fa(i, 2) - fa(i,0)) / dsamp) + ibase;
      F.line = ((F.sample - ibase) * (fa(i, 3) - fa(i,1)) / dsamp  + fa(i,1)  - Y )/ p_space;
    } else {
      int i = index_from_y(Y.value());
      int ibase = i * dsamp;
      F.sample = (Y - fa(i,1)) / ((fa(i, 3) - fa(i,1)) / dsamp) + ibase;
      F.line = ((F.sample - ibase) * (fa(i, 2) - fa(i,0)) / dsamp  + fa(i,0)  - X)/ p_space;
    }
  }
  int index_from_x(double X) const
  {
    if(p_space > 0) {
      for(int i = 0; i < fa.rows(); ++i)
	if(X < fa(i,2))
	  return i;
      return fa.rows() - 1;
    }
    for(int i = 0; i < fa.rows(); ++i)
      if(X > fa(i,2))
	return i;
    return fa.rows() - 1;
  }
  int index_from_y(double Y) const
  {
    if(p_space > 0) {
      for(int i = 0; i < fa.rows(); ++i)
	if(Y < fa(i,3))
	  return i;
      return fa.rows() - 1;
    }
    for(int i = 0; i < fa.rows(); ++i)
      if(Y > fa(i,3))
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
    // We assume sample direction is mostly in x or y direction. .
    double x_space = (x_end - x_0)/nsamp;
    double y_space = (y_end - y_0)/nsamp;
    if(fabs(x_space) > fabs(y_space)) {
      p_space = x_space;
      line_is_y = true;
    } else {
      p_space = y_space;
      line_is_y = false;
    }
    // Sanity check on data, we assume sample direction isn't zero
    // size. Note it is not uncommon and ok for the line direction to
    // be zero size, and that is ok. But we divide by spacing in
    // sample space for xy_to_fc, so make sure we don't have zeros there.
    for(int i = 0; i < fa.rows(); ++i)
      if(line_is_y) {
	if(fa(i,0) == fa(i,2)) {
	  Exception e;
	  e << "field_alignment has zero size in sample direction:\n"
	    << "  Index: " << i << "\n"
	    << "  Values: " << fa(i,0) << ", " << fa(i,1) << ", "
	    << fa(i,2) << ", " << fa(i,3) << "\n";
	  throw e;
	}
      } else {
	if(fa(i,1) == fa(i,3)) {
	  Exception e;
	  e << "field_alignment has zero size in sample direction:\n"
	    << "  Index: " << i << "\n"
	    << "  Values: " << fa(i,0) << ", " << fa(i,1) << ", "
	    << fa(i,2) << ", " << fa(i,3) << "\n";
	  throw e;
	}
      }	
    cache_stale = false;
  }
  mutable double x_0, x_end, p_space, y_0, y_end;
  mutable bool line_is_y;
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
    // Right now, just have one block.
    double l = (F.line - cam.first_line_block()(0)) / cam.delta_line_block()(0);
    double s = (F.sample - cam.first_sample_block()(0)) / cam.delta_sample_block()(0);
    int i = (int) floor(l);
    int j = (int) floor(s);
    if(i < 0)
      i = 0;
    if(i > fa.rows() - 1)
      i = fa.rows() -1;
    if(j < 0)
      j = 0;
    if(j > fa.cols() - 1)
      j = fa.cols() - 1;
    double x0 = (l - i) * (fa(i,j,1,0,0) - fa(i,j,0,0,0)) + fa(i,j,0,0,0);
    double x1 = (l - i) * (fa(i,j,1,1,0) - fa(i,j,0,1,0)) + fa(i,j,0,1,0);
    double y0 = (l - i) * (fa(i,j,1,0,1) - fa(i,j,0,0,1)) + fa(i,j,0,0,1);
    double y1 = (l - i) * (fa(i,j,1,1,1) - fa(i,j,0,1,1)) + fa(i,j,0,1,1);
    X = (s - j) * (x1 - x0) + x0;
    Y = (s - j) * (y1 - y0) + y0;
  }
  virtual void fc_to_xy(const FrameCoordinateWithDerivative& F,
			AutoDerivative<double>& X,
			AutoDerivative<double>& Y) const
  {
    fill_cache();
    // Right now, just have one block
    AutoDerivative<double> l = (F.line - cam.first_line_block()(0)) / cam.delta_line_block()(0);
    AutoDerivative<double> s = (F.sample - cam.first_sample_block()(0)) / cam.delta_sample_block()(0);
    int i = (int) floor(l.value());
    int j = (int) floor(s.value());
    if(i < 0)
      i = 0;
    if(i > fa.rows() - 1)
      i = fa.rows() -1;
    if(j < 0)
      j = 0;
    if(j > fa.cols() - 1)
      j = fa.cols() - 1;
    AutoDerivative<double> x0 =
      (l - i) * (fa(i,j,1,0,0) - fa(i,j,0,0,0)) + fa(i,j,0,0,0);
    AutoDerivative<double> x1 =
      (l - i) * (fa(i,j,1,1,0) - fa(i,j,0,1,0)) + fa(i,j,0,1,0);
    AutoDerivative<double> y0 =
      (l - i) * (fa(i,j,1,0,1) - fa(i,j,0,0,1)) + fa(i,j,0,0,1);
    AutoDerivative<double> y1 =
      (l - i) * (fa(i,j,1,1,1) - fa(i,j,0,1,1)) + fa(i,j,0,1,1);
    X = (s - j) * (x1 - x0) + x0;
    Y = (s - j) * (y1 - y0) + y0;
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
    if(cam.first_line_block().rows() != 1)
      throw Exception("Currently only support one block");
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

//-----------------------------------------------------------------------
/// It is common to create a GlasGfmCamera by fitting it to another
/// camera. This does that in one step, taking a QuaternionCamera and
/// fitting for the given Band. This version is for Pushbroom "S" type
/// sensor.
///
/// See note in field_alignment_fit about handling frame_to_sc
/// quaternion.
//-----------------------------------------------------------------------

GlasGfmCamera::GlasGfmCamera(const QuaternionCamera& Cam, int Band,
			     double Delta_sample,
			     const std::string& Band_type,
			     double Band_wavelength,
			     const Time& Focal_length_time)
: focal_length_(Cam.focal_length() * 1e-3), // 1e-3 to change units to meter
  nline_(Cam.number_line(Band)),
  nsamp_(Cam.number_sample(Band)),
  frame_to_sc_(Cam.frame_to_sc()),
  band_type_(Band_type),
  band_wavelength_(Band_wavelength),
  focal_length_time_(Focal_length_time),
  sample_number_first_(0),
  delta_sample_pair_(Delta_sample),
  field_angle_type_(0),
  field_angle_interpolation_type_(1),
  field_alignment_block_(9),
  ppoff_(3)
{
  // Leave these as empty
  // band_index_, irepband_, isubcat_.
  frame_to_sc_nd_ = value(frame_to_sc_);
  init_model();
  field_alignment_fit(Cam, delta_sample_pair_, Band);
  ppoff_ = 0,0,0;
  parameter_mask_.resize(4);
  parameter_mask_ = true;
}

//-----------------------------------------------------------------------
/// It is common to create a GlasGfmCamera by fitting it to another
/// camera. This does that in one step, taking a QuaternionCamera and
/// fitting for the given Band. This version is for Frame "F" type
/// sensor.
///
/// See note in field_alignment_block about handling frame_to_sc
/// quaternion.
//-----------------------------------------------------------------------

GlasGfmCamera::GlasGfmCamera(const QuaternionCamera& Cam, int Band,
			     double Delta_line,
			     double Delta_sample,
			     const std::string& Band_type,
			     double Band_wavelength,
			     const Time& Focal_length_time)
: focal_length_(Cam.focal_length() * 1e-3), // 1e-3 to change units to meter
  nline_(Cam.number_line(Band)),
  nsamp_(Cam.number_sample(Band)),
  frame_to_sc_(Cam.frame_to_sc()),
  band_type_(Band_type),
  band_wavelength_(Band_wavelength),
  focal_length_time_(Focal_length_time),
  sample_number_first_(0),
  delta_sample_pair_(Delta_sample),
  field_angle_type_(0),
  field_angle_interpolation_type_(1),
  field_alignment_block_(9),
  ppoff_(3)
{
  // Leave these as empty
  // band_index_, irepband_, isubcat_.
  frame_to_sc_nd_ = value(frame_to_sc_);
  init_model();
  field_alignment_block(Cam, Delta_line, Delta_sample, Band);
  ppoff_ = 0,0,0;
  parameter_mask_.resize(4);
  parameter_mask_ = true;
}

GlasGfmCamera::GlasGfmCamera(int Number_line, int Number_sample)
: nline_(Number_line),
  nsamp_(Number_sample),
  field_alignment_block_(9),
  ppoff_(3)
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
  ppoff_ = 0,0,0;
  init_model();
  parameter_mask_.resize(4);
  parameter_mask_ = true;
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
/// "xyz". This is the same as delta, beta, epsilon for euler angles
/// (so a reverse order than we often use, e.g. QuaternionCamera in
/// epsilon, beta, delta order).
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

void GlasGfmCamera::ppoff(const blitz::Array<double, 1>& V)
{
  if(V.rows() != 3)
    throw Exception("ppoff must be size 3");
  ppoff_ = V;
  notify_update();
}

//-----------------------------------------------------------------------
/// Populate the field_alignment, first_line_block,
/// first_sample_block, delta_line and delta_sample to match the given
/// camera. This creates only one block - we currently don't support
/// multiple blocks.
///
/// Only applicable for sensor type "F" and field_angle_type 0.
///
/// You may want to call compare_camera to check how accurate the
/// approximation is.
///
/// Note that you should be careful not to double count any
/// frame_t quaternion. If you pass that in with the Cam, then
/// this is already accounted for in the field angle map (which has
/// the effect of the quaternion embedded in it). If you want to
/// assign the frame_to_sc to the GlasGfmCamera, then you should make
/// sure to pass a Camera with a identity frame_to_sc. So a reasonable
/// process (in python) would be something like:
///
/// q_original = cam.frame_to_sc
/// cam.frame_to_sc = Quaternion_double(1,0,0,0)
/// gcam = GlasGfmCamera(cam, 0, ...)
/// gcam.frame_to_sc = q_original
/// cam.frame_to_sc = q_original
//-----------------------------------------------------------------------

void GlasGfmCamera::field_alignment_block
(const Camera& Cam, double Delta_line, double Delta_sample, int Band)
{
  if(sensor_type() != "F")
    throw Exception("field_alignment_block only applies to sensor type 'F'");
  field_angle_type_ = 0;
  nline_ = Cam.number_line(Band);
  nsamp_ = Cam.number_sample(Band);
  field_angle_interpolation_type_ = 1;
  init_model();
  first_line_block_.resize(1);
  first_sample_block_.resize(1);
  delta_line_block_.resize(1);
  delta_sample_block_.resize(1);
  first_line_block_(0) = 0;
  first_sample_block_(0) = 0;
  delta_line_block_(0) = Delta_line;
  delta_sample_block_(0) = Delta_sample;
  int nlblock = (int) floor(number_line(0) / Delta_line);
  int nsblock = (int) floor(number_sample(0) / Delta_sample);
  blitz::Array<double, 5>& fa = field_alignment_block_[0];
  fa.resize(nlblock, nsblock, 2, 2, 2);
  for(int i = 0; i < fa.rows(); ++i)
    for(int j = 0; j < fa.cols(); ++j) {
      FrameCoordinate fc1(i * Delta_line, j * Delta_sample);
      FrameCoordinate fc2(fc1.line, fc1.sample + Delta_sample);
      FrameCoordinate fc3(fc1.line + Delta_line, fc1.sample + Delta_sample);
      FrameCoordinate fc4(fc1.line + Delta_line, fc1.sample);
      ScLookVector slv1 = Cam.sc_look_vector(fc1, Band);
      ScLookVector slv2 = Cam.sc_look_vector(fc2, Band);
      ScLookVector slv3 = Cam.sc_look_vector(fc3, Band);
      ScLookVector slv4 = Cam.sc_look_vector(fc4, Band);
      fa(i,j,0,0,0) = slv1.look_vector[0] /
	(slv1.look_vector[2] / focal_length());
      fa(i,j,0,0,1) = slv1.look_vector[1] /
	(slv1.look_vector[2] / focal_length());
      fa(i,j,0,1,0) = slv2.look_vector[0] /
	(slv2.look_vector[2] / focal_length());
      fa(i,j,0,1,1) = slv2.look_vector[1] /
	(slv2.look_vector[2] / focal_length());
      fa(i,j,1,1,0) = slv3.look_vector[0] /
	(slv3.look_vector[2] / focal_length());
      fa(i,j,1,1,1) = slv3.look_vector[1] /
	(slv3.look_vector[2] / focal_length());
      fa(i,j,1,0,0) = slv4.look_vector[0] /
	(slv4.look_vector[2] / focal_length());
      fa(i,j,1,0,1) = slv4.look_vector[1] /
	(slv4.look_vector[2] / focal_length());
    }
  notify_update();
}

//-----------------------------------------------------------------------
/// Return the maximum difference in frame coordinate line and sample
/// between this camera and another camera. You may want to call this
/// after doing field_alignment_block or field_alignment_fit.
//-----------------------------------------------------------------------

void GlasGfmCamera::compare_camera(const Camera& Cam, double& max_line_diff,
				   double& max_sample_diff, int Band) const
{
  if(Cam.number_line(Band) != number_line(0) ||
     Cam.number_sample(Band) != number_sample(0))
    throw Exception("Cam doesn't match the size of the GlasGfmCamera");
  max_line_diff = 0;
  max_sample_diff = 0;
  for(int i = 0; i < number_line(0); ++i)
    for(int j = 0; j < number_sample(0); ++j) {
      FrameCoordinate fc(i,j);
      ScLookVector slv = Cam.sc_look_vector(fc, Band);
      ScLookVector slv2 = sc_look_vector(fc, 0);
      FrameCoordinate fc1 = frame_coordinate(slv, 0);
      FrameCoordinate fc2 = Cam.frame_coordinate(slv2, Band);
      max_line_diff = std::max(std::max(max_line_diff,
					fabs(fc1.line - fc.line)),
			       fabs(fc2.line - fc.line));
      max_sample_diff = std::max(std::max(max_sample_diff,
					  fabs(fc1.sample - fc.sample)),
				 fabs(fc2.sample - fc.sample));
    }
}

//-----------------------------------------------------------------------
/// Populate the field_alignment, sample_number_first_, delta_sample_pair_
/// to match the given camera.
///
/// Only applicable for sensor type "S".
///
/// You may want to call compare_camera to check how accurate the
/// approximation is.
///
/// Note that you should be careful not to double count any
/// frame_t quaternion. If you pass that in with the Cam, then
/// this is already accounted for in the field angle map (which has
/// the effect of the quaternion embedded in it). If you want to
/// assign the frame_to_sc to the GlasGfmCamera, then you should make
/// sure to pass a Camera with a identity frame_to_sc. So a reasonable
/// process (in python) would be something like:
///
/// q_original = cam.frame_to_sc
/// cam.frame_to_sc = Quaternion_double(1,0,0,0)
/// gcam = GlasGfmCamera(cam, 0, ...)
/// gcam.frame_to_sc = q_original
/// cam.frame_to_sc = q_original
//-----------------------------------------------------------------------

void GlasGfmCamera::field_alignment_fit
(const Camera& Cam, double Delta_sample, int Band)
{
  if(sensor_type() != "S")
    throw Exception("field_alignment only applies to sensor type 'S'");
  if(Cam.number_line(Band) != 1)
    throw Exception("field_alignment only applies to camera with number_line 1");
  nsamp_ = Cam.number_sample(Band);
  init_model();
  // Right now only support sample number first of 0. We can extend
  // this if needed.
  sample_number_first_ = 0;
  delta_sample_pair_ = Delta_sample;
  int npair = (int) floor(nsamp_ / delta_sample_pair_  + 0.5);
  field_alignment_.resize(npair, 4);
  blitz::Array<double, 2>& fa = field_alignment_;
  for(int i = 0; i < fa.rows(); ++i) {
    FrameCoordinate fc1(0, i * Delta_sample);
    FrameCoordinate fc2(fc1.line, fc1.sample + Delta_sample);
    ScLookVector slv1 = Cam.sc_look_vector(fc1, Band);
    ScLookVector slv2 = Cam.sc_look_vector(fc2, Band);
    fa(i,0) = slv1.look_vector[0] / (slv1.look_vector[2] / focal_length());
    fa(i,1) = slv1.look_vector[1] / (slv1.look_vector[2] / focal_length());
    fa(i,2) = slv2.look_vector[0] / (slv2.look_vector[2] / focal_length());
    fa(i,3) = slv2.look_vector[1] / (slv2.look_vector[2] / focal_length());
  }
  notify_update();
}

//-----------------------------------------------------------------------
/// Set delta Sample Pair. This is applicable for sensor type "S" only.
//-----------------------------------------------------------------------

void GlasGfmCamera::delta_sample_pair(double V)
{
  if(!(V > 0))
    throw Exception("delta_sample_pair must be > 0");
  delta_sample_pair_ = V;
  notify_update();
}

void GlasGfmCamera::print(std::ostream& Os) const
{
  Os << "GlasGfmCamera\n"
     << "  Id:                " << id() << "\n"
     << "  Band type:         " << band_type() << "\n"
     << "  Band wavelength:   " << band_wavelength() << " micrometer\n"
     << "  Number line:       " << number_line(0) << "\n"
     << "  Number sample:     " << number_sample(0) << "\n"
     << "  Focal length:      " << focal_length() << " m\n"
     << "  Focal length time: " << focal_length_time() << "\n"
     << "  Ppoff:             ("
     << ppoff()(0) << ", " << ppoff()(1) << ", " << ppoff()(2) << ")\n"
     << "  Angoff:            ("
     << angoff()(0) << ", " << angoff()(1) << ", " << angoff()(2) << ")\n";
}

//-----------------------------------------------------------------------
/// Set parameter. Right now this is Euler epsilon, beta, delta, focal
/// length. 
//-----------------------------------------------------------------------

void GlasGfmCamera::parameter(const blitz::Array<double, 1>& Parm)
{
  if(Parm.rows() != 4)
    throw Exception("Wrong sized parameter passed.");
  focal_length_ = Parm(3);
  frame_to_sc(quat_rot("zyx", Parm(0), Parm(1), Parm(2)));
  notify_update();
}

void GlasGfmCamera::parameter_with_derivative(const ArrayAd<double, 1>& Parm)
{
  if(Parm.rows() != 4)
    throw Exception("Wrong sized parameter passed.");
  focal_length_ = Parm(3);
  frame_to_sc_with_derivative(quat_rot("zyx", Parm(0), Parm(1), Parm(2)));
  notify_update();
}

blitz::Array<double, 1> GlasGfmCamera::parameter() const
{
  blitz::Array<double, 1> res(4);
  quat_to_euler(frame_to_sc_nd_, res(0), res(1), res(2));
  res(3) = focal_length();
  return res;
}

ArrayAd<double, 1> GlasGfmCamera::parameter_with_derivative() const
{
  blitz::Array<AutoDerivative<double>, 1> res(4);
  quat_to_euler(frame_to_sc_, res(0), res(1), res(2));
  res(3) = focal_length_with_derivative();
  return ArrayAd<double, 1>(res);
}

std::vector<std::string> GlasGfmCamera::parameter_name() const
{
  std::vector<std::string> res;
  res.push_back("Camera Euler Epsilon");
  res.push_back("Camera Euler Beta");
  res.push_back("Camera Euler Delta");
  res.push_back("Camera focal length");
  return res;
}
