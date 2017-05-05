#include "geocal_rpc.h"
#include "planet_coordinate.h"
#include "raster_image.h"
#include "geocal_gsl_fit.h"
#include "geocal_gsl_root.h"
#include "simple_dem.h"
#include "image_ground_connection.h"
#include "geocal_serialize_support.h"
#include <boost/foreach.hpp>
#include <limits>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Rpc::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Rpc);
  ar & GEOCAL_NVP(error_bias) & GEOCAL_NVP(error_random)
    & GEOCAL_NVP(rpc_type) 
    & GEOCAL_NVP(height_offset) & GEOCAL_NVP(height_scale) 
    & GEOCAL_NVP(latitude_offset) & GEOCAL_NVP(latitude_scale)
    & GEOCAL_NVP(longitude_offset) & GEOCAL_NVP(longitude_scale)
    & GEOCAL_NVP(line_offset) & GEOCAL_NVP(line_scale) 
    & GEOCAL_NVP(sample_offset) & GEOCAL_NVP(sample_scale) 
    & GEOCAL_NVP(line_denominator) & GEOCAL_NVP(line_numerator)
    & GEOCAL_NVP(sample_denominator) & GEOCAL_NVP(sample_numerator)
    & GEOCAL_NVP(fit_line_numerator) & GEOCAL_NVP(fit_sample_numerator);
  // Older version didn't have coordinate_converter. The default of a
  // null point uses Geodetic, so we can just skip setting this for
  // older versions.
  if(version > 0)
    ar & GEOCAL_NVP(coordinate_converter);
}

GEOCAL_IMPLEMENT(Rpc);
#endif

boost::shared_ptr<CoordinateConverter>
Rpc::default_coordinate_converter(new GeodeticConverter);

//-----------------------------------------------------------------------
/// Generate a RPC that approximates the calculation done by a
/// ImageGroundConnection.  We determine that latitude and longitude
/// range to use automatically to cover the range given by the
/// ImageGroundConnection.
///
/// This routine always ignores ImageGroundConnectionFailed
/// exceptions, and just skips to the next point. But if we are using
/// python code for the ImageGroundConnection we can't translate
/// errors to ImageGroundConnectionFailed (this is a limitation of
/// SWIG). So you can optionally specify Ignore_error as true, in
/// which case we ignore *all* exceptions and just skip to the next
/// point.
///
/// We normally look at all image points when generating the RPC. You
/// can optionally specify Skip_masked_point to skip all image points
/// that are masked.
//-----------------------------------------------------------------------

Rpc Rpc::generate_rpc(const ImageGroundConnection& Igc,
		      double Min_height, double Max_height,
		      int Nln, int Nsmp, int Nheight, bool Skip_masked_point,
		      bool Ignore_error)
{
  Rpc rpc;
  rpc.error_bias = 0;		// We don't calculate these right now
  rpc.error_random = 0;
  rpc.rpc_type = Rpc::RPC_B;

  // The NITF convention is to have these be integers.
  rpc.height_offset = floor((Max_height + Min_height) / 2.0 + 0.5);
  rpc.height_scale = floor((Max_height - Min_height) / 2.0 + 0.5);
  rpc.line_offset = floor(Igc.number_line() / 2.0 + 0.5);
  rpc.line_scale = floor(Igc.number_line() / 2.0 + 0.5);
  rpc.sample_offset = floor(Igc.number_sample() / 2.0 + 0.5);
  rpc.sample_scale = floor(Igc.number_sample() / 2.0 + 0.5);

  std::vector<double> ln, smp, lat, lon, h;

  // Make sure to nail down the edges of the image, since RPC
  // polynomial extrapolates poorly
  double min_lat = -999, max_lat = -999, min_lon = -999, max_lon = -999;
  bool first = true;
  for(int ih = 0; ih < Nheight; ++ih)
    for(int i = 0; i < Nln + 1; ++i)
      for(int j = 0; j < Nsmp + 1; ++j) {
	try {
	  double lnv = std::min((Igc.number_line() / Nln) * i, 
				Igc.number_line() - 1);
	  double smpv = std::min((Igc.number_sample() / Nsmp) * j, 
				 Igc.number_sample() - 1);
	  if(Skip_masked_point && Igc.image_mask()->mask(lnv, smpv))
	    continue;
	  double hdem = rpc.height_offset + rpc.height_scale * 
	    (ih * 2.0 / Nheight - 1);
	  ImageCoordinate ic(lnv, smpv);
	  boost::shared_ptr<GroundCoordinate> 
	    pt(Igc.ground_coordinate_approx_height(ic, hdem));
	  lat.push_back(pt->latitude());
	  lon.push_back(pt->longitude());
	  h.push_back(pt->height_reference_surface());
	  ln.push_back(ic.line);
	  smp.push_back(ic.sample);
	  if(first) {
	    min_lat = max_lat = pt->latitude();
	    min_lon = max_lon = pt->longitude();
	    rpc.naif_code(pt->convert_to_cf()->naif_code());
	    first = false;
	  } else {
	    min_lat = std::min(min_lat, pt->latitude());
	    max_lat = std::max(max_lat, pt->latitude());
	    min_lon = std::min(min_lon, pt->longitude());
	    max_lon = std::max(max_lon, pt->longitude());
	  }
	} catch(const ImageGroundConnectionFailed&) {
	  // Ignore failures, just go to next point.
	} catch(...) {
	  if(!Ignore_error)
	    throw;
	}
      }
  if(first)
    throw Exception("Did not get any points for RPC fitting in fit_rpc");
  rpc.latitude_offset = (max_lat + min_lat) / 2.0;
  rpc.latitude_scale = (max_lat - min_lat) / 2.0;
  rpc.longitude_offset = (max_lon + min_lon) / 2.0;
  rpc.longitude_scale = (max_lon - min_lon) / 2.0;

  // The NITF convention is to have these have 4 digits after
  // the decimal point.
  rpc.latitude_offset = floor(rpc.latitude_offset * 10000.0 + 0.5) / 10000.0;
  rpc.latitude_scale = floor(rpc.latitude_scale * 10000.0 + 0.5) / 10000.0;
  rpc.longitude_offset = floor(rpc.longitude_offset * 10000.0 + 0.5) / 10000.0;
  rpc.longitude_scale = floor(rpc.longitude_scale * 10000.0 + 0.5) / 10000.0;
  rpc.fit_all(ln, smp, lat, lon, h);
  return rpc;
}

//-----------------------------------------------------------------------
/// Default constructor. Initializes fit_line_numerator and
/// fit_sample_numerator to all false.
//-----------------------------------------------------------------------

Rpc::Rpc()
{
  std::fill(fit_line_numerator.begin(), fit_line_numerator.end(), false);
  std::fill(fit_sample_numerator.begin(), fit_sample_numerator.end(), false);
}

//-----------------------------------------------------------------------
/// Set the coordinate_converter based on the given NAIF code.
//-----------------------------------------------------------------------

void Rpc::naif_code(int Naif_code)
{
  // Default is earth/geodetic
  if(Naif_code != CoordinateConverter::EARTH_NAIF_CODE)
    coordinate_converter.reset(new PlanetocentricConverter(Naif_code));
  else
    coordinate_converter.reset((CoordinateConverter*) 0);
}

//-----------------------------------------------------------------------
///  Print a Rpc to a stream
//-----------------------------------------------------------------------

void Rpc::print(std::ostream& Os) const
{
  Os << "RPC:\n"
     << "  Type:             " << (rpc_type == RPC_A ? "A" : 
				   (rpc_type == RPC_B ? "B" : "Unknown"))
     << "\n"
     << "  Planet:           " << PlanetConstant::name(naif_code()) << "\n"
     << "  Error Bias:       " << error_bias << "\n"
     << "  Error Random:     " << error_random << "\n"
     << "  Line offset:      " << line_offset << "\n"
     << "  Line scale:       " << line_scale << "\n"
     << "  Sample offset:    " << sample_offset << "\n"
     << "  Sample scale:     " << sample_scale << "\n"
     << "  Longitude offset: " << longitude_offset << " degrees\n"
     << "  Longitude scale:  " << longitude_scale << " degrees\n"
     << "  Latitude offset:  " << latitude_offset << " degrees\n"
     << "  Latitude scale:   " << latitude_scale << " degrees\n"
     << "  Height offset:    " << height_offset << " m\n"
     << "  Height scale:     " << height_scale << " m\n";
  Os << "  Line Numerator:   ";
  BOOST_FOREACH(double i, line_numerator)
    Os << i << " ";
  Os << "\n  Line Denominator: ";
  BOOST_FOREACH(double i, line_denominator)
    Os << i << " ";
  Os << "\n  Sample Numerator:   ";
  BOOST_FOREACH(double i, sample_numerator)
    Os << i << " ";
  Os << "\n  Sample Denominator: ";
  BOOST_FOREACH(double i, sample_denominator)
    Os << i << " ";
  Os << "\n";
}

//-----------------------------------------------------------------------
/// This uses the RPC to project a RasterImage to a
/// MapInfo. On entry, the RasterImage Res should
/// already have the size and MapInfo that we want to project to.
///
/// In some cases, Img might be a lower resolution image than is
/// described by the RPC (e.g., we are imaging matching 60 cm
/// Quickbird data to 5 m CIB data, so we down sample by a factor of
/// 9). In those cases, a scale to apply to the line and sample
/// returned by the RPC before looking into Img can be given. The
/// default value is to not scale.
//-----------------------------------------------------------------------

void Rpc::rpc_project(RasterImage& Res, const RasterImage& Img, 
		      const Dem& D, double Line_scale, 
		      double Sample_scale) const 
{
  blitz::Array<double, 2> lat(Res.number_line(), Res.number_sample());
  blitz::Array<double, 2> lon(Res.number_line(), Res.number_sample());
  blitz::Array<double, 2> height(Res.number_line(), 
				 Res.number_sample());
  for(int i = 0; i < Res.number_line(); ++i)
    for(int j = 0; j < Res.number_sample(); ++j) {
      boost::shared_ptr<GroundCoordinate> gc = 
	Res.map_info().ground_coordinate(j, i);
      lat(i, j) = gc->latitude();
      lon(i, j) = gc->longitude();
      height(i, j) = D.height_reference_surface(*gc);
    }
  blitz::Array<double, 3> ic = image_coordinate(lat, lon, height);
  for(int i = 0; i < Res.number_line(); ++i)
    for(int j = 0; j < Res.number_sample(); ++j) {
      double l = ic(0, i, j) / Line_scale;
      double s = ic(1, i, j) / Sample_scale;
      if(l > 0 && l < Img.number_line() - 1 &&
	 s > 0 && s < Img.number_sample() - 1)
	Res.write(i, j, (int) Img.interpolate(l, s));
    }
}

//-----------------------------------------------------------------------
/// This return a Rpc that has type RPC_A. If this rpc is already type
/// RPC_A, then this is just a copy. Otherwise we rearrange the
/// coefficients to give a type RPC_A.
//-----------------------------------------------------------------------

Rpc Rpc::rpc_type_a() const
{
  check_valid();
  if(rpc_type == RPC_A)
    return *this;
  else {
    Rpc res(*this);
    res.rpc_type = RPC_A;
    res.line_numerator[10] = line_numerator[7];
    res.line_numerator[7] = line_numerator[8];
    res.line_numerator[8] = line_numerator[9];
    res.line_numerator[9] = line_numerator[10];
    res.line_numerator[14] = line_numerator[12];
    res.line_numerator[17] = line_numerator[13];
    res.line_numerator[12] = line_numerator[14];
    res.line_numerator[18] = line_numerator[16];
    res.line_numerator[13] = line_numerator[17];
    res.line_numerator[16] = line_numerator[18];

    res.line_denominator[10] = line_denominator[7];
    res.line_denominator[7] = line_denominator[8];
    res.line_denominator[8] = line_denominator[9];
    res.line_denominator[9] = line_denominator[10];
    res.line_denominator[14] = line_denominator[12];
    res.line_denominator[17] = line_denominator[13];
    res.line_denominator[12] = line_denominator[14];
    res.line_denominator[18] = line_denominator[16];
    res.line_denominator[13] = line_denominator[17];
    res.line_denominator[16] = line_denominator[18];

    res.sample_numerator[10] = sample_numerator[7];
    res.sample_numerator[7] = sample_numerator[8];
    res.sample_numerator[8] = sample_numerator[9];
    res.sample_numerator[9] = sample_numerator[10];
    res.sample_numerator[14] = sample_numerator[12];
    res.sample_numerator[17] = sample_numerator[13];
    res.sample_numerator[12] = sample_numerator[14];
    res.sample_numerator[18] = sample_numerator[16];
    res.sample_numerator[13] = sample_numerator[17];
    res.sample_numerator[16] = sample_numerator[18];

    res.sample_denominator[10] = sample_denominator[7];
    res.sample_denominator[7] = sample_denominator[8];
    res.sample_denominator[8] = sample_denominator[9];
    res.sample_denominator[9] = sample_denominator[10];
    res.sample_denominator[14] = sample_denominator[12];
    res.sample_denominator[17] = sample_denominator[13];
    res.sample_denominator[12] = sample_denominator[14];
    res.sample_denominator[18] = sample_denominator[16];
    res.sample_denominator[13] = sample_denominator[17];
    res.sample_denominator[16] = sample_denominator[18];
    return res;
  }
}

//-----------------------------------------------------------------------
/// Calculate the approximate resolution on the ground. This finds the 
/// intersection for the center pixel with the surface + 1 in the line
/// and sample direction. We find the difference in meters between
/// these points, and select the maximum value. We the height from
/// the Dem.
//-----------------------------------------------------------------------

double Rpc::resolution_meter(const Dem& D) const
{
  boost::shared_ptr<GroundCoordinate> g1 =
    ground_coordinate(ImageCoordinate(line_offset, sample_offset), D);
  boost::shared_ptr<GroundCoordinate> g2 =
    ground_coordinate(ImageCoordinate(line_offset + 1, sample_offset), D);
  boost::shared_ptr<GroundCoordinate> g3 =
    ground_coordinate(ImageCoordinate(line_offset, sample_offset + 1), D);
  double d1 = distance(*g1, *g2);
  double d2 = distance(*g1, *g3);
  return std::max(d1, d2);
}

//-----------------------------------------------------------------------
/// This return a Rpc that has type RPC_B. If this rpc is already type
/// RPC_B, then this is just a copy. Otherwise we rearrange the
/// coefficients to give a type RPC_B.
//-----------------------------------------------------------------------

Rpc Rpc::rpc_type_b() const
{
  check_valid();
  if(rpc_type ==RPC_B)
    return *this;
  else {
    Rpc res(*this);
    res.rpc_type = RPC_B;
    res.line_numerator[7] = line_numerator[10];
    res.line_numerator[8] = line_numerator[7];
    res.line_numerator[9] = line_numerator[8];
    res.line_numerator[10] = line_numerator[9];
    res.line_numerator[12] = line_numerator[14];
    res.line_numerator[13] = line_numerator[17];
    res.line_numerator[14] = line_numerator[12];
    res.line_numerator[16] = line_numerator[18];
    res.line_numerator[17] = line_numerator[13];
    res.line_numerator[18] = line_numerator[16];

    res.line_denominator[7] = line_denominator[10];
    res.line_denominator[8] = line_denominator[7];
    res.line_denominator[9] = line_denominator[8];
    res.line_denominator[10] = line_denominator[9];
    res.line_denominator[12] = line_denominator[14];
    res.line_denominator[13] = line_denominator[17];
    res.line_denominator[14] = line_denominator[12];
    res.line_denominator[16] = line_denominator[18];
    res.line_denominator[17] = line_denominator[13];
    res.line_denominator[18] = line_denominator[16];

    res.sample_numerator[7] = sample_numerator[10];
    res.sample_numerator[8] = sample_numerator[7];
    res.sample_numerator[9] = sample_numerator[8];
    res.sample_numerator[10] = sample_numerator[9];
    res.sample_numerator[12] = sample_numerator[14];
    res.sample_numerator[13] = sample_numerator[17];
    res.sample_numerator[14] = sample_numerator[12];
    res.sample_numerator[16] = sample_numerator[18];
    res.sample_numerator[17] = sample_numerator[13];
    res.sample_numerator[18] = sample_numerator[16];

    res.sample_denominator[7] = sample_denominator[10];
    res.sample_denominator[8] = sample_denominator[7];
    res.sample_denominator[9] = sample_denominator[8];
    res.sample_denominator[10] = sample_denominator[9];
    res.sample_denominator[12] = sample_denominator[14];
    res.sample_denominator[13] = sample_denominator[17];
    res.sample_denominator[14] = sample_denominator[12];
    res.sample_denominator[16] = sample_denominator[18];
    res.sample_denominator[17] = sample_denominator[13];
    res.sample_denominator[18] = sample_denominator[16];
    return res;
  }
}

//-----------------------------------------------------------------------
/// You may not want to use this function directly, but rather use
/// Rpc::generate_rpc which sets up the entire RPC.
///
/// This is a low level function used to create an RPC that matches a
/// set of data. The RPC should have already had the various offset
/// and scales filled in. This function then fills in
/// line_denominator, line_numerator, sample_denominator, and
/// sample_numerator.
///
/// This fits all of the rpc parameters (the full 78 parameters) doing
/// a linear least squares fit. This doesn't to any blunder detection,
/// or allow a subset to be used.
///
/// The idea is that through some other method you can determine 
/// latitude, longitude, height vs line, sample (e.g., using a Orbit
/// and Camera). You then generate a regular set of points, set up the
/// offset and scale, and finally call this function to fit the RPC.
//-----------------------------------------------------------------------

void Rpc::fit_all(const std::vector<double>& Line,
		  const std::vector<double>& Sample,
		  const std::vector<double>& Latitude,
		  const std::vector<double>& Longitude,
		  const std::vector<double>& Height)
{
  if(Line.size() != Sample.size() ||
     Line.size() != Latitude.size() ||
     Line.size() != Longitude.size() ||
     Line.size() != Height.size())
    throw Exception("Line, Sample, Latitude, Longitude and Height all need to be the same size");
  Array<double, 1> ln_lhs(Line.size());
  Array<double, 1> smp_lhs(Line.size());
  Array<double, 2> ln_jac(Line.size(), 39);
  Array<double, 2> smp_jac(Line.size(), 39);
  ln_jac = 0.0;
  smp_jac = 0.0;
  for(int i = 0; i < (int) Line.size(); ++i) {
    double x = (Latitude[i] - latitude_offset) / latitude_scale;
    double y = (Longitude[i] - longitude_offset) / longitude_scale;
    double z = (Height[i] - height_offset) / height_scale;
    ln_lhs(i) = (Line[i] - line_offset) / line_scale;
    smp_lhs(i) = (Sample[i] - sample_offset) / sample_scale;
    boost::array<double, 20> pjac;
    polynomial_jac(x, y, z, pjac);
    for(int j = 0; j < 20; ++j) {
      ln_jac(i, j) = pjac[j];
      smp_jac(i, j) = pjac[j];
    }
    for(int j = 0; j < 19; ++j) {
      ln_jac(i, j + 20) = -ln_lhs(i) * pjac[j + 1];
      smp_jac(i, j + 20) = -smp_lhs(i) * pjac[j + 1];
    }
  }
  GslMatrix cov;
  GslVector ln_c;
  GslVector smp_c;
  double chisq;
  gsl_fit(ln_jac, ln_lhs, ln_c, cov, chisq);
  gsl_fit(smp_jac, smp_lhs, smp_c, cov, chisq);
  for(int i = 0; i < 20; ++i) {
    line_numerator[i] = ln_c.blitz_array()(i);
    sample_numerator[i] = smp_c.blitz_array()(i);
  }
  line_denominator[0] = 1.0;
  sample_denominator[0] = 1.0;
  for(int i = 0; i < 19; ++i) {
    line_denominator[i + 1] = ln_c.blitz_array()(i + 20);
    sample_denominator[i + 1] = smp_c.blitz_array()(i + 20);
  }
}

//-----------------------------------------------------------------------
/// This updates the RPC parameters to better match a given set of
/// data. This class takes a list of GroundCoordinates, and the
/// ImageCoordinate that should be returned by the RPC. Each of the
/// ImageCoordinate has an associated line and sample sigma used to
/// weight it (i.e., the line and sigma returned by ImageMatcher).
///
/// We update the set of parameters given by fit_line_numerator and
/// fit_sample_numerator. Right now, we only do a linear fit and don't
/// try to fit the nonlinear denominator pieces. We can add that in
/// the future if it is needed.
///
/// The blunder detection used is iterative data snooping, where the
/// point with the largest standardized residual is removed. If the
/// Chisq of the match improves by the Chisq_threshold, we toss the
/// point out, otherwise we keep it. We then proceed to the next
/// largest residual, and so.
///
/// However, if we expect large numbers of blunders in otherwise well
/// matched data (e.g., image matching between two images with lots of
/// differences), it might be preferable to toss out all large
/// outliers. So, if Chisq_threshold is >= 1 we just toss out all
/// points outside of the Blunder_threshold. 
//-----------------------------------------------------------------------

void Rpc::fit(const std::vector<boost::shared_ptr<GroundCoordinate> >& Gc,
	      const std::vector<ImageCoordinate>& Ic, 
	      const std::vector<double>& Line_sigma,
	      const std::vector<double>& Sample_sigma,
	      std::set<int>& Blunder,
	      double Blunder_threshold,
	      double Chisq_threshold,
	      bool Blunder_detect
	      )
{
  if(Gc.size() != Ic.size() ||
     Gc.size() != Line_sigma.size() ||
     Gc.size() != Sample_sigma.size())
    throw Exception("Gc, Ic, Line_sigma, and Sample_sigma all need to be the same length");


//-----------------------------------------------------------------------
// Set up equation to solve. y = Ic - rpc calculated, the weight w are 
// 1 / sigma, and the Jacobian jac is for the parameters to be varied. We
// solve for y = jac * (parameter updates)  
//-----------------------------------------------------------------------

  Array<double, 1> y(2 * Gc.size());
  Array<double, 1> w(2 * Gc.size());
  std::vector<bool> blund(2 * Gc.size(), false);
  int nlparm = std::count(fit_line_numerator.begin(), 
			  fit_line_numerator.end(), true); 
  int nsparm = std::count(fit_sample_numerator.begin(), 
			  fit_sample_numerator.end(), true);
  if(nlparm < 1 || nsparm < 1)
    throw Exception("Need to fit at least one of the line and sample numerator parameters");
  int nparm = nlparm + nsparm;
  Range rline(0, nlparm - 1);
  Range rsamp(nlparm, nparm - 1);
  Array<double, 2> jac(2 * Gc.size(), nparm);
  jac = 0.0;
  for(int i = 0; i < y.extent(0); i += 2) {
    w(i) = 1.0 / Line_sigma[i / 2];
    w(i + 1) = 1.0 / Sample_sigma[i / 2];
    ImageCoordinate ic_calc = image_coordinate(*(Gc[i / 2]));
    y(i) = Ic[i / 2].line - ic_calc.line;
    y(i + 1) = Ic[i / 2].sample - ic_calc.sample;
    double xt = (Gc[i / 2]->latitude() - latitude_offset) / latitude_scale;
    double yt = (Gc[i / 2]->longitude() - longitude_offset) / longitude_scale;
    double zt = (Gc[i / 2]->height_reference_surface() - 
		 height_offset) / height_scale;
    Array<double, 1> ljac(jac(i, rline));
    Array<double, 1> sjac(jac(i + 1, rsamp));
    jac_line(xt, yt, zt, ljac);
    jac_sample(xt, yt, zt, sjac);
  }

//-----------------------------------------------------------------------
// Solve equations, and update parameters.
//-----------------------------------------------------------------------
  
  GslVector c;
  double chisq; 
  Array<double, 1> std_res;
  std::vector<int> res_index_map;
  fit_no_blunder(y, w, jac, blund, std_res, res_index_map, chisq, c);

//-----------------------------------------------------------------------
// If Chisq_threshold >= 1, then don't do data snooping.
//-----------------------------------------------------------------------

  if(Blunder_detect) {
    if(Chisq_threshold >= 1.0) {
      for(int i = 0; i < std_res.extent(0); ++i)
	if(fabs(std_res(i)) > Blunder_threshold) {
	  int bi = 2 * res_index_map[i];
	  blund[bi] = true;
	  blund[bi + 1] = true;
	}
      fit_no_blunder(y, w, jac, blund, std_res, res_index_map, chisq, c);
    } else {

//-----------------------------------------------------------------------
// Otherwise, proceed with iterative data snooping.
//-----------------------------------------------------------------------

      bool blunder_found = true;	// Value to start loop
      while(blunder_found) {

//-----------------------------------------------------------------------
// Search for blunders. Sort from smallest to largest absolute residual.
//-----------------------------------------------------------------------

	std::vector<std::pair<double, int> > potential_blunder;
	for(int i = 0; i < std_res.extent(0); ++i)
	  if(fabs(std_res(i)) > Blunder_threshold)
	    potential_blunder.push_back(std::pair<double, int>(fabs(std_res(i)),
						       res_index_map[i]));
	std::sort(potential_blunder.begin(), potential_blunder.end());

//-----------------------------------------------------------------------
// Try removing blunders, starting with the one with the largest
// residual. If chi squared is not reduced by the required amount,
// toss out the blunder and go to the next one. If it is, then the
// trial blunder is considered a real blunder and we go back to the
// last step.
//-----------------------------------------------------------------------

	blunder_found = false;
	while(!blunder_found && !potential_blunder.empty()) {
	  int trial_blunder = potential_blunder.back().second;
	  potential_blunder.pop_back();
	  blund[2 * trial_blunder] = true;
	  blund[2 * trial_blunder + 1] = true;
	  double chisq_new;
	  GslVector c_new;
	  fit_no_blunder(y, w, jac, blund, std_res, res_index_map, 
			 chisq_new, c_new);
	  if(chisq_new / chisq < Chisq_threshold) {
	    chisq = chisq_new;
	    c.blitz_array() = c_new.blitz_array();
	    blunder_found = true;
	  } else
	    blund[2 * trial_blunder] = false;
	  blund[2 * trial_blunder + 1] = false;
	}
      }
    }
  }
  int cindex = 0;
  for(boost::array<bool, 20>::size_type i = 0; 
      i < fit_line_numerator.size(); ++i)
    if(fit_line_numerator[i]) {
      line_numerator[i] += c.blitz_array()(cindex);
      ++cindex;
    }
  for(boost::array<bool, 20>::size_type i = 0; 
      i < fit_sample_numerator.size(); ++i)
    if(fit_sample_numerator[i]) {
      sample_numerator[i] += c.blitz_array()(cindex);
      ++cindex;
    }
  Blunder.clear();
  for(int i = 0; i < (int) blund.size(); ++i)
    if(blund[i])
      Blunder.insert(i / 2);
}

//-----------------------------------------------------------------------
/// Invert the RPC to find the ground coordinate that maps to a give
/// ImageCoordinate.
///
/// This routine may fail to find a solution, in which case a 
/// ConvergenceFailure exception will be thrown.
//-----------------------------------------------------------------------
   
boost::shared_ptr<GroundCoordinate>
Rpc::ground_coordinate(const ImageCoordinate& Ic, const Dem& D) const
{
  // This is a common enough special case to treat specially:
  const SimpleDem* sdem = dynamic_cast<const SimpleDem*>(&D);
  if(sdem)
    return ground_coordinate(Ic, sdem->h());
  boost::shared_ptr<GroundCoordinate> gc1 =
    ground_coordinate(Ic, height_offset);
  double delta_h = 10;
  boost::shared_ptr<GroundCoordinate> gc2 =
    ground_coordinate(Ic, height_offset + delta_h);
  boost::shared_ptr<CartesianFixed> p = gc1->convert_to_cf();
  boost::shared_ptr<CartesianFixed> p2 = gc2->convert_to_cf();
  CartesianFixedLookVector lv;
  lv.look_vector[0] = p->position[0] - p2->position[0];
  lv.look_vector[1] = p->position[1] - p2->position[1];
  lv.look_vector[2] = p->position[2] - p2->position[2];
  double resolution = 1.0;
  boost::shared_ptr<CartesianFixed> surfp = D.intersect(*p, lv, resolution);
  return coor_conv_or_default()->create(*surfp);
}

//-----------------------------------------------------------------------
/// Invert the RPC to find the ground coordinate that maps to a give
/// ImageCoordinate.
///
/// This routine may fail to find a solution, in which case a 
/// ConvergenceFailure exception will be thrown.
//-----------------------------------------------------------------------
   
boost::shared_ptr<GroundCoordinate> Rpc::ground_coordinate(const ImageCoordinate& Ic, double Height) const
{
//-----------------------------------------------------------------------
// Class describing RcpEquation. This just takes the latitude and
// longitude in, uses a supplied Dem to get a height, and then
// calculate image coordinates using the Rpc. We then return the
// difference between this and the desired ImageCoordinates.
//-----------------------------------------------------------------------

  class RpcEq : public VFunctorWithDerivative {
  public:
    RpcEq(const Rpc& R, const ImageCoordinate& Ic, double Height)
      : r_(R), ic_(Ic), h_(Height) {}
    virtual ~RpcEq() {}
    virtual blitz::Array<double, 1> operator()(const 
      blitz::Array<double, 1>& X) const {
      ImageCoordinate icres = r_.image_coordinate(X(0), X(1), h_);
      blitz::Array<double, 1> res(2);
      res(0) = icres.line - ic_.line;
      res(1) = icres.sample - ic_.sample;
      return res;
    }
    virtual blitz::Array<double, 2> df
    (const blitz::Array<double, 1>& X) const {
      blitz::Array<double, 2> jac = r_.image_coordinate_jac(X(0), X(1), h_);
      blitz::Array<double, 2> res(jac(Range::all(), Range(0,1)));
      return res;
    }
  private:
    const Rpc& r_;
    ImageCoordinate ic_;
    double h_;
  };

//-----------------------------------------------------------------------
// Solve the equation to get the latitude and longitude. We only look
// for a 0.01 pixel accuracy.
//-----------------------------------------------------------------------

  RpcEq eq(*this, Ic, Height);

// Initial guess is solution to linear approximation of Rpc. This
// gives the rough latitude and longitude.

  double f1 = (Ic.line - line_offset) / line_scale;
  double f3 = (Ic.sample - sample_offset) / sample_scale;
  f1 -= line_numerator[0];
  f3 -= sample_numerator[0];
  double y = (f1 * sample_numerator[2] - f3 * line_numerator[2]) /
    (line_numerator[1] * sample_numerator[2] - 
     line_numerator[2] * sample_numerator[1]);
  double x = (f3 * line_numerator[1] - f1 * sample_numerator[1]) /
    (sample_numerator[2] * line_numerator[1] - 
     line_numerator[2] * sample_numerator[1]);
  blitz::Array<double, 1> xint(2);
  xint = x * latitude_scale + latitude_offset,
    y * longitude_scale + longitude_offset;
  blitz::Array<double, 1> res = gsl_root(eq, xint, 0.1);
  return coor_conv_or_default()->convert_from_coordinate(res(1), res(0), Height);
}

//-----------------------------------------------------------------------
/// Calculate Jacobian of polynomial wrt to each coefficient. This is
/// the full Jacobian, not just the parameters listed in
/// fit_line_numerator or fit_sample_numerator.
//-----------------------------------------------------------------------

void Rpc::polynomial_jac(double x, double y, double z,
			 boost::array<double, 20>& jac) const
{
  if(rpc_type == RPC_B) {
    jac[0] = 1;
    jac[1] = y;
    jac[2] = x;
    jac[3] = z;
    jac[4] = x * y;
    jac[5] = y * z;
    jac[6] = z * x;
    jac[7] = y * y;
    jac[8] = x * x;
    jac[9] = z * z;
    jac[10] = x * y * z;
    jac[11] = y * y * y;
    jac[12] = x * x * y;
    jac[13] = z * z * y;
    jac[14] = y * y * x;
    jac[15] = x * x * x;
    jac[16] = z * z * x;
    jac[17] = y * y * z;
    jac[18] = x * x * z;
    jac[19] = z * z * z;
  } else {
    jac[0] = 1;
    jac[1] = y;
    jac[2] = x;
    jac[3] = z;
    jac[4] = x * y;
    jac[5] = y * z;
    jac[6] = z * x;
    jac[7] = x * y * z;
    jac[8] = y * y; 
    jac[9] = x * x;
    jac[10] = z * z; 
    jac[11] = y * y * y;
    jac[12] = y * y * x;
    jac[13] = y * y * z; 
    jac[14] = y * x * x;
    jac[15] = x * x * x; 
    jac[16] = x * x * z;
    jac[17] = y * z * z; 
    jac[18] = x * z * z;
    jac[19] = z * z * z;
  }
}

//-----------------------------------------------------------------------
/// Calculate Jacobian of line wrt to each coefficient. This includes
/// only those parameters listed in fit_line_numerator.
//-----------------------------------------------------------------------

void Rpc::jac_line(double x, double y, double z,
		   blitz::Array<double, 1> jac) const
{
  boost::array<double, 20> pjac;
  polynomial_jac(x, y, z, pjac);
  double f2 = evaluate_polynomial(line_denominator, x, y, z);
  int jind = 0;
  for(boost::array<bool, 20>::size_type i = 0; 
      i < fit_line_numerator.size(); ++i)
    if(fit_line_numerator[i]) {
      jac(jind) = pjac[i] / f2 * line_scale;
      ++jind;
    }
}  

//-----------------------------------------------------------------------
/// Calculate Jacobian of sample wrt to each coefficient. This includes
/// only those parameters listed in fit_sample_numerator.
//-----------------------------------------------------------------------

void Rpc::jac_sample(double x, double y, double z,
		   blitz::Array<double, 1> jac) const
{
  boost::array<double, 20> pjac;
  polynomial_jac(x, y, z, pjac);
  double f3 = evaluate_polynomial(sample_denominator, x, y, z);
  int jind = 0;
  for(boost::array<bool, 20>::size_type i = 0; 
      i < fit_sample_numerator.size(); ++i)
    if(fit_sample_numerator[i]) {
      jac(jind) = pjac[i] / f3 * sample_scale;
      ++jind;
    }
}  

//-----------------------------------------------------------------------
/// This does a coarse fit to the RPC parameters to better match a
/// given set of data. This class takes a list of GroundCoordinates,
/// and the ImageCoordinate that should be returned by the RPC.
///
/// This does an coarse fit, changing just the line_offset and 
/// sample_offset. This is used to get the RPC roughly right so we can do 
/// a more careful fit using fit().
//-----------------------------------------------------------------------

void Rpc::fit_coarse(const std::vector<boost::shared_ptr<GroundCoordinate> >& 
		     Gc,
		     const std::vector<ImageCoordinate>& Ic)
{
  if(Gc.size() != Ic.size())
    throw Exception("Gc and Ic need to be the same length");
  double lcorr = 0;
  double scorr = 0;
  for(std::vector<boost::shared_ptr<GroundCoordinate> >::size_type i = 0; 
      i < Gc.size(); ++i) {
    ImageCoordinate iref = image_coordinate(*(Gc[i]));
    lcorr += (Ic[i].line - iref.line);
    scorr += (Ic[i].sample - iref.sample);
  }
  lcorr /= Gc.size();
  scorr /= Gc.size();
  line_offset += lcorr;
  sample_offset += scorr;
}

//-----------------------------------------------------------------------
/// This does a fit for the solution, w/o doing blunder detection. The
/// function fit() uses this to do the full solution. The vector
/// Blunder indicates which points have been marked as blunders.
///
/// On exit, c contains the updates to the rpc parameters for this
/// particular fit, Standard_residual is the standardized residual of
/// the solution
/// Res_index_to_ic_index maps the index number of Standard_residual
/// to the index that should be used in Blunder
//-----------------------------------------------------------------------

void Rpc::fit_no_blunder(const Array<double, 1>& y,
			 const Array<double, 1>& w,
			 const Array<double, 2>& jac,
			 const std::vector<bool>& Blunder,
			 Array<double, 1>& Standard_residual,
			 std::vector<int>& Res_index_map,
			 double& Chisq,
			 GslVector& C)
{
  int gpoint = std::count(Blunder.begin(), Blunder.end(), false);
  Array<double, 1> yp(gpoint);
  Array<double, 1> wp(gpoint);
  Array<double, 2> jacp(gpoint, jac.extent(1));
  Res_index_map.resize(gpoint);
  Standard_residual.resize(gpoint);
  int j = 0;
  for(int i = 0; i < y.extent(0); ++i)
    if(Blunder[i] ==false) {
      yp(j) = y(i);
      wp(j) = w(i);
      jacp(j, Range::all()) = jac(i, Range::all());
      Res_index_map[j] = i / 2;
      ++j;
    }
  GslMatrix cov;
  gsl_fit(jacp, wp, yp, C, cov, Chisq);
  firstIndex i1;
  secondIndex i2;
  Standard_residual = (yp - sum(jacp(i1, i2) * C.blitz_array()(i2), i2)) * wp;
  Standard_residual /= mean(fabs(Standard_residual));
}

//-----------------------------------------------------------------------
/// Jacobian of image_coordinate with respect to the parameters that
/// are marked as ones we are fitting. This has two rows, the first is
/// for line and the second sample. The columns are first the true
/// values in fit_line_numerator, and then fit_sample_numerator.
//-----------------------------------------------------------------------


Array<double, 2> Rpc::image_coordinate_jac_parm
(double Latitude, 
 double Longitude, 
 double Height_ellipsoid) const
{
  int nlparm = std::count(fit_line_numerator.begin(), 
			  fit_line_numerator.end(), true); 
  int nsparm = std::count(fit_sample_numerator.begin(), 
			  fit_sample_numerator.end(), true);
  Array<double, 2> res(2, nlparm + nsparm);
  res = 0;
  double x = (Latitude - latitude_offset) / latitude_scale;
  double y = (Longitude - longitude_offset) / longitude_scale;
  double z = (Height_ellipsoid - height_offset) / height_scale;
  Array<double, 1> ljac(res(0, Range(0, nlparm - 1)));
  Array<double, 1> sjac(res(1, Range(nlparm, nlparm + nsparm- 1)));
  jac_line(x, y, z, ljac);
  jac_sample(x, y, z, sjac);
  return res;
}
