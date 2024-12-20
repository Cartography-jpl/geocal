#include "rsm.h"
#include "geocal_matrix.h"
#include "local_rectangular_coordinate.h"
#include "ostream_pad.h"
#include "geocal_gsl_root.h"
#include "geocal_gsl_fit.h"
#include "geocal_serialize_support.h"
#include "simple_dem.h"
#include "planet_coordinate.h"
#include "image_ground_connection.h"
#include <boost/make_shared.hpp>
#include <algorithm>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Rsm::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(Rsm, WithParameter);
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
  if(rparm)
    add_object(rparm);
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
/// Generate data that can be used for fitting. This is useful if we
/// are exploring how many sections to generate, or other
/// logic. Because we use this at the python level, we return a
/// blitz::Array. Each row is a data point, that is line, sample, x,
/// y, z.
///
/// By default we fit all the line/samples in the Igc, but you can
/// select a subset if desired. Errors with the IGC can either be
/// skipped, or case an exception depending on Ignore_igc_error
///
/// We use the same algorithm used in RsmRationalPolynomial::fit.
//-----------------------------------------------------------------------

blitz::Array<double, 2> Rsm::generate_data
(const ImageGroundConnection& Igc, double Min_height, double Max_height,
 int Nline_fit, int Nsample_fit, int Nheight_fit,
 int Nsecond_pass_fit, 
 int Min_line, int Max_line, int Min_sample,
 int Max_sample, bool Ignore_igc_error) const
{
  if(Max_line < 0)
    Max_line = Igc.number_line()-1;
  if(Max_sample < 0)
    Max_sample = Igc.number_sample()-1;
  boost::shared_ptr<CoordinateConverter> cconv = coordinate_converter();
  std::vector<double> line, sample, x, y, z;
  for(int i = 0; i < Nline_fit; ++i)
    for(int j = 0; j < Nsample_fit; ++j)
      for(int k = 0; k < Nheight_fit; ++k) {
	try {
	  double ln = Min_line + (Max_line - Min_line) /
	    (Nline_fit - 1.0) * i;
	  double smp = Min_sample + (Max_sample - Min_sample) /
	    (Nsample_fit - 1.0) * j;
	  double h = Min_height + (Max_height - Min_height) /
	    (Nheight_fit - 1.0) * k;
	  boost::shared_ptr<GroundCoordinate> gc =
	    Igc.ground_coordinate_approx_height(ImageCoordinate(ln, smp), h);
	  line.push_back(ln);
	  sample.push_back(smp);
	  double xv, yv, zv;
	  cconv->convert_to_coordinate(*gc, xv, yv, zv);
	  x.push_back(xv);
	  y.push_back(yv);
	  z.push_back(zv);
	} catch(const ImageGroundConnectionFailed&) {
	  // Ignore failures, just go to next point.
	} catch(...) {
	  if(!Ignore_igc_error)
	    throw;
	}
      }
  if(line.size() == 0)
    throw Exception("Did not get any points for Rsm::generate_data");
  double min_x = *std::min_element(x.begin(), x.end());
  double max_x = *std::max_element(x.begin(), x.end());
  double min_y = *std::min_element(y.begin(), y.end());
  double max_y = *std::max_element(y.begin(), y.end());
  double min_z = *std::min_element(z.begin(), z.end());
  double max_z = *std::max_element(z.begin(), z.end());

  // Go back through, and also make even spacing in x, y, and
  // z. Without this we can get a polynomial that acts badly at the
  // edge of the fitted range
  if(Nsecond_pass_fit > 1) {
    for(int i = 0; i < Nsecond_pass_fit; ++i)
      for(int j = 0; j < Nsecond_pass_fit; ++j)
	for(int k = 0; k < Nsecond_pass_fit; ++k) {
	  double xv = min_x + (max_x - min_x) / (Nsecond_pass_fit-1) * i;
	  double yv = min_y + (max_y - min_y) / (Nsecond_pass_fit-1) * j;
	  double zv = min_z + (max_z - min_z) / (Nsecond_pass_fit-1) * k;
	  boost::shared_ptr<GroundCoordinate> gc =
	    cconv->convert_from_coordinate(xv, yv, zv);
	  ImageCoordinate ic;
	  bool success;
	  try {
	    Igc.image_coordinate_with_status(*gc, ic, success);
	    if(success && ic.line >= Min_line && ic.line <= Max_line &&
	       ic.sample >= Min_sample && ic.sample <= Max_sample) {
	      line.push_back(ic.line);
	      sample.push_back(ic.sample);
	      x.push_back(xv);
	      y.push_back(yv);
	      z.push_back(zv);
	    }
	  } catch(const ImageGroundConnectionFailed&) {
	    // Ignore failures, just go to next point.
	  } catch(...) {
	    if(!Ignore_igc_error)
	      throw;
	  }
	}
  }
  blitz::Array<double, 2> res(line.size(), 5);
  for(int i = 0; i < res.rows(); ++i) {
    res(i,0) = line[i];
    res(i,1) = sample[i];
    res(i,2) = x[i];
    res(i,3) = y[i];
    res(i,4) = z[i];
  }
  return res;
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
    return ground_coordinate_z(Ic, sdem->h());
  const PlanetSimpleDem* pdem = dynamic_cast<const PlanetSimpleDem*>(&D);
  const PlanetocentricConverter* pconv = dynamic_cast<const PlanetocentricConverter*>(coordinate_converter().get());
  if(pdem && pconv)
    return ground_coordinate_z(Ic, sdem->h());

  // We now have more general case.
  const double delta_z = 10.0;
  double z0 = rp->initial_guess_z(Ic.line, Ic.sample);
  boost::shared_ptr<GroundCoordinate> gc1 = ground_coordinate_z(Ic, z0);
  boost::shared_ptr<GroundCoordinate> gc2 =
    ground_coordinate_z(Ic, z0 + delta_z);
  boost::shared_ptr<CartesianFixed> p = gc1->convert_to_cf();
  boost::shared_ptr<CartesianFixed> p2 = gc2->convert_to_cf();
  // Pick sign so stepping along lv gives lower heights
  int sign = 1;
  if(p->height_reference_surface() > p2->height_reference_surface())
    sign = -1;
  CartesianFixedLookVector lv;
  lv.look_vector[0] = sign * (p->position[0] - p2->position[0]);
  lv.look_vector[1] = sign * (p->position[1] - p2->position[1]);
  lv.look_vector[2] = sign * (p->position[2] - p2->position[2]);
  double resolution = 10.0; // Not sure how to get this. The resolution
			    // here is the resolution of the DEM
  try {
    boost::shared_ptr<CartesianFixed> surfp = D.intersect(*p, lv, resolution);
    return polish_intersection(Ic, D, *surfp);
  } catch(const ConvergenceFailure& E) {
    // DEM intersection will sometimes fail, particularly if the RSM
    // doesn't have well defined look vectors. Fall back to a full
    // solution
    return ground_intersection(Ic, D);
  }
}

//-----------------------------------------------------------------------
/// Calculate a ground intersection, without any assumptions about
/// look vectors being on a line. This can only find data in the
/// range of min_z to max_z.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> Rsm::ground_intersection
(const ImageCoordinate& Ic, const Dem& D, double Z_accuracy) const
{
  class Zeq: public DFunctor {
  public:
    Zeq(const Rsm& R, const ImageCoordinate& Ic, const Dem& D)
      : r(R), ic(Ic), d(D)
    {}
    virtual ~Zeq() {}
    virtual double operator()(const double& Z) const
    {
      return d.distance_to_surface(*r.ground_coordinate_z(ic, Z));
    }
    const Rsm& r;
    const ImageCoordinate& ic;
    const Dem& d;
  };
  Zeq eq(*this, Ic, D);
  if(eq(rp->min_z()) * eq(rp->max_z()) > 0)
    throw ConvergenceFailure("Surface is not found between min_z() and max_z()");
  double z = root(eq, rp->min_z(), rp->max_z(), 1e-8, Z_accuracy);
  return ground_coordinate_z(Ic, z);
}

//-----------------------------------------------------------------------
/// The look vectors in a RSM projection are not strictly lines. Once
/// we have a solution for the intersection with a DEM, we can
/// "polish" this to account for the small nonlinearities in the
/// RSM. The surface point should be pretty close to the intersection,
/// we don't account for obscuration in this function. 
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> Rsm::polish_intersection
(const ImageCoordinate& Ic, const Dem& D,
 const GroundCoordinate& Surfp, double Z_accuracy) const
{
  class Zeq: public DFunctor {
  public:
    Zeq(const Rsm& R, const ImageCoordinate& Ic, const Dem& D)
      : r(R), ic(Ic), d(D)
    {}
    virtual ~Zeq() {}
    virtual double operator()(const double& Z) const
    {
      return d.distance_to_surface(*r.ground_coordinate_z(ic, Z));
    }
    const Rsm& r;
    const ImageCoordinate& ic;
    const Dem& d;
  };
  double x, y, z_guess;
  coordinate_converter()->convert_to_coordinate(Surfp, x, y, z_guess);
  Zeq eq(*this, Ic, D);
  double zmin = z_guess;
  double zmax = z_guess;
  double step = 1.0;
  const double tolerance = 1.0; // It can be useful to go outside of
				 // the range allowed by the RSM,
				 // although at a certain point we'll
				 // just need to fail
  while(eq(zmin) * eq(zmax) > 0 &&
	(zmin >= rp->min_z()-tolerance * (rp->max_z() - rp->min_z())
	 || zmax <= rp->max_z()+tolerance * (rp->max_z() - rp->min_z()))) {
    zmin -= step;
    zmax += step;
    step *= 2;
  }
  if(eq(zmin) * eq(zmax) > 0)
    throw ConvergenceFailure("Surface is not found between min_z() and max_z()");
  double z = root(eq, zmin, zmax, 1e-8, Z_accuracy);
  return ground_coordinate_z(Ic, z);
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

boost::shared_ptr<GroundCoordinate> Rsm::ground_coordinate_z
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
      ImageCoordinate icres;
      bool in_valid_range;
      r_.image_coordinate(X(0), X(1), z_, icres, in_valid_range);
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
    return ground_coordinate_z(Ic, H);
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
///
/// Note that the RSM is really only valid for Gc that fall in the
/// range min_x, max_x, min_y, max_y, min_z, max_z in
/// rsm_base, and for the image coordinate in the range min_line,
/// max_line, min_sample, max_sample. Numerically we can often
/// calculate a value outside of this range, and that can be
/// useful (e.g., points just a little outside of the valid range).
///
/// This function will always calculate image coordinates if it
/// can. But depending on the application, callers of this function
/// should check the value of In_valid_range returned. If false, the
/// user may want to treat this as invalid data (e..g, Nan).
//-----------------------------------------------------------------------

void Rsm::image_coordinate(const GroundCoordinate& Gc, ImageCoordinate& Res, bool& In_valid_range) const
{
  double x, y, z;
  coordinate_converter()->convert_to_coordinate(Gc, x, y, z);
  image_coordinate(x, y, z, Res, In_valid_range);
}

//-----------------------------------------------------------------------
/// Variation where the Gc is already in our native coordinate system.
//-----------------------------------------------------------------------

void Rsm::image_coordinate(double X, double Y, double Z, ImageCoordinate& Res,
			   bool& In_valid_range) const
{
  if(rparm) {
    boost::shared_ptr<GroundCoordinate> gc =
      coordinate_converter()->convert_from_coordinate(X, Y, Z);
    boost::shared_ptr<GroundCoordinate> gcadj;
    double lndelta, smpdelta;
    rparm->adjustment(*gc, gcadj, lndelta, smpdelta);
    double xadj, yadj, zadj;
    coordinate_converter()->convert_to_coordinate(*gcadj, xadj, yadj, zadj);
    Res = rp->image_coordinate(xadj, yadj, zadj);
    In_valid_range = (xadj >= rp->min_x() && xadj <= rp->max_x() &&
		      yadj >= rp->min_y() && yadj <= rp->max_y() &&
		      zadj >= rp->min_z() && zadj <= rp->max_z());
    Res.line += lndelta;
    Res.sample += smpdelta;
  } else {
    Res = rp->image_coordinate(X, Y, Z);
    In_valid_range = (X >= rp->min_x() && X <= rp->max_x() &&
		      Y >= rp->min_y() && Y <= rp->max_y() &&
		      Z >= rp->min_z() && Z <= rp->max_z());
  }
  // Already checked that we were in the ground domain, now check that
  // we are in the image domain. Allow a little slop, because round
  // off may take us slightly out of range.
  double tolerance = 0.2;
  if(Res.line < rp->min_line() - tolerance
     || Res.line > rp->max_line() + tolerance
     || Res.sample < rp->min_sample() - tolerance
     || Res.sample > rp->max_sample() + tolerance)
    In_valid_range = false;
}

//-----------------------------------------------------------------------
/// This the jacobian of the line, sample with respect to X, Y, Z.
/// This is a 2x3 matrix.
//-----------------------------------------------------------------------

blitz::Array<double, 2> Rsm::image_coordinate_jacobian
(double X, double Y, double Z) const
{
  if(rparm) {
    boost::shared_ptr<GroundCoordinate> gc =
      coordinate_converter()->convert_from_coordinate(X, Y, Z);
    boost::shared_ptr<GroundCoordinate> gcadj;
    double lndelta, smpdelta;
    rparm->adjustment(*gc, gcadj, lndelta, smpdelta);
    double xadj, yadj, zadj;
    coordinate_converter()->convert_to_coordinate(*gcadj, xadj, yadj, zadj);
    return rp->image_coordinate_jacobian(xadj, yadj, zadj);
  } 
  return rp->image_coordinate_jacobian(X, Y, Z);
}

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// parameters (what we have is RsmAdjustableParameter object)
//-----------------------------------------------------------------------

blitz::Array<double, 2> 
Rsm::image_coordinate_jac_parm(const GroundCoordinate& Gc) const
{
  if(!rparm)
    return blitz::Array<double, 2>(2, 0);
  blitz::Range ra = blitz::Range::all();
  blitz::firstIndex i1; blitz::secondIndex i2; blitz::thirdIndex i3;
  // Currently only handle LocalRcConverter if we need to include
  // ground coordinate parameters.
  boost::shared_ptr<LocalRcConverter> gconv =
    boost::dynamic_pointer_cast<LocalRcConverter>(coordinate_converter());
  if(!gconv && rparm->has_ground_coordinate_parameter())
    throw Exception("Currently we can only calculate image_coordinate jacobians for adjustments including ground coordinates if we are using LocalRectangularCoordinate as our RSM coordinate system.");
  
  ArrayAd<double, 1> poriginal = rparm->parameter_with_derivative();
  blitz::Array<double, 2> jac(2, poriginal.rows());
  const_cast<RsmAdjustableParameter&>(*rparm).add_identity_gradient();
  ArrayAd<double, 1> gcadj;
  AutoDerivative<double> lndelta, smpdelta;
  rparm->adjustment_with_derivative(Gc, gcadj, lndelta, smpdelta);
  if(rparm->has_ground_coordinate_parameter()) {
    blitz::Array<AutoDerivative<double>, 1> x(3);
    gconv->convert_from_cf(gcadj, x(0), x(1), x(2));
    blitz::Array<double, 2> dx_dp = ArrayAd<double, 1>(x).jacobian();
    blitz::Array<double, 2> dic_dx =
      rp->image_coordinate_jacobian(x(0).value(), x(1).value(), x(2).value());
    jac = blitz::sum(dic_dx(i1, i3) * dx_dp(i3, i2), i3);
  } else {
    jac = 0;
  }
  if(jac.cols() > 0) {
    jac(0, ra) += lndelta.gradient();
    jac(1, ra) += smpdelta.gradient();
  }
  const_cast<RsmAdjustableParameter&>(*rparm).parameter_with_derivative(poriginal);
  return jac;
}

//-----------------------------------------------------------------------
/// Generate a Rsm that approximates the calculation
/// done by a ImageGroundConnection.
//-----------------------------------------------------------------------

void Rsm::fit(const ImageGroundConnection& Igc, double Min_height,
	      double Max_height)
{
  rp->fit(Igc, *coordinate_converter(), Min_height, Max_height, 0,
	  Igc.number_line() - 1, 0, Igc.number_sample() - 1);
  fill_in_ground_domain_vertex(Igc, Min_height, Max_height);
}

//-----------------------------------------------------------------------
/// Variation of fit that takes data generated by generate_data. This
/// is intended for use with adaptive strategies such as varying the
/// number of sections to meet a certain tolerance. Note that this
/// does *not* call fill_in_ground_domain_vertex (since we don't have
/// the Igc in this call). You should call
/// fill_in_ground_domain_vertex separately once you are happy with
/// the fit.
//-----------------------------------------------------------------------

void Rsm::fit_data(const blitz::Array<double, 2>& Data,
		   int Nline, int Nsamp) const
{
  rp->fit_data(Data, 0, Nline-1, 0, Nsamp-1);
}

//-----------------------------------------------------------------------
/// Fill in the ground domain vertex information. Note that you don't
/// normally need to call this directly, the "fit" function already
/// does this. But it can be useful in unit testing and perhaps other
/// contexts to directly calculate this. Also for fit_data, you *have*
/// to call this function separately since it doesn't call this function.
///
/// There is specific requirements about the ordering the vertices (see
/// the RSM documentation). We make sure the data is given in this
/// order.
///
/// Note we use to generate this from the RSM itself, but for some
/// types the very edges used by ground domain fails. Since we are
/// usually doing this in a context of a fit anyways, we go ahead and
/// just use the original Igc to calculate this.
//-----------------------------------------------------------------------

void Rsm::fill_in_ground_domain_vertex(const ImageGroundConnection& Igc,
				       double Min_height, double Max_height)
{
  // Turns out the vertex really needs to be at constant z
  // values. This isn't actually required, but is effectively required
  // by the constraints on the vertex relationships. So calculate the
  // min and max z values first.
  std::vector<ImageCoordinate> iclist;
  iclist.push_back(ImageCoordinate(rp->min_line(), rp->min_sample()));
  iclist.push_back(ImageCoordinate(rp->max_line(), rp->min_sample()));
  iclist.push_back(ImageCoordinate(rp->max_line(), rp->max_sample()));
  iclist.push_back(ImageCoordinate(rp->min_line(), rp->max_sample()));
  double min_z=0;
  double max_z=0;
  for(int hind = 0; hind < 2; ++hind)
    for(int i = 0; i < 4; ++i) {
      double p[3];
      auto gc = Igc.ground_coordinate_approx_height(
		   iclist[i],
		   (hind == 0 ? Min_height : Max_height));
      coordinate_converter()->convert_to_coordinate(*gc, p[0], p[1], p[2]);
      if(hind == 0 && i == 0) {
	min_z = p[2];
	max_z = p[2];
      }
      min_z = std::min(min_z, p[2]);
      max_z = std::max(max_z, p[2]);
    }
  if(min_z < rsm_base()->min_z())
    min_z = rsm_base()->min_z();
  if(max_z > rsm_base()->max_z())
    max_z = rsm_base()->max_z();
  boost::shared_ptr<Dem> dem_min_z, dem_max_z;
  boost::shared_ptr<LocalRcConverter> lconv =
    boost::dynamic_pointer_cast<LocalRcConverter>(coordinate_converter());
  if(lconv) {
    dem_min_z = boost::make_shared<LocalZDem>(lconv, min_z);
    dem_max_z = boost::make_shared<LocalZDem>(lconv, max_z);
  } else if(coordinate_converter()->naif_code() == CoordinateConverter::EARTH_NAIF_CODE) {
    dem_min_z = boost::make_shared<SimpleDem>(min_z);
    dem_max_z = boost::make_shared<SimpleDem>(max_z);
  } else {
    dem_min_z = boost::make_shared<PlanetSimpleDem>(min_z,
			    coordinate_converter()->naif_code());
    dem_max_z = boost::make_shared<PlanetSimpleDem>(max_z,
			    coordinate_converter()->naif_code());
  }
  double bottom_pt[4][3];
  double top_pt[4][3];
  std::deque<int> ptind;
  for(int i = 0; i < 4; ++i) {
    ptind.push_back(i);
    auto gcmin = Igc.ground_coordinate_dem(iclist[i], *dem_min_z);
    auto gcmax = Igc.ground_coordinate_dem(iclist[i], *dem_max_z);
    coordinate_converter()->convert_to_coordinate(*gcmin, bottom_pt[i][0],
				  bottom_pt[i][1], bottom_pt[i][2]);
    coordinate_converter()->convert_to_coordinate(*gcmax, top_pt[i][0],
				  top_pt[i][1], top_pt[i][2]);
  }
  // Check if linear ring is in a clockwise direction
  boost::array<double, 3> t;
  boost::array<double, 3> x1,x2;
  for(int i = 0; i < 3; ++i) {
    x1[i] = bottom_pt[1][i]-bottom_pt[0][i];
    x2[i] = bottom_pt[3][i]-bottom_pt[0][i];
  }
  cross(x1,x2,t);
  bool is_clockwise = (t[2] < 0);
  // If not clockwise, swap corners so it is
  if(!is_clockwise)
    std::swap(ptind[1], ptind[3]);
  // Shift the linear ring until the first element is the lower left corner.
  int i = 0;
  while(bottom_pt[ptind[0]][1] > bottom_pt[ptind[1]][1] ||
	bottom_pt[ptind[0]][0] > bottom_pt[ptind[3]][0]) {
    ptind.push_back(ptind[0]);
    ptind.pop_front();
    // Might be some weird pathological case where condition never met
    // (e.g., we have NaN or something like that).
    if(++i > 4) {
      // Fall back to just using the bounding cube
      fill_in_ground_domain_vertex();
      return;
    }
  }
  // Save points. Note that these don't go in a linear ring order, just
  // by convention 0 is lower left corner, 1 is lower right, 2 upper left,
  // 3 is upper right.
  int ptind2[4];
  ptind2[0] = ptind[0];
  ptind2[1] = ptind[3];
  ptind2[2] = ptind[1];
  ptind2[3] = ptind[2];
  for(int i = 0; i < 4; i++) {
    rid->ground_domain_vertex()[i] = coordinate_converter()->
      convert_from_coordinate(bottom_pt[ptind2[i]][0], bottom_pt[ptind2[i]][1],
			      bottom_pt[ptind2[i]][2]);
    if(false)
      std::cerr << "(" << bottom_pt[ptind2[i]][0] << ", "
		<< bottom_pt[ptind2[i]][1] << ", "
		<< bottom_pt[ptind2[i]][2] << ")\n";
    rid->ground_domain_vertex()[i+4] = coordinate_converter()->
      convert_from_coordinate(top_pt[ptind2[i]][0], top_pt[ptind2[i]][1],
			      top_pt[ptind2[i]][2]);
  }
  rid->full_number_line(rp->max_line());
  rid->full_number_sample(rp->max_sample());
  rid->min_line(rp->min_line());
  rid->max_line(rp->max_line());
  rid->min_sample(rp->min_sample());
  rid->max_sample(rp->max_sample());
  
  // Check that we actually contain our data in the ground domain. We can get
  // IGCs where odd things happen where this isn't the case. We then
  // fall back to the bounding cube.
  //fill_in_ground_domain_vertex();
  std::vector<ImageCoordinate> iclist2;
  iclist2.push_back(ImageCoordinate(rp->min_line()+0.5, rp->min_sample()+0.5));
  iclist2.push_back(ImageCoordinate(rp->max_line()-0.5, rp->min_sample()+0.5));
  iclist.push_back(ImageCoordinate(rp->max_line()-0.5, rp->max_sample()-0.5));
  iclist.push_back(ImageCoordinate(rp->min_line()+0.5, rp->max_sample()-0.5));
  int nstep = 10;
  double h_step = (Max_height - Min_height) / (nstep - 1);
  for(int hi = 0; hi < nstep; ++hi) {
    double h = Min_height + h_step * hi;
    for(int i = 0; i < 4; ++i) {
      auto gc = Igc.ground_coordinate_approx_height(iclist[i], h);
      if(!rid->in_ground_domain(*gc)) {
	if(false)
	  std::cerr << "Point not in ground_domain."
		    << "i: " << i << "\n"
		    << "hi: " << hi << "\n"
		    << "x: " << rp->min_x() << " to " << rp->max_x() << "\n"
		    << "y: " << rp->min_y() << " to " << rp->max_y() << "\n"
		    << "z: " << rp->min_z() << " to " << rp->max_z() << "\n";
	fill_in_ground_domain_vertex();
	return;
      }
    }
  }
}


//-----------------------------------------------------------------------
/// Variation of fill_in_ground_domain_vertex that just uses the
/// bounding cube of the RSM. This is less exact than using the igc to
/// calculate a smaller region, but we sometimes have odd Igcs that
/// aren't properly captured in the fill_in_ground_domain_vertex.
//-----------------------------------------------------------------------

void Rsm::fill_in_ground_domain_vertex()
{
  // Save points. Note that these don't go in a linear ring order, just
  // by convention 0 is lower left corner, 1 is lower right, 2 upper left,
  // 3 is upper right.
  rid->ground_domain_vertex()[0] = coordinate_converter()->
    convert_from_coordinate(rp->min_x(), rp->min_y(), rp->min_z());
  rid->ground_domain_vertex()[1] = coordinate_converter()->
    convert_from_coordinate(rp->max_x(), rp->min_y(), rp->min_z());
  rid->ground_domain_vertex()[2] = coordinate_converter()->
    convert_from_coordinate(rp->min_x(), rp->max_y(), rp->min_z());
  rid->ground_domain_vertex()[3] = coordinate_converter()->
    convert_from_coordinate(rp->max_x(), rp->max_y(), rp->min_z());
  rid->ground_domain_vertex()[4] = coordinate_converter()->
    convert_from_coordinate(rp->min_x(), rp->min_y(), rp->max_z());
  rid->ground_domain_vertex()[5] = coordinate_converter()->
    convert_from_coordinate(rp->max_x(), rp->min_y(), rp->max_z());
  rid->ground_domain_vertex()[6] = coordinate_converter()->
    convert_from_coordinate(rp->min_x(), rp->max_y(), rp->max_z());
  rid->ground_domain_vertex()[7] = coordinate_converter()->
    convert_from_coordinate(rp->max_x(), rp->max_y(), rp->max_z());
  rid->full_number_line(rp->max_line());
  rid->full_number_sample(rp->max_sample());
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
  Os << "  Rsm Adjustable Parameter:\n";
  if(!rparm)
    opad << "None" << "\n"
	 << "\n";
  else
    opad << *rparm << "\n";
  opad.strict_sync();
  Os << "  Rsm Direct Covariance:\n";
  if(!rdcov)
    opad << "None" << "\n"
	 << "\n";
  else
    opad << *rdcov << "\n";
  opad.strict_sync();
  Os << "  Rsm Indirect Covariance:\n";
  if(!ricov)
    opad << "None" << "\n"
	 << "\n";
  else
    opad << *ricov << "\n";
  opad.strict_sync();
  Os << "  Rsm ID:\n";
  opad << *rid << "\n";
  opad.strict_sync();
}

//-----------------------------------------------------------------------
/// After fitting, you can compare data as generated by generate_data
/// against the rsm calculation. This return the difference in line and
/// sample (true - calculated). A row is a nan if the x,y,z is out
/// of range.
//-----------------------------------------------------------------------

blitz::Array<double, 2> Rsm::compare_data(const blitz::Array<double, 2>& Data)
  const
{
  blitz::Array<double, 2> res(Data.rows(), 2);
  for(int i = 0; i < res.rows(); ++i) {
    ImageCoordinate ic;
    bool in_valid_range;
    image_coordinate(Data(i,2), Data(i,3), Data(i,4), ic, in_valid_range);
    if(!in_valid_range) {
      res(i,0) = std::numeric_limits<double>::quiet_NaN();
      res(i,1) = std::numeric_limits<double>::quiet_NaN();
    } else {
      res(i,0) = Data(i,0) - ic.line;
      res(i,1) = Data(i,1) - ic.sample;
    }
  }
  return res;
}

//-----------------------------------------------------------------------
/// Like compare_data, but goes the other way and give a distance in meters
//-----------------------------------------------------------------------

blitz::Array<double, 1>
Rsm::compare_data_dist(const blitz::Array<double, 2>& Data) const
{
  blitz::Array<double, 1> res(Data.rows());
  boost::shared_ptr<CoordinateConverter> cconv = coordinate_converter();
  
  for(int i = 0; i < res.rows(); ++i) {
    try {
      ImageCoordinate ic(Data(i,0), Data(i,1));
      boost::shared_ptr<GroundCoordinate> gc = ground_coordinate_z(ic, Data(i,4));
      boost::shared_ptr<GroundCoordinate> gc2 =
	cconv->convert_from_coordinate(Data(i,2), Data(i,3), Data(i,4));
      res(i) = distance(*gc, *gc2);
    } catch(const ConvergenceFailure&) {
      res(i) = std::numeric_limits<double>::quiet_NaN();
    }
  }
  return res;
}

//-----------------------------------------------------------------------
/// After fitting an Igc, it is good to see how accurate the Rsm
/// captures the Igc. This function take an Igc, and fixed height,
/// and generates a regular grid of the "True" line and sample. We
/// then project this to the surface using the Igc, and then use the
/// Rsm to calculate the line sample. If the Rsm is perfect, it would
/// give the same values as "True".
///
/// The line/sample differences can sometimes be misleading, so we
/// also include ground distance in meters. Sometimes a large line
/// difference is actually a short distance on the ground, so isn't as
/// bad as it may seem.
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
 blitz::Array<double, 2>& Calc_sample,
 blitz::Array<double, 2>& Distance_true_vs_calc)
  const
{
  True_line.resize(Number_line_spacing, Number_sample_spacing);
  True_sample.resize(Number_line_spacing, Number_sample_spacing);
  Calc_line.resize(Number_line_spacing, Number_sample_spacing);
  Calc_sample.resize(Number_line_spacing, Number_sample_spacing);
  Distance_true_vs_calc.resize(Number_line_spacing, Number_sample_spacing);
  for(int i = 0; i < Number_line_spacing; ++i) {
    int ln = (int) floor(Igc.number_line() / double(Number_line_spacing) * i);
    for(int j = 0; j < Number_sample_spacing; ++j) {
      int smp = (int) floor(Igc.number_sample() / double(Number_sample_spacing)
			    * j);
      True_line(i,j) = ln;
      True_sample(i,j) = smp;
      try {
	ImageCoordinate iccalc;
	bool in_valid_range;
	auto gp_true = Igc.ground_coordinate_approx_height(ImageCoordinate(ln, smp), Height);
	image_coordinate(*gp_true, iccalc, in_valid_range);
	Distance_true_vs_calc(i, j) = std::numeric_limits<double>::quiet_NaN();
	if(in_valid_range) {
	  Calc_line(i,j) = iccalc.line;
	  Calc_sample(i, j) = iccalc.sample;
	  if(iccalc.line >=0 && iccalc.line < Igc.number_line() &&
	     iccalc.sample >= 0 && iccalc.sample < Igc.number_sample()) {
	    try {
	      auto gp_calc = Igc.ground_coordinate_approx_height(iccalc, Height);
	      Distance_true_vs_calc(i, j) = distance(*gp_true, *gp_calc);
	    } catch(const ImageGroundConnectionFailed&) {
	      // Ignore failure, we just don't update
	      // Distance_true_vs_calc
	    }
	  }
	} else {
	  Calc_line(i,j) = std::numeric_limits<double>::quiet_NaN();
	  Calc_sample(i, j) = std::numeric_limits<double>::quiet_NaN();
	}
      } catch(const ImageGroundConnectionFailed&) {
	Calc_line(i,j) = std::numeric_limits<double>::quiet_NaN();
	Calc_sample(i, j) = std::numeric_limits<double>::quiet_NaN();
      }
    }
  }
}

//-----------------------------------------------------------------------
/// Generate mapping matrix. This could be done in python, but there
/// is a bit of looping here that is much faster in C++.
//-----------------------------------------------------------------------

blitz::Array<double, 2> Rsm::mapping_matrix
(const ImageGroundConnection& Igc,
 double Min_height, double Max_height,
 int Nline_fit,
 int Nsample_fit,
 int Nheight_fit,
 bool Ignore_igc_error_in_fit)
{
  blitz::Range ra = blitz::Range::all();
  blitz::firstIndex i1;
  blitz::secondIndex i2;
  blitz::thirdIndex i3;
  blitz::fourthIndex i4;
  std::vector<blitz::Array<double, 2> > igc_jacv;
  std::vector<blitz::Array<double, 2> > rsm_jacv;
  for(int i = 0; i < Nline_fit; ++i)
    for(int j = 0; j < Nsample_fit; ++j)
      for(int k = 0; k < Nheight_fit; ++k) {
	try {
	  double ln = Igc.number_line() / (Nline_fit - 1.0) * i;
	  double smp = Igc.number_sample() / (Nsample_fit - 1.0) * j;
	  // Round off can take ln/smp slightly out of range.
	  if(ln >= Igc.number_line())
	    ln = Igc.number_line() - 0.01;
	  if(smp >= Igc.number_sample())
	    smp = Igc.number_sample() - 0.01;
	  double h = Min_height + (Max_height - Min_height) /
	    (Nheight_fit - 1.0) * k;
	  if(h > Max_height)
	    h = Max_height - 0.01;
	  boost::shared_ptr<GroundCoordinate> gc =
	    Igc.ground_coordinate_approx_height(ImageCoordinate(ln, smp), h);
	  Array<double, 2> a1(Igc.image_coordinate_jac_parm(*gc));
	  Array<double, 2> a2(image_coordinate_jac_parm(*gc));
	  if(a1.cols() == 0)
	    throw Exception("Need to have a nonempty jacobian for Igc");
	  if(a2.cols() == 0)
	    throw Exception("Need to have a nonempty jacobian image_coordinate_jac_parm. Did you forget and rsm_adjustable_parameter?");
	  igc_jacv.push_back(a1.copy());
	  rsm_jacv.push_back(a2.copy());
	} catch(const ImageGroundConnectionFailed&) {
	  // Ignore failures, just go to next point.
	} catch(...) {
	  if(!Ignore_igc_error_in_fit)
	    throw;
	}
      }
  blitz::Array<double, 2> igc_jac(2 * int(igc_jacv.size()),
				  igc_jacv[0].cols());
  blitz::Array<double, 2> rsm_jac(2 * int(rsm_jacv.size()),
				  rsm_jacv[0].cols());
  for(int i = 0; i < int(igc_jacv.size()); ++i) {
    igc_jac(2 * i, ra) = igc_jacv[i](0, ra);
    igc_jac(2 * i+1, ra) = igc_jacv[i](1, ra);
    rsm_jac(2 * i, ra) = rsm_jacv[i](0, ra);
    rsm_jac(2 * i+1, ra) = rsm_jacv[i](1, ra);
  }
  blitz::Array<double, 2> t(rsm_jac.cols(), rsm_jac.cols());
  t = blitz::sum(rsm_jac(i3, i1) * rsm_jac(i3, i2), i3);
  blitz::Array<double, 2> tinv = gsl_invert(t);
  blitz::Array<double, 2> res(rsm_jac.cols(), igc_jac.cols());
  res = sum(sum(tinv(i1, i3) * rsm_jac(i4,i3) * igc_jac(i4, i2), i4), i3);
  return res;
}
