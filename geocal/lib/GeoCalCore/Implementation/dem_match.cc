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
//-----------------------------------------------------------------------

blitz::Array<double, 2> DemMatch::surface_point
(int Lstart, int Sstart, int Lend, int Send, int Lstride, int Sstride) const
{
  std::vector<Geodetic> res;
  ImageCoordinate ic2;
  double line_sigma, sample_sigma, dist;
  boost::shared_ptr<CartesianFixed> p;
  bool success;
  for(int i = Lstart; i < Lend;  i += Lstride)
    for(int j = Sstart; j < Send; j += Sstride) {
      match_->match(ImageCoordinate(i,j), ic2, line_sigma, sample_sigma, 
		   success);
      if(success) {
	ri->two_ray_intersect(ImageCoordinate(i, j), ic2, p, dist);
	if(dist < max_dist) {
	  Ecr* ecr = dynamic_cast<Ecr*>(p.get());
	  if(!ecr)
	    throw Exception("Must be Ecr");
	  res.push_back(ecr->convert_to_geodetic());
	}
      }
    }
  blitz::Array<double, 2> res2((int) res.size(), 3);
  for(int i = 0; i < res2.rows(); ++i) {
    res2(i, 0) = res[i].latitude();
    res2(i, 1) = res[i].longitude();
    res2(i, 2) = res[i].height_reference_surface();
  }
  return res2;
}

