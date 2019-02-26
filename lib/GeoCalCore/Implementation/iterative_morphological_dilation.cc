#include "iterative_morphological_dilation.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IterativeMorphologicalDilation::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IterativeMorphologicalDilation);
  //ar & GEOCAL_NVP(min_tm) & GEOCAL_NVP(max_tm);
}

GEOCAL_IMPLEMENT(IterativeMorphologicalDilation);
#endif

//-----------------------------------------------------------------------
/// Constructor.
///
/// The Mask is "true" where we don't have Image data and wish to fill
/// in data (i.e., this is the same sense as ImageMask)
///
/// Right now, this just works with blitz arrays. We could extend this
/// to work with RasterImage and ImageMask. But this is often called
/// from python, which interacts better with blitz arrays (which map
/// to numpy arrays in python).
//-----------------------------------------------------------------------

IterativeMorphologicalDilation::IterativeMorphologicalDilation
(const blitz::Array<double, 2>& Image,
 const blitz::Array<bool, 2>& Mask)
{
  if(Image.rows() != Mask.rows() ||
     Image.cols() != Mask.cols())
    throw Exception("Image and Mask need to be the same size");
}
