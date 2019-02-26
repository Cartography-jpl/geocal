#ifndef ITERATIVE_MORPHOLOGICAL_DILATION
#define ITERATIVE_MORPHOLOGICAL_DILATION
#include <blitz/array.h>
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple algorithm to fill in missing pixels in an image.
  This looks at all the pixels that are on the "edge" of the image -
  so pixels that aren't filled in but have at least one neighbor. We
  then fill in those pixels, and repeat the process iteratively. This
  is sometimes referred to as the "grass fire" algorithm.
*******************************************************************/
class IterativeMorphologicalDilation : public Printable<IterativeMorphologicalDilation> {
public:
  IterativeMorphologicalDilation(const blitz::Array<double, 2>& Image,
				 const blitz::Array<bool, 2>& Mask);
private:
  IterativeMorphologicalDilation() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(IterativeMorphologicalDilation);
#endif
  
  
