#include "rsm_adjustable_parameter_b.h"
#include "rsm_id.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "ostream_pad.h"
#include "ecr.h"
#include "planet_coordinate.h"
#include "image_ground_connection.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmBParameterDesc::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(RsmBParameterDesc, WithParameter);
  ar & GEOCAL_NVP(cconv)
    & GEOCAL_NVP_(normalization_scale)
    & GEOCAL_NVP_(normalization_offset)
    & GEOCAL_NVP_(row_power)
    & GEOCAL_NVP_(col_power)
    & GEOCAL_NVP_(parameter);
}

template<class Archive>
void RsmAdjustableParameterB::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmAdjustableParameter)
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBParameterDesc);
}

GEOCAL_IMPLEMENT(RsmBParameterDesc);
GEOCAL_IMPLEMENT(RsmAdjustableParameterB);
#endif

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

static boost::format numint("%|1$02d|");
static boost::format numint_missing("%|1$2s|");
static boost::format num("%|1$+21.14E|");
static boost::format numpow("%|1$1d|%|2$1d|%|3$1d|");

std::string RsmBParameterDesc::rsm_b_parm_tre_string() const
{
  std::string res;
  res += str_check_size(numint % number_parameter(), 2);
  std::string aptype = (has_ground_coordinate_parameter() ? "G" : "I");
  res += aptype;
  const LocalRcConverter* gconv = 
    dynamic_cast<const LocalRcConverter*>(coordinate_converter().get());
  if(!gconv)
    throw Exception("We don't currently support LOCTYP of N");
  res += "R";
  for(int i = 0; i < 3; ++i)
    res += str_check_size(num % normalization_scale_(i), 21);
  for(int i = 0; i < 3; ++i)
    res += str_check_size(num % normalization_offset_(i), 21);
  for(int i = 0; i < 3; ++i)
    res += str_check_size(num % gconv->parameter()->cf_offset[i], 21);
  // This is column major order
  for(int j = 0; j < 3; ++j)
    for(int i = 0; i < 3; ++i)
      res += str_check_size(num % gconv->parameter()->cf_to_rc[i][j], 21);
  std::string apbase = (using_basis() ? "Y" : "N");
  res += apbase;
  if(!has_ground_coordinate_parameter()) {
    res += str_check_size(numint % (row_power_.rows() + col_power_.rows()), 2);
    res += str_check_size(numint % row_power_.rows(), 2);
    for(int i = 0; i < row_power_.rows(); ++i)
      res += str_check_size(numpow % row_power_(i, 0) % row_power_(i, 1) %
			    row_power_(i, 2), 3);
    res += str_check_size(numint % col_power_.rows(), 2);
    for(int i = 0; i < col_power_.rows(); ++i)
      res += str_check_size(numpow % col_power_(i, 0) % col_power_(i, 1) %
			    col_power_(i, 2), 3);
  } else {
    // TODO Fill this in
  }
  if(using_basis()) {
    // TODO Fill this in
  }
  return res;
}

void RsmBParameterDesc::rsm_b_parm_read_tre_string(std::istream& In)
{
  int nparm = read_size<int>(In, 2);
  std::string aptype = read_size<std::string>(In, 1);
  std::string loctype = read_size<std::string>(In, 1);
  if(loctype != "R")
    throw Exception("We don't currently support LOCTYP of N");
  for(int i = 0; i < 3; ++i)
    normalization_scale_(i) = read_size<double>(In, 21);
  for(int i = 0; i < 3; ++i)
    normalization_offset_(i) = read_size<double>(In, 21);
  boost::shared_ptr<LocalRcParameter> lp =
    boost::make_shared<LocalRcParameter>();
  lp->cf_prototype = boost::make_shared<Ecr>(0,0,0);
  for(int i = 0; i < 3; ++i)
    lp->cf_offset[i] = read_size<double>(In, 21);
  // Column major order
  for(int j = 0; j < 3; ++j)
    for(int i = 0; i < 3; ++i)
      lp->cf_to_rc[i][j] = read_size<double>(In, 21);
  cconv = boost::make_shared<LocalRcConverter>(lp);
  std::string apbase = read_size<std::string>(In, 1);
  if(aptype == "I") {
    int nisap __attribute__((unused)) = read_size<int>(In, 2);
    int nisapr = read_size<int>(In, 2);
    row_power_.resize(nisapr, 3);
    for(int i = 0; i < nisapr; ++i) {
      row_power_(i,0) = read_size<int>(In,1);
      row_power_(i,1) = read_size<int>(In,1);
      row_power_(i,2) = read_size<int>(In,1);
    }
    int nisapc = read_size<int>(In, 2);
    col_power_.resize(nisapc, 3);
    for(int i = 0; i < nisapc; ++i) {
      col_power_(i,0) = read_size<int>(In,1);
      col_power_(i,1) = read_size<int>(In,1);
      col_power_(i,2) = read_size<int>(In,1);
    }
  } else {
    // TODO Fill this in
  }
  if(apbase == "Y") {
    // TODO Fill this in
  }
  parameter_.resize(nparm, 0);
  parameter_ = 0;
}

std::string RsmAdjustableParameterB::tre_string() const
{
  std::string res = base_tre_string();
  res += rsm_b_parm_tre_string();
  for(int i = 0; i < parameter_.rows(); ++i)
    res += str_check_size(num % parameter_.value()(i), 21);
  return res;
}

//-----------------------------------------------------------------------
/// Read a TRE string. Note that the TRE does not contain all the
/// fields we have in a RsmRationalPolynomial. However the fields that
/// aren't contained are ones used for fitting the RSM, so in practice
/// this doesn't matter. We just set the various fields to the default
/// values found in the constructor.
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

boost::shared_ptr<RsmAdjustableParameterB>
RsmAdjustableParameterB::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmAdjustableParameterB> res(new RsmAdjustableParameterB);
  res->base_read_tre_string(in);
  res->rsm_b_parm_read_tre_string(in);
  for(int i = 0; i < res->parameter_.rows(); ++i)
    res->parameter_(i) = read_size<double>(in, 21);
  return res;
}

//-----------------------------------------------------------------------
/// Create a RsmAdjustableParameterB. We create a LocalRcConverter
/// that matches the given Igc, set the Image_identifier and
/// Rsm_support_data_edition to match that of the given Rsm_id.
//-----------------------------------------------------------------------

RsmAdjustableParameterB::RsmAdjustableParameterB
(const ImageGroundConnection& Igc,
 double H_min, double H_max,
 const RsmId& Rsm_id,
 const std::string& Triangulation_id)
  : RsmBParameterDesc(Igc, H_min, H_max),
    RsmAdjustableParameter(Rsm_id.image_identifier(),
			   Rsm_id.rsm_support_data_edition(),
			   Triangulation_id)
{
}

//-----------------------------------------------------------------------
/// Create a RsmBParameterDesc. We create a LocalRcConverter
/// that matches the given Igc.
//-----------------------------------------------------------------------

RsmBParameterDesc::RsmBParameterDesc
(const ImageGroundConnection& Igc,
 double H_min, double H_max)
  : normalization_scale_(3),
    normalization_offset_(3)
{
  blitz::Range ra = blitz::Range::all();
  cconv = boost::make_shared<LocalRcConverter>(boost::make_shared<LocalRcParameter>(Igc, (H_max + H_min) / 2.0));
  blitz::Array<double, 2> pos(8,3);
  int ind = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 2; ++j)
      for(int k = 0; k < 2; ++k) {
	double h = (i == 0 ? H_min : H_max);
	double ln = (j == 0 ? 0 : Igc.number_line() - 1);
	double smp = (k == 0 ? 0 : Igc.number_sample() - 1);
	auto gp = Igc.ground_coordinate_approx_height(ImageCoordinate(ln, smp),
						      h);
	cconv->convert_to_coordinate(*gp, pos(ind,0), pos(ind,1), pos(ind,2));
	++ind;
      }
  for(int i = 0; i < 3; ++i) {
    double mn = blitz::min(pos(ra,i));
    double mx = blitz::max(pos(ra,i));
    normalization_offset_(i) = (mx + mn) / 2.0;
    normalization_scale_(i) = (mx - mn) / 2.0;
  }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmBParameterDesc::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmBParameterDesc:\n"
     << "  Local Coordinate System:\n";
  opad << *cconv << "\n";
  opad.strict_sync();
  Os << "  Normalization scale:\n";
  opad << normalization_scale_;
  opad.strict_sync();
  Os << "  Normalization offset:\n";
  opad << normalization_offset_;
  opad.strict_sync();
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmAdjustableParameterB::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmAdjustableParameterB:\n"
     << "  Image Identifier: " << image_identifier() << "\n"
     << "  RSM Edition:      " << rsm_support_data_edition() << "\n"
     << "  Triangulation ID: " << triangulation_id() << "\n";
  RsmBParameterDesc::print(opad);
  opad.strict_sync();
}

// See base class for description.
bool RsmBParameterDesc::has_ground_coordinate_parameter() const
{
  return false;
}

// See base class for description.
void RsmAdjustableParameterB::adjustment
(const GroundCoordinate& Gc, boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
 double& Lndelta, double& Smpdelta) const
{
  double x, y, z;
  convert_to_coordinate(Gc, x, y, z);
  double xdelta = 0, ydelta = 0, zdelta = 0;
  delta_x(x, y, z, xdelta, ydelta, zdelta);
  delta_ls(x, y, z, Lndelta, Smpdelta);
  Gc_adjusted = convert_from_coordinate(x + xdelta, y + ydelta, z + zdelta);
}

// See base class for description.

void RsmAdjustableParameterB::adjustment_with_derivative
(const GroundCoordinate& Gc, ArrayAd<double, 1>& Cf_adjusted,
 AutoDerivative<double>& Lndelta, AutoDerivative<double>& Smpdelta) const
{
  double x, y, z;
  convert_to_coordinate(Gc, x, y, z);
  AutoDerivative<double> xdelta, ydelta, zdelta;
  delta_x(x, y, z, xdelta, ydelta, zdelta);
  delta_ls(x, y, z, Lndelta, Smpdelta);
  Cf_adjusted.reference(convert_to_cf(x + xdelta, y + ydelta, z + zdelta));
}

// See base class for description.
void RsmBParameterDesc::naif_code(int Naif_code)
{
  boost::shared_ptr<LocalRcParameter> p = cconv->parameter();
  if(Naif_code == Ecr::EARTH_NAIF_CODE)
    p->cf_prototype = boost::make_shared<Ecr>();
  else
    p->cf_prototype = boost::make_shared<PlanetFixed>(Naif_code);
}

void RsmBParameterDesc::delta_x
(double x, double y, double z, double& xdelta,
 double& ydelta, double& zdelta) const
{
  if(!has_ground_coordinate_parameter()) {
    xdelta = 0;
    ydelta = 0;
    zdelta = 0;
    return;
  }
  // TODO fill this in
  xdelta = 0;
  ydelta = 0;
  zdelta = 0;
}

void RsmBParameterDesc::delta_x
(double x, double y, double z, AutoDerivative<double>& xdelta,
 AutoDerivative<double>& ydelta,
 AutoDerivative<double>& zdelta) const
{
  if(!has_ground_coordinate_parameter()) {
    xdelta = 0;
    ydelta = 0;
    zdelta = 0;
    return;
  }
  // TODO fill this in
  xdelta = 0;
  ydelta = 0;
  zdelta = 0;
}

void RsmBParameterDesc::delta_ls
(double x, double y, double z, double& lndelta,
 double& smpdelta) const
{
  blitz::Range ra = blitz::Range::all();
  lndelta = 0;
  smpdelta = 0;
  if(has_ground_coordinate_parameter())
    return;
  blitz::Array<double, 2> xpow(6,3);
  xpow(0,ra) = 1;
  xpow(1,ra) = x, y, z;
  for(int i = 2; i < 6; ++i)
    xpow(i, ra) = xpow(i-1,ra) * xpow(1,ra);
  if(row_power_.rows() > 0) {
    blitz::Array<double, 1>
      prow(parameter_.value()(blitz::Range(0,row_power_.rows()-1)));
    for(int i = 0; i < row_power_.rows(); ++i)
      lndelta += prow(i) * (xpow(row_power_(i,0), 0) *
			    xpow(row_power_(i,1), 1) *
			    xpow(row_power_(i,2), 2));
  }
  if(col_power_.rows() > 0) {
    blitz::Array<double, 1>
      pcol(parameter_.value()(blitz::Range(row_power_.rows(),
					   number_parameter() - 1)));
    for(int i = 0; i < col_power_.rows(); ++i)
      smpdelta += pcol(i) * (xpow(col_power_(i,0), 0) *
			     xpow(col_power_(i,1), 1) *
			     xpow(col_power_(i,2), 2));
  }
}

void RsmBParameterDesc::delta_ls
(double x, double y, double z, AutoDerivative<double>& lndelta,
 AutoDerivative<double>& smpdelta) const
{
  blitz::Range ra = blitz::Range::all();
  lndelta = 0;
  smpdelta = 0;
  if(has_ground_coordinate_parameter())
    return;
  blitz::Array<double, 2> xpow(6,3);
  xpow(0,ra) = 1;
  xpow(1,ra) = x, y, z;
  for(int i = 2; i < 6; ++i)
    xpow(i, ra) = xpow(i-1,ra) * xpow(1,ra);
  if(row_power_.rows() > 0) {
    ArrayAd<double, 1>
      prow(parameter_(blitz::Range(0,row_power_.rows()-1)));
    for(int i = 0; i < row_power_.rows(); ++i)
      lndelta += prow(i) * (xpow(row_power_(i,0), 0) *
			    xpow(row_power_(i,1), 1) *
			    xpow(row_power_(i,2), 2));
  }
  if(col_power_.rows() > 0) {
    ArrayAd<double, 1>
      pcol(parameter_(blitz::Range(row_power_.rows(),
				   number_parameter() - 1)));
    for(int i = 0; i < col_power_.rows(); ++i)
      smpdelta += pcol(i) * (xpow(col_power_(i,0), 0) *
			     xpow(col_power_(i,1), 1) *
			     xpow(col_power_(i,2), 2));
  }
}

void RsmBParameterDesc::convert_to_coordinate
(const GroundCoordinate& Gc, double& X, double& Y, double& Z) const
{
  cconv->convert_to_coordinate(Gc, X, Y, Z);
  X = (X - normalization_offset_(0)) / normalization_scale_(0);
  Y = (Y - normalization_offset_(1)) / normalization_scale_(1);
  Z = (Z - normalization_offset_(2)) / normalization_scale_(2);
}

boost::shared_ptr<GroundCoordinate>
RsmBParameterDesc::convert_from_coordinate
(double X, double Y, double Z) const
{
  double x = X * normalization_scale_(0) + normalization_offset_(0);
  double y = Y * normalization_scale_(1) + normalization_offset_(1);
  double z = Z * normalization_scale_(2) + normalization_offset_(2);
  return cconv->convert_from_coordinate(x, y, z);
}

ArrayAd<double, 1>
RsmBParameterDesc::convert_to_cf
(const AutoDerivative<double>& X, const AutoDerivative<double>& Y,
 const AutoDerivative<double>& Z) const
{
  AutoDerivative<double> x =
    X * normalization_scale_(0) + normalization_offset_(0);
  AutoDerivative<double> y =
    Y * normalization_scale_(1) + normalization_offset_(1);
  AutoDerivative<double> z =
    Z * normalization_scale_(2) + normalization_offset_(2);
  return cconv->convert_to_cf(x, y, z);
}
