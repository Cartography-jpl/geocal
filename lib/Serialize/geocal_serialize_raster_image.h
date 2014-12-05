#ifndef GEOCAL_SERIALIZE_RASTER_IMAGE_H
#define GEOCAL_SERIALIZE_RASTER_IMAGE_H
// Serialization of Dem and derived objects.
#include "geocal_serialize_common.h"
#include "raster_image.h"
#include "gdal_raster_image.h"

BOOST_CLASS_EXPORT(GeoCal::RasterImage);
BOOST_CLASS_EXPORT(GeoCal::GdalRasterImage);
BOOST_CLASS_EXPORT(GeoCal::VicarLiteFile);
BOOST_CLASS_EXPORT(GeoCal::VicarLiteRasterImage);

#ifdef HAVE_VICAR_RTL
#include "vicar_raster_image.h"
#include "vicar_lite_file.h"
BOOST_CLASS_EXPORT(GeoCal::VicarFile);
BOOST_CLASS_EXPORT(GeoCal::VicarRasterImage);
#endif

#endif
