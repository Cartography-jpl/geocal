#ifndef TIME_TABLE_H
#define TIME_TABLE_H
#include "printable.h"
#include "geocal_time.h"
#include "frame_coordinate.h"
#include "image_coordinate.h"
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class is used to relate time to image line number and vice
  versa.

  Often the relationship is pretty simply, there is just a fixed time
  interval between one line and the next. However this class can be
  used to model any instrument complications that need to be modelled
  (e.g., missing lines, drift in spacing).
*******************************************************************/

class TimeTable : public Printable<TimeTable> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  TimeTable() {}
  virtual ~TimeTable() {}

//-----------------------------------------------------------------------
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const = 0;

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const = 0;

//-----------------------------------------------------------------------
/// Minimum line table is valid for.
//-----------------------------------------------------------------------

  virtual int min_line() const = 0;

//-----------------------------------------------------------------------
/// Maximum line table is valid for.
//-----------------------------------------------------------------------

  virtual int max_line() const = 0;

//-----------------------------------------------------------------------
/// Minimum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time min_time() const = 0;

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time max_time() const = 0;

  virtual void print(std::ostream& Os) const = 0;
};

/****************************************************************//**
  This is a time table that has a constant spacing between lines.
*******************************************************************/

class ConstantSpacingTimeTable : public TimeTable {
public:
  ConstantSpacingTimeTable(Time Min_time, Time Max_time, 
			   double Time_space = 40.8e-3);
  virtual ~ConstantSpacingTimeTable() {}
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  virtual void print(std::ostream& Os) const;
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const;

//-----------------------------------------------------------------------
/// Minimum line table is valid for.
//-----------------------------------------------------------------------

  virtual int min_line() const {return 0;}

//-----------------------------------------------------------------------
/// Maximum line table is valid for.
//-----------------------------------------------------------------------

  virtual int max_line() const {return max_l;}

//-----------------------------------------------------------------------
/// Minimum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time min_time() const {return min_t;}

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time max_time() const {return min_t + tspace * max_l;}

  double time_space() const {return tspace;}
private:
  Time min_t;
  int max_l;
  double tspace;
};

/****************************************************************//**
  This is a time table that has a time associated with each line.
*******************************************************************/

class MeasuredTimeTable : public TimeTable {
public:
  MeasuredTimeTable(const std::vector<Time>& Time_list,
		    int Min_line = 0);
  virtual ~MeasuredTimeTable() {}
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  virtual void print(std::ostream& Os) const;
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const;

//-----------------------------------------------------------------------
/// Minimum line table is valid for.
//-----------------------------------------------------------------------

  virtual int min_line() const {return min_line_;}

//-----------------------------------------------------------------------
/// Maximum line table is valid for.
//-----------------------------------------------------------------------

  virtual int max_line() const {return min_line_ + (int) tlist.size() - 1;}

//-----------------------------------------------------------------------
/// Minimum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time min_time() const {return tlist.front();}

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time max_time() const {return tlist.back();}

//-----------------------------------------------------------------------
/// List of times
//-----------------------------------------------------------------------
  const std::vector<Time>& time_list() const {return tlist;}
private:
  int min_line_;
  std::vector<Time> tlist;
};

}
#endif

