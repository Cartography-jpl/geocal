#include "landsat7_panchromatic.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Landsat7Panchromatic::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GdalCartLabMultifile);
}

GEOCAL_IMPLEMENT(Landsat7Panchromatic);
#endif

//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for Landsat 7
/// panchromatic data, or if you leave this blank we use the value of
/// environment variable LANDSAT7_PANCHROMATIC_15M.
//-----------------------------------------------------------------------

Landsat7Panchromatic::Landsat7Panchromatic(const std::string& Dir)
: GdalCartLabMultifile(Dir, "LANDSAT7_PANCHROMATIC_15M", 
		       -1, -1, 4, 4, true, 0)
{
  file_name_end = "_band8.tif";
  // We got the values here by looking at one of the map info files,
  // and just recording the information. This is from n40w80_band8.tif
  // Note that n40w80 is actually the lower left corner, rather than
  // the upper left corner. This doesn't really change anything in
  // GdalCartLabMultifile, except that we need to make sure to get the
  // mapinfo right.
  lat_ref = 40, lon_ref = -80;
  int size_tile_deg = 5;
  int num_pix = 37900;
  mi_ref = MapInfo(boost::shared_ptr<CoordinateConverter> 
		   (new GeodeticConverter()),
		   lon_ref, lat_ref + size_tile_deg, 
		   lon_ref + size_tile_deg, lat_ref, num_pix, num_pix);
  mi_ref = mi_ref.subset(0.5, 0.5, num_pix + 1, num_pix + 1);
  init_loc_to_file();
}

