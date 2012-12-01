#ifndef DEM_FIT_H
#define DEM_FIT_H
#include "rpc_image.h"
#include "memory_dem.h"
#ifdef DO_SPARSE
#include <gmm/gmm.h>
#endif

namespace GeoCal {
/****************************************************************//**
  This is a set of equations for fitting a Dem to a stereo pair of RPC
  images. This is a work in progress, we are likely to change this.
*******************************************************************/
class DemFit : public PrintAndMarkable<DemFit> {
public:
#ifdef DO_SPARSE
  typedef gmm::row_matrix< gmm::wsvector<double> > sparse_matrix;
#endif
  DemFit(const boost::shared_ptr<RasterImage> &Raw_img1, const Rpc &R1, 
	 const boost::shared_ptr<RasterImage> &Raw_img2, const Rpc &R2, 
	 const boost::shared_ptr<MemoryDem> &D, const MapInfo& M,
	 double Stiff);
  void value(std::vector<double>& Res) const;
#ifdef DO_SPARSE
  void jacobian(sparse_matrix& Res) const;
#endif
  void write(const std::string& fname1, const std::string& fname2, 
	     const std::string& fname3, const std::string& fname4) const;
  blitz::Array<double, 2> correlation() const;
protected:
  void mark() {mark_it(dem);}
private:
  RpcImage rpc_img1;
  RpcImage rpc_img2;
  blitz::Array<bool, 2> mask;
  boost::shared_ptr<MemoryDem> dem;
  boost::multi_array<double, 2> original_elevation;
  double stiffness;
  int nval;
  int nstiffeq;
  double h0;
  double h1;
};

}
#endif

