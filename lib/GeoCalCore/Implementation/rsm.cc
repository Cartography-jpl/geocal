#include "rsm.h"
#include "ostream_pad.h"
#include "geocal_gsl_root.h"
#include "geocal_serialize_support.h"
#include "simple_dem.h"
#include "planet_coordinate.h"
#include <boost/make_shared.hpp>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Rsm::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Rsm);
  ar & GEOCAL_NVP(rp);
  // Older version didn't have rid.
  if(version > 0) {
    ar & GEOCAL_NVP(rid)
      & GEOCAL_NVP(rdcov)
      & GEOCAL_NVP(ricov)
      & GEOCAL_NVP(rparm);
  } else {
    ar & GEOCAL_NVP2("cconv", cconv_not_used);
  }
  boost::serialization::split_member(ar, *this, version);
}

template<class Archive>
void Rsm::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void Rsm::load(Archive & ar, const unsigned int version)
{
  // Older version didn't have rid
  if(version == 0) {
    rid = boost::make_shared<RsmId>(rp, cconv_not_used);
  }
}

GEOCAL_IMPLEMENT(Rsm);
#endif

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

Rsm::Rsm(const boost::shared_ptr<RsmBase>& Rp,
	 const boost::shared_ptr<CoordinateConverter>& Cconv)
: rp(Rp)
{
  rid = boost::make_shared<RsmId>(rp, Cconv);
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
  const GeodeticConverter* gconv = dynamic_cast<const GeodeticConverter*>(coordinate_converter().get());
  const GeodeticRadianConverter* gconv2 = dynamic_cast<const GeodeticRadianConverter*>(coordinate_converter().get());
  const GeodeticRadian2piConverter* gconv3 = dynamic_cast<const GeodeticRadian2piConverter*>(coordinate_converter().get());
  if(sdem && (gconv || gconv2 || gconv3))
    return ground_coordinate(Ic, sdem->h());
  const PlanetSimpleDem* pdem = dynamic_cast<const PlanetSimpleDem*>(&D);
  const PlanetocentricConverter* pconv = dynamic_cast<const PlanetocentricConverter*>(coordinate_converter().get());
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
  return coordinate_converter()->create(*surfp);
}

//-----------------------------------------------------------------------
/// Invert the image_coordinate function to find the ground
/// coordinates at a particular Z value. For the special case that the
/// coordinate converter is a GeodeticConverter or PlanetocentricConverter Z
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
  return coordinate_converter()->convert_from_coordinate(res(0), res(1), Z);
}

//-----------------------------------------------------------------------
/// Return ground coordinate at the height above the reference ellipsoid.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate>
Rsm::ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
{
  const GeodeticConverter* gconv = dynamic_cast<const GeodeticConverter*>(coordinate_converter().get());
  const GeodeticRadianConverter* gconv2 = dynamic_cast<const GeodeticRadianConverter*>(coordinate_converter().get());
  const GeodeticRadian2piConverter* gconv3 = dynamic_cast<const GeodeticRadian2piConverter*>(coordinate_converter().get());
  const PlanetocentricConverter* pconv = dynamic_cast<const PlanetocentricConverter*>(coordinate_converter().get());
  if(gconv || gconv2 || gconv3 || pconv)
    return ground_coordinate(Ic, H);
  if(coordinate_converter()->naif_code() == CoordinateConverter::EARTH_NAIF_CODE) {
    SimpleDem d(H);
    return ground_coordinate(Ic, d);
  }
  PlanetSimpleDem d(H, coordinate_converter()->naif_code());
  return ground_coordinate(Ic, d);
}

//-----------------------------------------------------------------------
/// Return the image coordinates for the given Gc. This can return Nan
/// if we are outside the range of a RsmGrid, indicating we can't
/// calculate the ImageCoordinate.
//-----------------------------------------------------------------------

ImageCoordinate Rsm::image_coordinate(const GroundCoordinate& Gc) const
{
  double x, y, z;
  coordinate_converter()->convert_to_coordinate(Gc, x, y, z);
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
//-----------------------------------------------------------------------

void Rsm::fit(const ImageGroundConnection& Igc, double Min_height,
	      double Max_height)
{
  rp->fit(Igc, *coordinate_converter(), Min_height, Max_height, 0, Igc.number_line(),
	  0, Igc.number_sample());
  fill_in_ground_domain_vertex(Min_height, Max_height);
}

//-----------------------------------------------------------------------
/// Fill in the ground domain vertex information. Note that you don't
/// normally need to call this directly, the "fit" function already
/// does this. But it can be useful in unit testing and perhaps other
/// contexts to directly calculate this.
//-----------------------------------------------------------------------

void Rsm::fill_in_ground_domain_vertex(double Min_height, double Max_height)
{
  int ind = 0;
  for(int hind = 0; hind < 2; ++hind)
    for(int lind = 0; lind < 2; ++lind)
      for(int sind = 0; sind < 2; ++sind, ++ind) {
	rid->ground_domain_vertex()[ind] = ground_coordinate_approx_height
	  (ImageCoordinate(
	      (lind == 0 ? rp->min_line() : rp->max_line()),
	      (sind == 0 ? rp->min_sample() : rp->max_sample())),
	   (hind == 0 ? Min_height : Max_height));
      }
  rid->min_line(rp->min_line());
  rid->max_line(rp->max_line());
  rid->min_sample(rp->min_sample());
  rid->max_sample(rp->max_sample());
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
  Os << "  Rsm ID:\n";
  opad << *rid << "\n";
}

//-----------------------------------------------------------------------
/// After fitting an Igc, it is good to see how accurate the Rsm
/// captures the Igc. This function take an Igc, and fixed height,
/// and generates a regular grid of the "True" line and sample. We
/// then project this to the surface using the Igc, and then use the
/// Rsm to calculate the line sample. If the Rsm is perfect, it would
/// give the same values as "True".
///
/// This returns Nan where we can't calculate this (e.g., Igc fails,
/// or outside of our RsmGrid).
///
/// This function could just be done in python, but we have it in C++
/// for performance. We may want to adjust what we calculate as we get
/// a better feel for how to characterize a Rsm. But this is our
/// initial version of this.
//-----------------------------------------------------------------------
void Rsm::compare_igc
(const ImageGroundConnection& Igc, int Number_line_spacing,
 int Number_sample_spacing, double Height,
 blitz::Array<double, 2>& True_line,
 blitz::Array<double, 2>& True_sample,
 blitz::Array<double, 2>& Calc_line,
 blitz::Array<double, 2>& Calc_sample)
  const
{
  True_line.resize(Number_line_spacing, Number_sample_spacing);
  True_sample.resize(Number_line_spacing, Number_sample_spacing);
  Calc_line.resize(Number_line_spacing, Number_sample_spacing);
  Calc_sample.resize(Number_line_spacing, Number_sample_spacing);
  for(int i = 0; i < Number_line_spacing; ++i) {
    int ln = (int) floor(Igc.number_line() / double(Number_line_spacing) * i);
    for(int j = 0; j < Number_sample_spacing; ++j) {
      int smp = (int) floor(Igc.number_sample() / double(Number_sample_spacing)
			    * j);
      True_line(i,j) = ln;
      True_sample(i,j) = smp;
      try {
	ImageCoordinate iccalc = 
	  image_coordinate(*Igc.ground_coordinate_approx_height(ImageCoordinate(ln,smp), Height));
	Calc_line(i,j) = iccalc.line;
	Calc_sample(i, j) = iccalc.sample;
      } catch(const ImageGroundConnectionFailed&) {
	Calc_line(i,j) = std::numeric_limits<double>::quiet_NaN();
	Calc_sample(i, j) = std::numeric_limits<double>::quiet_NaN();
      }
    }
  }
}
