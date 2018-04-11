#include "rsm_image_ground_connection.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <cmath>
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmImageGroundConnection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnection);
  ar & GEOCAL_NVP_(rsm);
}

GEOCAL_IMPLEMENT(RsmImageGroundConnection);
#endif

void
RsmImageGroundConnection::cf_look_vector
(const ImageCoordinate& Ic,
 CartesianFixedLookVector& Lv,
 boost::shared_ptr<CartesianFixed>& P) const
{
  double z = rsm_->rsm_base()->initial_guess_z(Ic.line, Ic.sample);
  boost::shared_ptr<GroundCoordinate> gc1 =
    rsm_->ground_coordinate(Ic, z);
  double delta_h = 10;
  boost::shared_ptr<GroundCoordinate> gc2 =
    rsm_->ground_coordinate(Ic, z + delta_h);
  P = gc1->convert_to_cf();
  boost::shared_ptr<CartesianFixed> ec2 = gc2->convert_to_cf();
  Lv.look_vector[0] = ec2->position[0] - P->position[0];
  Lv.look_vector[1] = ec2->position[1] - P->position[1];
  Lv.look_vector[2] = ec2->position[2] - P->position[2];
}

ImageCoordinate RsmImageGroundConnection::image_coordinate(const GroundCoordinate& Gc) const
{
  ImageCoordinate res = rsm_->image_coordinate(Gc);
  if(std::isnan(res.line) || std::isnan(res.sample))
    throw ImageGroundConnectionFailed();
  return res;
}

void RsmImageGroundConnection::image_coordinate_with_status
(const GroundCoordinate& Gc, ImageCoordinate& Res, bool& Success) const
{
  Res = rsm_->image_coordinate(Gc);
  if(std::isnan(Res.line) || std::isnan(Res.sample))
    Success = false;
  else
    Success = true;
}

Array<double, 2> RsmImageGroundConnection::image_coordinate_jac_cf
(const CartesianFixed& Gc) const
{
  firstIndex i1; secondIndex i2; thirdIndex i3;
  Array<double, 2> res(2, 3);
  double x, y, z;
  rsm_->coordinate_converter()->convert_to_coordinate(Gc, x, y, z);
  Array<double, 2> dxyz_dcf(3, 3);
  boost::shared_ptr<CartesianFixed> gc2 = Gc.convert_to_cf();
  const double eps = 10;
  for(int i = 0; i < 3; ++i) {
    gc2->position[i] += eps;
    double x2, y2, z2;
    rsm_->coordinate_converter()->convert_to_coordinate(*gc2, x2, y2, z2);
    dxyz_dcf(0, i) = (x2 - x) / eps;
    dxyz_dcf(1, i) = (y2 - y) / eps;
    dxyz_dcf(2, i) = (z2 - z) / eps;
    gc2->position[i] -= eps;
  }
  Array<double, 2> jac_xyz = 
    rsm_->image_coordinate_jacobian(x, y, z);
  res = sum(jac_xyz(i1, i3) * dxyz_dcf(i3, i2), i3);
  return res;
}

void RsmImageGroundConnection::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmImageGroundConnection" << "\n"
     << "  Rsm: \n";
  opad << *rsm_ << "\n";
  opad.strict_sync();
}


