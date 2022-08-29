#include "ipi.h"
#include "geocal_gsl_root.h"
#include "ostream_pad.h"
#include <boost/foreach.hpp>
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Ipi::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Ipi);
  ar & GEOCAL_NVP(orb)
    & GEOCAL_NVP(cam)
    & GEOCAL_NVP_(band)
    & GEOCAL_NVP(tt)
    & GEOCAL_NVP_(min_time)
    & GEOCAL_NVP_(max_time)
    & GEOCAL_NVP_(local_time_window_size)
    & GEOCAL_NVP_(root_min_separation)
    & GEOCAL_NVP_(time_tolerance);
  last_time = min_time_;
  // Older version didn't have max_frame_extend_. The default was to 
  // not check this.
  if(version > 0)
    ar & GEOCAL_NVP_(max_frame_extend);
  // Older version didn't have Refraction
  if(version > 1)
    ar & GEOCAL_NVP(ref);
  // Older version didn't have VelocityAberration
  if(version > 2)
    ar & GEOCAL_NVP_(velocity_aberration);
  // Older version didn't have time_acquisition_adjustment_
  if(version > 3)
    ar & GEOCAL_NVP_(time_acquisition_adjustment);
}

GEOCAL_IMPLEMENT(Ipi);
#endif

//-----------------------------------------------------------------------
/// Constructor. If you only want to get the Time from the Ipi and not
/// ImageCoordinate, it is ok for Tt to be null.
///
/// We find solutions to the Collinearity equations over the full time
/// range Tmin to Tmax. However, as a performance enhancement we first
/// look around the given Local_time_window_size in seconds around the
/// solution from the last call to time() or image_coordinate(). We
/// look for all solutions that are Root_min_separation apart. We find
/// time with a accuracy given by Time_tolerance.
//-----------------------------------------------------------------------

Ipi::Ipi(const boost::shared_ptr<Orbit>& Orb, const 
	 boost::shared_ptr<Camera>& Cam, int Band,
	 Time Tmin, Time Tmax, const boost::shared_ptr<TimeTable>& Tt,
	 const boost::shared_ptr<Refraction>& Ref,
	 const boost::shared_ptr<VelocityAberration>& Vabb,
	 double Local_time_window_size,
	 double Root_min_separation, 
	 double Time_tolerance, double Max_frame_extend)
  : orb(Orb), cam(Cam), band_(Band), tt(Tt), ref(Ref),
    velocity_aberration_(Vabb),
    min_time_(Tmin), max_time_(Tmax), 
    last_time(Tmin), local_time_window_size_(Local_time_window_size),
    root_min_separation_(Root_min_separation), 
    time_tolerance_(Time_tolerance),
    max_frame_extend_(Max_frame_extend),
    time_acquisition_adjustment_(false)
{
  // I'm not sure if we want to make this a argument or not. For
  // now, do time_acquisition_adjustment_ for Pushframe, but not
  // for Pushbroom cameras.
  if(Cam->number_line(Band) > 1)
    time_acquisition_adjustment_ = true;
}

//-----------------------------------------------------------------------
/// This is like image_coordinate, by we try to handle points out of
/// the time and sample range of the data (so this returns line and
/// samples outside of the range of the TimeTable and Camera). In some
/// cases this is more useful than having image_coordinate fail. Note
/// that we still might have Success false, if for example the point
/// is outside the range of the Orbit.
///
/// For points outside of the TimeTable range, we just extrapolate the
/// last two lines in the time table.
//-----------------------------------------------------------------------

void Ipi::image_coordinate_extended(const GroundCoordinate& Gp, 
				  ImageCoordinate& Ic,
				  bool& Success) const
{
  Time tres;
  FrameCoordinate fres;
  time(Gp, tres, fres, Success);
  if(Success) {
    if(tres >= tt->min_time() && tres < tt->max_time())
      Ic = tt->image_coordinate(tres, fres);
    else if(tres < tt->min_time()) {
      Time t1, t2;
      FrameCoordinate fc;
      tt->time(ImageCoordinate(tt->min_line(), fres.sample), t1, fc);
      tt->time(ImageCoordinate(tt->min_line() + 1, fres.sample), t2, fc);
      double tspace = t2 - t1;
      Ic.sample = fres.sample;
      Ic.line = (tres - tt->min_time()) / tspace + tt->min_line();
    } else {
      Time t1, t2;
      FrameCoordinate fc;
      tt->time(ImageCoordinate(tt->max_line() - 2, fres.sample), t1, fc);
      tt->time(ImageCoordinate(tt->max_line() - 1, fres.sample), t2, fc);
      double tspace = t2 - t1;
      Ic.sample = fres.sample;
      Ic.line = (tres - tt->max_time()) / tspace + tt->max_line();
    }
  }
}

void Ipi::image_coordinate_with_derivative_extended(const GroundCoordinate& Gp, 
				  ImageCoordinateWithDerivative& Ic,
				  bool& Success) const
{
  TimeWithDerivative tres;
  FrameCoordinateWithDerivative fres;
  time_with_derivative(Gp, tres, fres, Success);
  if(Success) {
    if(tres.value() >= tt->min_time() && tres.value() < tt->max_time())
      Ic = tt->image_coordinate_with_derivative(tres, fres);
    else if(tres.value() < tt->min_time()) {
      TimeWithDerivative t1, t2;
      FrameCoordinateWithDerivative fc;
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->min_line(), fres.sample), t1, fc);
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->min_line() + 1, fres.sample), t2, fc);
      AutoDerivative<double> tspace = t2 - t1;
      Ic.sample = fres.sample;
      Ic.line = (tres - tt->min_time()) / tspace + tt->min_line();
    } else {
      TimeWithDerivative t1, t2;
      FrameCoordinateWithDerivative fc;
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->max_line() - 2, fres.sample), t1, fc);
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->max_line() - 1, fres.sample), t2, fc);
      AutoDerivative<double> tspace = t2 - t1;
      Ic.sample = fres.sample;
      Ic.line = (tres - tt->max_time()) / tspace + tt->max_line();
    }
  }
}

void Ipi::image_coordinate_with_derivative_extended
(const GroundCoordinate& Gp, 
 const boost::array<AutoDerivative<double>, 3>& Gp_with_der,
 ImageCoordinateWithDerivative& Ic, bool& Success) const
{
  TimeWithDerivative tres;
  FrameCoordinateWithDerivative fres;
  time_with_derivative(Gp, Gp_with_der, tres, fres, Success);
  if(Success) {
    if(tres.value() >= tt->min_time() && tres.value() < tt->max_time())
      Ic = tt->image_coordinate_with_derivative(tres, fres);
    else if(tres.value() < tt->min_time()) {
      TimeWithDerivative t1, t2;
      FrameCoordinateWithDerivative fc;
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->min_line(), fres.sample), t1, fc);
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->min_line() + 1, fres.sample), t2, fc);
      AutoDerivative<double> tspace = t2 - t1;
      Ic.sample = fres.sample;
      Ic.line = (tres - tt->min_time()) / tspace + tt->min_line();
    } else {
      TimeWithDerivative t1, t2;
      FrameCoordinateWithDerivative fc;
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->max_line() - 2, fres.sample), t1, fc);
      tt->time_with_derivative(ImageCoordinateWithDerivative(tt->max_line() - 1, fres.sample), t2, fc);
      AutoDerivative<double> tspace = t2 - t1;
      Ic.sample = fres.sample;
      Ic.line = (tres - tt->max_time()) / tspace + tt->max_line();
    }
  }
}

//-----------------------------------------------------------------------
/// Look for the image location that a point is seen. Note that unlike
/// time, if a point is seen only outside of the sample range of the
/// image then we consider the Ipi a failure. If Success is true, then
/// Ic will have a line and sample number inside of the image given by
/// the Camera and TimeTable.
//-----------------------------------------------------------------------

void Ipi::image_coordinate(const GroundCoordinate& Gp, ImageCoordinate& Ic,
			   bool& Success) const
{
  Time tres;
  FrameCoordinate fres;
  time(Gp, tres, fres, Success);
  if(Success) {
    if(fres.sample < 0 || fres.sample >= cam->number_sample(band_) ||
       tres < tt->min_time() || tres >= tt->max_time())
      Success = false;
    else
      Ic = tt->image_coordinate(tres, fres);
  }
}

void Ipi::image_coordinate_with_derivative(const GroundCoordinate& Gp, ImageCoordinateWithDerivative& Ic,
			   bool& Success) const
{
  TimeWithDerivative tres;
  FrameCoordinateWithDerivative fres;
  time_with_derivative(Gp, tres, fres, Success);
  if(Success) {
    if(fres.sample.value() < 0 || fres.sample.value() >= cam->number_sample(band_) ||
       tres.value() < tt->min_time() || tres.value() >= tt->max_time())
      Success = false;
    else
      Ic = tt->image_coordinate_with_derivative(tres, fres);
  }
}

void Ipi::image_coordinate_with_derivative
(const GroundCoordinate& Gp, 
 const boost::array<AutoDerivative<double>, 3>& Gp_with_der,
 ImageCoordinateWithDerivative& Ic,
 bool& Success) const
{
  TimeWithDerivative tres;
  FrameCoordinateWithDerivative fres;
  time_with_derivative(Gp, Gp_with_der, tres, fres, Success);
  if(Success) {
    if(fres.sample.value() < 0 || fres.sample.value() >= cam->number_sample(band_) ||
       tres.value() < tt->min_time() || tres.value() >= tt->max_time())
      Success = false;
    else
      Ic = tt->image_coordinate_with_derivative(tres, fres);
  }
}

//-----------------------------------------------------------------------
/// Look for solution for when the GroundCoordinate Gp is seen. If we
/// don't find a solution, then Success will be false.
///
/// Note that we may return a solution where Fres has a sample number
/// < 0 or > cam.number_sample(). This means that a point is not
/// actually seen in the image, but would be if we had an infinite
/// extent camera. Often this is useful information, however depending
/// on the application you may want to reject points with Fres outside
/// of the image range.
///
/// If it is more useful to you, image_coordinate has the opposite
/// behavior, it rejects any points that are outside of the actual
/// image. 
//-----------------------------------------------------------------------

void Ipi::time(const GroundCoordinate& Gp, Time& Tres, FrameCoordinate& Fres,
	       bool& Success) const
{
  class IpiEq: public DFunctor {
  public:
    IpiEq(const boost::shared_ptr<Camera>& Cam,
	  const boost::shared_ptr<Orbit>& Orb,
	  const boost::shared_ptr<CartesianFixed>& P,
	  const boost::shared_ptr<Refraction>& Ref,
	  const boost::shared_ptr<VelocityAberration>& Vabb,
	  Time Tmin,
	  int Band)
      : cam(Cam), orb(Orb), p(P), ref(Ref), vabb(Vabb),
	band(Band), tmin(Tmin)
    {
      height = p->height_reference_surface();
    }
    virtual ~IpiEq() {}
    virtual double operator()(const double& Toffset) const
    {
      return cam->frame_line_coordinate(orb->sc_look_vector(tmin + Toffset,
							    *p, ref, vabb), 
					band);
    }
    FrameCoordinate frame_coordinate(const double& Toffset) const
    {
      return orb->frame_coordinate(tmin + Toffset, *p, *cam, band, ref, vabb);
    }
    bool false_root(double Toffset) const
    {
      // Note we don't need to worry about refraction or
      // velocity_aberration here, this can be pretty rough to find
      // a false root
      
      boost::shared_ptr<CartesianFixed> p2 = orb->position_cf(tmin + Toffset);
      boost::array<double, 3> p1 = p->position;
      boost::array<double, 3> p3 = p2->position;
      CartesianFixedLookVector lv;
      lv.look_vector[0] = p1[0] - p3[0];
      lv.look_vector[1] = p1[1] - p3[1];
      lv.look_vector[2] = p1[2] - p3[2];
      const double allowed_intersection_error = 50000;
				// How far off we can be from actual position 
				// of point and still call it a true
				// solution.
      return (distance(*(p2->
			 reference_surface_intersect_approximate(lv, height)),
			 *p) > allowed_intersection_error);
    }
  private:
    boost::shared_ptr<Camera> cam;
    boost::shared_ptr<Orbit> orb;
    boost::shared_ptr<CartesianFixed> p;
    boost::shared_ptr<Refraction> ref;
    boost::shared_ptr<VelocityAberration> vabb;
    int band;
    Time tmin;
    double height;
  };

  IpiEq eq(cam, orb, Gp.convert_to_cf(), ref, velocity_aberration_,
	   min_time_, band_);
  double min_eq_t = std::max(0.0, orb->min_time() - min_time_);
  // The time_tolerance / 2 is because we usually use a range 
  // min <= x < max, but root_list uses min <= x <= max. We move off
  // a small amount from the end point so we don't run into any
  // problems with it.
  double max_eq_t = std::min(max_time_ - min_time_,
			     orb->max_time() - min_time_ - time_tolerance_ / 2);
  std::vector<double> sol = 
    root_list(eq, std::max(min_time_guess() - min_time_, min_eq_t),
	      std::min(max_time_guess() - min_time_, max_eq_t),
	      root_min_separation_, time_tolerance_);

//-----------------------------------------------------------------------
// Reject false roots, and find out how many good solutions there are.
//-----------------------------------------------------------------------

  double true_sol = 0;
  FrameCoordinate fc_sol;
  int num_sol = 0;
  BOOST_FOREACH(double x, sol) {
    if(!eq.false_root(x)) {
      FrameCoordinate fc = eq.frame_coordinate(x);
      if(fc.sample >= -max_frame_extend_ &&
	 fc.sample <= cam->number_sample(band_) + max_frame_extend_) {
	num_sol++;
	true_sol = x;
	fc_sol = fc;
      }
    }
  }

//-----------------------------------------------------------------------
// Search in whole region if we didn't find a solution in the small
// time window.
//-----------------------------------------------------------------------
  
  if(num_sol ==0) {
    sol = root_list(eq, min_eq_t, max_eq_t, root_min_separation_,
		    time_tolerance_);
    BOOST_FOREACH(double x, sol) {
      if(!eq.false_root(x)) {
	FrameCoordinate fc = eq.frame_coordinate(x);
	if(fc.sample >= -max_frame_extend_ &&
	   fc.sample <= cam->number_sample(band_) + max_frame_extend_) {
	  num_sol++;
	  true_sol = x;
	  fc_sol = fc;
	}
      }
    }
  }

  // Suppose this is actually ok, we just don't have a solution
  // if(num_sol > 1) {
  //   throw Exception("Have more than one solution to ipi equations");
  //}
  if(num_sol ==1) {
    Success = true;
    Tres = min_time_ + true_sol;
    last_time = Tres;
    Fres = fc_sol;
    if(time_acquisition_adjustment_) {
      Time t1, t2;
      tt->time_acquisition(Tres, Fres, t1, t2);
      // I *think* we always want the smaller image coordinate t. We
      // may need to change the logic here if we run into a problem of
      // some sort.
      Tres = t1;
      Fres = eq.frame_coordinate(t1 - min_time_);
    }
  } else {
    Success = false;
  }
}

void Ipi::time_with_derivative
(const GroundCoordinate& Gp, TimeWithDerivative& Tres, 
FrameCoordinateWithDerivative& Fres,
 bool& Success) const
{
  boost::shared_ptr<CartesianFixed> p = Gp.convert_to_cf();
  boost::array<AutoDerivative<double>, 3>  pd;
  pd[0] = p->position[0];
  pd[1] = p->position[1];
  pd[2] = p->position[2];
  time_with_derivative(*p, pd, Tres, Fres, Success);
}

void Ipi::time_with_derivative
(const GroundCoordinate& Gp, 
 const boost::array<AutoDerivative<double>, 3>& Gp_with_der,
 TimeWithDerivative& Tres, 
 FrameCoordinateWithDerivative& Fres,
 bool& Success) const
{
  class IpiEq: public DFunctorWithDerivative {
  public:
    IpiEq(const boost::shared_ptr<Camera>& Cam,
	  const boost::shared_ptr<Orbit>& Orb,
	  const boost::shared_ptr<CartesianFixed> P,
	  const boost::array<AutoDerivative<double>, 3>& P_with_der,
	  const boost::shared_ptr<Refraction>& Ref,
	  const boost::shared_ptr<VelocityAberration>& Vabb,
	  Time Tmin,
	  int Band)
      : cam(Cam), orb(Orb), p(P), p_with_der(P_with_der),
	ref(Ref), vabb(Vabb),
	band(Band), tmin(Tmin)
    {
      height = p->height_reference_surface();
    }
    virtual ~IpiEq() {}
    virtual double operator()(const double& Toffset) const
    {
      return cam->frame_line_coordinate(orb->sc_look_vector(tmin + Toffset,
							    *p, ref, vabb), 
					band);
    }
    virtual double df(double Toffset) const
    {
      // Right now just do a finite difference. We can revisit this if
      // needed. 
      double eps = 1e-3;
      // This might go past the edge of the time table, so add handling
      // to switch where eps is applied if needed.
      if(Toffset + eps < orb->max_time() - tmin)
	return ((*this)(Toffset + eps) - (*this)(Toffset)) / eps;
      else
	return ((*this)(Toffset) - (*this)(Toffset - eps)) / eps;
    }
    virtual AutoDerivative<double> f_with_derivative(double Toffset) const
    {
      return orb->frame_coordinate_with_derivative
	(TimeWithDerivative(tmin) + Toffset, *p, *cam, band, ref, vabb).line;
    }
    FrameCoordinateWithDerivative frame_coordinate(const AutoDerivative<double>& Toffset) const
    {
      return orb->frame_coordinate_with_derivative
	(TimeWithDerivative(tmin) + Toffset, *p, *cam, band, ref, vabb);
    }
    bool false_root(double Toffset) const
    {
      // Note we don't need to worry about refraction or
      // velocity_aberration here, this can be pretty rough to find
      // a false root
      boost::shared_ptr<CartesianFixed> p2 = orb->position_cf(tmin + Toffset);
      boost::array<double, 3> p1 = p->position;
      boost::array<double, 3> p3 = p2->position;
      CartesianFixedLookVector lv;
      lv.look_vector[0] = p1[0] - p3[0];
      lv.look_vector[1] = p1[1] - p3[1];
      lv.look_vector[2] = p1[2] - p3[2];
      boost::shared_ptr<OrbitData> od = orb->orbit_data(tmin + Toffset);
      const double allowed_intersection_error = 50000;
				// How far off we can be from actual position 
				// of point and still call it a true
				// solution.
      return (distance(*(od->position_cf()->
		 reference_surface_intersect_approximate(lv, 
					 height)),
			 *p) > allowed_intersection_error);
    }
  private:
    boost::shared_ptr<Camera> cam;
    boost::shared_ptr<Orbit> orb;
    boost::shared_ptr<CartesianFixed> p;
    boost::array<AutoDerivative<double>, 3> p_with_der;
    boost::shared_ptr<Refraction> ref;
    boost::shared_ptr<VelocityAberration> vabb;
    int band;
    Time tmin;
    double height;
  };

  IpiEq eq(cam, orb, Gp.convert_to_cf(), Gp_with_der, ref,
	   velocity_aberration_, min_time_, band_);
  double min_eq_t = std::max(0.0, orb->min_time() - min_time_);
  // The time_tolerance / 2 is because we usually use a range 
  // min <= x < max, but root_list uses min <= x <= max. We move off
  // a small amount from the end point so we don't run into any
  // problems with it.
  double max_eq_t = std::min(max_time_ - min_time_,
			     orb->max_time() - min_time_ - time_tolerance_ / 2);
  std::vector<AutoDerivative<double> > sol = 
    root_list(eq, std::max(min_time_guess() - min_time_, min_eq_t),
	      std::min(max_time_guess() - min_time_, max_eq_t),
	      root_min_separation_, time_tolerance_);

//-----------------------------------------------------------------------
// Reject false roots, and find out how many good solutions there are.
//-----------------------------------------------------------------------

  AutoDerivative<double> true_sol = 0;
  int num_sol = 0;
  BOOST_FOREACH(AutoDerivative<double> x, sol) {
    if(!eq.false_root(x.value())) {
      num_sol++;
      true_sol = x;
    }
  }

//-----------------------------------------------------------------------
// Search in whole region if we didn't find a solution in the small
// time window.
//-----------------------------------------------------------------------
  
  if(num_sol ==0) {
    sol = root_list(eq, min_eq_t, max_eq_t, root_min_separation_,
		    time_tolerance_);
    BOOST_FOREACH(AutoDerivative<double> x, sol) {
      if(!eq.false_root(x.value())) {
	num_sol++;
	true_sol = x;
      }
    }
  }

  if(num_sol > 1)
    throw Exception("Have more than one solution to ipi equations");
  if(num_sol ==1) {
    Success = true;
    Tres = TimeWithDerivative(min_time_) + true_sol;
    last_time = Tres.value();
    Fres = eq.frame_coordinate(true_sol);
    if(time_acquisition_adjustment_) {
      Time t1, t2;
      tt->time_acquisition(Tres.value(), Fres.value(), t1, t2);
      // I *think* we always want the smaller image coordinate t. We
      // may need to change the logic here if we run into a problem of
      // some sort.
      Tres = TimeWithDerivative(t1);
      Fres = eq.frame_coordinate(t1 - min_time_);
    }
  } else {
    Success = false;
  }
}


//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void Ipi::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "Ipi\n"
     << "  Band: " << band_ << "\n"
     << "  Tmin: " << min_time_ << "\n"
     << "  Tmax: " << max_time_ << "\n"
     << "  Orbit:\n";
  opad << *orb;
  opad.strict_sync();
  Os << "  Camera:\n";
  opad << *cam;
  opad.strict_sync();
  if(tt.get()) {
    Os << "  Time Table:\n";
    opad << *tt;
    opad.strict_sync();
  } else 
    Os << "  Time Table: None\n";
  if(ref.get()) {
    Os << "  Refraction:\n";
    opad << *ref;
    opad.strict_sync();
  } else 
    Os << "  Refraction: None\n";
}

//-----------------------------------------------------------------------
/// Give the approximate ground resolution in meters. This is for the
/// center pixel in the area covered by the Ipi.
//-----------------------------------------------------------------------

double Ipi::resolution_meter() const
{
  // Pretty sure we can ignore refraction here
  Time mt = min_time() + (max_time() - min_time()) / 2;
  return orbit()->orbit_data(mt)->resolution_meter(*camera(), band());
}

//-----------------------------------------------------------------------
/// Return the footprint of the data on the ground. This is just the
/// footprint of the orbit data at the start and end of the time range.
//-----------------------------------------------------------------------

std::vector<boost::shared_ptr<GroundCoordinate> > 
Ipi::footprint(const Dem& D) const
{
  // TODO Include refraction and velocity_aberration
  std::vector<boost::shared_ptr<GroundCoordinate> > fp = 
    orbit()->orbit_data(min_time())->footprint(*camera(), D, 30, band());
  std::vector<boost::shared_ptr<GroundCoordinate> > fp2 =
       orbit()->orbit_data(max_time() - time_tolerance_)->footprint(*camera(), 
	     D, 30, band());
  fp.insert(fp.end(), fp2.begin(), fp2.end());
  return fp;
}
