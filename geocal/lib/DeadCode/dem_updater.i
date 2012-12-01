// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "dem_updater.h"
%}


%geocal_markfunc(DemUpdater);
namespace GeoCal {
class DemUpdater {
public:
  DemUpdater(const boost::shared_ptr<RasterImage>& Raw_img1,
	     const boost::shared_ptr<RasterImage>& Raw_img2,
	     const Rpc& R1, const Rpc& R2,
	     const boost::shared_ptr<Dem>& D, const MapInfo& Mi,
	     int Template_size = 21, double Max_height_correction = 50,
	     double Initial_step = 10);
  double compare_image(int Line, int Sample, double H) const;
  double find_height(int Line, int Sample) const;
  std::string print_to_string() const;
// blitz::Array semantics confuse SWIG. Add an extra level of indirection.
  %extend {
    boost::shared_ptr<blitz::Array<double, 2> > generate_dem() const
    { 
      return boost::shared_ptr<blitz::Array<double, 2> >(
         new blitz::Array<double, 2>($self->generate_dem()));
    }
  }

};

}
