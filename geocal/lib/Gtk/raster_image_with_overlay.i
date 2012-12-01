// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "raster_image_with_overlay.h"
%}

// This doesn't work with Python. This may go away, so don't spend any
// time trying to figure out the problem here.
#ifdef SWIGRUBY
%geocal_markfunc(RasterImageWithOverlay);
%feature("director") GeoCal::RasterImageWithOverlay;
%rename("val=") GeoCal::RasterImageWithOverlay::set_val;
%rename("red=") GeoCal::RasterImageWithOverlay::set_red;
%rename("green=") GeoCal::RasterImageWithOverlay::set_green;
%rename("blue=") GeoCal::RasterImageWithOverlay::set_blue;
%rename("alpha=") GeoCal::RasterImageWithOverlay::set_alpha;
%rename("zoom_factor=") GeoCal::RasterImageWithOverlay::zoom_factor(double zf);
namespace GeoCal {
class RasterImageWithOverlay {
public:
  std::string print_to_string() const;
  RasterImageWithOverlay(const boost::shared_ptr<RasterImageMultiBand>& Data,
			 const boost::shared_ptr<DisplayStretch>& Ds,
			 int Red_band = 1, int Green_band = 2, 
			 int Blue_band = 3);
  virtual ~RasterImageWithOverlay();
  GtkWidget *widget();
  void color_value(int v, unsigned char r, unsigned char g, unsigned char b);
  virtual void calculate_overlay_value();
  virtual void calculate_overlay();
  void set_data(const boost::shared_ptr<RasterImageMultiBand>& Data_new,
		bool do_calculate_overlay = true);
  void alpha_overlay(double alpha);
  int red_band() const;
  int green_band() const;
  int blue_band() const;
  void set_band(int rb, int gb, int bb);
  double zoom_factor() const;
  void zoom_factor(double zf);
  void update_overlay();
  unsigned char alpha_value;
  void val_to_color();

  // typemap for blitz::Array to narray. We may move this to a global
  // scope later, but for now just define local to this
  // class. blitz_array_to_narray is defined in blitz_array.i
  // blitz::array has odd copy semantics, a = b copies the data from b
  // to a, and if a isn't already the same size as b this will cause a
  // segmentation fault. This confuses SWIG. We avoid the problem by
  // return pointers rather than actual blitz::Array. This extra level
  // of indirection keeps swig from trying to copy the blitz::Array
  // (instead it copies a pointer to a blitz::Array, which is fine).
  %typemap(out) blitz::Array<int, 3>* {
    return blitz_array_to_narray(*$1);
  }
  %typemap(out) blitz::Array<int, 2>* {
    return blitz_array_to_narray(*$1);
  }
  %typemap(in) blitz::Array<int, 2> {
    $1.reference(narray_to_blitz<int, 2>($input));
  }
  %typemap(out) blitz::Array<unsigned char, 3>* {
    return blitz_array_to_narray(*$1);
  }
  %typemap(in) blitz::Array<unsigned char, 2> {
    $1.reference(narray_to_blitz<unsigned char, 2>($input));
  }
  %extend {
    blitz::Array<int, 3>* data() {return &($self->data);}
    blitz::Array<int, 2>* val() {return &($self->val);}
    blitz::Array<unsigned char, 3>* pix() {return &($self->pix);}
    void set_val(blitz::Array<int, 2> v) 
    {
      if(v.shape()(0) != $self->val.shape()(0) ||
	 v.shape()(1) != $self->val.shape()(1))
	throw GeoCal::Exception("Shape of na doesn't match the expected shape");
      $self->val = v;
    }
    void set_red(blitz::Array<unsigned char, 2> v) 
    {
      if(v.shape()(0) != $self->red.shape()(0) ||
	 v.shape()(1) != $self->red.shape()(1))
	throw GeoCal::Exception("Shape of na doesn't match the expected shape");
      $self->red = v;
    }
    void set_green(blitz::Array<unsigned char, 2> v) 
    {
      if(v.shape()(0) != $self->green.shape()(0) ||
	 v.shape()(1) != $self->green.shape()(1))
	throw GeoCal::Exception("Shape of na doesn't match the expected shape");
      $self->green = v;
    }
    void set_blue(blitz::Array<unsigned char, 2> v) 
    {
      if(v.shape()(0) != $self->blue.shape()(0) ||
	 v.shape()(1) != $self->blue.shape()(1))
	throw GeoCal::Exception("Shape of na doesn't match the expected shape");
      $self->blue = v;
    }
    void set_alpha(blitz::Array<unsigned char, 2> v) 
    {
      if(v.shape()(0) != $self->alpha.shape()(0) ||
	 v.shape()(1) != $self->alpha.shape()(1))
	throw GeoCal::Exception("Shape of na doesn't match the expected shape");
      $self->alpha = v;
    }
  }
};
}
#endif
