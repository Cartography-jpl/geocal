#include "tiled_file.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<std::size_t D> template<class Archive>
void TiledFileBase<D>::save(Archive& ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(TiledFileBase<D>);
  ar & GEOCAL_NVP_(size)
    & GEOCAL_NVP_(number_tile)
    & GEOCAL_NVP_(tile_size);
}

template<std::size_t D> template<class Archive>
void TiledFileBase<D>::load(Archive& ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(TiledFileBase<D>);
  ar & GEOCAL_NVP_(size)
    & GEOCAL_NVP_(number_tile)
    & GEOCAL_NVP_(tile_size);
  reset_number_swap();
}

template<class T, std::size_t D> template<class Archive>
void TiledFile<T, D>::save(Archive& ar, const unsigned int version) const
{
  // Note this other form seems to be required. Not sure exactly why,
  // but some template thing or other.
  //ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiledFileBase<D>);
  ar & boost::serialization::base_object<TiledFileBase<D> >(*this);
}

template<class T, std::size_t D> template<class Archive>
void TiledFile<T, D>::load(Archive& ar, const unsigned int version)
{
  // Note this other form seems to be required. Not sure exactly why,
  // but some template thing or other.
  //ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiledFileBase<D>);
  ar & boost::serialization::base_object<TiledFileBase<D> >(*this);
  tile_.resize(TiledFileBase<D>::number_tile_);
  next_swap_ = tile_.begin();
}


GEOCAL_SPLIT_IMPLEMENT(TiledFileBase<2>);
GEOCAL_SPLIT_IMPLEMENT(TiledFile_int_2);
GEOCAL_SPLIT_IMPLEMENT(TiledFile_float_2);
#endif
