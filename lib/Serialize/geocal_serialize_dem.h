#ifndef GEOCAL_SERIALIZE_DEM_H
#define GEOCAL_SERIALIZE_DEM_H
// Serialization of Dem and derived objects.
// We include Datum also, just because these are really closely
// related and we pretty much always want to use these 2 classes
// together. 
#include "geocal_serialize_common.h"
#include "geocal_datum.h"
#include "did_datum.h"
#include "datum_geoid96.h"
#include "gdal_datum.h"

#include "dem.h"
#include "simple_dem.h"
#include "dem_map_info.h"
#include "gdal_dem.h"
#include "vicar_dem.h"
#include "dem_map_info_offset.h"
#include "srtm_dem.h"

BOOST_CLASS_EXPORT(GeoCal::Datum);
BOOST_CLASS_EXPORT(GeoCal::SimpleDatum);
BOOST_CLASS_EXPORT(GeoCal::DidDatum);
BOOST_CLASS_EXPORT(GeoCal::DatumGeoid96);
BOOST_CLASS_EXPORT(GeoCal::GdalDatum);

BOOST_CLASS_EXPORT(GeoCal::Dem);
BOOST_CLASS_EXPORT(GeoCal::SimpleDem);
BOOST_CLASS_EXPORT(GeoCal::DemMapInfo);
BOOST_CLASS_EXPORT(GeoCal::DemTiledFile);
BOOST_CLASS_EXPORT(GeoCal::GdalDem);
BOOST_CLASS_EXPORT(GeoCal::VicarDem);
BOOST_CLASS_EXPORT(GeoCal::DemMapInfoOffset);
BOOST_CLASS_EXPORT(GeoCal::SrtmDem);
#endif
