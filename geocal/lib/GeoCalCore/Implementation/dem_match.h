#ifndef DEM_MATCH_H
#define DEM_MATCH_H
#include "image_to_image_match.h"
#include "ray_intersect.h"

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
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  DemMatch(const boost::shared_ptr<ImageToImageMatch>& Match,
	   const boost::shared_ptr<RayIntersect>& Ray_intersect,
	   double Max_dist_good_point)
    : match_(Match), ri(Ray_intersect), max_dist(Max_dist_good_point) {}
  virtual ~DemMatch() {}
  blitz::Array<double, 2> surface_point(int Lstart, int Sstart,
					int Lend, int Send,
					int Lstride = 1, 
					int Sstride = 1) const;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const { Os << "DemMatch"; }

  boost::shared_ptr<ImageToImageMatch> match() const {return match_;}
  boost::shared_ptr<RayIntersect> ray_intersect() const {return ri;}
  double max_distance() const {return max_dist;}
private:
  boost::shared_ptr<ImageToImageMatch> match_;
  boost::shared_ptr<RayIntersect> ri;
  double max_dist;
};
}
#endif
