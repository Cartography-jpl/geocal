#include "usgs_dem.h"
#include "geocal_serialize_support.h"
#ifdef HAVE_VICAR_RTL
#include "datum_geoid96.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void UsgsDemData::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GdalCartLabMultifile);
}

template<class Archive>
void UsgsDem::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(DemMapInfo, Dem);
  GEOCAL_BASE(UsgsDem, DemMapInfo);
  ar & GEOCAL_NVP_(datum)
    & GEOCAL_NVP_(outside_dem_is_error)
    & GEOCAL_NVP2("usgs_dem_data", f);
}

template<class Archive>
void UsgsDem::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(DemMapInfo, Dem);
  GEOCAL_BASE(UsgsDem, DemMapInfo);
  ar & GEOCAL_NVP_(datum)
    & GEOCAL_NVP_(outside_dem_is_error)
    & GEOCAL_NVP2("usgs_dem_data", f);
  map_info_ = f->map_info();
}

GEOCAL_IMPLEMENT(UsgsDemData);
GEOCAL_SPLIT_IMPLEMENT(UsgsDem);
#endif

//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for USGS DEM
/// data, or if you leave this blank we use the value of environment
/// variable USGSDATA.
///
/// We don't have USGS files that completely cover the area. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of FILL_VALUE
/// instead. This is controlled by No_coverage_is_error.
///
/// There are two kinds of tiling going on. At the top level, we have
/// a number of files open at one time, given by Number_file. For each
/// file, we read it with tiles with the given Number_line_per_tile x
/// Number_sample_per_tile, having up to Number_tile_each_file tiles.
/// If the Number_line_per_tile or Number_sample_per_tile is -1 we
/// read the entire file.
//-----------------------------------------------------------------------

UsgsDemData::UsgsDemData
(const std::string& Dir,
 bool No_coverage_is_error,
 int Number_line_per_tile,
 int Number_sample_per_tile, 
 int Number_tile_each_file, 
 int Number_file
)
: GdalCartLabMultifile(Dir, "USGSDATA", Number_line_per_tile,
		       Number_sample_per_tile,
		       Number_tile_each_file,
		       Number_file,
		       No_coverage_is_error,
		       FILL_VALUE)
{
  file_name_end = "_10m.tif";
  // We got the values here by looking at one of the map info files,
  // and just recording the information. This is from n47w087_10m.tif.
  // We start with a 1 degree tile, and then add a 6 pixel border
  lat_ref = 47, lon_ref = -87;
  int num_per_deg_lat = -10800, num_per_deg_lon = 10800;
  int border = 6;
  mi_ref = MapInfo(boost::shared_ptr<CoordinateConverter> 
		   (new GeodeticConverter()),
		   lon_ref, lat_ref, lon_ref + 1, lat_ref - 1, num_per_deg_lon, 
		   -num_per_deg_lat);
  mi_ref = mi_ref.subset(-border, -border, num_per_deg_lon + 2 * border,
			 -num_per_deg_lat + 2 * border);
  init_loc_to_file();
}

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

void UsgsDem::print(std::ostream& Os) const
{     
  OstreamPad opad(Os, "    ");
  Os << "USGS DEM:\n"
     << "  Datum:\n";
  opad << datum();
  opad.strict_sync();
  Os << "  Data directory: " << directory_base() << "\n";
  Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
}


//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for USGS DEM
/// data, or if you leave this blank we use the value of environment
/// variable USGSDATA.
///
/// We don't have USGS files that completely cover the area. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of 0
/// instead. This is controlled by Outside_dem_is_error.
//-----------------------------------------------------------------------

UsgsDem::UsgsDem
(const std::string& Dir,
 bool Outside_dem_is_error,
 const boost::shared_ptr<Datum>& D
)
: 
  f(new UsgsDemData(Dir, Outside_dem_is_error, 10812, 10812))
{
  boost::shared_ptr<Datum> d(D);
  if(!d) {
#ifdef HAVE_VICAR_RTL    
    d.reset(new DatumGeoid96());
#else
    throw Exception("DatumGeoid96 wasn't included in the build");
#endif
  }
  // The 10812 up above is the size of the file, so we are reading all
  // the data. Not sure if we really want this hardcoded, or if we
  // really want all this data read. But for now, leave this like this
  // and we can revisit if needed.
  initialize(d, f->map_info(), Outside_dem_is_error);
}

