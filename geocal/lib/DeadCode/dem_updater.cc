#include "dem_updater.h"
#include <boost/timer.hpp>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Create a DemUpdater given the 2 raw images, the 2 RPCs for the raw
/// images, the original Dem, and the MapInfo for where we want the
/// final Dem. The Rpc should give map projected images that only
/// differ because of errors in the Dem, other errors should have been
/// already removed (e.g., use fit of RpcImage).
//-----------------------------------------------------------------------

DemUpdater::DemUpdater(const boost::shared_ptr<RasterImage>& Raw_img1,
		       const boost::shared_ptr<RasterImage>& Raw_img2,
		       const Rpc& R1, const Rpc& R2,
		       const boost::shared_ptr<Dem>& D, const MapInfo& Mi,
		       int Template_size, double Max_height_correction,
		       double Initial_step)
: dem(D), template_size(Template_size), 
  max_height_correction(Max_height_correction),
  initial_step(Initial_step)
{
  sd.reset(new SimpleDem(0));
  img_proj1.reset(new RpcImage(Raw_img1, R1, sd, Mi));
  img_proj2.reset(new RpcImage(Raw_img2, R2, sd, Mi));
}

//-----------------------------------------------------------------------
/// Compare two images. The Line and Sample are relative to the
/// MapInfo passed in. We assign the given height to this point, and
/// then check how similar the images are to each other. This returns
/// a value, the smaller the value the closer the images are to each
/// other.
/// 
/// I'm not exactly sure how we should calculate this metric, but
/// right now we scale and add a constant to image 2 to have the same
/// mean and variance as image 1, and then take the sum of the square
/// of the differences between image 1 and 2.
//-----------------------------------------------------------------------

double DemUpdater::compare_image(int Line, int Sample, double H) const
{
  sd->h(H);
  blitz::Array<int, 2> p1 = 
    img_proj1->read_blitz(Line - (template_size - 1) / 2,
			  Sample - (template_size - 1) / 2,
			  template_size, template_size);
  blitz::Array<int, 2> p2 = 
    img_proj2->read_blitz(Line - (template_size - 1) / 2,
			  Sample - (template_size - 1) / 2,
			  template_size, template_size);
  double mean1 = mean(p1);
  double var1 = (sum(sqr(p1)) - p1.size() * mean1 * mean1) / (p1.size() - 1);
  double mean2 = mean(p2);
  double var2 = (sum(sqr(p2)) - p2.size() * mean2 * mean2) / (p2.size() - 1);
  return sum(sqr(p1 - (p2 + mean1 - mean2) * var1 / var2));
}

//-----------------------------------------------------------------------
/// Find the height that gives the minimum compare_image. Note that
/// this algorithm can probably be improved on.
//-----------------------------------------------------------------------

double DemUpdater::find_height(int Line, int Sample) const
{
  double hstart = dem->height_reference_surface(
	*(img_proj1->ground_coordinate(ImageCoordinate(Line, Sample))));
  double hmin = 0;
  double minv = 1e90;		// A large number, to get process started.
  for(double h = hstart - max_height_correction; 
      h <= hstart + max_height_correction; h += initial_step) {
    double t = compare_image(Line, Sample, h);
    if(t < minv) {
      minv = t;
      hmin = h;
    }
  }
  double hmin2 = hmin;
  // This is a bit ad hoc, need to come up with a cleaner algorithm.
  for(double h = hmin - initial_step / 2; h <= hmin + initial_step / 2;
      h += initial_step / 10.0) {
    double t = compare_image(Line, Sample, h);
    if(t < minv) {
      minv = t;
      hmin2 = h;
    }
  }
  return hmin2;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void DemUpdater::print(std::ostream& Os) const
{
  Os << "Dem updater\n"
     << "Raw image 1:\n" << img_proj1->raw_image()
     << "Raw image 2:\n" << img_proj2->raw_image()
     << "Dem:\n" << *dem
     << "Map info:\n" << img_proj1->map_info();
}

//-----------------------------------------------------------------------
/// Generate the full Dem.
//-----------------------------------------------------------------------

blitz::Array<double, 2> DemUpdater::generate_dem() const
{
  blitz::Array<double, 2> res(img_proj1->map_info().number_y_pixel(),
			      img_proj1->map_info().number_x_pixel());
  int ths = (template_size - 1) / 2;
  boost::timer tm;
  for(int i = ths; i < res.extent(0) - ths; ++i) {
    std::cerr << "Starting line " << i << ", elapsed time: " 
	      << tm.elapsed() << "\n";
    for(int j = ths; j < res.extent(1) - ths; ++j) {
      if(j % 1000 == 0)
	std::cerr << "  Starting sample " << j << ", elapsed time: " 
		  << tm.elapsed() << "\n";
      res(i, j) = find_height(i, j);
    }
  }
  return res;
}

