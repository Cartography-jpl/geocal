#ifndef TOOLKIT_TIME_INTERFACE_H
#define TOOLKIT_TIME_INTERFACE_H
#include "geocal_exception.h"

namespace GeoCal {
class Time;

/****************************************************************//**
  Each toolkit supplies methods to convert to and from strings.
*******************************************************************/
class ToolkitTimeInterface {
public:
  virtual ~ToolkitTimeInterface() {}

//-----------------------------------------------------------------------
/// Parse string to get a Time. The specific formats that are
/// recognized depends on the toolkit used, but they all support CCSDS 
/// ASCII format (e.g., "1996-07-03T04:13:57.987654Z")
//-----------------------------------------------------------------------

  virtual Time parse_time(const std::string Time_string) const = 0;

//-----------------------------------------------------------------------
/// Generate CCSDS format of time (e.g., "1996-07-03T04:13:57.987654Z")
//-----------------------------------------------------------------------

  virtual std::string to_string(const Time& T) const = 0;
};

}
#endif
