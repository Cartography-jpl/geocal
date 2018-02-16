#include "landsat7_global.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Landsat7Global::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VicarCartLabMultifile)
    & GEOCAL_NVP_(band);
}

GEOCAL_IMPLEMENT(Landsat7Global);
#endif

//-----------------------------------------------------------------------
/// Constructor. You provide the directory to look for Landsat 7
/// data, along with the band type.
//-----------------------------------------------------------------------

Landsat7Global::Landsat7Global(const std::string& Dir, Band B)
: VicarCartLabMultifile(Dir, "", 100, -1, 4, 4, true, true, 0, false),
  band_(B)
{
  int num_pix = -1;
  switch(band_) {
  case BAND62:
    file_name_end = "_band62";
    num_pix = 9475;
    break;
  default:
    throw Exception("Unrecognized band value");
  }
    
  // We got the values here by looking at one of the map info files,
  // and just recording the information. This is from n40w80_band8.tif
  // Note that n40w80 is actually the lower left corner, rather than
  // the upper left corner. This doesn't really change anything in
  // GdalCartLabMultifile, except that we need to make sure to get the
  // mapinfo right.
  lat_ref = 40, lon_ref = -80;
  int size_tile_deg = 5;
  mi_ref = MapInfo(boost::shared_ptr<CoordinateConverter> 
		   (new GeodeticConverter()),
		   lon_ref, lat_ref + size_tile_deg, 
		   lon_ref + size_tile_deg, lat_ref, num_pix, num_pix);
  mi_ref = mi_ref.subset(0.5, 0.5, num_pix + 1, num_pix + 1);
  init_loc_to_file();
}

void Landsat7Global::print(std::ostream& Os) const
{
  Os << "Landsat7Global:\n"
     << "  Directory base: " << directory_base() << "\n"
     << "  Band:           ";
  switch(band_) {
  case BAND62:
    Os << "BAND62";
    break;
  default:
    Os << "Unknown";
  }
  Os << "\n";
}

