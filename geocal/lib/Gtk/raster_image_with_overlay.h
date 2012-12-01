#ifndef RASTER_IMAGE_WITH_OVERLAY_H
#define RASTER_IMAGE_WITH_OVERLAY_H
#include "raster_image_multi_band.h"
#include "display_stretch.h"
#include "markable.h"
#include <gtk/gtk.h>
#include <goocanvas.h>

namespace GeoCal {
/****************************************************************//**
  This is a simple widget. Because I'm not really sure what the right
  abstractions are yet, this is purposely limited in what it can do. I
  imagine we'll completely replace this with something more general
  once we figure out what we are trying to do.
*******************************************************************/

class RasterImageWithOverlay : public PrintAndMarkable<RasterImageWithOverlay> {
public:
  RasterImageWithOverlay(const boost::shared_ptr<RasterImageMultiBand>& Data,
			 const boost::shared_ptr<DisplayStretch>& Ds,
			 int Red_band = 1, int Green_band = 2, 
			 int Blue_band = 3);
  virtual ~RasterImageWithOverlay() { }
  void set_data(const boost::shared_ptr<RasterImageMultiBand>& Data_new,
		bool do_calculate_overlay = true);

//-----------------------------------------------------------------------
/// Calculate overlay. The default just calls calculate_overlay_value
/// followed by val_to_color.
//-----------------------------------------------------------------------

  virtual void calculate_overlay()
  {
    calculate_overlay_value();
    val_to_color();
    update_overlay();
  }

//-----------------------------------------------------------------------
/// Calculate the overlay value.
//-----------------------------------------------------------------------

  virtual void calculate_overlay_value();

  void color_value(int v, unsigned char r, unsigned char g, unsigned char b);

//-----------------------------------------------------------------------
/// Return underlying widget.
//-----------------------------------------------------------------------
  
  GtkWidget *widget() {return image_viewer;}

  virtual void print(std::ostream& Os) const { Os << "RasterImageWithOverlay"; }
  blitz::Array<int, 3> data;
  blitz::Array<int, 2> val;
  virtual void alpha_overlay(double alpha);

//-----------------------------------------------------------------------
/// Band in the red channel for the image.
//-----------------------------------------------------------------------

  int red_band() const {return red_band_;}

//-----------------------------------------------------------------------
/// Band in the green channel for the image.
//-----------------------------------------------------------------------

  int green_band() const {return green_band_;}

//-----------------------------------------------------------------------
/// Band in the blue channel for the image.
//-----------------------------------------------------------------------

  int blue_band() const {return blue_band_;}
  void set_band(int rb, int gb, int bb);
  double zoom_factor() const;
  void zoom_factor(double zf);

//-----------------------------------------------------------------------
/// Update the overlay image. You need to call this after change red,
/// blue, green, or alpha (the change to the image isn't automatic).
//-----------------------------------------------------------------------
  
  void update_overlay()
  {
    g_object_set(G_OBJECT(imgover), "pixbuf", pb, NULL);
  }

//-----------------------------------------------------------------------
/// This points to the same data as red, green, blue and alpha. But
/// this works better with ruby (which doesn't like holes in NArray)
//-----------------------------------------------------------------------
  blitz::Array<unsigned char, 3> pix;

  blitz::Array<unsigned char, 2> red, green, blue, alpha;
  unsigned char alpha_value;
  void val_to_color();
protected:
  void mark() {mark_it(data_rb); mark_it(ds);}
private:
  std::vector<std::vector<unsigned char> > color_map;
  GtkWidget *image_viewer;
  boost::shared_ptr<RasterImageMultiBand> data_rb;
  boost::shared_ptr<DisplayStretch> ds;
  int red_band_, green_band_, blue_band_;
  GdkPixbuf *pb;
  GooCanvasItem *img, *imgover;
};
}
#endif

