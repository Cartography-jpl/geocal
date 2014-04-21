#include "dem_match.h"
#include "surface_image_to_image_match.h"
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
  include_ic = Include_ic;
  process_setup();
  for(int i = Lstart; i < Lend;  i += Lstride)
    for(int j = Sstart; j < Send; j += Sstride) {
      try {
	++npoint;
	ic1 = ImageCoordinate(i, j);
	match_->match(ic1, ic2, line_sigma, sample_sigma, 
		      success, &diagnosticv);
	process_match();
      } catch(ImageGroundConnectionFailed& E) {
	// Just treat like out of range error
	success = false;
	diagnosticv = IMAGE_GROUND_CONNECTION_FAILED;
	process_match();
      } catch(Exception& E) {
	E << "Error occurred in Dem Match at point (" << i << ", " << j << ")";
	throw;
      }
    }
  return process_res();
}

//-----------------------------------------------------------------------
/// Variation of surface_point where we go from the Surface. This
/// requires that our image matcher is a SurfaceImageToImageMatch. But
/// this runs much faster for ImageGroundConnection where calculating
/// ground_coordinate is expensive.
//-----------------------------------------------------------------------

blitz::Array<double, 2> DemMatch::surface_point(const MapInfo& Mi,
						bool Include_ic) const
{
  include_ic = Include_ic;
  process_setup();
  boost::shared_ptr<GroundCoordinate> gc;
  boost::shared_ptr<SurfaceImageToImageMatch> m =
    boost::dynamic_pointer_cast<SurfaceImageToImageMatch>(match_);
  if(!m)
    throw Exception("match_ must be a SurfaceImageToImageMatch.");
  for(int i = 0; i < Mi.number_x_pixel();  ++i)
    for(int j = 0; j < Mi.number_y_pixel();  ++j) {
      try {
	++npoint;
	gc = Mi.ground_coordinate(i, j);
	m->match_surf(*gc, ic1, ic2, line_sigma, sample_sigma, 
		      success, &diagnosticv);
	process_match();
      } catch(ImageGroundConnectionFailed& E) {
	// Just treat like out of range error
	success = false;
	diagnosticv = IMAGE_GROUND_CONNECTION_FAILED;
	process_match();
      } catch(Exception& E) {
	E << "\nError occurred in Dem Match at surface point (" << i << ", " << j << "), gc: " << *gc;
	throw;
      }
      
    }
  return process_res();
}

void DemMatch::process_setup() const
{
  res.clear();
  ic1v.clear();
  ic2v.clear();
  npoint = 0;
  diagnostic_value = 0;
  all_stat.reset(new Statistic);
  good_stat.reset(new Statistic);
}

void DemMatch::process_match() const
{
  if(success) {
    try {
      ri->two_ray_intersect(ic1, ic2, p, dist);
    } catch(...) {
      success = false;
      diagnostic_value(RAY_INTERSECT_FAILED) += 1;
    }
    if(success) {
      all_stat->add(dist);
      if(dist < max_dist) {
	good_stat->add(dist);
	Ecr* ecr = dynamic_cast<Ecr*>(p.get());
	if(!ecr)
	  throw Exception("Must be Ecr");
	res.push_back(ecr->convert_to_geodetic());
	if(include_ic) {
	  ic1v.push_back(ic1);
	  ic2v.push_back(ic2);
	}
      }
    }
  } else {
    if(diagnosticv >= 0 && diagnosticv <= MAX_DIAGNOSTIC_VALUE)
      diagnostic_value(diagnosticv) += 1;
    else
      diagnostic_value(MAX_DIAGNOSTIC_VALUE + 1) += 1;
  }
}

blitz::Array<double, 2> DemMatch::process_res() const
{
  blitz::Array<double, 2> res2((int) res.size(), (include_ic ? 3 + 4 : 3));
  for(int i = 0; i < res2.rows(); ++i) {
    res2(i, 0) = res[i].latitude();
    res2(i, 1) = res[i].longitude();
    res2(i, 2) = res[i].height_reference_surface();
    if(include_ic) {
      res2(i, 3) = ic1v[i].line;
      res2(i, 4) = ic1v[i].sample;
      res2(i, 5) = ic2v[i].line;
      res2(i, 6) = ic2v[i].sample;
    }
  }
  return res2;
}
