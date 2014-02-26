#ifndef FRAME_COORDINATE_H
#define FRAME_COORDINATE_H
#include "printable.h"

namespace GeoCal {

/****************************************************************//**
  This is the coordinates of a frame camera.
*******************************************************************/

class FrameCoordinate : public Printable<FrameCoordinate> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  FrameCoordinate() {}

//-----------------------------------------------------------------------
/// Create a FrameCoordinate with the given coordinates.
//-----------------------------------------------------------------------

  FrameCoordinate(double L, double S) :line(L), sample(S) {}
  double line;			///< Line number
  double sample;		///< Sample number
  void print(std::ostream& Os) const;
};
}

#endif
