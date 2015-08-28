// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "scale_image.h"
%}
%base_import(calc_raster)
%base_import(calc_raster_multi_band)
%base_import(image_ground_connection)
%geocal_shared_ptr(GeoCal::ScaleImage);
%geocal_shared_ptr(GeoCal::ScaleImageMultiBand);
%geocal_shared_ptr(GeoCal::ScaleImageGroundConnection);

namespace GeoCal {
class ScaleImage : public CalcRaster {
public:
  ScaleImage(const boost::shared_ptr<RasterImage>& Data,
	     double Scale_factor);
  %python_attribute(raw_data, boost::shared_ptr<RasterImage>)
  %python_attribute(scale_factor, double)
  %pickle_serialization();
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

class ScaleImageMultiBand : public CalcRasterMultiBand {
public:
  ScaleImageMultiBand(const boost::shared_ptr<RasterImageMultiBand>& Data,
	     double Scale_factor);
  %python_attribute(raw_data, boost::shared_ptr<RasterImageMultiBand>)
  %python_attribute(scale_factor, double)
  %pickle_serialization();
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

class ScaleImageGroundConnection: public ImageGroundConnectionCopy {
public:
  ScaleImageGroundConnection(const boost::shared_ptr<ImageGroundConnection>& Igc, double Scale_factor);
  %pickle_serialization();
};

}
