#include "igc_array.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
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
