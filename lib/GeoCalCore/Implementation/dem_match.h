#ifndef DEM_MATCH_H
#define DEM_MATCH_H
#include "image_to_image_match.h"
#include "ray_intersect.h"
#include "statistic.h"

namespace GeoCal {
/****************************************************************//**
  This class does matching between two images, and then finds the
  intersection of these two images. This defines a Dem surface.

  I'm not really sure how this class should be arranged. Currently,
  this is used by the python class DemGenerate, and this is just the
  part that we want to have in C++. This may evolve over time, and
  change exactly what we do here.
*******************************************************************/

class DemMatch: public Printable<DemMatch> {
public:
// This comes from looking at CcorrMatcher and LsmMatcher, + 1 for 
// RayIntersect failure we report here, and +1 for 
// ImageGroundConectionFailed that we also report here
  enum {MAX_DIAGNOSTIC_VALUE = 9 + 2};
  enum Diagnostic {RAY_INTERSECT_FAILED = 10, 
		   IMAGE_GROUND_CONNECTION_FAILED = 11};

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  DemMatch(const boost::shared_ptr<ImageToImageMatch>& Match,
	   const boost::shared_ptr<RayIntersect>& Ray_intersect,
	   double Max_dist_good_point)
    : npoint(0), all_stat(new Statistic), good_stat(new Statistic), 
      diagnostic_value(MAX_DIAGNOSTIC_VALUE + 2),
      match_(Match), ri(Ray_intersect), 
      max_dist(Max_dist_good_point) {}
  virtual ~DemMatch() {}
  blitz::Array<double, 2> surface_point(int Lstart, int Sstart,
					int Lend, int Send,
					int Lstride = 1, 
					int Sstride = 1,
					bool Include_ic = false) const;
  blitz::Array<double, 2> surface_point(const MapInfo& Mi,
					bool Include_ic = false) const;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const { Os << "DemMatch"; }

  boost::shared_ptr<ImageToImageMatch> match() const {return match_;}
  boost::shared_ptr<RayIntersect> ray_intersect() const {return ri;}
  double max_distance() const {return max_dist;}

//-----------------------------------------------------------------------
/// Total number of points we attempted matching in last call to
/// surface_point. 
//-----------------------------------------------------------------------

  int number_point() const { return npoint;}

//-----------------------------------------------------------------------
/// Number of successful matches in last call to surface_point.
//-----------------------------------------------------------------------

  int number_match() const { return all_stat->count();}

//-----------------------------------------------------------------------
/// Number of point matched that then satisfied epipolar constraint in
/// last call to surface_point, so total number of points returned.
//-----------------------------------------------------------------------

  int number_success() const { return good_stat->count(); }

//-----------------------------------------------------------------------
/// Diagnostic values for failed image matches in the
/// last call to surface_point.
//-----------------------------------------------------------------------

  blitz::Array<int, 1> diagnostic() const { return diagnostic_value; }

//-----------------------------------------------------------------------
/// Statistics on distance for last call to surface point, including
/// all matches (including those later rejected).
//-----------------------------------------------------------------------

  const boost::shared_ptr<Statistic>& all_distance_stat() 
    const { return all_stat; }

//-----------------------------------------------------------------------
/// Statistics on distance for last call to surface point, only
/// including matches that are accepted as good points.
//-----------------------------------------------------------------------

  const boost::shared_ptr<Statistic>& good_distance_stat() 
    const { return good_stat; }
private:
  mutable int npoint;
  mutable boost::shared_ptr<Statistic> all_stat, good_stat;
  mutable blitz::Array<int, 1> diagnostic_value;
  boost::shared_ptr<ImageToImageMatch> match_;
  boost::shared_ptr<RayIntersect> ri;
  double max_dist;

  // Scratch variables used between functions.
  mutable std::vector<double> res_lat;
  mutable std::vector<double> res_lon;
  mutable std::vector<double> res_h;
  mutable std::vector<ImageCoordinate> ic1v, ic2v;
  mutable ImageCoordinate ic1, ic2;
  mutable double line_sigma, sample_sigma, dist;
  mutable boost::shared_ptr<CartesianFixed> p;
  mutable bool success, include_ic;
  mutable int diagnosticv;
  void process_setup() const;
  void process_match() const;
  blitz::Array<double, 2> process_res() const;
};
}
#endif
