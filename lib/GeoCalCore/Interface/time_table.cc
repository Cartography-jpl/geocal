#include "time_table.h"
#include "geocal_exception.h"
#include <cmath>
#include <algorithm>
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void TimeTable::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(TimeTable);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(TimeTable, WithParameter);
}

template<class Archive>
void ConstantSpacingTimeTable::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(TimeTable);
  GEOCAL_BASE(ConstantSpacingTimeTable, TimeTable);
  // We renamed min_t to t_min_line when we added support for negative
  // time spacing. Not worth updating the serialization, so we just
  // use the old name when saving.
  ar & GEOCAL_NVP2("min_t", t_min_line) & GEOCAL_NVP(max_l)
    & GEOCAL_NVP(tspace);
}

template<class Archive>
void MeasuredTimeTable::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(TimeTable);
  GEOCAL_BASE(MeasuredTimeTable, TimeTable);
  ar & GEOCAL_NVP_(min_line)
    & GEOCAL_NVP(tlist);
}

GEOCAL_IMPLEMENT(TimeTable);
GEOCAL_IMPLEMENT(ConstantSpacingTimeTable);
GEOCAL_IMPLEMENT(MeasuredTimeTable);
#endif

//-----------------------------------------------------------------------
/// Constructor, creates time table from Time_min_line to
/// Time_max_line with given Time spacing.
/// We adjust Max_time to exactly Time_min_line + i *
/// Time_space, rounding to nearest integer i, so it ok if Max_time is
/// a little sloppy.
/// Note Time_space can be negative, and Time_max_line < Time_min_line
//-----------------------------------------------------------------------
  
ConstantSpacingTimeTable::ConstantSpacingTimeTable(Time Time_min_line, 
	   Time Time_max_line, double Time_space)
: t_min_line(Time_min_line),
  tspace(Time_space)
{
  if(Time_space > 0)
    range_min_check(Time_max_line, Time_min_line);
  else
    range_min_check(Time_min_line, Time_max_line);
  max_l = (int) round((Time_max_line - Time_min_line) / tspace);
}

//-----------------------------------------------------------------------
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

ImageCoordinate ConstantSpacingTimeTable::image_coordinate(Time T, 
const FrameCoordinate& F) const
{
  range_check_inclusive(T, min_time(), max_time());
  double line = (T  - t_min_line) / tspace + F.line;
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
  range_check_inclusive(T.value(), min_time(), max_time());
  AutoDerivative<double> line = (T  - t_min_line) / tspace + F.line;
  return ImageCoordinateWithDerivative(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

void ConstantSpacingTimeTable::time(const ImageCoordinate& Ic, Time& T, 
				    FrameCoordinate& F) const
{
  // We add a border of 1 line to handle edge cases.
  range_check_inclusive(Ic.line, (double) min_line() - 1.0,
			(double) max_line() + 1.0);
  T = t_min_line + Ic.line * tspace;
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
  range_check_inclusive(Ic.line.value(), (double) min_line() - 1.0,
			(double) max_line() + 1.0);
  T = TimeWithDerivative(t_min_line) + Ic.line * tspace;
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
///
/// We often have trouble with edge cases (so time 1 ms before start
/// of table). We pad the table with a single line extrapolation.
///
/// We currently assume that the timing is monotonic increasing. We
/// could probably relax that if useful.
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
  // Add padding to table.
  Time tlist_pad = tlist[0] - (tlist[1] - tlist[0]);
  tlist.insert(tlist.begin(), tlist_pad);
  auto i = tlist.size() - 1;
  Time tlist_pad2 = tlist[i] + (tlist[i] - tlist[i-1]);
  tlist.push_back(tlist_pad2);
}

//-----------------------------------------------------------------------
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

ImageCoordinate MeasuredTimeTable::image_coordinate(Time T, 
const FrameCoordinate& F) const
{
  range_check_inclusive(T, min_time(), max_time());
  int i = (int)(std::lower_bound(tlist.begin(), tlist.end(), T)
		- tlist.begin());
  double line;
  if(i == 0)
    line = min_line_ - 1;
  else {
    line = (T - tlist[i - 1]) / (tlist[i] - tlist[i - 1]) + (i - 1) + 
      min_line_ - 1;
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
  range_check_inclusive(T.value(), min_time(), max_time());
  int i = (int)(std::lower_bound(tlist.begin(), tlist.end(), T.value())
		- tlist.begin());
  AutoDerivative<double> line;
  if(i == 0)
    line = min_line_ - 1;
  else {
    line = (T - tlist[i - 1]) / (tlist[i] - tlist[i - 1]) + (i - 1) + 
      min_line_ - 1;
  }
  return ImageCoordinateWithDerivative(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

void MeasuredTimeTable::time(const ImageCoordinate& Ic, Time& T, 
			     FrameCoordinate& F) const
{
  range_check_inclusive(Ic.line, (double) min_line() - 1.0,
			(double) max_line() + 1.0);
  int i = (int) floor(Ic.line);
  int j = i - (min_line_ - 1);
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
  range_check_inclusive(Ic.line.value(), (double) min_line() - 1.0,
			(double) max_line() + 1.0);
  int i = (int) floor(Ic.line.value());
  int j = i - (min_line_ - 1);
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
