#ifndef GEOCAL_PIXBUF_H
#define GEOCAL_PIXBUF_H
#include "raster_image_multi_band.h"
#include "display_stretch.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

namespace GeoCal {
  GdkPixbuf* pixbuf(const RasterImageMultiBand& Rband, 
		    const DisplayStretch& Stretch,
		    int red_band = 1, int blue_band = 2, int green_band = 3);
};

#endif
