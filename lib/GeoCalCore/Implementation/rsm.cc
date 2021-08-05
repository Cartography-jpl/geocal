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
  CartesianFixedLookVector lv;
  lv.look_vector[0] = p->position[0] - p2->position[0];
  lv.look_vector[1] = p->position[1] - p2->position[1];
  lv.look_vector[2] = p->position[2] - p2->position[2];
  double resolution = 1.0;
  boost::shared_ptr<CartesianFixed> surfp = D.intersect(*p, lv, resolution);
  return polish_intersection(Ic, D, *surfp);
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
  while(eq(zmin) > 0) {
    zmin -= step;
    step *= 2;
  }
  step = 1.0;
  while(eq(zmax) < 0) {
    zmax += step;
    step *= 2;
  };
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
      //      std::cerr << "X: " << X  << "  Res: " << res << "\n";
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
/// rsm_base. Numerically we can often calculate a value outside of
/// this range, and that can be useful (e.g., points just a little
/// outside of the valid range).
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
  // Allow a little slop in the line/sample calculated, so value like
  // -0.0001 don't get called out of range.
  //const double tol = 0.01;
  if(rparm) {
    boost::shared_ptr<GroundCoordinate> gc =
      coordinate_converter()->convert_from_coordinate(X, Y, Z);
    boost::shared_ptr<GroundCoordinate> gcadj;
    double lndelta, smpdelta;
    rparm->adjustment(*gc, gcadj, lndelta, smpdelta);
    double xadj, yadj, zadj;
    coordinate_converter()->convert_to_coordinate(*gcadj, xadj, yadj, zadj);
    Res = rp->image_coordinate(xadj, yadj, zadj);
    // Think we want to skip checking line/sample range. Seems to
    // cause failures that aren't really real, so we'll just check the
    // X,Y,Z range. We may want to come back to this at some point.
    // In_valid_range = (xadj >= rp->min_x() && xadj <= rp->max_x() &&
    // 		      yadj >= rp->min_y() && yadj <= rp->max_y() &&
    // 		      zadj >= rp->min_z() && zadj <= rp->max_z() &&
    // 		      Res.line >= rp->min_line() - tol &&
    // 		      Res.line <= rp->max_line() + tol &&
    // 		      Res.sample >= rp->min_sample() - tol &&
    // 		      Res.sample <= rp->max_sample() + tol);
    In_valid_range = (xadj >= rp->min_x() && xadj <= rp->max_x() &&
		      yadj >= rp->min_y() && yadj <= rp->max_y() &&
		      zadj >= rp->min_z() && zadj <= rp->max_z());
    Res.line += lndelta;
    Res.sample += smpdelta;
  } else {
    Res = rp->image_coordinate(X, Y, Z);
    // Think we want to skip checking line/sample range. Seems to
    // cause failures that aren't really real, so we'll just check the
    // X,Y,Z range. We may want to come back to this at some point.
    // In_valid_range = (X >= rp->min_x() && X <= rp->max_x() &&
    // 		      Y >= rp->min_y() && Y <= rp->max_y() &&
    // 		      Z >= rp->min_z() && Z <= rp->max_z() &&
    // 		      Res.line >= rp->min_line() - tol &&
    // 		      Res.line <= rp->max_line() + tol &&
    // 		      Res.sample >= rp->min_sample() - tol &&
    // 		      Res.sample <= rp->max_sample() + tol);
    In_valid_range = (X >= rp->min_x() && X <= rp->max_x() &&
		      Y >= rp->min_y() && Y <= rp->max_y() &&
		      Z >= rp->min_z() && Z <= rp->max_z());
  }
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
/// Fill in the ground domain vertex information. Note that you don't
/// normally need to call this directly, the "fit" function already
/// does this. But it can be useful in unit testing and perhaps other
/// contexts to directly calculate this.
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
  for(int hind = 0; hind < 2; ++hind) {
    std::deque<boost::array<double, 3> > pts;
    boost::shared_ptr<GroundCoordinate> gc;
    boost::array<double,3> p;
    gc = Igc.ground_coordinate_approx_height(
		   ImageCoordinate(rp->min_line(), rp->min_sample()),
		   (hind == 0 ? Min_height : Max_height));
    coordinate_converter()->convert_to_coordinate(*gc, p[0], p[1], p[2]);
    pts.push_back(p);
    gc = Igc.ground_coordinate_approx_height(
		   ImageCoordinate(rp->max_line(), rp->min_sample()),
		   (hind == 0 ? Min_height : Max_height));
    coordinate_converter()->convert_to_coordinate(*gc, p[0], p[1], p[2]);
    pts.push_back(p);
    gc = Igc.ground_coordinate_approx_height(
		   ImageCoordinate(rp->max_line(), rp->max_sample()),
		   (hind == 0 ? Min_height : Max_height));
    coordinate_converter()->convert_to_coordinate(*gc, p[0], p[1], p[2]);
    pts.push_back(p);
    gc = Igc.ground_coordinate_approx_height(
		   ImageCoordinate(rp->min_line(), rp->max_sample()),
		   (hind == 0 ? Min_height : Max_height));
    coordinate_converter()->convert_to_coordinate(*gc, p[0], p[1], p[2]);
    pts.push_back(p);
    // Check if linear ring is in a clockwise direction
    boost::array<double, 3> t;
    boost::array<double, 3> x1,x2;
    for(int i = 0; i < 3; ++i) {
      x1[i] = pts[1][i]-pts[0][i];
      x2[i] = pts[3][i]-pts[0][i];
    }
    cross(x1,x2,t);
    bool is_clockwise = (t[2] < 0);
    // If not clockwise, swap corners so it is
    if(!is_clockwise)
      std::swap(pts[1], pts[3]);
    // Shift the linear ring until the first element is the lower left corner.
    int i = 0;
    while(pts[0][1] > pts[1][1] || pts[0][0] > pts[3][0]) {
      pts.push_back(pts[0]);
      pts.pop_front();
      // Might be some weird pathological case where condition never met
      // (e.g., we have NaN or something like that.
      if(++i > 4)
	throw Exception("This shouldn't be able to happen");
    }
    // Save points. Note that these don't go in a linear ring order, just
    // by convention 0 is lower left corner, 1 is lower right, 2 upper left,
    // 3 is upper right.
    int b = (hind == 0 ? 0 : 4);
    rid->ground_domain_vertex()[b + 0] = coordinate_converter()->
      convert_from_coordinate(pts[0][0],pts[0][1],pts[0][2]);
    rid->ground_domain_vertex()[b + 1] = coordinate_converter()->
      convert_from_coordinate(pts[3][0],pts[3][1],pts[3][2]);
    rid->ground_domain_vertex()[b + 2] = coordinate_converter()->
      convert_from_coordinate(pts[1][0],pts[1][1],pts[1][2]);
    rid->ground_domain_vertex()[b + 3] = coordinate_converter()->
      convert_from_coordinate(pts[2][0],pts[2][1],pts[2][2]);
  }
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
	ImageCoordinate iccalc;
	bool in_valid_range;
	image_coordinate(*Igc.ground_coordinate_approx_height(ImageCoordinate(ln,smp), Height), iccalc, in_valid_range);
	if(in_valid_range) {
	  Calc_line(i,j) = iccalc.line;
	  Calc_sample(i, j) = iccalc.sample;
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
	  double h = Min_height + (Max_height - Min_height) /
	    (Nheight_fit - 1.0) * k;
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
