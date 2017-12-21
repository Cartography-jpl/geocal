#include "geometric_model.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GeometricModel::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GeometricModel);
}

template<class Archive>
void GeometricTiePoints::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GeometricTiePoints);
  ar & GEOCAL_NVP(y_tie)
    & GEOCAL_NVP(x_tie);
}

GEOCAL_IMPLEMENT(GeometricModel);
GEOCAL_IMPLEMENT(GeometricTiePoints);
#endif

//-----------------------------------------------------------------------
/// Add a point
//-----------------------------------------------------------------------

void GeometricTiePoints::add_point
(const ImageCoordinate& X_ic,
 const ImageCoordinate& Y_ic)
{
  if(replace_point_ >= (int) y_tie.size())
    replace_point_ = -1;
  if(replace_point_ < 0) {
    x_tie.push_back(X_ic);
    y_tie.push_back(Y_ic);
    return;
  }
  x_tie[replace_point_] = X_ic;
  y_tie[replace_point_] = Y_ic;
  ++replace_point_;
  notify_update();
}

//-----------------------------------------------------------------------
/// Return the resampled_ic as 2 columns, first is line second is sample;
//-----------------------------------------------------------------------
blitz::Array<double, 2> GeometricTiePoints::x() const
{
  blitz::Array<double, 2> res((int) x_tie.size(), 2);
  for(int i = 0; i < res.rows(); ++i) {
    res(i, 0) = x_tie[i].line;
    res(i, 1) = x_tie[i].sample;
  }
  return res;
}

//-----------------------------------------------------------------------
/// Remove the point at the given index.
//-----------------------------------------------------------------------

void GeometricTiePoints::remove_point(int Index)
{
  range_check(Index, 0, (int) x_tie.size());
  y_tie.erase(y_tie.begin() + Index);
  x_tie.erase(x_tie.begin() + Index);
  notify_update();
}


//-----------------------------------------------------------------------
/// Return the resampled_ic as 2 columns, first is line second is sample;
//-----------------------------------------------------------------------

blitz::Array<double, 2> GeometricTiePoints::y() const
{
  blitz::Array<double, 2> res((int) y_tie.size(), 2);
  for(int i = 0; i < res.rows(); ++i) {
    res(i, 0) = y_tie[i].line;
    res(i, 1) = y_tie[i].sample;
  }
  return res;
}

void GeometricTiePoints::print(std::ostream& Os) const
{
  Os << "GeometricTiePoints\n";
  blitz::Array<double, 2> xv = x();
  blitz::Array<double, 2> yv = y();
  for(int i=0; i < xv.rows(); ++i) 
    Os << "   " << i << ": (" << xv(i,0) << ", " << xv(i, 1) << "), ("
       << yv(i, 0) << ", " << yv(i,1) << ")\n";
}
