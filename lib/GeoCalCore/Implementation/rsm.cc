#include "rsm.h"
#include "ostream_pad.h"
#include "geocal_gsl_root.h"
#include "geocal_serialize_support.h"
#include "simple_dem.h"
#include "planet_coordinate.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Rsm::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Rsm);
  ar & GEOCAL_NVP(rp)
    & GEOCAL_NVP(cconv);
}

GEOCAL_IMPLEMENT(Rsm);
#endif

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

Rsm::Rsm(const boost::shared_ptr<RsmBase>& Rp,
	 const boost::shared_ptr<CoordinateConverter>& Cconv)
: rp(Rp), cconv(Cconv)
{
}

//-----------------------------------------------------------------------
/// Invert the image_coordinate function to find the ground coordinate
/// that maps to a given ImageCoordinate. 
///
/// This routine may fail to find a solution, in which case a 
/// ConvergenceFailure exception will be thrown.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> Rsm::ground_coordinate
(const ImageCoordinate& Ic, const Dem& D) const
{
  // This is a common enough special case to treat specially:
  const SimpleDem* sdem = dynamic_cast<const SimpleDem*>(&D);
  const GeodeticConverter* gconv = dynamic_cast<const GeodeticConverter*>(cconv.get());
  if(sdem && gconv)
    return ground_coordinate(Ic, sdem->h());
  const PlanetSimpleDem* pdem = dynamic_cast<const PlanetSimpleDem*>(&D);
  const PlanetocentricConverter* pconv = dynamic_cast<const PlanetocentricConverter*>(cconv.get());
  if(pdem && pconv)
    return ground_coordinate(Ic, sdem->h());

  // We now have more general case.
  const double delta_z = 10.0;
  double z0 = rp->initial_guess_z(Ic.line, Ic.sample);
  boost::shared_ptr<GroundCoordinate> gc1 = ground_coordinate(Ic, z0);
  boost::shared_ptr<GroundCoordinate> gc2 =
    ground_coordinate(Ic, z0 + delta_z);
  boost::shared_ptr<CartesianFixed> p = gc1->convert_to_cf();
  boost::shared_ptr<CartesianFixed> p2 = gc2->convert_to_cf();
  CartesianFixedLookVector lv;
  lv.look_vector[0] = p->position[0] - p2->position[0];
  lv.look_vector[1] = p->position[1] - p2->position[1];
  lv.look_vector[2] = p->position[2] - p2->position[2];
  double resolution = 1.0;
  boost::shared_ptr<CartesianFixed> surfp = D.intersect(*p, lv, resolution);
  return cconv->create(*surfp);
}

//-----------------------------------------------------------------------
/// Invert the image_coordinate function to find the ground
/// coordinates at a particular Z value. For the special case that the
/// cconv is a GeodeticConverter or PlanetocentricConverter Z
/// corresponds to height.
///
/// This routine may fail to find a solution, in which case a 
/// ConvergenceFailure exception will be thrown.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> Rsm::ground_coordinate
(const ImageCoordinate& Ic, double Z) const
{
//-----------------------------------------------------------------------
// Class describing RsmEquation. This just takes the X and Y
// in, using the fixed Z, and then
// calculate image coordinates using the Rsm. We then return the
// difference between this and the desired ImageCoordinates.
//-----------------------------------------------------------------------

  class RsmEq : public VFunctorWithDerivative {
  public:
    RsmEq(const Rsm& R, const ImageCoordinate& Ic, double Z)
      : r_(R), ic_(Ic), z_(Z) {}
    virtual ~RsmEq() {}
    virtual blitz::Array<double, 1> operator()(const 
      blitz::Array<double, 1>& X) const {
      ImageCoordinate icres = r_.image_coordinate(X(0), X(1), z_);
      blitz::Array<double, 1> res(2);
      res(0) = icres.line - ic_.line;
      res(1) = icres.sample - ic_.sample;
      return res;
    }
    virtual blitz::Array<double, 2> df
    (const blitz::Array<double, 1>& X) const {
      blitz::Array<double, 2> jac =
	r_.image_coordinate_jacobian(X(0), X(1), z_);
      blitz::Array<double, 2> res(jac(Range::all(), Range(0,1)));
      return res;
    }
  private:
    const Rsm& r_;
    ImageCoordinate ic_;
    double z_;
  };

//-----------------------------------------------------------------------
// Solve the equation to get the latitude and longitude. We only look
// for a 0.01 pixel accuracy.
//-----------------------------------------------------------------------

  RsmEq eq(*this, Ic, Z);

// Initial guess comes from lower level class.
  blitz::Array<double, 1> xint(2);
  rp->initial_guess(Ic.line, Ic.sample, Z, xint(0), xint(1));
  blitz::Array<double, 1> res = gsl_root(eq, xint, 0.1);
  return cconv->convert_from_coordinate(res(0), res(1), Z);
}

//-----------------------------------------------------------------------
/// Return ground coordinate at the height above the reference ellipsoid.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate>
Rsm::ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
{
  const GeodeticConverter* gconv = dynamic_cast<const GeodeticConverter*>(cconv.get());
  const PlanetocentricConverter* pconv = dynamic_cast<const PlanetocentricConverter*>(cconv.get());
  if(gconv || pconv)
    return ground_coordinate(Ic, H);
  if(cconv->naif_code() == CoordinateConverter::EARTH_NAIF_CODE) {
    SimpleDem d(H);
    return ground_coordinate(Ic, d);
  }
  PlanetSimpleDem d(H, cconv->naif_code());
  return ground_coordinate(Ic, d);
}

//-----------------------------------------------------------------------
/// Return the image coordinates for the given Gc.
//-----------------------------------------------------------------------

ImageCoordinate Rsm::image_coordinate(const GroundCoordinate& Gc) const
{
  double x, y, z;
  cconv->convert_to_coordinate(Gc, x, y, z);
  return image_coordinate(x, y, z);
}

//-----------------------------------------------------------------------
/// Variation where the Gc is already in our native coordinate system.
//-----------------------------------------------------------------------

ImageCoordinate Rsm::image_coordinate(double X, double Y, double Z) const
{
  return rp->image_coordinate(X, Y, Z);
}

//-----------------------------------------------------------------------
/// This the jacobian of the line, sample with respect to X, Y, Z.
/// This is a 2x3 matrix.
//-----------------------------------------------------------------------

blitz::Array<double, 2> Rsm::image_coordinate_jacobian
(double X, double Y, double Z) const
{
  return rp->image_coordinate_jacobian(X, Y, Z);
}

//-----------------------------------------------------------------------
/// Generate a Rsm that approximates the calculation
/// done by a ImageGroundConnection.
///
/// This routine always ignores ImageGroundConnectionFailed
/// exceptions, and just skips to the next point. But if we are using
/// python code for the ImageGroundConnection we can't translate
/// errors to ImageGroundConnectionFailed (this is a limitation of
/// SWIG). So you can optionally specify Ignore_error as true, in
/// which case we ignore *all* exceptions and just skip to the next
/// point.
///
/// We normally look at all image points when generating the Rsm. You
/// can optionally specify Skip_masked_point to skip all image points
/// that are masked.
///
/// The Nline, Nsample, Nheight is used for any RsmRationalPolynomial
/// we fit. A RsmGrid uses the size of the grid to determine how many
/// points it needs to calculate.
//-----------------------------------------------------------------------

void Rsm::fit(const ImageGroundConnection& Igc, double Min_height,
	      double Max_height,
	      int Nline, int Nsample, int Nheight,
	      bool Skip_masked_point,
	      bool Ignore_error)
{
  rp->fit(Igc, *cconv, Min_height, Max_height, 0, Igc.number_line(),
	  0, Igc.number_sample(), Nline, Nsample, Nheight, Skip_masked_point,
	  Ignore_error);
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void Rsm::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "Rsm:\n"
     << "  Rational Polynomial:\n";
  opad << *rp << "\n";
  opad.strict_sync();
  Os << "  Coordinate Converter:\n";
  opad << *cconv << "\n";
}
