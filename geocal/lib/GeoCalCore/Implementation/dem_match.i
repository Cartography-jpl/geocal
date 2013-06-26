// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "dem_match.h"
%}
%base_import(generic_object)
%import "image_to_image_match.i"
%import "ray_intersect.i"
%import "statistic.i"
%geocal_shared_ptr(GeoCal::DemMatch);
namespace GeoCal {
class DemMatch : public GenericObject {
public:
  DemMatch(const boost::shared_ptr<ImageToImageMatch>& Match,
	   const boost::shared_ptr<RayIntersect>& Ray_intersect,
	   double Max_dist_good_point);
  blitz::Array<double, 2> surface_point
    (int Lstart, int Sstart, int Lend, int Send,
     int Lstride = 1, int Sstride = 1, bool Include_ic = false) const;
  std::string print_to_string() const;
  %python_attribute(match, boost::shared_ptr<ImageToImageMatch>)
  %python_attribute(ray_intersect, boost::shared_ptr<RayIntersect>)
  %python_attribute(max_distance, double)
  %python_attribute(number_point, int);
  %python_attribute(number_match, int);
  %python_attribute(number_success, int)
  %python_attribute(diagnostic, blitz::Array<int, 1>)
  %python_attribute(all_distance_stat, boost::shared_ptr<Statistic>)
  %python_attribute(good_distance_stat, boost::shared_ptr<Statistic>)
  %pickle_init(1, self.match, self.ray_intersect, self.max_distance)
};
}



