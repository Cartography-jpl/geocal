#include "rsm_id.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmId::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmId);
  ar & GEOCAL_NVP_(image_identifier)
    & GEOCAL_NVP_(rsm_suport_data_edition)
    & GEOCAL_NVP_(image_sequence_identifier)
    & GEOCAL_NVP_(sensor_identifier)
    & GEOCAL_NVP_(sensor_type)
    & GEOCAL_NVP_(image_acquistion_time);
}

GEOCAL_IMPLEMENT(RsmId);
#endif

static boost::format f("%|1$-80s|%|2$-40s|%|3$-40|%|4$-40|%|5$-40|");
static boost::format tac("%|1$4s|%|2$2s|%|3$2s|%|4$2s|%|5$2s|%|6$9s|");
static boost::format holder("%|1$-59|");
static boost::format rsm_sz("%|1$08d|%|2$08d|%|3$08d|%|4$08d|%|5$08d|%|6$08d|");
static boost::format num_missing("%|1$21s|");
static boost::format num("%|1$+21.14E|");

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
  res += str_check_size(f % image_identifier_ % rsm_suport_data_edition_ %
			image_sequence_identifier_ % sensor_identifier_ %
			sensor_type_, 240);
  if(image_acquistion_time_) {
    std::string t(image_acquistion_time_->to_string());
    res += str_check_size(tac % t.substr(0, 4) % t.substr(5,2) %
			  t.substr(8,2) % t.substr(11,2) % t.substr(14,2) %
			  t.substr(17, 9), 21);
  } else
    res += str_check_size(tac % "" % "" % "" % "" % "" % "" % "", 21);
  res += str_check_size(holder % "", 59);
  for(int i = 0; i < 12; ++i)
    res += str_check_size(num_missing % "", 21);
  for(int i = 0; i < 24; ++i)
    res += str_check_size(num % 0.0, 21);
  for(int i = 0; i < 3; ++i)
    res += str_check_size(num_missing % "", 21);
  res += str_check_size(rsm_sz % 0 % 0 % 0 % 0 % 0 % 0, 8*6);
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
  res->rsm_suport_data_edition_ = read_size<std::string>(in, 40);
  res->image_sequence_identifier_ = read_size<std::string>(in, 40);
  res->sensor_identifier_ = read_size<std::string>(in, 40);
  res->sensor_type_ = read_size<std::string>(in, 40);
  boost::trim(res->image_identifier_);
  boost::trim(res->rsm_suport_data_edition_);
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
  } else {
    std::string trash = read_size<std::string>(in, 2 * 4 + 9);
  }
  std::string placeholder = read_size<std::string>(in, 59);
  for(int i = 0; i < 12; ++i)
    double x = read_size_nan(in, 21);
  for(int i = 0; i < 24; ++i)
    double x = read_size<double>(in, 21);
  for(int i = 0; i < 3; ++i)
    double x = read_size_nan(in, 21);
  for(int i = 0; i < 2; ++i)
    int x = read_size_fill(in, 8);
  for(int i = 0; i < 4; ++i)
    int x = read_size<int>(in, 8);
  for(int i = 0; i < 21; ++i)
    double x = read_size_nan(in, 21);
  check_end_of_stream(in);
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmId::print(std::ostream& Os) const
{
  Os << "RsmId:\n"
     << "  Image Identifier:   " << image_identifier_ << "\n"
     << "  RSM Edition:        " << rsm_suport_data_edition_ << "\n";
}
