#ifndef IGC_SIMULATED_RAY_CASTER_H
#define IGC_SIMULATED_RAY_CASTER_H
#include "image_ground_connection.h"
#include "calc_raster.h"
namespace GeoCal {
/****************************************************************//**
  This is a RasterImage that is a simulated based on an underlying 
  map projected image. For each point in the image of an
  ImageGroundConnection, we calculate where that point appears on the
  ground and then assign a value to it from the map projected image. 
  This is done using a IgcRayCaster.

  This is essentially the inverse of IgcMapProjected, if you replace
  the image of an ImageGroundConnection with this simulated image and
  then run IgcMapProjected you'll get the original map projected image
  back (with changes from being resampled twice). This is similar to
  the old MisrSim program.

  This class calculates the data on the fly. Sometimes
  this is what you want, but if you are going to be using the
  resulting data a few times, you may want to use a
  MemoryRasterImage to generate a copy once and keep it in
  memory.
*******************************************************************/

class IgcSimulatedRayCaster : public CalcRaster {
public:
  IgcSimulatedRayCaster(const boost::shared_ptr<ImageGroundConnection>& Igc,
	      const boost::shared_ptr<RasterImage>& Map_projected_image, 
              int Number_integration_step = 1,
              double Raycast_resolution = -1,
              double Max_height = 10e3,
              int Number_tile_line = -1,
	      int Number_tile_sample = -1,
	      double Fill_value = 0.0
	      );
  virtual ~IgcSimulatedRayCaster() {}
  virtual void print(std::ostream& Os) const;
  const boost::shared_ptr<ImageGroundConnection>& igc() const
  { return igc_; }
  int number_integration_step() const { return number_integration_step_; }
  double fill_value() const {return fill_value_;}
  double max_height() const {return max_height_;}
  double raycast_resolution() const {return raycast_resolution_;}
  const boost::shared_ptr<RasterImage>& map_projected_image() const
  { return img_; }
protected:
  virtual void calc(int Lstart, int Sstart) const;
  IgcSimulatedRayCaster() {}
private:
  boost::shared_ptr<ImageGroundConnection> igc_;
  int number_integration_step_;
  double fill_value_, raycast_resolution_, max_height_;
  boost::shared_ptr<RasterImage> img_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcSimulatedRayCaster);
#endif
