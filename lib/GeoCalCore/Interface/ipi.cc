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
  // Temp, break this up so we can see exactly what fails. We can join
  // this when all done.
  std::cerr << "In ipi\n";
  ar & GEOCAL_NVP(orb)
    & GEOCAL_NVP(cam)
    & GEOCAL_NVP_(band);
  std::cerr << "Skipping tt\n";
  // ar & GEOCAL_NVP(tt);
  ar & GEOCAL_NVP_(min_time)
    & GEOCAL_NVP_(max_time)
    & GEOCAL_NVP_(local_time_window_size)
    & GEOCAL_NVP_(root_min_separation)
    & GEOCAL_NVP_(time_tolerance);
  last_time = min_time_;
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
	 double Local_time_window_size,
	 double Root_min_separation, 
	 double Time_tolerance)
  : orb(Orb), cam(Cam), band_(Band), tt(Tt), min_time_(Tmin), max_time_(Tmax), 
    last_time(Tmin), local_time_window_size_(Local_time_window_size),
    root_min_separation_(Root_min_separation), 
    time_tolerance_(Time_tolerance)
{
  if(Cam->number_line(Band) != 1)
    throw Exception("I think we only want to do an IPI with a pushbroom camera with 1 line. If this is wrong, you can remove this exception");
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
	  const boost::shared_ptr<CartesianFixed> P,
	  Time Tmin,
	  int Band)
      : cam(Cam), orb(Orb), p(P), band(Band), tmin(Tmin)
    {
      height = p->height_reference_surface();
    }
    virtual ~IpiEq() {}
    virtual double operator()(const double& Toffset) const
    {
      Time t = tmin + Toffset;
      return cam->frame_line_coordinate(orb->sc_look_vector(t, look_vector(t)), 
					band);
    }
    FrameCoordinate frame_coordinate(const double& Toffset) const
    {
      Time t = tmin + Toffset;
      return cam->frame_coordinate(orb->sc_look_vector(t, look_vector(t)), 
				   band);
    }
    bool false_root(double Toffset) const
    {
      Time t = tmin + Toffset;
      const double allowed_intersection_error = 50000;
				// How far off we can be from actual position 
				// of point and still call it a true
				// solution.
      return (distance(*(orb->position_cf(t)->
		 reference_surface_intersect_approximate(look_vector(t), 
					 height)),
			 *p) > allowed_intersection_error);
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
  private:
    boost::shared_ptr<Camera> cam;
    boost::shared_ptr<Orbit> orb;
    boost::shared_ptr<CartesianFixed> p;
    int band;
    Time tmin;
    double height;
  };

  IpiEq eq(cam, orb, Gp.convert_to_cf(), min_time_, band_);
  std::vector<double> sol = 
    root_list(eq, min_time_guess() - min_time_, max_time_guess() - min_time_,
	      root_min_separation_, time_tolerance_);

//-----------------------------------------------------------------------
// Reject false roots, and find out how many good solutions there are.
//-----------------------------------------------------------------------

  double true_sol = 0;
  int num_sol = 0;
  BOOST_FOREACH(double x, sol) {
    if(!eq.false_root(x)) {
      num_sol++;
      true_sol = x;
    }
  }

//-----------------------------------------------------------------------
// Search in whole region if we didn't find a solution in the small
// time window.
//-----------------------------------------------------------------------
  
  if(num_sol ==0) {
    // The time_tolerance / 2 is because we usually use a range 
    // min <= x < max, but root_list uses min <= x <= max. We move off
    // a small amount from the end point so we don't run into any
    // problems with it.
    sol = root_list(eq, 0.0, max_time_ - min_time_ - time_tolerance_ / 2,
		    root_min_separation_, time_tolerance_);
    BOOST_FOREACH(double x, sol) {
      if(!eq.false_root(x)) {
	num_sol++;
	true_sol = x;
      }
    }
  }

  if(num_sol > 1)
    throw Exception("Have more than one solution to ipi equations");
  if(num_sol ==1) {
    Success = true;
    Tres = min_time_ + true_sol;
    last_time = Tres;
    Fres = eq.frame_coordinate(true_sol);
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
}

//-----------------------------------------------------------------------
/// Give the approximate ground resolution in meters. This is for the
/// center pixel in the area covered by the Ipi.
//-----------------------------------------------------------------------

double Ipi::resolution_meter() const
{
  Time mt = min_time() + (max_time() - min_time()) / 2;
  return orbit().orbit_data(mt)->resolution_meter(camera(), band());
}

//-----------------------------------------------------------------------
/// Return the footprint of the data on the ground. This is just the
/// footprint of the orbit data at the start and end of the time range.
//-----------------------------------------------------------------------

std::vector<boost::shared_ptr<GroundCoordinate> > 
Ipi::footprint(const Dem& D) const
{
  std::vector<boost::shared_ptr<GroundCoordinate> > fp = 
    orbit().orbit_data(min_time())->footprint(camera(), D, 30, band());
  std::vector<boost::shared_ptr<GroundCoordinate> > fp2 =
       orbit().orbit_data(max_time() - time_tolerance_)->footprint(camera(), 
	     D, 30, band());
  fp.insert(fp.end(), fp2.begin(), fp2.end());
  return fp;
}
