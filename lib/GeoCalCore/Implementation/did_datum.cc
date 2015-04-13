#include "did_datum.h"
#include "geocal_serialize_support.h"
#include <cmath>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DidDatum::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(Datum);
  GEOCAL_BASE(DidDatum, Datum);
  std::string fname = file_name();
  ar & GEOCAL_NVP(fname);
}

template<class Archive>
void DidDatum::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Datum);
  GEOCAL_BASE(DidDatum, Datum);
  std::string fname;
  ar & GEOCAL_NVP(fname);
  initialize(fname);
}

GEOCAL_SPLIT_IMPLEMENT(DidDatum);
#endif

const int msl_number_row = 2160;
const int msl_number_col = 4320;

//-----------------------------------------------------------------------
/// This creates a DidDatum.  This takes the name of the mean sea
/// level data, i.e., 
/// /data/bank/anc/DID/MeanSeaLevel/current/geoid_2160h_4320w.half
//-----------------------------------------------------------------------

DidDatum::DidDatum(const std::string& Fname)
{
  initialize(Fname);
}

void DidDatum::initialize(const std::string& Fname) 
{
  mi_ = MapInfo(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter),
		-180, 90, 180, -90, msl_number_col, msl_number_row);
  msl_.reset(new MemoryMapArray<char, 3>(Fname, boost::extents[msl_number_row][msl_number_col][2]));
}

//-----------------------------------------------------------------------
/// Undulation, which is the distance form mean sea level to the
/// reference ellipsoid, for the given ground location. This is in
/// meters. 
//-----------------------------------------------------------------------

double DidDatum::undulation(const GroundCoordinate& Gc) const
{

//-----------------------------------------------------------------------
// We find the data point closest to the given coordinate. We don't do
// any interpolation.
//-----------------------------------------------------------------------

  double x, y;
  mi_.coordinate(Gc, x, y);
  int row = (int) floor(y + 0.5);
  int col = (int) floor(x + 0.5);

//-----------------------------------------------------------------------
// We assemble the results from the byte level, so there isn't any
// endian problems.
//-----------------------------------------------------------------------

  int res = ((int) msl_->data()[row][col][0]) << 8 | msl_->data()[row][col][1];
  return res;
}

double DidDatum::undulation(const Geodetic& Gc) const
{

//-----------------------------------------------------------------------
// We find the data point closest to the given coordinate. We don't do
// any interpolation.
//-----------------------------------------------------------------------

  double x, y;
  mi_.coordinate(Gc, x, y);
  int row = (int) floor(y + 0.5);
  int col = (int) floor(x + 0.5);

//-----------------------------------------------------------------------
// We assemble the results from the byte level, so there isn't any
// endian problems.
//-----------------------------------------------------------------------

  int res = ((int) msl_->data()[row][col][0]) << 8 | msl_->data()[row][col][1];
  return res;
}
