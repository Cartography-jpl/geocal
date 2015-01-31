#include "igc_collection_rolling_shutter.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcCollectionRollingShutter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(IgcCollection, WithParameter);
  GEOCAL_BASE(IgcCollection, IgcCollection);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(WithParameterNested);
  ar & GEOCAL_NVP(orb) & GEOCAL_NVP(cam) & GEOCAL_NVP_(dem)
    & GEOCAL_NVP(img_list) & GEOCAL_NVP(tt_list);
}

GEOCAL_IMPLEMENT(IgcCollectionRollingShutter);
#endif

boost::shared_ptr<ImageGroundConnection> 
IgcCollectionRollingShutter::image_ground_connection(int Image_index) const
{
  range_check(Image_index, 0, number_image());
  igc_cache.resize(number_image());
  if(!igc_cache[Image_index])
    igc_cache[Image_index].reset
      (new IgcRollingShutter(orb, tt_list[Image_index], cam, dem_,
			     img_list[Image_index]));
  return igc_cache[Image_index];
}

void IgcCollectionRollingShutter::print(std::ostream& Os) const
{
  Os << "IgcCollectionRollingShutter:\n"
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

boost::shared_ptr<IgcCollection> 
IgcCollectionRollingShutter::subset(const std::vector<int>& Index_set) const
{
  std::vector<boost::shared_ptr<RasterImage> > ilist;
  std::vector<boost::shared_ptr<TimeTable> > tlist;
  BOOST_FOREACH(int i, Index_set) {
    range_check(i, 0, number_image());
    ilist.push_back(img_list[i]);
    tlist.push_back(tt_list[i]);
  }
  return boost::shared_ptr<IgcCollection>
    (new IgcCollectionRollingShutter(ilist, tlist, orb, cam, dem_));
}
