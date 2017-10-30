#include "rsm_base.h"
#include "geocal_serialize_support.h"
#include "image_ground_connection.h"
#include "tre_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmBase::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmBase);
  // Older version didn't have image_identifier_ or
  // rsm_suport_data_edition_
  if(version > 0) {
    ar & GEOCAL_NVP_(image_identifier)
      & GEOCAL_NVP_(rsm_suport_data_edition);
  }
  boost::serialization::split_member(ar, *this, version);
}
template<class Archive>
void RsmBase::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void RsmBase::load(Archive & ar, const unsigned int version)
{
  // Older version didn't have image_identifier_ or
  // rsm_suport_data_edition_
  if(version == 0) {
    image_identifier_ = "";
    rsm_suport_data_edition_ = "fake-1";
  }
}

GEOCAL_IMPLEMENT(RsmBase);
#endif

static boost::format f("%|1$-80s|%|2$-40s|");

//-----------------------------------------------------------------------
/// Write the part of the TRE string for the image identification and
/// RSM support data edition.
//-----------------------------------------------------------------------

std::string RsmBase::base_tre_string() const
{
  return str_check_size(f % image_identifier_ % rsm_suport_data_edition_,
			120);
}

void RsmBase::base_read_tre_string(std::istream& In)
{
  image_identifier_ = read_size<std::string>(In, 80);
  rsm_suport_data_edition_ = read_size<std::string>(In, 40);
}

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

