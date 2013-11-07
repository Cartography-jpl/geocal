#ifndef SUB_RASTER_IMAGE_MULTI_BAND_H
#define SUB_RASTER_IMAGE_MULTI_BAND_H

#include "raster_image_multi_band_variable.h"

namespace GeoCal {
/****************************************************************//**
  This is a subset of RasterImageMultiBand.
*******************************************************************/

class SubRasterImageMultiBand : public RasterImageMultiBandVariable {
public:
  SubRasterImageMultiBand(const boost::shared_ptr<RasterImageMultiBand>& Rmb,
			  int Start_line, int Start_sample,
			  int Number_line, int Number_sample);
  virtual ~SubRasterImageMultiBand() {}
  virtual blitz::Array<int, 3> read(int Lstart, int Sstart, int Nline, 
				  int Nsamp) const; 
  virtual blitz::Array<double, 3> read_double(int Lstart, int Sstart, 
					      int Nline, int Nsamp) const; 
  void write(int Lstart, int Sstart, const blitz::Array<int, 3>& Data);
  virtual void print(std::ostream& Os) const;
  int start_line() const {return sl;}
  int start_sample() const {return ss;}
  boost::shared_ptr<RasterImageMultiBand> full_raster_image_multi_band()
    const { return rmb; }
private:
  boost::shared_ptr<RasterImageMultiBand> rmb;
  int sl, ss, nl, ns;
};

}
#endif
