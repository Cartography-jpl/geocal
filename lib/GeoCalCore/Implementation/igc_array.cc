#include "igc_array.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcArray::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(IgcCollection, WithParameter);
  GEOCAL_BASE(IgcArray, IgcCollection);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(WithParameterNested);
  ar & GEOCAL_NVP(igc_list);
}

GEOCAL_IMPLEMENT(IgcArray);
#endif

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

IgcArray::IgcArray
(const std::vector<boost::shared_ptr<ImageGroundConnection> >& Igc_list)
    : igc_list(Igc_list) 
{
  BOOST_FOREACH(boost::shared_ptr<ImageGroundConnection> igc, igc_list)
    add_object(igc);
}

// See base class for description.
void IgcArray::print(std::ostream& Os) const
{
  Os << "IgcArray:\n"
     << "  Number of images: " << number_image() << "\n"
     << "  Images:\n";
  for(int i = 0; i < number_image(); ++i)
    Os << "    " << title(i) << "\n";
  Os << "  Parameter:\n";
  blitz::Array<double, 1> p = parameter_subset();
  std::vector<std::string> pname = parameter_name_subset();
  for(int i = 0; i < p.rows(); ++i)
    Os << "    " << pname[i] << ": " << p(i) << "\n";
}

// See base class for description.
boost::shared_ptr<IgcCollection> 
IgcArray::subset(const std::vector<int>& Index_set) const
{
  std::vector<boost::shared_ptr<ImageGroundConnection> > igclist;
  BOOST_FOREACH(int i, Index_set)
    igclist.push_back(image_ground_connection(i));
  return boost::shared_ptr<IgcCollection>(new IgcArray(igclist));
}

blitz::Array<double, 2> 
IgcArray::collinearity_residual_jacobian
(int Image_index,
 const GroundCoordinate& Gc,
 const ImageCoordinate& Ic_actual) const
{
  range_check(Image_index, 0, number_image());
  // The jacobian calculated by the ImageGroundConnection is relative
  // to those parameters only, we need to translate to the full jacobian.
  int nvar = parameter_subset().rows();
  int cstart = 0;
  for(int i = 0; i < Image_index; ++i)
    cstart += igc_list[i]->parameter_subset().rows();
  int cend = cstart + igc_list[Image_index]->parameter_subset().rows();
  Array<double, 2> jac = 
    igc_list[Image_index]->collinearity_residual_jacobian(Gc, Ic_actual);
  Array<double, 2> jac_res(2, nvar + 3);
  jac_res = 0;
  jac_res(Range::all(), Range(jac_res.cols() - 3, toEnd)) = 
    jac(Range::all(), Range(jac.cols() - 3, toEnd));
  if(cend > cstart)
    jac_res(Range::all(), Range(cstart, cend-1)) =
      jac(Range::all(), Range(0, jac.cols() - 4));
  return jac_res;
}

void IgcArray::add_identity_gradient()
{
  // For IgcArray, add the identity gradient to all contained classes.
  BOOST_FOREACH(const boost::shared_ptr<ImageGroundConnection>& igc, igc_list)
    igc->add_identity_gradient();
}
