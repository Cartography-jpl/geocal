#ifndef DISPLAY_STRETCH_STD_DEV_H
#define DISPLAY_STRETCH_STD_DEV_H
#include "display_stretch.h"
#include "geocal_exception.h"
namespace GeoCal {
/****************************************************************//**
  This stretches data linearly between a two values.
  
  The values are given by min_dev and max_dev, the default is to use 2
  sigma.

  Note that the advantage of this is simplicity. For most images, it
  doesn't give a particularly good stretch for viewing the data. If
  you are looking for a good default, then do DisplayStretchStdDev. 
*******************************************************************/

class DisplayStretchStdDev : public DisplayStretch {
public:
//-----------------------------------------------------------------------
/// Constructor. We stretch between mean + sigma * Min_sig to mean +
/// sigma * Max_sig.
//-----------------------------------------------------------------------
  DisplayStretchStdDev(double Min_sig = -2.0, double Max_sig = 2.0)
    : min_sig(Min_sig), max_sig(Max_sig)
  { range_max_check(Min_sig, Max_sig); }
  virtual ~DisplayStretchStdDev() {}
  virtual void stretch(const blitz::Array<int, 2>& Raw_data, 
       blitz::Array<unsigned char, 2>& Stretched_data) const;
  virtual void print(std::ostream& Os) const { Os << "DisplayStretchStdDev"; }
private:
  double min_sig, max_sig;
};
}
#endif

