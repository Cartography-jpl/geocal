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
void ConstantSpacingFrameletTimeTable::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(TimeTable);
  GEOCAL_BASE(ConstantSpacingFrameletTimeTable, TimeTable);
  ar & GEOCAL_NVP(t_min_line) & GEOCAL_NVP(max_l)
    & GEOCAL_NVP_(framelet_size)
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
GEOCAL_IMPLEMENT(ConstantSpacingFrameletTimeTable);
GEOCAL_IMPLEMENT(MeasuredTimeTable);
#endif

//-----------------------------------------------------------------------
/// Sometimes we want to know the acquisition times around a
/// particular time. For a simple pushbroom camera, this is just the
/// times for an integral image coordinates around that time. But this
/// can be a little more complicated, e.g, ConstantSpacingFrameletTimeTable  
/// it would be the framelet times surround the given time. This is
/// returned as the time for the smaller image line index first, and
/// the time of the larger line index second (so for a time table
/// increasing in time this t1 < t2. For a time table in descreasing
/// time t1 > t2).
//-----------------------------------------------------------------------

void TimeTable::time_acquisition(const Time& T, const FrameCoordinate& Fc,
			    Time& T1, Time& T2) const
{
  // This default implementation should work for a good number of Time
  // tables. Derived class should override this if this isn't
  // appropriate.
  ImageCoordinate ic = image_coordinate(T, Fc);
  ic.line = floor(ic.line);
  FrameCoordinate fc;
  time(ic, T1, fc);
  ic.line += 1;
  time(ic, T2, fc);
}

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
/// Constructor, creates time table from Time_min_line to
/// Time_max_line with given Time spacing.
/// We adjust Max_time to exactly Time_min_line + i *
/// Time_space, rounding to nearest integer i, so it ok if Max_time is
/// a little sloppy.
/// Note Time_space can be negative, and Time_max_line < Time_min_line
//-----------------------------------------------------------------------
  
ConstantSpacingFrameletTimeTable::ConstantSpacingFrameletTimeTable
(Time Time_min_line, 
 Time Time_max_line, int Framelet_size, double Time_space)
: t_min_line(Time_min_line),
  framelet_size_(Framelet_size),
  tspace(Time_space)
{
  if(Time_space > 0)
    range_min_check(Time_max_line, Time_min_line);
  else
    range_min_check(Time_min_line, Time_max_line);
  max_l = ((int) round((Time_max_line - Time_min_line) / tspace)) * framelet_size_;
}

//-----------------------------------------------------------------------
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

ImageCoordinate ConstantSpacingFrameletTimeTable::image_coordinate(Time T, 
const FrameCoordinate& F) const
{
  range_check_inclusive(T, min_time(), max_time());
  double line = (T  - t_min_line) / tspace * framelet_size_ + F.line;
  return ImageCoordinate(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from TimeWithDerivative and FrameCoordinateWithDerivative 
/// to ImageCoordinateWithDerivative.
//-----------------------------------------------------------------------

ImageCoordinateWithDerivative 
ConstantSpacingFrameletTimeTable::image_coordinate_with_derivative
(const TimeWithDerivative& T, 
 const FrameCoordinateWithDerivative& F) const
{
  range_check_inclusive(T.value(), min_time(), max_time());
  AutoDerivative<double> line = (T  - t_min_line) / tspace * framelet_size_ +
    F.line;
  return ImageCoordinateWithDerivative(line, F.sample);
}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

void ConstantSpacingFrameletTimeTable::time(const ImageCoordinate& Ic, Time& T, 
				    FrameCoordinate& F) const
{
  // We add a border of 1 line to handle edge cases.
  range_check_inclusive(Ic.line, (double) min_line() - framelet_size_,
			(double) max_line() + framelet_size_);
  // Plus 0.5 to handle border, so -0.02 goes with time for line 0,
  // not for line -14.
  T = t_min_line + floor((Ic.line+0.5) / framelet_size_) * tspace;
  F = FrameCoordinate(Ic.line -
		      floor((Ic.line+0.5) / framelet_size_) * framelet_size_,
		      Ic.sample);
}


//-----------------------------------------------------------------------
/// Convert from ImageCoordinateWithDerivative to TimeWithDerivative 
/// and FrameCoordinateWithDerivative.
//-----------------------------------------------------------------------

void ConstantSpacingFrameletTimeTable::time_with_derivative
(const ImageCoordinateWithDerivative& Ic, TimeWithDerivative& T, 
 FrameCoordinateWithDerivative& F) const
{
  range_check_inclusive(Ic.line.value(), (double) min_line() - framelet_size_,
			(double) max_line() + framelet_size_);
  T = TimeWithDerivative(t_min_line) +
    floor((Ic.line.value()+0.5) / framelet_size_) * tspace;
  F = FrameCoordinateWithDerivative(Ic.line -
	    floor((Ic.line.value()+0.5) / framelet_size_) * framelet_size_,
	    Ic.sample);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void ConstantSpacingFrameletTimeTable::print(std::ostream& Os) const
{
  Os << "Constant spacing framelet Time Table:\n"
     << "  Min line:      " << min_line() << "\n"
     << "  Max line:      " << max_line() << "\n"
     << "  Min time:      " << min_time() << "\n"
     << "  Max time:      " << max_time() << "\n"
     << "  Time spacing:  " << tspace << "s\n"
     << "  Framelet size: " << framelet_size() << "\n";
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

// See base class for description
void ConstantSpacingFrameletTimeTable::time_acquisition
(const Time& T, const FrameCoordinate& Fc, Time& T1, Time& T2) const
{
  ImageCoordinate ic = image_coordinate(T, Fc);
  ic.line = floor(ic.line / framelet_size_) * framelet_size_;
  FrameCoordinate fc;
  time(ic, T1, fc);
  ic.line += framelet_size_;
  time(ic, T2, fc);
}
