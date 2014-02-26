#include "time_table.h"
#include "geocal_exception.h"
#include <cmath>

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
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

void ConstantSpacingTimeTable::time(const ImageCoordinate& Ic, Time& T, 
				    FrameCoordinate& F) const
{
  range_check(Ic.line, (double) min_line(), (double) max_line() + 0.5);
  T = min_time() + Ic.line * tspace;
  F = FrameCoordinate(0, Ic.sample);
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


