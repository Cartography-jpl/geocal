#ifndef DEM_UPDATER_H
#define DEM_UPDATER_H
#include "rpc_image.h"
#include "simple_dem.h"
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This is used to update a Dem that is used to map project a pair of
  images using Rpc. 

  This class is currently experimental, I'm not exactly sure what the
  interface and functionality should look like.
*******************************************************************/

class DemUpdater : public PrintAndMarkable<DemUpdater> {
public:
  DemUpdater(const boost::shared_ptr<RasterImage>& Raw_img1,
	     const boost::shared_ptr<RasterImage>& Raw_img2,
	     const Rpc& R1, const Rpc& R2,
	     const boost::shared_ptr<Dem>& D, const MapInfo& Mi,
	     int Template_size = 21, double Max_height_correction = 50,
	     double Initial_step = 10);
  double compare_image(int Line, int Sample, double H) const;
  double find_height(int Line, int Sample) const;
  void print(std::ostream& Os) const;
  blitz::Array<double, 2> generate_dem() const;
protected:
  void mark() {mark_it(img_proj1); mark_it(img_proj2); mark_it(dem);}
private:
  boost::shared_ptr<RpcImage> img_proj1;
  boost::shared_ptr<RpcImage> img_proj2;
  boost::shared_ptr<Dem> dem;
  mutable boost::shared_ptr<SimpleDem> sd;
  int template_size;
  double max_height_correction;
  double initial_step;
};

}
#endif
