#include "dem_fit.h"
#include "gdal_map_projected_image.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

DemFit::DemFit(const boost::shared_ptr<RasterImage> &Raw_img1, const Rpc &R1, 
	       const boost::shared_ptr<RasterImage> &Raw_img2, const Rpc &R2, 
	       const boost::shared_ptr<MemoryDem> &D, const MapInfo& M,
	       double Stiff)
  : rpc_img1(Raw_img1, R1, D, M),
    rpc_img2(Raw_img2, R2, D, M),
    mask(M.number_y_pixel(), M.number_x_pixel()),
    dem(D),
    original_elevation(D->elevation_data),
    stiffness(Stiff)
{
  const double threshold = 0.3;
  mask = (correlation() < threshold);
  nval = count(!mask);
  Array<double, 1> r1(nval);
  Array<double, 1> r2(nval);
  int k = 0;
  for(int i = 0; i < rpc_img1.number_line(); ++i)
    for(int j = 0; j < rpc_img1.number_sample(); ++j) {
      if(!mask(i, j)) {
	r1(k) = rpc_img1.value(i, j);
	r2(k) = rpc_img2.value(i, j);
	++k;
      }
    }
  double m1 = mean(r1);
  double m2 = mean(r2);
  double v1 = mean(pow2(r1 - m1));
  double v2 = mean(pow2(r2 - m2));
  h0 = m1 - m2;
  h1 = v1 / v2;
  // We can probably work out an equation of this, but for now
  // explicitly calculate so we 
  nstiffeq = 0;
  for(boost::multi_array<double, 2>::size_type i = 0; 
      i < dem->elevation_data.shape()[0]; ++i)
    for(boost::multi_array<double, 2>::size_type  j = 0; 
	j < dem->elevation_data.shape()[1]; ++j) {
      if(j + 1 < dem->elevation_data.shape()[1]) {
	if(i > 1)
	  ++nstiffeq;
	++nstiffeq;
	if(i + 1 < dem->elevation_data.shape()[0])
	  ++nstiffeq;
      }
      if(i + 1 < dem->elevation_data.shape()[0])
	++nstiffeq;
    }
}

//-----------------------------------------------------------------------
/// Differences between 2 images.
//-----------------------------------------------------------------------

void DemFit::value(std::vector<double>& Res) const
{
  Res.resize(nval + dem->elevation_data.num_elements() + nstiffeq);
  int k = 0;
  for(int i = 0; i < rpc_img1.number_line(); ++i)
    for(int j = 0; j < rpc_img1.number_sample(); ++j)
      if(!mask(i, j)) {
	Res[k] = rpc_img1.value(i, j) - (h0 + h1 * rpc_img2.value(i, j));
	++k;
      }
  boost::multi_array<double, 2> diff(dem->elevation_data);
  for(boost::multi_array<double, 2>::size_type i = 0; 
      i < dem->elevation_data.shape()[0]; ++i)
    for(boost::multi_array<double, 2>::size_type j = 0; 
	j < dem->elevation_data.shape()[1]; ++j, ++k) {
      diff[i][j] -= original_elevation[i][j];
      Res[k] = diff[i][j];
    }
  for(boost::multi_array<double, 2>::size_type i = 0; 
      i < dem->elevation_data.shape()[0]; ++i)
    for(boost::multi_array<double, 2>::size_type j = 0; 
	j < dem->elevation_data.shape()[1]; ++j) {
      if(j + 1 < dem->elevation_data.shape()[1]) {
	if(i > 1) {
	  Res[k] = stiffness * (diff[i][j] - diff[i - 1][j + 1]);
	  ++k;
	}
	Res[k] = stiffness * (diff[i][j] - diff[i][j + 1]);
	++k; 
	if(i + 1 < dem->elevation_data.shape()[0]) {
	  Res[k] = stiffness * (diff[i][j] - diff[i + 1][j + 1]);
	  ++k;
	}
      }
      if(i + 1 < dem->elevation_data.shape()[0]) {
	Res[k] = stiffness * (diff[i][j] - diff[i + 1][j]);
	++k;
      }
    }
}

#ifdef DO_SPARSE
using namespace gmm;
void DemFit::jacobian(sparse_matrix& Res) const
{
  Res.resize(nval + dem->elevation_data.num_elements() + nstiffeq, 
	     dem->elevation_data.num_elements());
  clear(Res);
  int k = 0;
  for(int i = 0; i < rpc_img1.number_line(); ++i)
    for(int j = 0; j < rpc_img1.number_sample(); ++j) 
      if(!mask(i, j)) {
      // This is dh/dparm
      dem->jacobian(*rpc_img1.ground_coordinate(ImageCoordinate(i + 0.5, j + 0.4)),
		    mat_row(Res, k));

      // Multiply by dv/dh to get dv/dparm
      double dv_dh = rpc_img1.value_dh(i, j) - h1 * rpc_img2.value_dh(i, j);
      scale(mat_row(Res, k), dv_dh);
      ++k;
    }
  for(int i = 0; i < dem->elevation_data.num_elements(); ++i, ++k)
    Res[k][i] = 1;
  int nl = dem->elevation_data.shape()[0];
  int ns = dem->elevation_data.shape()[1];
  for(int i = 0; i < nl; ++i)
    for(int j = 0; j < ns; ++j) {
      if(j + 1 < ns) {
	if(i - 1 > 0) {
	  Res[k][i * nl + j] = stiffness;
	  Res[k][(i - 1) * nl + j + 1] = -stiffness;
	  ++k;
	}
	Res[k][i * nl + j] = stiffness;
	Res[k][i * nl + j + 1] = -stiffness;
	++k; 
	if(i + 1 < nl) {
	  Res[k][i * nl + j] = stiffness;
	  Res[k][(i + 1) * nl + j + 1] = -stiffness;
	  ++k;
	}
      }
      if(i + 1 < nl) {
	Res[k][i * nl + j] = stiffness;
	Res[k][(i + 1) * nl + j] = -stiffness;
	++k;
      }
    }
}
#endif

void DemFit::write(const std::string& fname1, const std::string& fname2, 
		   const std::string& fname3, const std::string& fname4) const
{
  GdalMapProjectedImage rpc1_out(fname1, "hfa", rpc_img1.map_info(), 
				 1, GDT_UInt16);
  GdalMapProjectedImage rpc2_out(fname2, "hfa", rpc_img2.map_info(), 
				 1, GDT_UInt16);
  GdalMapProjectedImage dem_out(fname3, "hfa", rpc_img1.map_info(), 
				1, GDT_UInt16);
  GdalMapProjectedImage corr_out(fname4, "hfa", rpc_img1.map_info(), 
				 1, GDT_UInt16);
  copy(rpc_img1, rpc1_out);
  copy(rpc_img2, rpc2_out);
//   Array<double, 2> corr = correlation();
//   Array<GUInt16, 2> corr_scale(
//       cast<GUInt16>(where(corr < 0, 0.0, where(corr > 1, 1.0, corr)) *  65535));
  for(int i = 0; i < dem_out.number_line(); ++i)
    for(int j = 0; j < dem_out.number_sample(); ++j) {
      dem_out.write(i, j, (int) floor(dem_out.ground_coordinate(ImageCoordinate(i + 0.5, j + 0.5), *dem)->height_reference_surface() + 0.5));
//      corr_out.write(i, j, corr_scale(i, j));
    }
}

Array<double, 2> DemFit::correlation() const
{
  Array<double, 2> v1(rpc_img1.number_line(), rpc_img1.number_sample());
  Array<double, 2> v2(v1.shape());
  for(int i = 0; i < v1.extent(0); ++i)
    for(int j = 0; j < v1.extent(1); ++j) {
      v1(i, j) = rpc_img1.value(i, j);
      v2(i, j) = rpc_img2.value(i, j);
    }
  int ths = 4;
  Array<double, 2> res(rpc_img1.number_line(), 
		       rpc_img1.number_sample());
  res = 0;
  for(int i = ths; i < res.extent(0) - ths - 1; ++i)
    for(int j = ths; j < res.extent(1) - ths - 1; ++j) {
      Range r1(i - ths, i + ths);
      Range r2(j - ths, j + ths);
      Array<double, 2> v1sub(v1(r1, r2));
      Array<double, 2> v2sub(v2(r1, r2));
      double v1mean = mean(v1sub);
      double v2mean = mean(v2sub);
      double v1var = (sum(sqr(v1sub)) - v1sub.size() * v1mean * v1mean) / 
	(v1sub.size() - 1);
      double v2var = (sum(sqr(v2sub)) - v2sub.size() * v2mean * v2mean) / 
	(v2sub.size() - 1);
      double ccorr_covariance =
	(sum(v1sub * v2sub) - v1sub.size() * v1mean * v2mean) / 
	(v1sub.size() - 1);
      res(i, j) = fabs(ccorr_covariance) * ccorr_covariance / (v1var * v2var);
    }
  return res;
}
