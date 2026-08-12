#include "vicar_raster_image.h"
#include "vicar_lite_file.h"
#include "geocal_config.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void VicarRasterImage::save(Archive & ar, const unsigned int version) const
{
  unsigned int ntile = number_tile();
  int tile_number_line = number_tile_line();
  ar & GEOCAL_NVP2("number_tile", ntile)
    & GEOCAL_NVP(tile_number_line);
}

template<class Archive>
void VicarRasterImage::load(Archive & ar, const unsigned int version)
{
  unsigned int ntile;
  int tile_number_line;
  ar & GEOCAL_NVP2("number_tile", ntile)
    & GEOCAL_NVP(tile_number_line);
  initialize(band_id_, tile_number_line, ntile);
}

template<class Archive>
void VicarRasterImage::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImage)
    & GEOCAL_NVP_(vicar_file)
    & GEOCAL_NVP_(band_id);
  boost::serialization::split_member(ar, *this, version);
}

GEOCAL_IMPLEMENT(VicarRasterImage);
#endif

//-----------------------------------------------------------------------
/// Initialize the class, once vicar_file_ has been filled in.
//-----------------------------------------------------------------------

void VicarRasterImage::initialize(int Band_id, int Number_line_per_tile, 
				  int Number_tile)
{
  switch(vicar_file_->type()) {
  case VicarFile::VICAR_BYTE:
    initialize_t<VicarByte>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_HALF:
    initialize_t<VicarHalf>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_FULL:
    initialize_t<VicarFull>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_FLOAT:
    initialize_t<VicarFloat>(Band_id, Number_line_per_tile, Number_tile);
    break;
  case VicarFile::VICAR_DOUBLE:
    initialize_t<VicarDouble>(Band_id, Number_line_per_tile, Number_tile);
    break;
  }
}

//-----------------------------------------------------------------------
/// Often you want to open a memory mapped VicarLiteRasterImage if you
/// can because it is faster, but fall back to using a
/// VicarRasterImage if you can't (e.g., the data is compressed). This
/// utility routine works out the logic for this.
//-----------------------------------------------------------------------

boost::shared_ptr<RasterImage> 
GeoCal::vicar_open(const std::string& Fname, int Band_id,
	     VicarFile::access_type Access,
	     bool Favor_memory_mapped,
	     int Number_line_per_tile,
	     int Number_tile,
	     bool Force_area_pixel)
{
  VicarLiteFile::access_type access_lite;
  switch(Access) {
  case VicarFile::READ:
    access_lite = VicarLiteFile::READ;
    break;
  case VicarFile::WRITE:
    access_lite = VicarLiteFile::WRITE;
    break;
  case VicarFile::UPDATE:
    access_lite = VicarLiteFile::UPDATE;
    break;
  default:
    Exception e;
    e << "Unknown access type " << Access;
    throw e;
  }
  boost::shared_ptr<RasterImage> res;
  try {
    if(Favor_memory_mapped) {
      boost::shared_ptr<VicarLiteRasterImage> 
	f2(new VicarLiteRasterImage(Fname, Band_id, access_lite,
				    -1, -1, Force_area_pixel));
      if(!f2->is_compressed())  // Can only use memory mapped for
				// uncompressed files.
	res = f2;
    }
  } catch(const Exception& E) {
    // Ignore errors, we drop down to using the Vicar routines below.
  }
  if(!res.get()) {
#ifdef HAVE_VICAR_RTL
    res.reset(new VicarRasterImage(Fname, Band_id, Access,
				 Number_line_per_tile, 
				 Number_tile, Force_area_pixel));
#else
    throw Exception("Wasn't compiled with support for VICAR RTL. If you need thsi functionality, install the VICAR RTL and rebuild GeoCal");
#endif
  }
  return res;
}

