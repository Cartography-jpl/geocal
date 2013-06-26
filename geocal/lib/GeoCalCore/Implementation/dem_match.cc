#include "dem_match.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Look through the given range in line and sample for the first
/// image. Try image matching, and where it succeeds look at the
/// intersection of the points. We allow a little slop, but the
/// intersection need to be close to where each image would put the
/// point (e.g., epipolar constraint), so we reject any point where
/// the intersection is farther than the maximum distance in meters
/// that was passed to the constructor.
///
/// We return an array of points that we find. This is a N X 3 array,
/// with each row corresponding to a point. The columns are latitude,
/// longitude, and height (in degrees and meters). This is a bit odd
/// way to return this, but it works well with the python class that
/// uses this code.
///
/// Note that you can optionally specify Include_ic as true. If this
/// is true, we return a N x (3 + 4) array, where the last four
/// columns are the image coordinate for the first and second images.
//-----------------------------------------------------------------------

blitz::Array<double, 2> DemMatch::surface_point
(int Lstart, int Sstart, int Lend, int Send, int Lstride, int Sstride,
 bool Include_ic) const
{
  std::vector<Geodetic> res;
  std::vector<ImageCoordinate> ic1v, ic2v;
  ImageCoordinate ic2;
  double line_sigma, sample_sigma, dist;
  boost::shared_ptr<CartesianFixed> p;
  bool success;
  int diagnostic;
  npoint = 0;
  diagnostic_value = 0;
  all_stat.reset(new Statistic);
  good_stat.reset(new Statistic);
  for(int i = Lstart; i < Lend;  i += Lstride)
    for(int j = Sstart; j < Send; j += Sstride) {
      ++npoint;
      match_->match(ImageCoordinate(i,j), ic2, line_sigma, sample_sigma, 
		    success, &diagnostic);
      if(success) {
	ri->two_ray_intersect(ImageCoordinate(i, j), ic2, p, dist);
	all_stat->add(dist);
	if(dist < max_dist) {
	  good_stat->add(dist);
	  Ecr* ecr = dynamic_cast<Ecr*>(p.get());
	  if(!ecr)
	    throw Exception("Must be Ecr");
	  res.push_back(ecr->convert_to_geodetic());
	  if(Include_ic) {
	    ic1v.push_back(ImageCoordinate(i, j));
	    ic2v.push_back(ic2);
	  }
	}
      } else {
	if(diagnostic >= 0 && diagnostic <= MAX_DIAGNOSTIC_VALUE)
	  diagnostic_value(diagnostic) += 1;
	else
	  diagnostic_value(MAX_DIAGNOSTIC_VALUE + 1) += 1;
      }
      
    }
  blitz::Array<double, 2> res2((int) res.size(), (Include_ic ? 3 + 4 : 3));
  for(int i = 0; i < res2.rows(); ++i) {
    res2(i, 0) = res[i].latitude();
    res2(i, 1) = res[i].longitude();
    res2(i, 2) = res[i].height_reference_surface();
    if(Include_ic) {
      res2(i, 3) = ic1v[i].line;
      res2(i, 4) = ic1v[i].sample;
      res2(i, 5) = ic2v[i].line;
      res2(i, 6) = ic2v[i].sample;
    }
  }
  return res2;
}

