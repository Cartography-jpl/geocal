#include "geocal_pixbuf.h"

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Create a GdkPixbuf to display the given raster data.
//-----------------------------------------------------------------------

GdkPixbuf* GeoCal::pixbuf(const RasterImageMultiBand& Rband, 
			  const DisplayStretch& Stretch,
			  int red_band, int blue_band, int green_band)
{
  const RasterImage& r = Rband.raster_image(red_band);
  const RasterImage& g = Rband.raster_image(blue_band);
  const RasterImage& b = Rband.raster_image(green_band);
  int nl = r.number_line();
  int ns = r.number_sample();
  if(g.number_line()   != nl ||
     b.number_line()   != nl ||
     g.number_sample() != ns ||
     b.number_sample() != ns)
    throw Exception("The red, blue, and green bands need to be the same size");
  GdkPixbuf *pb = gdk_pixbuf_new(GDK_COLORSPACE_RGB,FALSE,8, ns, nl);
  int nch = gdk_pixbuf_get_n_channels(pb);
  int rs = gdk_pixbuf_get_rowstride(pb);
  Array<unsigned char, 3> pix(gdk_pixbuf_get_pixels(pb),
			      shape(nl, ns, nch), 
			      shape(rs, nch, 1),
			      neverDeleteData);
  Array<int, 2> raw(nl, ns);
  Array<unsigned char, 2> rp(pix(Range::all(), Range::all(), 0));
  Array<unsigned char, 2> gp(pix(Range::all(), Range::all(), 1));
  Array<unsigned char, 2> bp(pix(Range::all(), Range::all(), 2));
  r.read_ptr(0,0,nl,ns,raw.dataFirst());
  Stretch.stretch(raw, rp);
  g.read_ptr(0,0,nl,ns,raw.dataFirst());
  Stretch.stretch(raw, gp);
  b.read_ptr(0,0,nl,ns,raw.dataFirst());
  Stretch.stretch(raw, bp);
  return pb;
}
