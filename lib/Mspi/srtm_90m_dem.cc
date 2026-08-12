#include "srtm_90m_dem.h"
#include "geocal_serialize_support.h"
#ifdef HAVE_VICAR_RTL
#include "datum_geoid96.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Srtm90mData::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GdalCartLabMultifile);
}

template<class Archive>
void Srtm90mDem::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(DemMapInfo, Dem);
  GEOCAL_BASE(Srtm90mDem, DemMapInfo);
  ar & GEOCAL_NVP_(datum)
    & GEOCAL_NVP_(outside_dem_is_error)
    & GEOCAL_NVP2("srtm_90m_dem_data", f);
}

template<class Archive>
void Srtm90mDem::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(DemMapInfo, Dem);
  GEOCAL_BASE(Srtm90mDem, DemMapInfo);
  ar & GEOCAL_NVP_(datum)
    & GEOCAL_NVP_(outside_dem_is_error)
    & GEOCAL_NVP2("srtm_90m_dem_data", f);
  map_info_ = f->map_info();
}

GEOCAL_IMPLEMENT(Srtm90mData);
GEOCAL_SPLIT_IMPLEMENT(Srtm90mDem);
#endif

//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for DEM
/// data, or if you leave this blank we use the value of environment
/// variable USGSDATA.
///
/// We don't have DEM files that completely cover the area. If you
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

Srtm90mData::Srtm90mData
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
  file_name_end = "_L1.tif";

  lat_ref = 47,
  lon_ref = -87;
  int num_per_deg= 1200;
  mi_ref = MapInfo(boost::shared_ptr<CoordinateConverter>
    		   (new GeodeticConverter()),
    		   lon_ref, lat_ref + 1, lon_ref + 1, lat_ref, num_per_deg,
    		   num_per_deg);

  mi_ref = mi_ref.subset(-0.5,-0.5,1201,1201);
  file_spacing_number_line = 1200;
  file_spacing_number_sample = 1200;
  init_loc_to_file();
}

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

void Srtm90mDem::print(std::ostream& Os) const
{     
  OstreamPad opad(Os, "    ");
  Os << "SRTM 90m DEM:\n"
     << "  Datum:\n";
  opad << datum();
  opad.strict_sync();
  Os << "  Data directory: " << directory_base() << "\n";
  Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
}


//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for DEM
/// data, or if you leave this blank we use the value of environment
/// variable USGSDATA.
///
/// We don't have DEM files that completely cover the area. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of 0
/// instead. This is controlled by Outside_dem_is_error.
//-----------------------------------------------------------------------

Srtm90mDem::Srtm90mDem
(const std::string& Dir,
 bool Outside_dem_is_error,
 const boost::shared_ptr<Datum>& D
)
: 
  f(new Srtm90mData(Dir, Outside_dem_is_error, 1201, 1201))
{
  boost::shared_ptr<Datum> d(D);
  if(!d) {
#ifdef HAVE_VICAR_RTL    
    d.reset(new DatumGeoid96());
#else
    throw Exception("DatumGeoid96 wasn't included in the build");
#endif
  }
  // The 1201 up above is the size of the file, so we are reading all
  // the data. Not sure if we really want this hardcoded, or if we
  // really want all this data read. But for now, leave this like this
  // and we can revisit if needed.
  initialize(d, f->map_info(), Outside_dem_is_error);
}

