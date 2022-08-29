#ifndef TIME_TABLE_H
#define TIME_TABLE_H
#include "printable.h"
#include "geocal_time.h"
#include "frame_coordinate.h"
#include "image_coordinate.h"
#include "with_parameter.h"
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

class TimeTable : public Printable<TimeTable>, public WithParameter {
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
/// Convert from Time and FrameCoordinate to ImageCoordinate.
//-----------------------------------------------------------------------

  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative& T, 
				   const FrameCoordinateWithDerivative& F)
    const = 0;

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const = 0;

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate to Time and FrameCoordinate.
//-----------------------------------------------------------------------

  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
				    TimeWithDerivative& T, 
				    FrameCoordinateWithDerivative& F)
    const = 0;

  virtual void time_acquisition(const Time& T, const FrameCoordinate& Fc,
				Time& T1, Time& T2) const;

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
///  
/// *Note* often padding is added, so this is not necessarily the time
/// of the minimum line.
///
/// Also, there is no requirement that the time table is in increasing
/// time, or is even monotonic. So the min_time is the minimum time
/// that image_coordinate is valid for, not necessarily the time for
/// the minimum line.
//-----------------------------------------------------------------------

  virtual Time min_time() const = 0;

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
///
/// *Note* often padding is added, so this is not necessarily the time
/// of the maximum line.
///
/// Also, there is no requirement that the time table is in increasing
/// time, or is even monotonic. So the max_time is the maximum time
/// that image_coordinate is valid for, not necessarily the time for
/// the maximum line.
//-----------------------------------------------------------------------

  virtual Time max_time() const = 0;

  virtual void print(std::ostream& Os) const = 0;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a time table that has a constant spacing between lines.
*******************************************************************/

class ConstantSpacingTimeTable : public TimeTable {
public:
  ConstantSpacingTimeTable(Time Time_min_line, Time Time_max_line, 
			   double Time_space = 40.8e-3);
  virtual ~ConstantSpacingTimeTable() {}
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative& T, 
				   const FrameCoordinateWithDerivative& F)
    const;
  virtual void print(std::ostream& Os) const;
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const;
  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
				    TimeWithDerivative& T, 
				    FrameCoordinateWithDerivative& F) const;

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
///
/// Note we often have trouble with boundary cases, so something like
/// like a time 1ms before the end edge of this time table. We add
/// a border of tspace to the min_time.  
//-----------------------------------------------------------------------

  virtual Time min_time() const
  {
    return (tspace > 0 ? t_min_line - tspace :
	    t_min_line + tspace * max_l + tspace);
  }

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
///
/// Note we often have trouble with boundary cases, so something like
/// like a time 1ms before the end edge of this time table. We add
/// a border of tspace to the max_time.  
//-----------------------------------------------------------------------

  virtual Time max_time() const
  {
    return (tspace > 0 ? t_min_line + tspace * max_l + tspace:
	    t_min_line - tspace);
  }
  double time_space() const {return tspace;}
private:
  Time t_min_line;
  int max_l;
  double tspace;
  ConstantSpacingTimeTable() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  Very similar to a ConstantSpacingTimeTable, but we have one than
  one frame line per time. This is like the time table for a 
  push frame camera.
*******************************************************************/

class ConstantSpacingFrameletTimeTable : public TimeTable {
public:
  ConstantSpacingFrameletTimeTable(Time Time_min_line, Time Time_max_line,
				  int Framelet_size,
				  double Time_space);
  virtual ~ConstantSpacingFrameletTimeTable() {}
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative& T, 
				   const FrameCoordinateWithDerivative& F)
    const;
  virtual void print(std::ostream& Os) const;
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const;
  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
				    TimeWithDerivative& T, 
				    FrameCoordinateWithDerivative& F) const;
  virtual void time_acquisition(const Time& T, const FrameCoordinate& Fc,
				Time& T1, Time& T2) const;

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
///
/// Note we often have trouble with boundary cases, so something like
/// like a time 1ms before the end edge of this time table. We add
/// a border of tspace to the min_time.  
//-----------------------------------------------------------------------

  virtual Time min_time() const
  {
    return (tspace > 0 ? t_min_line - tspace :
	    t_min_line + tspace * max_l / framelet_size_ + tspace);
  }

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
///
/// Note we often have trouble with boundary cases, so something like
/// like a time 1ms before the end edge of this time table. We add
/// a border of tspace to the max_time.  
//-----------------------------------------------------------------------

  virtual Time max_time() const
  {
    return (tspace > 0 ? t_min_line + tspace * max_l / framelet_size_ + tspace:
	    t_min_line - tspace);
  }
  double time_space() const {return tspace;}
  int framelet_size() const { return framelet_size_;}
private:
  Time t_min_line;
  int max_l;
  int framelet_size_;
  double tspace;
  ConstantSpacingFrameletTimeTable() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
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
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative& T, 
				   const FrameCoordinateWithDerivative& F)
    const;
  virtual void print(std::ostream& Os) const;
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const;
  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
				    TimeWithDerivative& T, 
				    FrameCoordinateWithDerivative& F) const;

//-----------------------------------------------------------------------
/// Minimum line table is valid for.
//-----------------------------------------------------------------------

  virtual int min_line() const {return min_line_;}

//-----------------------------------------------------------------------
/// Maximum line table is valid for.
//-----------------------------------------------------------------------

  virtual int max_line() const {return min_line_ + (int) tlist.size() - 3;}

//-----------------------------------------------------------------------
/// Minimum time table is valid for.
///
/// Note padding has been added, this is *not* the time of min_line.
//-----------------------------------------------------------------------

  virtual Time min_time() const {return tlist.front();}

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
///
/// Note padding has been added, this is *not* the time of max_line.
//-----------------------------------------------------------------------

  virtual Time max_time() const {return tlist.back();}

//-----------------------------------------------------------------------
/// List of times. Note that std::vector<Time> doesn't play well with
/// python for reasons I've not bothered to track down. So instead we
/// just provide access to the underlying list and have python set
/// this up.
//-----------------------------------------------------------------------
  int size_time_list() const {return (int) tlist.size(); }
  Time time_list(int i) const 
  { range_check(i, i, size_time_list());
    return tlist[i];
  }
protected:
  int min_line_;
  std::vector<Time> tlist;
  MeasuredTimeTable() { }
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(TimeTable);
GEOCAL_EXPORT_KEY(ConstantSpacingTimeTable);
GEOCAL_EXPORT_KEY(ConstantSpacingFrameletTimeTable);
GEOCAL_EXPORT_KEY(MeasuredTimeTable);
#endif

