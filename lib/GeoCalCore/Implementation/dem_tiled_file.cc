#include "dem_tiled_file.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DemTiledFile::serialize(Archive & ar, const unsigned int version)
{
  // Not sure if this serialization will ever be used, the classes
  // that use DemTiledFile have there own split serialization (e.g.,
  // GdalDem) and calls DemTiledFile initialize directly. But make
  // this available in case it ever gets used. 
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP_(data)
    & GEOCAL_NVP_(scale);
}

GEOCAL_IMPLEMENT(DemTiledFile);
#endif

//-----------------------------------------------------------------------
/// Return height in meters relative to datum(). Note that the call is
/// in line, sample order, which means Y and then X.
//-----------------------------------------------------------------------

double DemTiledFile::elevation(int Y_index, int X_index) const
{
  boost::array<index, 2> i = {{Y_index, X_index}};
  double t = data_->get_double(i);
  // I'm pretty sure we really do want == here. This is a specific
  // flag value, and is the bit pattern that will be in the value
  // for no data value.
  if(t == no_data_value_) {
    if(outside_dem_is_error()) {
      Exception e;
      e << "Height requested has NoDataValue in Dem data "
	<< "xindex: " << X_index << " yindex: " << Y_index << "\n";
      throw e;
    } else
      return 0.0;
  }
  return scale_ * t;
}

