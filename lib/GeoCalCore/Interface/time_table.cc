#include "time_table.h"
#include "geocal_exception.h"
#include <cmath>
#include <algorithm>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor, creates time table from Min_time to Max_time with
/// given Time spacing. We adjust Max_time to exactly Min_time + i *
/// Time_space, rounding to nearest integer i, so it ok if Max_time is
/// a little sloppy.
//-----------------------------------------------------------------------
  
ConstantSpacingTimeTable::ConstantSpacingTimeTable(Time Min_time, 
	   Time Max_time, double Time_space)
: min_t(Min_time),
  tspace(Time_space)
{
  range_min_check(Time_space, 0.0);
  range_min_check(Max_time, Min_time);
  max_l = (int) round((Max_time - Min_time) / tspace);
}

//-----------------------------------------------------------------------
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

ImageCoordinate ConstantSpacingTimeTable::image_coordinate(Time T, 
const FrameCoordinate& F) const
{
  range_check(T, min_time(), max_time());
  double line = (T  - min_time()) / tspace + F.line;
  return ImageCoordinate(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from TimeWithDerivative and FrameCoordinateWithDerivative 
/// to ImageCoordinateWithDerivative.
//-----------------------------------------------------------------------

ImageCoordinateWithDerivative 
ConstantSpacingTimeTable::image_coordinate_with_derivative
(const TimeWithDerivative& T, 
 const FrameCoordinateWithDerivative& F) const
{
  range_check(T.value(), min_time(), max_time());
  AutoDerivative<double> line = (T  - min_time()) / tspace + F.line;
  return ImageCoordinateWithDerivative(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

void ConstantSpacingTimeTable::time(const ImageCoordinate& Ic, Time& T, 
				    FrameCoordinate& F) const
{
  range_check(Ic.line, (double) min_line(), (double) max_line() + 1.0);
  T = min_time() + Ic.line * tspace;
  F = FrameCoordinate(0, Ic.sample);
}


//-----------------------------------------------------------------------
/// Convert from ImageCoordinateWithDerivative to TimeWithDerivative 
/// and FrameCoordinateWithDerivative.
//-----------------------------------------------------------------------

void ConstantSpacingTimeTable::time_with_derivative
(const ImageCoordinateWithDerivative& Ic, TimeWithDerivative& T, 
 FrameCoordinateWithDerivative& F) const
{
  range_check(Ic.line.value(), (double) min_line(), (double) max_line() + 1.0);
  T = TimeWithDerivative(min_time()) + Ic.line * tspace;
  F = FrameCoordinateWithDerivative(0, Ic.sample);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void ConstantSpacingTimeTable::print(std::ostream& Os) const
{
  Os << "Constant spacing Time Table:\n"
     << "  Min line:     " << min_line() << "\n"
     << "  Max line:     " << max_line() << "\n"
     << "  Min time:     " << min_time() << "\n"
     << "  Max time:     " << max_time() << "\n"
     << "  Time spacing: " << tspace << "s\n";
}


//-----------------------------------------------------------------------
/// Constructor.
/// This gives the time for every line. This list should be strictly
/// ordered. The first time is for the given Min_line (default of 0).
//-----------------------------------------------------------------------

MeasuredTimeTable::MeasuredTimeTable(const std::vector<Time>& Time_list,
		    int Min_line)
: min_line_(Min_line),
  tlist(Time_list)
{
  for(int i = 0; i < (int) tlist.size() - 1; ++i) {
    if(tlist[i + 1] <= tlist[i])
      throw Exception("Time_list needs to be strictly ordered");
  }
}

//-----------------------------------------------------------------------
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

ImageCoordinate MeasuredTimeTable::image_coordinate(Time T, 
const FrameCoordinate& F) const
{
  range_check(T, min_time(), max_time());
  int i = (int)(std::lower_bound(tlist.begin(), tlist.end(), T)
		- tlist.begin());
  double line;
  if(i == 0)
    line = min_line_;
  else {
    line = (T - tlist[i - 1]) / (tlist[i] - tlist[i - 1]) + (i - 1) + 
      min_line_;
  }
  return ImageCoordinate(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from TimeWithDerivative and FrameCoordinateWithDerivative
/// to ImageCoordinateWithDerivative.
//-----------------------------------------------------------------------

ImageCoordinateWithDerivative 
MeasuredTimeTable::image_coordinate_with_derivative
(const TimeWithDerivative& T, 
const FrameCoordinateWithDerivative& F) const
{
  range_check(T.value(), min_time(), max_time());
  int i = (int)(std::lower_bound(tlist.begin(), tlist.end(), T.value())
		- tlist.begin());
  AutoDerivative<double> line;
  if(i == 0)
    line = min_line_;
  else {
    line = (T - tlist[i - 1]) / (tlist[i] - tlist[i - 1]) + (i - 1) + 
      min_line_;
  }
  return ImageCoordinateWithDerivative(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

void MeasuredTimeTable::time(const ImageCoordinate& Ic, Time& T, 
			     FrameCoordinate& F) const
{
  range_check(Ic.line, (double) min_line(), (double) max_line() + 1.0);
  int i = (int) floor(Ic.line);
  int j = i - min_line_;
  // Allow extrapolation by 1 line.
  while(j + 1 >= (int) tlist.size()) {
    j = j - 1;
    i = i - 1;
  }
  if(j < 0)
    throw Exception("MeasuredTimeTable must have at least 2 lines");
  T = tlist[j] + (tlist[j + 1] - tlist[j]) * (Ic.line - i);
  F = FrameCoordinate(0, Ic.sample);
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinateWithDerivative to TimeWithDerivative
/// and FrameCoordinateWithDerivative.
//-----------------------------------------------------------------------

void MeasuredTimeTable::time_with_derivative
(const ImageCoordinateWithDerivative& Ic, TimeWithDerivative& T, 
 FrameCoordinateWithDerivative& F) const
{
  range_check(Ic.line.value(), (double) min_line(), (double) max_line() + 1.0);
  int i = (int) floor(Ic.line.value());
  int j = i - min_line_;
  // Allow extrapolation by 1 line.
  while(j + 1 >= (int) tlist.size()) {
    j = j - 1;
    i = i - 1;
  }
  if(j < 0)
    throw Exception("MeasuredTimeTable must have at least 2 lines");
  T = TimeWithDerivative(tlist[j]) + (tlist[j + 1] - tlist[j]) * (Ic.line - i);
  F = FrameCoordinateWithDerivative(0, Ic.sample);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void MeasuredTimeTable::print(std::ostream& Os) const
{
  Os << "Measured Time Table:\n"
     << "  Min line:     " << min_line() << "\n"
     << "  Max line:     " << max_line() << "\n"
     << "  Min time:     " << min_time() << "\n"
     << "  Max time:     " << max_time() << "\n";
}
