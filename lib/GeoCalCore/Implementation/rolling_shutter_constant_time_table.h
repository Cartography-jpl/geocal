#ifndef ROLLING_SHUTTER_CONSTANT_TIME_TABLE_H
#define ROLLING_SHUTTER_CONSTANT_TIME_TABLE_H
#include "time_table.h"

namespace GeoCal {
/****************************************************************//**
  Time table for a rolling shutter with constant time spacing.
*******************************************************************/

class RollingShutterConstantTimeTable : public TimeTable {
public:
  RollingShutterConstantTimeTable(Time Min_time, Time Max_time, 
				  double Time_space = 40.8e-3)
    : min_t(Min_time),
      tspace(Time_space)
  {
    max_l = (int) round((Max_time - Min_time) / tspace);
  }
  virtual ~RollingShutterConstantTimeTable() {}
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const
  {
    range_check_inclusive(T, min_time(), max_time());
    double line = (T  - min_time()) / tspace;
    return ImageCoordinate(line, F.sample);
  }
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative& T, 
				   const FrameCoordinateWithDerivative& F)
    const
  {
    range_check_inclusive(T.value(), min_time(), max_time());
    AutoDerivative<double> line = (T  - min_time()) / tspace;
    return ImageCoordinateWithDerivative(line, F.sample);
  }
  virtual void print(std::ostream& Os) const
  { 
    Os << "Rolling Shutter Constant spacing Time Table:\n"
       << "  Min line:     " << min_line() << "\n"
       << "  Max line:     " << max_line() << "\n"
       << "  Min time:     " << min_time() << "\n"
       << "  Max time:     " << max_time() << "\n"
       << "  Time spacing: " << tspace << "s\n";
  }
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const
  {
    range_check(Ic.line, (double) min_line(), (double) max_line() + 1.0);
    T = min_time() + Ic.line * tspace;
    F = FrameCoordinate(Ic.line, Ic.sample);
  }
  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
				    TimeWithDerivative& T, 
				    FrameCoordinateWithDerivative& F) const
  {
    range_check(Ic.line.value(), (double) min_line(), 
		(double) max_line() + 1.0);
    T = TimeWithDerivative(min_time()) + Ic.line * tspace;
    F = FrameCoordinateWithDerivative(Ic.line, Ic.sample);
  }
  virtual int min_line() const {return 0;}
  virtual int max_line() const {return max_l;}
  virtual Time min_time() const {return min_t;}
  virtual Time max_time() const {return min_t + tspace * max_l;}
  double time_space() const {return tspace;}
private:
  Time min_t;
  int max_l;
  double tspace;
};

}
#endif
