#include "raster_image_with_overlay.h"
#include "display_stretch_std_dev.h"
#include "gdal_multi_band.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

using namespace GeoCal;
// boost::shared_ptr<RasterImageBand> rimg;
// int band[3] = {3, 2, 0};

// static void
// cb_image_annotate(GtkImageViewer *imgv,
//                   GdkPixbuf *pixbuf,
//                   gint shift_x,
//                   gint shift_y,
//                   gdouble scale_x,
//                   gdouble scale_y,
//                   gpointer user_data
//                   )
// {
//   int img_width = gdk_pixbuf_get_width(pixbuf);
//   int img_height = gdk_pixbuf_get_height(pixbuf);
//   int row_stride = gdk_pixbuf_get_rowstride(pixbuf);
//   int pix_stride = 4;
//   guint8 *buf = gdk_pixbuf_get_pixels(pixbuf);
//   int col_idx, row_idx;

//   std::cerr << "Hi there\n";
//   std::cerr << "scale_x: " << scale_x << "\n";
//   double x1=(0+shift_x) / scale_x;
//   double y1=(0+shift_y) / scale_y;
//   double x2=(img_width - 1 +shift_x) / scale_x;
//   double y2=(img_height + 1 +shift_y) / scale_y;
//   std::cerr << "(" << y1 << ", " << x1 << ")  (" << y2 << ", " << x2 << ")\n";
//   for (row_idx=0; row_idx<img_height; row_idx++)
//     {
//       guint8 *row = buf + row_idx * row_stride;
//       for (col_idx=0; col_idx<img_width; col_idx++)
//         {
// 	  *(row+col_idx*pix_stride + 3) = 255; // Alpha channel
// 	  for(int i = 0; i < 3; ++i) {
// 	    int b = band[i];
// 	    gint gl = 0;
// 	    if(row_idx < rimg->raster_image(b).number_line() &&
// 	       col_idx < rimg->raster_image(b).number_sample()) {
// 	      double v = rimg->raster_image(b)(row_idx, col_idx) / 2000.0 * 256;
// 	      gl = (v > 255 ? (gint) 255 : (gint) v);
// 	    }
	    
// 	    *(row+col_idx*pix_stride + i) = gl;
// 	  }
//         }
//     }
// }

static gint
cb_key_press_event(GtkWidget *widget, GdkEventKey *event)
{
  gint k = event->keyval;

  if (k == 'q')
    exit(0);

  return FALSE;
}

int main(int argc, char **argv)
{
  GtkWidget *window, *scrolled_win;

  gtk_init (&argc, &argv);
  boost::shared_ptr<RasterImageMultiBand> rb(new GdalMultiBand(argv[1]));
  boost::shared_ptr<DisplayStretch> ds(new DisplayStretchStdDev());
  RasterImageWithOverlay img(rb, ds, 4, 3, 1);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_policy(GTK_WINDOW(window), TRUE, TRUE, FALSE);

  gtk_window_set_title (GTK_WINDOW (window), argv[0]);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_exit), NULL);

  scrolled_win = gtk_scrolled_window_new(NULL,NULL);
  gtk_container_add (GTK_CONTAINER (window), scrolled_win);
  gtk_widget_show(scrolled_win);

  // g_signal_connect(image_viewer,
  //                  "image-annotate",
  //                  G_CALLBACK(cb_image_annotate), NULL);

  gtk_widget_set_size_request (window, 500, 500);

  g_signal_connect (window, "key_press_event",
                    GTK_SIGNAL_FUNC(cb_key_press_event), NULL);

  gtk_container_add (GTK_CONTAINER (scrolled_win), img.widget());

  gtk_widget_show (img.widget());
  gtk_widget_show (window);

  // Set the scroll region and zoom range
  // gtk_image_viewer_set_scroll_region(GTK_IMAGE_VIEWER(image_viewer),
  //                                    -5,-5,5,5);
  // gtk_image_viewer_set_zoom_range(GTK_IMAGE_VIEWER(image_viewer),
  //                                 -HUGE, HUGE);

  // // Need to do a manual zoom fit at creation because a bug when
  // // not using an image.
  // gtk_image_viewer_zoom_fit(GTK_IMAGE_VIEWER(image_viewer));
  gtk_main ();

  exit(0);
  return(0);
}
