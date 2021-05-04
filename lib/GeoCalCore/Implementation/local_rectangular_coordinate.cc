#include "local_rectangular_coordinate.h"
#include "geocal_serialize_support.h"
#include "image_ground_connection.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void LocalRcParameter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(LocalRcParameter);
  ar & GEOCAL_NVP(cf_to_rc) & GEOCAL_NVP(cf_offset)
    & GEOCAL_NVP(cf_prototype);
}

template<class Archive>
void LocalRectangularCoordinate::serialize
(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GroundCoordinate)
    & GEOCAL_NVP(parameter)
    & GEOCAL_NVP(position);
}

template<class Archive>
void LocalRcConverter::serialize
(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CoordinateConverter)
    & GEOCAL_NVP(p);
}

GEOCAL_IMPLEMENT(LocalRcParameter);
GEOCAL_IMPLEMENT(LocalRectangularCoordinate);
GEOCAL_IMPLEMENT(LocalRcConverter);
#endif

template<class T> inline T dotb(const blitz::Array<T, 1>& x, 
				const blitz::Array<T, 1>& y)
{
  return sum(x * y);
}

template<class T> inline T normb(const blitz::Array<T, 1>& x)
{
  return std::sqrt(dotb(x, x));
}

template<class T> inline blitz::Array<T, 1> cross2
(const blitz::Array<T, 1>& x,
 const blitz::Array<T, 1>& y)
{
  blitz::Array<T, 1> res(3);
  res(0) = x(1) * y(2) - x(2) * y(1);
  res(1) = x(2) * y(0) - x(0) * y(2);
  res(2) = x(0) * y(1) - x(1) * y(0);
  return res;
}

//-----------------------------------------------------------------------
/// Constructor. This sets up coordinates so the origin is at the
/// given height on the surface for the center pixel of the Igc.
///
/// You can optionally override the line/sample used for the
/// origin. Most of the time you don't want to do this, but this can
/// be useful when generating testing data.
///
/// There are a couple of reasonable ways to create this. The default
/// is to set Z in the look direction, X mostly in the image
/// sample/scan direction, and Y completing the right hand coordinate
/// system. An alternative is to have Y set to be in the image
/// coordinate line/push direction, and X completing the RHCS. Often these
/// are similar, but we may have difference if the image scan line
/// isn't perpendicular to the push direction. There is also a
/// variation where we look local (+- 1 pixel) or over the whole image
/// (+- number_line/number_sample).
//-----------------------------------------------------------------------

LocalRcParameter::LocalRcParameter
(const ImageGroundConnection& Igc, double Height, double Line, double Sample,
 CoordinateCreation C)
{
  ImageCoordinate ic(Igc.number_line() / 2, Igc.number_sample() / 2);
  if(Line >= 0)
    ic.line = Line;
  if(Sample >= 0)
    ic.sample = Sample;
  if(C != FOLLOW_SAMPLE_LOCAL && C != FOLLOW_LINE_LOCAL &&
     C != FOLLOW_SAMPLE_FULL && C != FOLLOW_LINE_FULL)
    throw Exception("Unrecognized CoordinateCreation");
  ImageCoordinate ic_p1;
  ImageCoordinate ic_p2;
  if(C == FOLLOW_SAMPLE_LOCAL) {
    ic_p1 = ImageCoordinate(ic.line, ic.sample);
    ic_p2 = ImageCoordinate(ic.line, ic.sample + 1);
  } else if(C == FOLLOW_LINE_LOCAL) {
    ic_p1 = ImageCoordinate(ic.line, ic.sample);
    ic_p2 = ImageCoordinate(ic.line+1, ic.sample);
  } else if(C == FOLLOW_SAMPLE_FULL) {
    ic_p1 = ImageCoordinate(ic.line, 0);
    ic_p2 = ImageCoordinate(ic.line, Igc.number_sample()-1);
  } else {
    ic_p1 = ImageCoordinate(0, ic.sample);
    ic_p2 = ImageCoordinate(Igc.number_line()-1, ic.sample);
  }
  cf_prototype =
    Igc.ground_coordinate_approx_height(ic, Height)->convert_to_cf();
  boost::array<double, 3> p1 = cf_prototype->position;
  boost::array<double, 3> p2 = Igc.ground_coordinate_approx_height(ic, Height + 100)->convert_to_cf()->position;
  blitz::Array<double, 1> z(3), x(3), y(3);
  z = p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2];
  z /= normb(z);
  cf_offset = p1;
  p1 = Igc.ground_coordinate_approx_height(ic_p1, Height)->convert_to_cf()->position;
  p2 = Igc.ground_coordinate_approx_height(ic_p2, Height)->convert_to_cf()->position;
  if(C == FOLLOW_SAMPLE_LOCAL || C == FOLLOW_SAMPLE_FULL) {
    x = p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2];
    x -= z * dotb(x,z);
    x /= normb(x);
    y = cross2(z, x);
  } else {
    y = p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2];
    y -= z * dotb(y,z);
    y /= normb(y);
    x = cross2(y, z);
  }
  cf_to_rc[0][0]=x(0);
  cf_to_rc[0][1]=x(1);
  cf_to_rc[0][2]=x(2);
  cf_to_rc[1][0]=y(0);
  cf_to_rc[1][1]=y(1);
  cf_to_rc[1][2]=y(2);
  cf_to_rc[2][0]=z(0);
  cf_to_rc[2][1]=z(1);
  cf_to_rc[2][2]=z(2);
}

//-----------------------------------------------------------------------
/// The assumption with the LocalRcParameter is that we create a
/// coordinate system with z along the look direction, x mostly in the
/// sample direction, and y mostly in the line direction. This
/// function actually checks this by calculating the dot product
/// between a unit vector in the look direction, sample direction, and
/// line direction. These dot products should be close to zero, the
/// farther away from 0 the less true our assumption is.
///
/// Note that you can still completely defined LocalRcParameter (as
/// long as none of the directions are exactly parallel), the problem
/// is just that the LocalRcParameter isn't very useful when this
/// assumption doesn't hold
//-----------------------------------------------------------------------

void LocalRcParameter::check_rc_assumption
  (const ImageGroundConnection& Igc, double Height)
{
  ImageCoordinate ic(Igc.number_line() / 2, Igc.number_sample() / 2);
  ImageCoordinate ic_p1s(ic.line, ic.sample + 1);
  ImageCoordinate ic_p1l(ic.line+1, ic.sample);
    
  boost::array<double, 3> p1 = Igc.ground_coordinate_approx_height(ic, Height)->convert_to_cf()->position;
  boost::array<double, 3> p2 = Igc.ground_coordinate_approx_height(ic, Height + 100)->convert_to_cf()->position;
  boost::array<double, 3> p3 = Igc.ground_coordinate_approx_height(ic_p1s, Height)->convert_to_cf()->position;
  boost::array<double, 3> p4 = Igc.ground_coordinate_approx_height(ic_p1l, Height)->convert_to_cf()->position;
  blitz::Array<double, 1> lk_dir(3), smp_dir(3), ln_dir(3);
  lk_dir = p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2];
  lk_dir /= normb(lk_dir);
  smp_dir = p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2];
  smp_dir /= normb(smp_dir);
  ln_dir = p4[0] - p1[0], p4[1] - p1[1], p4[2] - p1[2];
  ln_dir /= normb(ln_dir);
  std::cout << "dot(Look direction, Sample direction): "
	    << dotb(lk_dir, smp_dir) << "\n"
	    << "dot(Look direction, Line direction): "
	    << dotb(lk_dir, ln_dir) << "\n"
	    << "dot(Sample direction, Line direction): "
	    << dotb(smp_dir, ln_dir) << "\n";
}

LocalRectangularCoordinate::LocalRectangularCoordinate
(const boost::shared_ptr<LocalRcParameter>& P,
 const GroundCoordinate& Gc)
  : parameter(P)
{
  // Special case worth checking
  const LocalRectangularCoordinate* lrc = dynamic_cast<const LocalRectangularCoordinate*>(&Gc);
  if(lrc && lrc->parameter.get() == P.get()) {
    position = lrc->position;
    return;
  }
  boost::shared_ptr<CartesianFixed> cf = Gc.convert_to_cf();
  if(cf->naif_code() != naif_code())
    throw Exception("Naif code of Gc doesn't match that of the LocalRcParameter");
  boost::array<double, 3> t;
  t[0] = cf->position[0] - parameter->cf_offset[0];
  t[1] = cf->position[1] - parameter->cf_offset[1];
  t[2] = cf->position[2] - parameter->cf_offset[2];
  mul(parameter->cf_to_rc, t, position);
}

boost::shared_ptr<CartesianFixed>
LocalRectangularCoordinate::convert_to_cf() const
{
  boost::array<double, 3> t;
  mul_t(parameter->cf_to_rc, position, t);
  t[0] += parameter->cf_offset[0];
  t[1] += parameter->cf_offset[1];
  t[2] += parameter->cf_offset[2];
  return parameter->cf_prototype->create(t);
}

ArrayAd<double, 1> LocalRcConverter::convert_to_cf
(const AutoDerivative<double>& X,
 const AutoDerivative<double>& Y,
 const AutoDerivative<double>& Z) const
{
  blitz::firstIndex i1; blitz::secondIndex i2;
  blitz::Array<AutoDerivative<double>, 1> coora(3);
  coora(0) = X; 
  coora(1) = Y; 
  coora(2) = Z;
  blitz::Array<AutoDerivative<double>, 1> res(3);
  res = blitz::sum(p->cf_to_rc_arr()(i2, i1) * coora(i2), i2) +
    p->cf_offset_arr()(i1);
  return ArrayAd<double, 1>(res);
}

void LocalRcConverter::convert_from_cf
(const ArrayAd<double, 1>& Cf,
 AutoDerivative<double>& X,
 AutoDerivative<double>& Y,
 AutoDerivative<double>& Z) const
{
  blitz::firstIndex i1; blitz::secondIndex i2;
  blitz::Array<AutoDerivative<double>, 1> res(3);
  res = blitz::sum(p->cf_to_rc_arr()(i1,i2) * (Cf.to_array()(i2) - p->cf_offset_arr()(i2)), i2);
  X = res(0);
  Y = res(1);
  Z = res(2);
}

bool LocalRcConverter::is_same
(const CoordinateConverter& Conv) const
{
  const LocalRcConverter* t =
    dynamic_cast<const LocalRcConverter*>(&Conv);
  if(!t || t->naif_code() != naif_code())
      return false;
  for(int i = 0; i < 3; ++i) {
    if(fabs(p->cf_offset[i] - t->p->cf_offset[i]) > 1e-6)
      return false;
    for(int j = 0; j < 3; ++j)
      if(fabs(p->cf_to_rc[i][j] - t->p->cf_to_rc[i][j]) > 1e-6)
	return false;
  }
  return true;
}
