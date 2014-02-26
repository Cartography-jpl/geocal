#ifndef RASTER_IMAGE_MULTI_BAND_H
#define RASTER_IMAGE_MULTI_BAND_H
#include "printable.h"
#include "raster_image.h"

namespace GeoCal {
  class RasterImageMultiBandVariable;
/****************************************************************//**
  This is a set of RasterImages, one for each band.
*******************************************************************/

class RasterImageMultiBand : public Printable<RasterImageMultiBand> {
public:
  virtual ~RasterImageMultiBand() {}

//-----------------------------------------------------------------------
/// Return number of bands.
//-----------------------------------------------------------------------

  virtual int number_band() const  = 0;

//-----------------------------------------------------------------------
/// Return given band.
//-----------------------------------------------------------------------

  virtual const RasterImage& raster_image(int band) const = 0;
  virtual RasterImage& raster_image(int band) = 0;
  virtual boost::shared_ptr<RasterImage> raster_image_ptr(int band) const = 0;
  virtual blitz::Array<int, 3> read(int Lstart, int Sstart, int Nline, 
				  int Nsamp) const; 
  virtual blitz::Array<double, 3> read_double(int Lstart, int Sstart, 
					      int Nline, int Nsamp) const; 
  void write(int Lstart, int Sstart, const blitz::Array<int, 3>& Data);
  virtual void print(std::ostream& Os) const { Os << "RasterImageMultiBand"; }
  RasterImageMultiBandVariable overview(int Min_number_sample) const;
};


void copy(const RasterImageMultiBand& Img_in, RasterImageMultiBand& Img_out, 
	  bool Log_progress = false);
}
#endif
