#ifndef GEOCAL_SERIALIZE_DEM_H
#define GEOCAL_SERIALIZE_DEM_H
// Serialization of Dem and derived objects.
#include "geocal_serialize_common.h"
#include "dem.h"
#include "geocal_datum.h"
#include "simple_dem.h"

BOOST_CLASS_EXPORT(GeoCal::Dem);
BOOST_CLASS_EXPORT(GeoCal::SimpleDem);
BOOST_CLASS_EXPORT(GeoCal::Datum);
BOOST_CLASS_EXPORT(GeoCal::SimpleDatum);
#endif
