#ifndef QUICKBIRD_TIME_TABLE_H
#define QUICKBIRD_TIME_TABLE_H
#include "time_table.h"
#include <map>

namespace GeoCal {
/****************************************************************//**
  This reads a quickbird .IMD file to get time table information.

  Note that Quickbird uses the same convention that we use in our
  code, an integer location is the center of a pixel. So (0, 0) is the
  center of the upper left pixel.
*******************************************************************/

class QuickBirdTimeTable : public TimeTable {
public:
  QuickBirdTimeTable(const std::string& Fname);
  virtual ~QuickBirdTimeTable() {}
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative& T, 
				   const FrameCoordinateWithDerivative& F)
    const;
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const;
  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
				    TimeWithDerivative& T, 
				    FrameCoordinateWithDerivative& F)
    const;

//-----------------------------------------------------------------------
/// Underlying file name.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname; }

//-----------------------------------------------------------------------
/// Minimum line table is valid for.
//-----------------------------------------------------------------------

  virtual int min_line() const {return minl;}

//-----------------------------------------------------------------------
/// Maximum line table is valid for.
//-----------------------------------------------------------------------

  virtual int max_line() const {return maxl;}

//-----------------------------------------------------------------------
/// Minimum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time min_time() const {return mint;}

//-----------------------------------------------------------------------
/// Maximum time table is valid for.
//-----------------------------------------------------------------------

  virtual Time max_time() const {return maxt;}

  virtual void print(std::ostream& Os) const;
private:
  std::string fname;
  int minl;
  int maxl;
  Time mint;
  Time maxt;
  struct Interval {
    Interval() {}
    Interval(Time Tstart, int Lstart, double Tspace) 
     : tstart(Tstart), lstart(Lstart), tspace(Tspace) {}
    Time tstart;
    int lstart;
    double tspace;
  };
  // This maps between line and time, and vice versa. Because of the
  // way map works, we index this by the *end* of the range that
  // Interval is valid for. This is because lower_bound(k) finds the
  // smallest key value kv >= k, so we can find the interval to use
  // easily. 
  std::map<int, Interval> line_to_time;
  std::map<Time, Interval> time_to_line;
};

}
#endif
