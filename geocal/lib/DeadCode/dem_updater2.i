// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "dem_updater2.h"
%}


%geocal_markfunc(DemUpdater2);
namespace GeoCal {
class DemUpdater2 {
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
  double height_reference_surface(double Line, double Sample) const;
  void find_height(int Line, int Sample, double Hstart, 
		   double& OUTPUT, bool& OUTPUT, 
		   double* OUTPUT = 0) const;
  std::string print_to_string() const;
  double height(int Line, int Sample) const;
};

}
