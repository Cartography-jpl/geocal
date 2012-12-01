#ifndef DISPLAY_STRETCH_H
#define DISPLAY_STRETCH_H
#include "printable.h"
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  This stretches data for displaying as 8 bit data.
*******************************************************************/

class DisplayStretch : public PrintAndMarkable<DisplayStretch> {
public:
  virtual ~DisplayStretch() {}

//-----------------------------------------------------------------------
/// Take the given data and stretch it to 8 bit display data. We take
/// the array to write this to, because often we already have a sink
/// for the data (e.g., creating a gdk_pixbuf).
//-----------------------------------------------------------------------

  virtual void stretch(const blitz::Array<int, 2>& Raw_data, 
       blitz::Array<unsigned char, 2>& Stretched_data) const  = 0;

  virtual void print(std::ostream& Os) const { Os << "DisplayStretch"; }
};
}
#endif

