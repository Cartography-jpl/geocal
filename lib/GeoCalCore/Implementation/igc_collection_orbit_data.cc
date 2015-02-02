#include "igc_collection_orbit_data.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcCollectionOrbitData::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(IgcCollection, WithParameter);
  GEOCAL_BASE(IgcCollection, IgcCollection);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(WithParameterNested);
  ar & GEOCAL_NVP(orb) & GEOCAL_NVP(cam) & GEOCAL_NVP_(dem)
    & GEOCAL_NVP(img_list) & GEOCAL_NVP(tm_list)
    & GEOCAL_NVP(title_list);
}

GEOCAL_IMPLEMENT(IgcCollectionOrbitData);
#endif

boost::shared_ptr<ImageGroundConnection> 
IgcCollectionOrbitData::image_ground_connection(int Image_index) const
{
  range_check(Image_index, 0, number_image());
  igc_cache.resize(number_image());
  if(!igc_cache[Image_index])
    igc_cache[Image_index].reset
      (new OrbitDataImageGroundConnection(*orb, tm_list[Image_index], cam, dem_,
			     img_list[Image_index],
			     title_list[Image_index]));
  return igc_cache[Image_index];
}

void IgcCollectionOrbitData::print(std::ostream& Os) const
{
  Os << "IgcCollectionOrbitData:\n"
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
IgcCollectionOrbitData::subset(const std::vector<int>& Index_set) const
{
  std::vector<boost::shared_ptr<RasterImage> > ilist;
  std::vector<Time> tlist;
  std::vector<std::string> tllist;
  BOOST_FOREACH(int i, Index_set) {
    range_check(i, 0, number_image());
    ilist.push_back(img_list[i]);
    tlist.push_back(tm_list[i]);
    tllist.push_back(title_list[i]);
  }
  return boost::shared_ptr<IgcCollection>
    (new IgcCollectionOrbitData(ilist, tlist, tllist, orb, cam, dem_));
}
