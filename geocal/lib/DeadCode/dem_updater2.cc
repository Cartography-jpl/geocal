#include "dem_updater2.h"
#include <boost/timer.hpp>
#include "geocal_gsl_root.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Create a DemUpdater given the 2 images projected to a reference
/// surface at height 0, the 2 RPCs for the raw images, and the
/// original Dem. 
//-----------------------------------------------------------------------

DemUpdater2::DemUpdater2(
           const boost::shared_ptr<MapProjectedImage>& Img_ref_surf1,
           const boost::shared_ptr<MapProjectedImage>& Img_ref_surf2,
           const Rpc& R1, const Rpc& R2,
           const boost::shared_ptr<Dem>& D, 
	   double Min_correlation,
	   int Template_size, double Max_height_correction,
	   double Max_diff_neighbor, double Step_size)
: dem(D), template_size(Template_size), 
  max_height_correction(Max_height_correction),
  min_correlation(Min_correlation),
  max_diff_neighbor(Max_diff_neighbor),
  step_size(Step_size),
  img_ref_surf1(Img_ref_surf1),
  img_ref_surf2(Img_ref_surf2),
  r1(R1), r2(R2),
  height_rsurf(Img_ref_surf1->number_line(), Img_ref_surf2->number_sample()),
  ref_surf(0.0)
{
  ths = (template_size - 1) / 2;
  height_rsurf = -9999;
}

double DemUpdater2::height_reference_surface(double Line, double Sample) const
{
    int i = (int) floor(Line);
    int j = (int) floor(Sample);
    double t1 = height_reference_surface(i, j);
    double t2 = height_reference_surface(i, j + 1);
    double t3 = height_reference_surface(i + 1, j);
    double t4 = height_reference_surface(i + 1, j + 1);
    double t5 = t1 + (t2 - t1) * (Sample - j);
    double t6 = t3 + (t4 - t3) * (Sample - j);
    return t5 + (t6 - t5) * (Line - i);
}

double DemUpdater2::height_reference_surface(int Line, int Sample) const
{
  if(height_rsurf(Line, Sample) < -9998) {
    bool success;
    find_height(Line, Sample, -9999, height_rsurf(Line, Sample), success);
    if(!success) {
      ImageCoordinate m1(Line, Sample);
      ImageCoordinate i1 = 		
	r1.image_coordinate(*img_ref_surf1->ground_coordinate(m1, ref_surf));
      Geodetic g1t = r1.ground_coordinate(i1, *dem);
      height_rsurf(Line, Sample) = g1t.height_reference_surface();
    }
  }
  return height_rsurf(Line, Sample);
}

double DemUpdater2::height_func(int Line, int Sample, double Height) const
{
  SimpleDem surf_h(Height);
  ImageCoordinate i1 = 		
    r1.image_coordinate(*img_ref_surf1->ground_coordinate(ImageCoordinate(Line, Sample), surf_h));
  Geodetic g = r1.ground_coordinate(i1, ref_surf);
  ImageCoordinate i2 = img_ref_surf1->coordinate(g);
  return height_reference_surface(i2.line, i2.sample);
}

class F : public DFunctor {
public:
  F(const DemUpdater2& D, int Line, int Sample) : d(D), l(Line), s(Sample) {}
  virtual ~F() {}
  virtual double operator()(const double& H) const { return H - d.height_func(l, s, H); }
private:
  const DemUpdater2& d;
  int l, s;
};

double DemUpdater2::height(int Line, int Sample) const
{
  F f(*this, Line, Sample);
  return gsl_root(f, 0, 1000.0, 1e-2);
}

//-----------------------------------------------------------------------
/// Figure out conjugate point in projected reference surface 2 for a
/// point in reference surface 1 if it is at a given height.
//-----------------------------------------------------------------------

ImageCoordinate DemUpdater2::ic_ref_surf2(int Line, int Sample, double H) const
{
  // This is a brute force method for finding the point in image 2
  // corresponding to point in image 1 if the height is actually H.

  ImageCoordinate m1(Line, Sample);
				// Image coordinate in image 1
				// projected to reference surface
  ImageCoordinate i1 = 		
    r1.image_coordinate(*img_ref_surf1->ground_coordinate(m1, ref_surf));
				// Image in original raw image that got
				// projected to m1.
  SimpleDem surf_h(H);
  Geodetic g1 = r1.ground_coordinate(i1, surf_h);
				// Image coordinate in image 1 projected to
				// surface at height H.
  ImageCoordinate i2 = r2.image_coordinate(g1);
				// Image coordinate in raw image 2
				// that would get projected to point
				// g1
  Geodetic g2 = r2.ground_coordinate(i2, ref_surf);
				// Location point i2 is projected to
				// in reference surface
  ImageCoordinate m2 = img_ref_surf2->coordinate(g2);
				// Finally, location in reference
				// image.
  return m2;
}

//-----------------------------------------------------------------------
/// Compare two images. The Line and Sample are relative to the
/// MapInfo of Image 1. We assign the given height to this point, and
/// then check how similar the images are to each other. This returns
/// a value, the smaller the value the closer the images are to each
/// other.
/// 
/// I'm not exactly sure how we should calculate this metric, but
/// right now we scale and add a constant to image 2 to have the same
/// mean and variance as image 1, and then take the sum of the square
/// of the differences between image 1 and 2.
//-----------------------------------------------------------------------

double DemUpdater2::compare_image(int Line, int Sample, double H) const
{
  blitz::Array<int, 2> p1 = 
    img_ref_surf1->read_blitz(Line - ths, Sample - ths,
			      template_size, template_size);
  ImageCoordinate m2 = ic_ref_surf2(Line, Sample, H);
  blitz::Array<double, 2> p2 = 
    img_ref_surf2->interpolate(m2.line - ths, m2.sample - ths,
			       template_size, template_size);
  double mean1 = mean(p1);
  double var1 = (sum(sqr(p1)) - p1.size() * mean1 * mean1) / (p1.size() - 1);
//   if(var1 < min_variance)
//     variance too low
  double mean2 = mean(p2);
  double var2 = (sum(sqr(p2)) - p2.size() * mean2 * mean2) / (p2.size() - 1);
  double ccorr_covariance =
    (sum(p1 * p2) - p1.size() * mean1 * mean2) / (p1.size() - 1);
  double correlation = fabs(ccorr_covariance) * ccorr_covariance /
    (var1 * var2);
//   if(correlation < min_correlation)
//     correlation too low
  return correlation;
}

//-----------------------------------------------------------------------
/// Find the height that gives the minimum compare_image. Note that
/// this algorithm can probably be improved on. The Line and Sample
/// are for the data projected to the reference surface. We return the
/// point in 3-space that is seen in image 1.
//-----------------------------------------------------------------------

void DemUpdater2::find_height(int Line, int Sample, double Hstart, 
			      double& Hres, bool& Success,
			      double *Max_correlation) const
{
  ImageCoordinate m1(Line, Sample);
  double hbeg, hend;
  if(Hstart > -9998) {
    hbeg = Hstart - max_diff_neighbor;
    hend = Hstart + max_diff_neighbor;
  } else {
    ImageCoordinate i1 = 		
      r1.image_coordinate(*img_ref_surf1->ground_coordinate(m1, ref_surf));
    Geodetic g1t = r1.ground_coordinate(i1, *dem);
    double hstart = g1t.height_reference_surface();
    hbeg = hstart - max_height_correction;
    hend = hstart + max_height_correction;
  }
  double hmin = 0;
  double maxv = -1e90;		// A large number, to get process started.
  for(double h = hbeg; h <= hend; h += step_size) {
    double t = compare_image(Line, Sample, h);
    if(t > maxv) {
      maxv = t;
      hmin = h;
    }
  }
  if(Max_correlation)
    *Max_correlation = maxv;
  if(maxv < min_correlation)
    Success = false;
  else {
    Success = true;
    Hres = hmin;
  }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void DemUpdater2::print(std::ostream& Os) const
{
  Os << "Dem updater2\n"
     << "  Template size:         " << template_size << "\n"
     << "  Max height correction: " << max_height_correction << "\n"
     << "  Max diff neighbor:     " << max_diff_neighbor << "\n"
     << "  Min correlation:       " << min_correlation << "\n"
     << "  Step size:             " << step_size << "\n"
     << "Reference projected image 1:\n" << *img_ref_surf1
     << "Reference projected image 2:\n" << *img_ref_surf2
     << "Dem:\n" << *dem;
}



