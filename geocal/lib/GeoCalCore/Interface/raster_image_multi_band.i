// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "raster_image_multi_band.h"
#include "raster_image_multi_band_variable.h"
%}
%base_import(generic_object)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::RasterImageMultiBand);

namespace GeoCal {
class RasterImageMultiBandVariable;
class RasterImageMultiBand : public GenericObject {
public:
  %rename(raster_image) raster_image_ptr;
  %python_attribute(number_band, virtual int)
  virtual boost::shared_ptr<RasterImage> raster_image_ptr(int band) = 0;
  blitz::Array<int, 3> read(int Lstart, int Sstart, int Nline, 
				  int Nsamp) const; 
  blitz::Array<double, 3> read_double(int Lstart, int Sstart, int Nline, 
				      int Nsamp) const; 
  void write(int Lstart, int Sstart, const blitz::Array<int, 3>& Data);
  std::string print_to_string() const;
  RasterImageMultiBandVariable subset(int line_offset, int sample_offset,
				      int nline, int nsamp) const;
  RasterImageMultiBandVariable overview(int Min_number_sample) const;
};

void copy(const RasterImageMultiBand& Img_in, RasterImageMultiBand& Img_out, 
	  bool Log_progress = false);

}

%import "raster_image_multi_band_variable.i"
