#include "igc_multiple_pass.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"
#include <boost/foreach.hpp>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcMultiplePass::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(IgcCollection, WithParameter);
  GEOCAL_BASE(IgcMultiplePass, IgcCollection);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(WithParameterNested)
    & GEOCAL_NVP(igc_list);
}

GEOCAL_IMPLEMENT(IgcMultiplePass);
#endif

// See base class for description
int IgcMultiplePass::number_image() const
{
  int res = 0;
  BOOST_FOREACH(const boost::shared_ptr<IgcCollection> igccol, igc_list)
    res += igccol->number_image();
  return res;
}

// See base class for description
boost::shared_ptr<ImageGroundConnection> 
IgcMultiplePass::image_ground_connection(int Image_index) const
{
  range_check(Image_index, 0, number_image());
  int ioff = 0;
  BOOST_FOREACH(const boost::shared_ptr<IgcCollection> igccol, igc_list) {
    if(Image_index < ioff + igccol->number_image())
      return igccol->image_ground_connection(Image_index - ioff);
    ioff += igccol->number_image();
  }
  throw Exception("This should be impossible");
}

// See base class for description
void IgcMultiplePass::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "IgcMultiplePass\n";
  BOOST_FOREACH(const boost::shared_ptr<IgcCollection> igccol, igc_list) {
    Os << "  IgcCollection:\n";
    opad << *igccol << "\n";
    opad.strict_sync();
  }
  Os << "  Parameter:\n";
  blitz::Array<double, 1> p = parameter_subset();
  std::vector<std::string> pname = parameter_name_subset();
  for(int i = 0; i < p.rows(); ++i)
    Os << "    " << pname[i] << ": " << p(i) << "\n";
}


// See base class for description
boost::shared_ptr<IgcCollection> 
IgcMultiplePass::subset(const std::vector<int>& Index_set) const
{
  int nimg = number_image();
  BOOST_FOREACH(int i, Index_set)
    range_check(i, 0, nimg);
  int ioff = 0;
  boost::shared_ptr<IgcMultiplePass> res(new IgcMultiplePass);
  BOOST_FOREACH(const boost::shared_ptr<IgcCollection> igccol, igc_list) {
    std::vector<int> s;
    BOOST_FOREACH(int i, Index_set)
      if(i >= ioff && i < ioff + igccol->number_image())
	s.push_back(i - ioff);
    if(s.size() > 0)
      res->add_igc(igccol->subset(s));
  }
  return res;
}
