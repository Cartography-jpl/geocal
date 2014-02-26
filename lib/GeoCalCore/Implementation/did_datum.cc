#include "did_datum.h"
#include <cmath>

using namespace GeoCal;

const int msl_number_row = 2160;
const int msl_number_col = 4320;

//-----------------------------------------------------------------------
/// This creates a DidDatum.  This takes the name of the mean sea
/// level data, i.e., 
/// /data/bank/anc/DID/MeanSeaLevel/current/geoid_2160h_4320w.half
//-----------------------------------------------------------------------

DidDatum::DidDatum(const std::string& Fname)
: mi_(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter),
      -180, 90, 180, -90, msl_number_col, msl_number_row),
  msl_(Fname, boost::extents[msl_number_row][msl_number_col][2])
{
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

  int res = ((int) msl_.data()[row][col][0]) << 8 | msl_.data()[row][col][1];
  return res;
}
