// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "dem_match.h"
%}

%geocal_shared_ptr(DemMatch);
namespace GeoCal {
class DemMatch{
public:
  DemMatch(const boost::shared_ptr<ImageToImageMatch>& Match,
	   const boost::shared_ptr<RayIntersect>& Ray_intersect,
	   double Max_dist_good_point);
  blitz::Array<double, 2> surface_point
    (int Lstart, int Sstart, int Lend, int Send,
     int Lstride = 1, int Sstride = 1) const;
  std::string print_to_string() const;
  %python_attribute(match, boost::shared_ptr<ImageToImageMatch>)
  %python_attribute(ray_intersect, boost::shared_ptr<RayIntersect>)
  %python_attribute(max_distance, double)
  %pickle_init(self.match, self.ray_intersect, self.max_distance)
};
}



