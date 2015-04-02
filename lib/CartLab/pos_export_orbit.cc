#include "pos_export_orbit.h"
#include "aircraft_orbit_data.h"
#include "geocal_serialize_support.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <boost/regex.hpp>
#include <boost/exception/diagnostic_information.hpp> 
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PosExportOrbit::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(PosExportOrbit);
  ar & GEOCAL_NVP(fname) & GEOCAL_NVP_(file_epoch);
}

template<class Archive>
void PosExportOrbit::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(PosExportOrbit);
  ar & GEOCAL_NVP(fname) & GEOCAL_NVP_(file_epoch);
  init(fname, file_epoch_);
}

GEOCAL_SPLIT_IMPLEMENT(PosExportOrbit);
#endif

//-----------------------------------------------------------------------
/// Read the given text file. Note that the time in the Applanix file
/// is in GPS seconds of the week, but the actual week isn't in the
/// file. This means we need to pass in the Epoch that the times are
/// relative to.
//-----------------------------------------------------------------------

PosExportOrbit::PosExportOrbit(const std::string& Fname, const Time& Epoch)
{
  init(Fname, Epoch);
}

void PosExportOrbit::init(const std::string& Fname, const Time& Epoch)
{
  fname = Fname;
  file_epoch_ = Epoch;
  std::ifstream in(Fname.c_str());
  in.exceptions(std::ios_base::badbit);
  bool first_line = false;
  std::string ln;
  while(!first_line && !in.eof()) {
    // Look for first line that has all numbers. This skips the text
    // header.
    getline(in, ln);
    if(boost::regex_search(ln, boost::regex("\\d")) &&
       !(boost::regex_search(ln, boost::regex("[[:alpha:]]"))))
    first_line = boost::regex_search(ln, boost::regex("\\d")) &&
      !(boost::regex_search(ln, boost::regex("[[:alpha:]]")));
  }
  Geodetic last_point;
  Time last_tm;
  process_line(Epoch, ln, last_point, last_tm);
  while(!in.eof()) {
    process_line(Epoch, ln, last_point, last_tm);
    getline(in, ln);
  }
  initialize();
}

//-----------------------------------------------------------------------
/// Process a single line of data to create a AircraftOrbitData and
/// put it into orbit_data_map. We approximate the velocity by looking
/// at the last point put in the orbit_data_map.
//-----------------------------------------------------------------------

void PosExportOrbit::process_line(const Time& Epoch, const std::string& ln,
				  Geodetic& Last_pt, Time& Last_tm)
{
  double tm, trash, lat, lon, h, roll, pitch, heading;
  // Nothing wrong with the following code, but on Mac 10.6 code that
  // uses istringstream or ostringstream fails when called through
  // Ruby. I have no idea why, I've tried tracking the reason down but
  // without success. In any case, this is easy enough to rewrite
  // using C strod.

  // std::istringstream in(ln);
  // in.exceptions(std::ios_base::badbit|std::ios_base::failbit|
  // 		std::ios_base::eofbit);
  // in >> tm >> trash >> trash >> trash >> trash >> lat >> lon >> h >> roll 
  //    >> pitch >> heading;

  char* sptr = const_cast<char*>(ln.c_str());
  tm = strtod(sptr, &sptr);
  trash = strtod(sptr, &sptr);
  trash = strtod(sptr, &sptr);
  trash = strtod(sptr, &sptr);
  trash = strtod(sptr, &sptr);
  lat = strtod(sptr, &sptr);
  lon = strtod(sptr, &sptr);
  h = strtod(sptr, &sptr);
  roll = strtod(sptr, &sptr);
  pitch = strtod(sptr, &sptr);
  heading = strtod(sptr, &sptr);
  Time t = Epoch + tm;
  if(orbit_data_map.size() == 0) {
    boost::array<double, 3> vel = {{0.0,0.0,0.0}};
    orbit_data_map[t] = boost::shared_ptr<QuaternionOrbitData>
      (new AircraftOrbitData(t, Geodetic(lat, lon, h), vel, roll, pitch, 
			     heading));
  } else
    orbit_data_map[t] = boost::shared_ptr<QuaternionOrbitData>
      (new AircraftOrbitData(t, Geodetic(lat, lon, h), Last_tm, Last_pt, 
			     roll, pitch, heading));
  Last_tm = t;
  Last_pt = Geodetic(lat, lon, h);
}

//-----------------------------------------------------------------------
/// Return the AircraftOrbitData closest to a given Time. This doesn't
/// interpolate, it just return the data point in the file closest to
/// the given time.
//-----------------------------------------------------------------------

const AircraftOrbitData& 
PosExportOrbit::aircraft_orbit_data(const Time& T) const
{
  range_check(T, min_time(), max_time());
  time_map::const_iterator i = orbit_data_map.lower_bound(T);
  // Really do want "!= 0.0" rather than comparing with a
  // tolerance. This it to handle the edge case when T is exactly the
  // min_time().
  if(i->first - T != 0.0) {
    double tdiff1 = fabs((i->first - T));
    --i;
    double tdiff2 = fabs((i->first - T));
    if(tdiff1 < tdiff2)
      ++i;
  }
  return *(dynamic_cast<const AircraftOrbitData*>(i->second.get()));
}

