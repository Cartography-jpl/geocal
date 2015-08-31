#include "igc_collection.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcCollection::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(IgcCollection, WithParameter);
}

GEOCAL_IMPLEMENT(IgcCollection);
#endif

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// parameters, calculated by taking a finite difference.
//-----------------------------------------------------------------------

blitz::Array<double, 2> 
IgcCollection::image_coordinate_jac_parm_fd
(int Image_index, const CartesianFixed& Gc,
 const blitz::Array<double, 1> Pstep) const
{ 
  blitz::Array<double, 1> p0 = parameter_subset();
  if(p0.rows() != Pstep.rows())
    throw Exception("Pstep needs to be the size of parameter_subset");
  ImageCoordinate ic0 = image_coordinate(Image_index, Gc);
  blitz::Array<double, 2> res(2, Pstep.rows());
  for(int i = 0; i < Pstep.rows(); ++i) {
    blitz::Array<double, 1> p(p0.copy());
    p(i) += Pstep(i);
    const_cast<IgcCollection*>(this)->parameter_subset(p);
    ImageCoordinate ic = image_coordinate(Image_index, Gc);
    res(0, i) = (ic.line - ic0.line) / Pstep(i);
    res(1, i) = (ic.sample - ic0.sample) / Pstep(i);
  }
  const_cast<IgcCollection*>(this)->parameter_subset(p0);
  return res;
}
