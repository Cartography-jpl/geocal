#ifndef RAY_CASTER_H
#define RAY_CASTER_H
#include "printable.h"
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  This class is used to do ray casting, determining what in instrument
  is looking at on the ground (e.g., a camera at a particular
  pose). This includes the calculation of obscuration. 

  This class chooses what order it is going to go through the various
  camera positions. You can step to the next position, getting the
  ground locations for that position, by calling "next_position". All
  positions have been stepped through from start_position() to
  start_position() + number_position() - 1 (in whatever order the
  class wants) when last_position() returns true.The current position
  can be queried by "current_position". The mapping of position to
  something like image line depends on the particular derived class
  used, so for example a push broom camera might have the position
  corresponding to line number.
*******************************************************************/
class RayCaster: public Printable<RayCaster> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------
  RayCaster() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~RayCaster() {}

//-----------------------------------------------------------------------
/// The smallest position covered by this class.
//-----------------------------------------------------------------------
  
  virtual int start_position() const = 0;

//-----------------------------------------------------------------------
/// The number of position covered by this class.
//-----------------------------------------------------------------------
  
  virtual int number_position() const = 0;

//-----------------------------------------------------------------------
/// Returns true when we have stepped through all positions from 
/// start_position() to start_position() + number_position() - 1
//-----------------------------------------------------------------------

  virtual bool last_position() const = 0;

//-----------------------------------------------------------------------
/// The position that was last returned by next_position
//-----------------------------------------------------------------------

  virtual int current_position() const = 0;

//-----------------------------------------------------------------------
/// Return the ground locations seen at the next position. This is
/// This is nline x nsamp x nsub_line x nsub_sample x
/// nintegration_step x 3 in size, where we give the ground location
/// as a CartesianFixed coordinate (e.g., Ecr for the Earth).
///
/// Note that this array should be considered "owned" by this class,
/// code calling this class should copy this data if it wants to make
/// any modifications to the underlying data.
//-----------------------------------------------------------------------
  
  virtual blitz::Array<double, 6> next_position() = 0;

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
};
}
#endif
