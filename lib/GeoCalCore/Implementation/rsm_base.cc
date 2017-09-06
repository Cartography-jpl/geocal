#include "rsm_base.h"
#include "geocal_serialize_support.h"
#include "image_ground_connection.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmBase::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmBase);
}

GEOCAL_IMPLEMENT(RsmBase);
#endif

blitz::Array<double, 4> RsmBase::generate_data
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_x, double Max_x,
 double Min_y, double Max_y,
 double Min_z, double Max_z,
 int Numx, int Numy, int Numz)
{
  Array<double, 4> res(2, Numx, Numy, Numz);
  for(int i = 0; i < Numx; ++i)
    for(int j = 0; j < Numy; ++j)
      for(int k = 0; k < Numz; ++k) {
	double x = Min_x + (Max_x - Min_x) / (Numx - 1) * i;
	double y = Min_y + (Max_y - Min_y) / (Numy - 1) * j;
	double z = Min_z + (Max_z - Min_z) / (Numz - 1) * k;
	boost::shared_ptr<GroundCoordinate> gc =
	  Cconv.convert_from_coordinate(x, y, z);
	ImageCoordinate ic;
	bool success;
	Igc.image_coordinate_with_status(*gc, ic, success);
	if(success) {
	  res(0,i,j,k) = ic.line;
	  res(1,i,j,k) = ic.sample;
	} else {
	  res(0,i,j,k) = -999;
	  res(1,i,j,k) = -999;
	}
      }
  return res;
}

