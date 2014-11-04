#include "quickbird_time_table.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/regex.hpp>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Read the given file.
//-----------------------------------------------------------------------

QuickBirdTimeTable::QuickBirdTimeTable(const std::string& Fname)
  : fname(Fname)
{
  std::ifstream in(Fname.c_str());
  in.exceptions(std::ifstream::eofbit | std::ifstream::failbit | 
		std::ifstream::badbit);
  std::string buf = "";
  while(buf.find("BEGIN_GROUP = IMAGE_1") == std::string::npos)
    getline(in, buf);
  while(buf.find("TLCTime") == std::string::npos)
    getline(in, buf);
  boost::smatch m;
  if(!regex_match(buf, m, boost::regex("\\s*TLCTime\\s*=\\s*([^ ]+)\\s*;")))
    throw Exception("File " + file_name() + " seems to be corrupt. Trouble reading TLCTime");
  Time tlctime = Time::parse_time(m[1]);
  while(buf.find("TLCList") == std::string::npos)
    getline(in, buf);
  std::vector<int> larr;
  std::vector<Time> tarr;
  while(buf.find(");") == std::string::npos) {
    getline(in, buf);
    std::stringstream ins(buf);
    char t1, t2;
    int line;
    double toffset;
    ins >> t1 >> line >> t2 >> toffset;
    larr.push_back(line);
    tarr.push_back(toffset + tlctime);
  }
  int lsize = (int) larr.size();
  minl = larr[0];
  maxl = larr[lsize - 1];
  mint = tarr[0];
  maxt = tarr[lsize - 1];
  for(int i = 0; i < lsize - 1; ++i) {
    Interval intv(tarr[i], larr[i], 
		  (tarr[i + 1] - tarr[i]) / (larr[i + 1] - larr[i]));
    line_to_time[larr[i + 1]] = intv;
    time_to_line[tarr[i + 1]] = intv;
  }
}

//-----------------------------------------------------------------------
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

ImageCoordinate 
QuickBirdTimeTable::image_coordinate(Time T, const FrameCoordinate& F) const
{
  range_check(T, mint, maxt);
  std::map<Time, Interval>::const_iterator i = time_to_line.lower_bound(T);
  if(i == time_to_line.end())
    throw Exception("This should be impossible");
  const Interval& intv = (*i).second;
  ImageCoordinate res;
  res.sample = F.sample;
  res.line = (T - intv.tstart) / intv.tspace + intv.lstart + F.line;
  return res;
}

//-----------------------------------------------------------------------
/// Convert from TimeWithDerivative and FrameCoordinateWithDerivative 
/// to ImageCoordinateWithDerivative.
//-----------------------------------------------------------------------

ImageCoordinateWithDerivative
QuickBirdTimeTable::image_coordinate_with_derivative
(const TimeWithDerivative& T, const FrameCoordinateWithDerivative& F) const
{
  range_check(T.value(), mint, maxt);
  std::map<Time, Interval>::const_iterator i = time_to_line.lower_bound(T.value());
  if(i == time_to_line.end())
    throw Exception("This should be impossible");
  const Interval& intv = (*i).second;
  ImageCoordinateWithDerivative res;
  res.sample = F.sample;
  res.line = (T - intv.tstart) / intv.tspace + intv.lstart + F.line;
  return res;
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

void QuickBirdTimeTable::time(const ImageCoordinate& Ic, Time& T, 
			      FrameCoordinate& F) const
{
  range_check(Ic.line, (double) minl, maxl + 0.5);
  std::map<int, Interval>::const_iterator i = 
    line_to_time.lower_bound((int) floor(Ic.line + 0.5));
  if(i == line_to_time.end())
    throw Exception("This should be impossible");
  const Interval& intv = (*i).second;
  F.line = 0;
  F.sample = Ic.sample;
  T = intv.tstart + (Ic.line - intv.lstart) * intv.tspace;
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinateWithDerivative to TimeWithDerivative 
/// and FrameCoordinateWithDerivative.
//-----------------------------------------------------------------------

void QuickBirdTimeTable::time_with_derivative
(const ImageCoordinateWithDerivative& Ic, TimeWithDerivative& T, 
 FrameCoordinateWithDerivative& F) const
{
  range_check(Ic.line.value(), (double) minl, maxl + 0.5);
  std::map<int, Interval>::const_iterator i = 
    line_to_time.lower_bound((int) floor(Ic.line.value() + 0.5));
  if(i == line_to_time.end())
    throw Exception("This should be impossible");
  const Interval& intv = (*i).second;
  F.line = 0;
  F.sample = Ic.sample;
  T = TimeWithDerivative(intv.tstart) + (Ic.line - intv.lstart) * intv.tspace;
}

//-----------------------------------------------------------------------
/// Print out time table to stream.
//-----------------------------------------------------------------------

void QuickBirdTimeTable::print(std::ostream& Os) const
{
  Os << "QuickBirdTimeTable:\n"
     << "  File:         " << fname << "\n"
     << "  Minimum line: " << min_line() << "\n"
     << "  Maximum line: " << max_line() << "\n"
     << "  Minimum time: " << min_time() << "\n"
     << "  Maximum time: " << max_time() << "\n";
}

