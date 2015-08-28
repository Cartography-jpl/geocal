#ifndef SCALE_IMAGE_H
#define SCALE_IMAGE_H
#include "calc_raster.h"
#include "calc_raster_multi_band.h"
#include "image_ground_connection.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple adapter that multiples a underlying image by a
  scale factor.
*******************************************************************/

class ScaleImage : public CalcRaster {
public:
  ScaleImage(const boost::shared_ptr<RasterImage>& Data,
	     double Scale_factor)
    : CalcRaster(*Data), raw_data_(Data), scale_(Scale_factor) {}
  virtual ~ScaleImage() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "ScaleImage\n"
       << "  scale factor: " << scale_ << "\n"
       << "  underlying raster: \n" << *raw_data_ << "\n";
  }
  const boost::shared_ptr<RasterImage>& raw_data() const {return raw_data_;}
  double scale_factor() const {return scale_;}
protected:
  virtual void calc(int Lstart, int Sstart) const
  {
    data = scale_ * raw_data_->read_double(Lstart, Sstart, data.rows(), 
					 data.cols());
  }
private:
  boost::shared_ptr<RasterImage> raw_data_;
  double scale_;
  ScaleImage() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a simple adapter that multiples a underlying image by a
  scale factor.
*******************************************************************/

class ScaleImageMultiBand : public CalcRasterMultiBand {
public:
  ScaleImageMultiBand(const boost::shared_ptr<RasterImageMultiBand>& Data,
		      double Scale_factor)
    : CalcRasterMultiBand(Data->raster_image(0), Data->number_band()), 
      raw_data_(Data), scale_(Scale_factor) {}
  virtual ~ScaleImageMultiBand() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "ScaleImageMultiBand\n"
       << "  scale factor: " << scale_ << "\n"
       << "  underlying raster multi_band: \n" << *raw_data_ << "\n";
  }
  const boost::shared_ptr<RasterImageMultiBand>& 
  raw_data() const {return raw_data_;}
  double scale_factor() const {return scale_;}
protected:
  virtual void calc(int Lstart, int Sstart) const
  {
    data = scale_ * raw_data_->read_double(Lstart, Sstart, data.cols(), 
					   data.depth());
  }
private:
  boost::shared_ptr<RasterImageMultiBand> raw_data_;
  double scale_;
  ScaleImageMultiBand() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

class ScaleImageGroundConnection: public virtual ImageGroundConnectionCopy {
public:
  ScaleImageGroundConnection(const boost::shared_ptr<ImageGroundConnection>& Igc, double Scale_factor);
  virtual ~ScaleImageGroundConnection() {}
  virtual void print(std::ostream& Os) const;
protected:
  ScaleImageGroundConnection() {}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(ScaleImage);
GEOCAL_EXPORT_KEY(ScaleImageMultiBand);
GEOCAL_EXPORT_KEY(ScaleImageGroundConnection);
#endif
