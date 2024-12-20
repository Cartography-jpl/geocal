#include "srtm_dem.h"
#include "geocal_serialize_support.h"
#include <stdlib.h>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void SrtmDemData::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VicarCartLabMultifile);
}

template<class Archive>
void SrtmLwmData::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VicarCartLabMultifile);
}

template<class Archive>
void SrtmDem::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP(f);
}


GEOCAL_IMPLEMENT(SrtmDemData);
GEOCAL_IMPLEMENT(SrtmLwmData);
GEOCAL_IMPLEMENT(SrtmDem);
#endif

//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for SRTM DEM
/// data, or if you leave this blank we use the value of environment
/// variable ELEV_ROOT.
///
/// The SRTM doesn't cover the whole globe. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of 0
/// instead. This is controlled by No_coverage_is_error.
///
/// There are two kinds of tiling going on. At the top level, we have
/// a number of files open at one time, given by Number_file. For each
/// file, we read it with tiles with the given Number_line_per_tile x
/// Number_sample_per_tile, having up to Number_tile_each_file tiles.
/// If the Number_line_per_tile or Number_sample_per_tile is -1 we
/// read the entire file.
//-----------------------------------------------------------------------

SrtmDemData::SrtmDemData
(const std::string& Dir,
 bool No_coverage_is_error,
 int Number_line_per_tile,
 int Number_sample_per_tile, 
 int Number_tile_each_file, 
 int Number_file,
 bool Favor_memory_mapped,
 bool Force_area_pixel
 )
: VicarCartLabMultifile(Dir, "ELEV_ROOT", Number_line_per_tile,
		       Number_sample_per_tile,
		       Number_tile_each_file,
		       Number_file,
		       Favor_memory_mapped,
		       No_coverage_is_error,
		       0, Force_area_pixel)
{
  file_name_end = "_L2.hlf";
  // We read n0e100_L2.hlf. Note that for some odd reason, n0e100 is
  // actually the lower left corner, rather than the upper left corner.
  // This doesn't really change anything in VicarCartLabMultifile, except that
  // we need to make sure to get the mapinfo right.
  lat_ref = 0, lon_ref = 100;
  int num_per_deg = 3600;
  mi_ref = MapInfo(boost::shared_ptr<CoordinateConverter> 
 		 (new GeodeticConverter()),
		   lon_ref, lat_ref + 1, lon_ref + 1, lat_ref, num_per_deg, 
		   num_per_deg);
  mi_ref = mi_ref.subset(0,0,3601,3601);
  if(!force_area_pixel)
    mi_ref = mi_ref.subset(-0.5,-0.5,3601,3601);
  init_loc_to_file();
}

//-----------------------------------------------------------------------
/// Constructor. You provide the directory to look for SRTM LWM
/// data.
///
/// The SRTM doesn't cover the whole globe. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of 0
/// instead. This is controlled by No_coverage_is_error.
///
/// There are two kinds of tiling going on. At the top level, we have
/// a number of files open at one time, given by Number_file. For each
/// file, we read it with tiles with the given Number_line_per_tile x
/// Number_sample_per_tile, having up to Number_tile_each_file tiles.
/// If the Number_line_per_tile or Number_sample_per_tile is -1 we
/// read the entire file.
//-----------------------------------------------------------------------

SrtmLwmData::SrtmLwmData
(const std::string& Dir,
 bool No_coverage_is_error,
 int Number_line_per_tile,
 int Number_sample_per_tile, 
 int Number_tile_each_file, 
 int Number_file,
 bool Favor_memory_mapped,
 bool Force_area_pixel
 )
: VicarCartLabMultifile(Dir, "", Number_line_per_tile,
		       Number_sample_per_tile,
		       Number_tile_each_file,
		       Number_file,
		       Favor_memory_mapped,
		       No_coverage_is_error,
		       0, Force_area_pixel)
{
  file_name_end = "_lwm_L2.img";
  // We read n0e100_L2.hlf. Note that for some odd reason, n0e100 is
  // actually the lower left corner, rather than the upper left corner.
  // This doesn't really change anything in VicarCartLabMultifile, except that
  // we need to make sure to get the mapinfo right.
  lat_ref = 0, lon_ref = 100;
  int num_per_deg = 3600;
  mi_ref = MapInfo(boost::shared_ptr<CoordinateConverter> 
 		 (new GeodeticConverter()),
		   lon_ref, lat_ref + 1, lon_ref + 1, lat_ref, num_per_deg, 
		   num_per_deg);
  mi_ref = mi_ref.subset(0,0,3601,3601);
  if(!force_area_pixel)
    mi_ref = mi_ref.subset(-0.5,-0.5,3601,3601);
  init_loc_to_file();
}

//-----------------------------------------------------------------------
/// Constructor. You can provide the directory where there data can be 
/// found. If you leave this as a blank string, we use ${ELEV_ROOT} as 
/// the directory and ${ELEV_ROOT}/L2_dem_db.int as the file.
//-----------------------------------------------------------------------

SrtmDem::SrtmDem(const std::string& Dirbase,
		 bool Outside_dem_is_error,
		 const boost::shared_ptr<Datum>& D)
{
  f.reset(new SrtmDemData(Dirbase, Outside_dem_is_error));
  initialize(D, f->map_info(), Outside_dem_is_error);
}

