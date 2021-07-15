#include "rsm_id.h"
#include "coordinate_converter.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "ostream_pad.h"
#include "ecr.h"
#include "planet_coordinate.h"
#include "local_rectangular_coordinate.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmIdTiming::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmIdTiming);
  ar & GEOCAL_NVP_(number_row_acquired_simultaneously)
    & GEOCAL_NVP_(number_col_acquired_simultaneously)
    & GEOCAL_NVP_(time_between_adjacent_row_group)
    & GEOCAL_NVP_(time_between_adjacent_col_group);
}

template<class Archive>
void RsmId::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmId);
  ar & GEOCAL_NVP(cconv)
    & GEOCAL_NVP_(image_identifier)
    & GEOCAL_NVP_(rsm_support_data_edition)
    & GEOCAL_NVP_(image_sequence_identifier)
    & GEOCAL_NVP_(sensor_identifier)
    & GEOCAL_NVP_(sensor_type)
    & GEOCAL_NVP_(image_acquistion_time)
    & GEOCAL_NVP_(timing)
    & GEOCAL_NVP_(ground_domain_vertex)
    & GEOCAL_NVP_(ground_reference_point)
    & GEOCAL_NVP_(full_number_line)
    & GEOCAL_NVP_(full_number_sample)
    & GEOCAL_NVP_(min_line)
    & GEOCAL_NVP_(min_sample)
    & GEOCAL_NVP_(max_line)
    & GEOCAL_NVP_(max_sample);
}

GEOCAL_IMPLEMENT(RsmIdTiming);
GEOCAL_IMPLEMENT(RsmId);
#endif

static boost::format f("%|1$-80s|%|2$-40s|%|3$-40|%|4$-40|%|5$-40|");
static boost::format tac("%|1$4s|%|2$2s|%|3$2s|%|4$2s|%|5$2s|%|6$9s|");
static boost::format rsm_sz("%|1$08d|%|2$08d|%|3$08d|%|4$08d|%|5$08d|%|6$08d|");
static boost::format num_missing("%|1$21s|");
static boost::format num("%|1$+21.14E|");
static boost::format numint("%|1$08d|");
static boost::format timingf("%|1$08d|%|2$08d|%|3$+21.14E|%|4$+21.14E|");
static boost::format timingf_missing("%|1$-58s|");


//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

std::string RsmId::tre_string() const
{
  std::string res = "";
  res += str_check_size(f % image_identifier_ % rsm_support_data_edition_ %
			image_sequence_identifier_ % sensor_identifier_ %
			sensor_type_, 240);
  if(image_acquistion_time_) {
    std::string t(image_acquistion_time_->to_string());
    res += str_check_size(tac % t.substr(0, 4) % t.substr(5,2) %
			  t.substr(8,2) % t.substr(11,2) % t.substr(14,2) %
			  t.substr(17, 9), 21);
  } else
    res += str_check_size(tac % "" % "" % "" % "" % "" % "", 21);
  if(timing_)
    res += str_check_size(timingf
			  % timing_->number_row_acquired_simultaneously()
			  % timing_->number_col_acquired_simultaneously()
			  % timing_->time_between_adjacent_row_group()
			  % timing_->time_between_adjacent_col_group(), 
			  8 + 8 + 21 + 21);
  else
    res += str_check_size(timingf_missing % "", 8 + 8 + 21 + 21);
  const GeodeticRadianConverter* gconv =
    dynamic_cast<const GeodeticRadianConverter*>(coordinate_converter().get());
  const GeodeticRadian2piConverter* gconv2 =
    dynamic_cast<const GeodeticRadian2piConverter*>
    (coordinate_converter().get());
  const LocalRcConverter* gconv3 = 
    dynamic_cast<const LocalRcConverter*>(coordinate_converter().get());
  if(gconv) {
    res += "G";
    for(int i = 0; i < 12; ++i)
      res += str_check_size(num_missing % "", 21);
  } else if(gconv2) {
    res += "H";
    for(int i = 0; i < 12; ++i)
      res += str_check_size(num_missing % "", 21);
  } else if(gconv3) {
    res += "R";
    for(int i = 0; i < 3; ++i)
      res += str_check_size(num % gconv3->parameter()->cf_offset[i], 21);
    // This is column major order
    for(int j = 0; j < 3; ++j)
      for(int i = 0; i < 3; ++i)
	res += str_check_size(num % gconv3->parameter()->cf_to_rc[i][j], 21);
  } else
    throw Exception("Writing a RSMIDA TRE only supports GeodeticRadianConverter, GeodeticRadian2piConverter and LocalRcConverter. This is a limitation of the NITF TRE format. Note boost serialization works fine with any CoordinateConverter, just not the NITF TRE");
  if(ground_domain_vertex_.size() != 8)
      throw Exception("Ground domain vertex needs to be exactly 8 points");
  double x[8], y[8], z[8];
  for(int i = 0; i < 8; ++i) {
    if(!ground_domain_vertex_[i])
      throw Exception("ground_domain_vertex needs to be filled in");
    cconv->convert_to_coordinate(*ground_domain_vertex_[i], x[i], y[i], z[i]);
  }
  // Check the vertex constraint
  if(!(x[0] < x[1] && y[0] < y[2] && z[0] < z[4]))
    throw Exception("ground_domain_vertex needs to obey the vertex order constraint V1X<V2X, V1Y<V3Y, and V1Z<V5Z (using 1 based numbering)");
  for(int i = 0; i < 8; ++i) {
    res += str_check_size(num % x[i], 21);
    res += str_check_size(num % y[i], 21);
    res += str_check_size(num % z[i], 21);
  }
  if(ground_reference_point_) {
    double x, y, z;
    cconv->convert_to_coordinate(*ground_reference_point_, x, y, z);
    res += str_check_size(num % x, 21);
    res += str_check_size(num % y, 21);
    res += str_check_size(num % z, 21);
  } else
    for(int i = 0; i < 3; ++i)
      res += str_check_size(num_missing % "", 21);
  res += write_optional(numint, full_number_line_, 8);
  res += write_optional(numint, full_number_sample_, 8);
  res += str_check_size(numint % min_line_, 8);
  res += str_check_size(numint % max_line_, 8);
  res += str_check_size(numint % min_sample_, 8);
  res += str_check_size(numint % max_sample_, 8);
  // This is the illumination and sensor position. We don't currently
  // have this in place, we'll need to add this in.
  for(int i = 0; i < 21; ++i)
    res += str_check_size(num_missing % "", 21);
  return res;
}


//-----------------------------------------------------------------------
/// Read a TRE string. 
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

boost::shared_ptr<RsmId>
RsmId::read_tre_string(const std::string& Tre_in)
{
  std::stringstream in(Tre_in);
  boost::shared_ptr<RsmId> res(new RsmId);
  res->image_identifier_ = read_size<std::string>(in, 80);
  res->rsm_support_data_edition_ = read_size<std::string>(in, 40);
  res->image_sequence_identifier_ = read_size<std::string>(in, 40);
  res->sensor_identifier_ = read_size<std::string>(in, 40);
  res->sensor_type_ = read_size<std::string>(in, 40);
  boost::trim(res->image_identifier_);
  boost::trim(res->rsm_support_data_edition_);
  boost::trim(res->image_sequence_identifier_);
  boost::trim(res->sensor_identifier_);
  boost::trim(res->sensor_type_);
  std::string t = read_size<std::string>(in, 4);
  if(t != "    ") {
    t += "-" + read_size<std::string>(in, 2);
    t += "-" + read_size<std::string>(in, 2);
    t += "T" + read_size<std::string>(in, 2); 
    t += ":" + read_size<std::string>(in, 2);
    t += ":" + read_size<std::string>(in, 9) + "Z";
    res->image_acquistion_time_ = boost::make_shared<Time>(Time::parse_time(t));
  } else
    read_size<std::string>(in, 2 * 4 + 9);
  // We have input data (the sample SNIP file) where there is trailing
  // space for the double. Not clear if this is actually valid or not,
  // but we do want to be able to read this.
  boost::optional<int> t1 = read_size<boost::optional<int> >(in, 8, true);
  boost::optional<int> t2 = read_size<boost::optional<int> >(in, 8, true);
  boost::optional<double> t3 = read_size<boost::optional<double> >(in, 21, true);
  boost::optional<double> t4 = read_size<boost::optional<double> >(in, 21, true);
  if(t1)
    res->timing_ = boost::make_shared<RsmIdTiming>(*t1, *t2, *t3, *t4);
  std::string conv_type = read_size<std::string>(in, 1);
  if(conv_type == "G") {
    res->cconv = boost::make_shared<GeodeticRadianConverter>();
    for(int i = 0; i < 12; ++i)
      read_size<boost::optional<double> >(in, 21, true);
  } else if (conv_type == "H") {
    res->cconv = boost::make_shared<GeodeticRadian2piConverter>();
    for(int i = 0; i < 12; ++i)
      read_size<boost::optional<double> >(in, 21, true);
  } else if (conv_type == "R") {
    boost::shared_ptr<LocalRcParameter> lp =
      boost::make_shared<LocalRcParameter>();
    lp->cf_prototype = boost::make_shared<Ecr>(0,0,0);
    for(int i = 0; i < 3; ++i)
      lp->cf_offset[i] = read_size<double>(in, 21, true);
    // Column major order
    for(int j = 0; j < 3; ++j)
      for(int i = 0; i < 3; ++i)
	lp->cf_to_rc[i][j] = read_size<double>(in, 21, true);
    res->cconv = boost::make_shared<LocalRcConverter>(lp);
  } else {
    Exception e;
    e << "Unrecognized ground domain form. We got " << conv_type << "but only recognize G, H and R";
    throw e;
  }
  res->ground_domain_vertex_.resize(8);
  for(int i = 0; i < 8; ++i) {
    double x = read_size<double>(in, 21, true);
    double y = read_size<double>(in, 21, true);
    double z = read_size<double>(in, 21, true);
    res->ground_domain_vertex_[i] =
      res->cconv->convert_from_coordinate(x, y, z);
  }
  boost::optional<double> x = read_size<boost::optional<double> >(in, 21, true);
  boost::optional<double> y = read_size<boost::optional<double> >(in, 21, true);
  boost::optional<double> z = read_size<boost::optional<double> >(in, 21, true);
  if(x)
    res->ground_reference_point_ = res->cconv->convert_from_coordinate(*x, *y, *z);
  res->full_number_line_ = read_size<boost::optional<int> >(in, 8, true);
  res->full_number_sample_ = read_size<boost::optional<int> >(in, 8, true);
  res->min_line_ = read_size<int>(in, 8, true);
  res->max_line_ = read_size<int>(in, 8, true);
  res->min_sample_ = read_size<int>(in, 8, true);
  res->max_sample_ = read_size<int>(in, 8, true);

  // This is the illumination and sensor position. We don't currently
  // have this in place, we'll need to add this in.
  for(int i = 0; i < 21; ++i) {
    boost::optional<double> x = read_size<boost::optional<double> >(in, 21, true);
    (void) x;			// Suppress unused variable warning
  }
  check_end_of_stream(in);
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmIdTiming::print(std::ostream& Os) const
{
  Os << "RsmIdTiming:\n"
     << "  Number row acquired simultaneously: "
     << number_row_acquired_simultaneously_ << "\n"
     << "  Number col acquired simultaneously: "
     << number_col_acquired_simultaneously_ << "\n"
     << "  Time between adjacent row group:    "
     << time_between_adjacent_row_group_ << "\n"
     << "  Time between adjacent col group:    "
     << time_between_adjacent_col_group_ << "\n";
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmId::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "RsmId:\n"
     << "  Image Identifier:          " << image_identifier_ << "\n"
     << "  RSM Edition:               " << rsm_support_data_edition_ << "\n"
     << "  Image Sequence Identifier: " << image_sequence_identifier_ << "\n"
     << "  Sensor Identifier:         " << sensor_identifier_ << "\n"
     << "  Sensor type:               " << sensor_type_ << "\n"
     << "  Image acqusition time:     ";
  if(image_acquistion_time_)
    Os << *image_acquistion_time_ << "\n";
  else
    Os << "Not used\n";
  Os << "  Timing:                    ";
  if(timing_) {
    Os << "\n";
    opad << *timing_ << "\n";
    opad.strict_sync();
  } else
    Os << "Not used\n";
}

//-----------------------------------------------------------------------
/// Set the NAIF code for the planet/body we are working with.
///
/// Note that the NITF TRE structure does not have a place to store
/// the NAIF code, it implicitly assumes earth. So when we read a TRE,
/// even for something like Mars, we have the NAIF code set to
/// earth. We need to update this with other metadata
/// (e.g. TARGET_NAME in PDS label).
///
/// This is not a problem for boost serialization (which keeps the
/// NAIF code), just for NITF TRE.
//-----------------------------------------------------------------------

void RsmId::naif_code(int Naif_code)
{
  const GeodeticRadianConverter* gconv =
    dynamic_cast<const GeodeticRadianConverter*>(coordinate_converter().get());
  const GeodeticRadian2piConverter* gconv2 =
    dynamic_cast<const GeodeticRadian2piConverter*>
    (coordinate_converter().get());
  const LocalRcConverter* gconv3 = 
    dynamic_cast<const LocalRcConverter*>(coordinate_converter().get());
  if(gconv) {
    if(Naif_code != Ecr::EARTH_NAIF_CODE)
      throw Exception("Right now, GeodeticRadianConverter is not supported for anything other than Earth");
    return;
  }
  if(gconv2) {
    if(Naif_code != Ecr::EARTH_NAIF_CODE)
      throw Exception("Right now, GeodeticRadian2piConverter is not supported for anything other than Earth");
    return;
  }
  if(gconv3) {
    boost::shared_ptr<LocalRcParameter> p = gconv3->parameter();
    if(Naif_code == Ecr::EARTH_NAIF_CODE)
      p->cf_prototype = boost::make_shared<Ecr>();
    else
      p->cf_prototype = boost::make_shared<PlanetFixed>(Naif_code);
    return;
  }
  throw Exception("Unsupported coordinate converter in naif_code");
}
