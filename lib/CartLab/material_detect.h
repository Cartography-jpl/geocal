#ifndef MATERIAL_DETECT_H
#define MATERIAL_DETECT_H
#include "calc_raster.h"
#include "raster_image_multi_band.h"

namespace GeoCal {

/****************************************************************//**
  This class is used to do change detection and identify the material
  for pixels that have changed.

  This duplicates what was done in the VICAR proc "detwvpan", and in
  particular the "f2ratio" process. 

  We read a IBIS file that describes the thresholds for a number of
  material classes. For each material class, we do the following:

  1. First, the difference in the pan bands is compared to a
     threshold. The difference is normally calculated by
     DoughnutAverage, using the pandif_raster_image. But something
     else could be used, all this class cares about is having a
     difference it can compare against a threshold.
  2. Compare pan data to a shadow threshold. This masks at very dark
     pixels that we assume are in shadow.
  3. For each point, we calculate the required ratios between the
     multispectral bands, take the difference with the supplied class
     mean and divide by the class sigma.
  4. We sum the abs value of for each of the band ratios divided by
     the number of band ratios (i.e, we use a L1 norm). This is
     compared against a second threshold.

  We may have more than one class that passes the second threshold. In
  that case, we sort the classes first by a class priority (with the
  lower number being selected first). For ties, we then pick the class
  that has the smallest difference norm.
*******************************************************************/

class MaterialDetect : public CalcRaster {
public:
  MaterialDetect(const boost::shared_ptr<RasterImage>& Pan_data,
		 const boost::shared_ptr<RasterImage>& Pan_diff,
		 const boost::shared_ptr<RasterImageMultiBand>& Mulspect,
		 const blitz::Array<double, 1>& Pan_diff_threshold,
		 const blitz::Array<double, 1>& Spectral_diff_threshold,
		 const blitz::Array<int, 1>& Class_priority,
		 const std::string& Ibis_fname,
		 double Pan_shadow_threshold);
  virtual ~MaterialDetect() {}
  boost::shared_ptr<RasterImage> closest_material_raster_image() const;
  boost::shared_ptr<RasterImageMultiBand> material_raster_image() const;
  boost::shared_ptr<RasterImage> material_raster_image(int Class_id) const;
  blitz::Array<double, 2> closest_material_dif(int Lstart, int Sstart, int Number_line,
					       int Number_sample) const;
  blitz::Array<double, 2> material_dif(int Lstart, int Sstart, int Number_line,
				       int Number_sample, int Class_id) const;

  struct MaterialClass {
    std::vector<int> first_ratio_band;
    std::vector<int> second_ratio_band;
    int class_id;
    std::vector<double> mean;
    std::vector<double> sigma;
    int class_priority;
    double pdiff_thresh, sdiff_thresh;
  };
protected:
  virtual void calc(int Lstart, int Sstart) const; 
private:
  boost::shared_ptr<RasterImage> pan_data;
  boost::shared_ptr<RasterImage> pan_diff;
  boost::shared_ptr<RasterImageMultiBand> mulspect;
  double pshadow_thresh;
  // Note materials are sorted by class priority and then by id (so
  // higher priority with a lower class priority number come first).
  // This way we can avoid processing lower priority materials if we
  // already have a match with a higher priority material.
  std::vector<MaterialClass> material;
};
}
#endif
