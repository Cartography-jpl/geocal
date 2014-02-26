#include "rpc_image_ground_connection.h"
#include "simple_dem.h"
#include <boost/lexical_cast.hpp>
using namespace GeoCal;
using namespace blitz;

// See base class for description
Array<double, 1> RpcImageGroundConnection::parameter() const
{
  int size = std::count(rpc_->fit_line_numerator.begin(), 
			rpc_->fit_line_numerator.end(), true) +
    std::count(rpc_->fit_sample_numerator.begin(), 
	       rpc_->fit_sample_numerator.end(), true);
  if(fit_height_offset())
    size += 1;
  Array<double, 1> res(size);
  int j = 0;
  for(int i = 0; i < (int) rpc_->fit_line_numerator.size();  ++i)
    if(rpc_->fit_line_numerator[i])
      res(j++) = rpc_->line_numerator[i];
  for(int i = 0; i < (int) rpc_->fit_sample_numerator.size();  ++i)
    if(rpc_->fit_sample_numerator[i])
      res(j++) = rpc_->sample_numerator[i];
  if(fit_height_offset())
    res(j++) = rpc_->height_offset;
  return res;
}

std::vector<std::string> RpcImageGroundConnection::parameter_name() const
{
  std::vector<std::string> res;
  for(int i = 0; i < (int) rpc_->fit_line_numerator.size();  ++i)
    if(rpc_->fit_line_numerator[i])
      res.push_back("RPC Line Numerator Parameter " + 
		    boost::lexical_cast<std::string>(i));
  for(int i = 0; i < (int) rpc_->fit_sample_numerator.size();  ++i)
    if(rpc_->fit_sample_numerator[i])
      res.push_back("RPC Sample Numerator Parameter " + 
		    boost::lexical_cast<std::string>(i));
  if(fit_height_offset())
    res.push_back("RPC Height Offset");
  return res;
}

void
RpcImageGroundConnection::cf_look_vector
(const ImageCoordinate& Ic,
 CartesianFixedLookVector& Lv,
 boost::shared_ptr<CartesianFixed>& P) const
{
  double h = rpc_->height_offset;
  Geodetic gc1 = rpc_->ground_coordinate(Ic, h);
  double delta_h = 10;
  Geodetic gc2 = rpc_->ground_coordinate(Ic, h + delta_h);
  P = gc1.convert_to_cf();
  boost::shared_ptr<CartesianFixed> ec2 = gc2.convert_to_cf();
  Lv.look_vector[0] = ec2->position[0] - P->position[0];
  Lv.look_vector[1] = ec2->position[1] - P->position[1];
  Lv.look_vector[2] = ec2->position[2] - P->position[2];
}

void RpcImageGroundConnection::parameter(const Array<double, 1>& Parm)
{
  int size = std::count(rpc_->fit_line_numerator.begin(), 
			rpc_->fit_line_numerator.end(), true) +
    std::count(rpc_->fit_sample_numerator.begin(), 
	       rpc_->fit_sample_numerator.end(), true);
  if(fit_height_offset())
    size += 1;
  if(Parm.rows() != size) {
    Exception e;
    e << "Parm is the wrong size\n"
      << "  Parm size:     " << Parm.rows() << "\n"
      << "  Expected size: " << size << "\n";
    throw e;
  }
  int j = 0;
  for(int i = 0; i < (int) rpc_->fit_line_numerator.size();  ++i)
    if(rpc_->fit_line_numerator[i])
      rpc_->line_numerator[i] = Parm(j++);
  for(int i = 0; i < (int) rpc_->fit_sample_numerator.size();  ++i)
    if(rpc_->fit_sample_numerator[i])
      rpc_->sample_numerator[i] = Parm(j++);
  if(fit_height_offset())
    rpc_->height_offset = Parm(j++);
}

Array<double, 2> RpcImageGroundConnection::image_coordinate_jac_ecr
(const Ecr& Gc) const
{
  firstIndex i1; secondIndex i2; thirdIndex i3;
  Array<double, 2> res(2, 3);
  Geodetic g0(Gc);
  Array<double, 2> dgeod_decr(3, 3);
  Ecr gc2(Gc);
  const double eps = 10;
  for(int i = 0; i < 3; ++i) {
    gc2.position[i] += eps;
    Geodetic g1(gc2);
    dgeod_decr(0, i) = (gc2.latitude() - g0.latitude()) / eps;
    dgeod_decr(1, i) = (gc2.longitude() - g0.longitude()) / eps;
    dgeod_decr(2, i) = (gc2.height_reference_surface() - 
			g0.height_reference_surface()) / eps;
    gc2.position[i] -= eps;
  }
  Array<double, 2> jac_geod = 
    rpc_->image_coordinate_jac(g0.latitude(), g0.longitude(), 
			      g0.height_reference_surface());
  res = sum(jac_geod(i1, i3) * dgeod_decr(i3, i2), i3);
  return res;
}

blitz::Array<double, 2> 
RpcImageGroundConnection::image_coordinate_jac_parm
(const GroundCoordinate& Gc) const
{
  Array<double, 2> res = rpc_->image_coordinate_jac_parm(Gc);
  if(fit_height_offset()) {
    res.resizeAndPreserve(res.rows(), res.cols() + 1);
    Array<double, 2> jac_geod = 
      rpc_->image_coordinate_jac(Gc.latitude(), Gc.longitude(), 
				 Gc.height_reference_surface());
    // Negative here because increasing height_offset is like
    // decreasing the height passed to the RPC
    res(0,res.cols() - 1) = -jac_geod(0,2);
    res(1,res.cols() - 1) = -jac_geod(1,2);
  }
  return res;
}

