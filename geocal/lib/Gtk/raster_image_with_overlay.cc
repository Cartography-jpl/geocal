#include "raster_image_with_overlay.h"
#include "geocal_pixbuf.h"
#include "memory_multi_band.h"
#include <goocanvas.h>
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Change the raster band we are using. This recalculates everything.
//-----------------------------------------------------------------------

void RasterImageWithOverlay::set_data(
	      const boost::shared_ptr<RasterImageMultiBand>& Data_new,
	      bool do_calculate_overlay)
{
  std::cerr << "Starting MemoryMultiBand\n";
  // Clear out existing data. For large images, we don't want two
  // versions active at the same time
  data_rb.reset();
  GdkPixbuf *gpix = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8, 1, 1);
  g_object_set(G_OBJECT(img), "pixbuf", gpix, NULL);
  g_object_set(G_OBJECT(imgover), "pixbuf", gpix, NULL);

  // Check to see if this is already a MemoryRasterImageBand.
  boost::shared_ptr<MemoryMultiBand> data_in_memory
    = boost::dynamic_pointer_cast<MemoryMultiBand>(Data_new);
  // If not, go ahead and read into memory.
  if(!data_in_memory)
    data_in_memory.reset(new MemoryMultiBand(*Data_new));
  data_rb = data_in_memory;
  data.reference(data_in_memory->data());
  std::cerr << "Starting pixbuf generation\n";
  gpix = pixbuf(*data_rb, *ds, red_band_, green_band_, blue_band_);
  std::cerr << "Setting to goocanvas\n";
  g_object_set(G_OBJECT(img), "pixbuf", gpix, NULL);
  std::cerr << "Starting misc\n";
  int nl = data_rb->raster_image(0).number_line();
  int ns = data_rb->raster_image(0).number_sample();
  pb = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8, ns, nl);
  int nch = gdk_pixbuf_get_n_channels(pb);
  int rs = gdk_pixbuf_get_rowstride(pb);
  pix.reference(Array<unsigned char, 3>(gdk_pixbuf_get_pixels(pb),
					shape(nl, ns, nch), 
					shape(rs, nch, 1),
					neverDeleteData));
  red.reference(Array<unsigned char, 2>(pix(Range::all(), Range::all(), 0)));
  green.reference(Array<unsigned char, 2>(pix(Range::all(), Range::all(), 1)));
  blue.reference(Array<unsigned char, 2>(pix(Range::all(), Range::all(), 2)));
  alpha.reference(Array<unsigned char, 2>(pix(Range::all(), Range::all(), 3)));
  val.resize(nl, ns);
  std::cerr << "Doing overlay calculation\n";
  if(do_calculate_overlay)
    calculate_overlay();
  std::cerr << "Done\n";
}

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

RasterImageWithOverlay::RasterImageWithOverlay
(const boost::shared_ptr<RasterImageMultiBand>& Data,
 const boost::shared_ptr<DisplayStretch>& Ds,
 int Red_band, int Green_band, int Blue_band)
: ds(Ds), red_band_(Red_band), green_band_(Green_band), 
  blue_band_(Blue_band)
{
  image_viewer = goo_canvas_new();
  pb = gdk_pixbuf_new(GDK_COLORSPACE_RGB,TRUE,8, 1, 1);
  GooCanvasItem* root = goo_canvas_get_root_item(GOO_CANVAS(image_viewer));
  img = goo_canvas_image_new(root, pb, 0,0, "can-focus", TRUE, NULL);
  g_object_set(G_OBJECT(image_viewer), 
	       "automatic-bounds", TRUE, NULL);
  imgover = goo_canvas_image_new(root, pb, 0, 0, NULL);
  alpha_value = 50;
  goo_canvas_grab_focus(GOO_CANVAS(image_viewer), img);
  color_map.push_back(std::vector<unsigned char>(0,(unsigned char) 0));
  color_map.push_back(std::vector<unsigned char>(0,(unsigned char) 0));
  color_map.push_back(std::vector<unsigned char>(0,(unsigned char) 0));
  set_data(Data, false);
}

//-----------------------------------------------------------------------
/// Assign color to given value. This is used by val_to_color.
/// A value of 0 is always assigned to transparent black, so you can't
/// override that value with this routine.
//-----------------------------------------------------------------------

void RasterImageWithOverlay::color_value(int v, unsigned char r, 
					 unsigned char g,
					 unsigned char b)
{ 
  range_min_check(v, 1);
  if(v >= (int) color_map[0].size()) {
    color_map[0].resize(v + 1);
    color_map[1].resize(v + 1);
    color_map[2].resize(v + 1);
  }
  color_map[0][v] = r;
  color_map[1][v] = g;
  color_map[2][v] = b;
}

//-----------------------------------------------------------------------
/// It is often convenient to calculate the overlay as one of a value
/// (e.g., one value for CLOUD, one value for WATER). This uses the
/// values found in color_map and val to assign data to the red, green
/// and blue channels
//-----------------------------------------------------------------------

void RasterImageWithOverlay::val_to_color()
{
  for(int i = 0; i < val.rows(); ++i)
    for(int j = 0; j < val.columns(); ++j) {
      int v = val(i, j);
      if(v <= 0 || v >= (int) color_map[0].size()) {
	red(i, j) = 0;
	green(i, j) = 0;
	blue(i, j) = 0;
	alpha(i, j) = 0;
      } else {
	red(i, j) = color_map[0][v];
	green(i, j) = color_map[1][v];
	blue(i, j) = color_map[2][v];
	alpha(i, j) = alpha_value;
      }
    }
}

void RasterImageWithOverlay::calculate_overlay_value()
{
  enum {SHADOW = 1, CLOUD = 2, CLOUD_LC = 3};
  color_value(SHADOW, 0, 0,255);
  color_value(CLOUD, 255, 0, 0);
  color_value(CLOUD_LC, 128, 0, 0);
  double c1 = 1.851735e-02;
  double c2 = 2.050828e-02;
  Array<int, 2> r1(data(4, Range::all(), Range::all()));
  Array<int, 2> r2(data(6, Range::all(), Range::all()));
  Array<int, 2> rblue(data(0, Range::all(), Range::all()));
  Array<double, 2> dval(pow(fabs((c1 * r1 - c2 * r2) / (c1 * r1 + c2 * r2)), 
			    0.6) / (c1 * r1 * c1 * r1));
  val = where(rblue > 700, (int) CLOUD,
	      where(rblue > 550, (int) CLOUD_LC,
		    where(dval > 5000.0 / 100000, (int) SHADOW, 0)));
}

//-----------------------------------------------------------------------
/// Update alpha of overlay. Alpha should be 0 to 100.0.
//-----------------------------------------------------------------------

void RasterImageWithOverlay::alpha_overlay(double a)
{
  range_check(a, 0.0, 100.0001);
  int t = round(255 * a / 100.0);
  t = (t < 1 ? 1 : (t > 255 ? 255 : t));
  alpha_value = (unsigned char) t;
  alpha = where(alpha > 0, alpha_value, (unsigned char) 0);
  update_overlay();
}


//-----------------------------------------------------------------------
/// Set the bands to view.
//-----------------------------------------------------------------------

void RasterImageWithOverlay::set_band(int rb, int gb, int bb)
{
  range_check(rb, 0, data_rb->number_band());
  range_check(gb, 0, data_rb->number_band());
  range_check(bb, 0, data_rb->number_band());
  red_band_ = rb;
  green_band_ = gb;
  blue_band_ = bb;
  g_object_set(G_OBJECT(img), "pixbuf", 
	       pixbuf(*data_rb, *ds, red_band_, green_band_, blue_band_), 
	       NULL);
}

//-----------------------------------------------------------------------
/// Return zoom factor.
//-----------------------------------------------------------------------

double RasterImageWithOverlay::zoom_factor() const 
{
  return goo_canvas_get_scale(GOO_CANVAS(image_viewer)); 
}

//-----------------------------------------------------------------------
/// Set zoom factor.
//-----------------------------------------------------------------------

void RasterImageWithOverlay::zoom_factor(double zf) 
{
  goo_canvas_set_scale(GOO_CANVAS(image_viewer), zf);
}

