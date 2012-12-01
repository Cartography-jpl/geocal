#ifndef DEM_UPDATER2_H
#define DEM_UPDATER2_H
#include "rpc_image.h"
#include "simple_dem.h"
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This is a second try at DemUpdater.

  This class is currently experimental, I'm not exactly sure what the
  interface and functionality should look like.
*******************************************************************/

class DemUpdater2 : public PrintAndMarkable<DemUpdater2> {
public:
  DemUpdater2(const boost::shared_ptr<MapProjectedImage>& Img_ref_surf1,
	      const boost::shared_ptr<MapProjectedImage>& Img_ref_surf2,
	      const Rpc& R1, const Rpc& R2,
	      const boost::shared_ptr<Dem>& D, 
	      double Min_correlation = 0.2,
	      int Template_size = 15, double Max_height_correction = 50,
	      double Max_diff_neighbor = 5.0, double Step_size = 1.0);
  double compare_image(int Line, int Sample, double H) const;
  ImageCoordinate ic_ref_surf2(int Line, int Sample, double H) const;
  void find_height(int Line, int Sample, double Hstart, 
		   double& Hres, bool& Success, 
		   double* Max_correlation = 0) const;
  double height_reference_surface(double Line, double Sample) const;
  double height_reference_surface(int Line, int Sample) const;
  double height_func(int Line, int Sample, double Height) const;
  double height(int Line, int Sample) const;
  void print(std::ostream& Os) const;
protected:
  void mark() {mark_it(dem); mark_it(img_ref_surf1); mark_it(img_ref_surf2);}
private:
  boost::shared_ptr<Dem> dem;
  int template_size;
  double max_height_correction;
  double min_correlation;
  double max_diff_neighbor;
  double step_size;
  boost::shared_ptr<MapProjectedImage> img_ref_surf1;
  boost::shared_ptr<MapProjectedImage> img_ref_surf2;
  Rpc r1;
  Rpc r2;
  mutable blitz::Array<double, 2> height_rsurf;
  SimpleDem ref_surf;
  int ths;
};

}
#endif
