#include "raster_image_tiled_file.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RasterImageTiledFile::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(RasterImage);
  GEOCAL_BASE(RasterImageVariable, RasterImage);
  GEOCAL_BASE(RasterImageTiledFile, RasterImageVariable);
  ar & GEOCAL_NVP_(data)
    & GEOCAL_NVP_(map_info)
    & GEOCAL_NVP_(rpc);
}

template<class Archive>
void RasterImageTiledFile::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RasterImage);
  GEOCAL_BASE(RasterImageVariable, RasterImage);
  GEOCAL_BASE(RasterImageTiledFile, RasterImageVariable);
  ar & GEOCAL_NVP_(data)
    & GEOCAL_NVP_(map_info)
    & GEOCAL_NVP_(rpc);
  initialize(data_);
}

GEOCAL_SPLIT_IMPLEMENT(RasterImageTiledFile);
#endif
