// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "rpc_image.h"
%}

%geocal_shared_ptr(RpcImage);
namespace GeoCal {
class RpcImage : public RasterImageVariable {
public:
  RpcImage(const boost::shared_ptr<RasterImage>& Raw_img, const Rpc& R,
	   const boost::shared_ptr<Dem>& D, const MapInfo& Mi, 
	   int Avg_fact = -1);
  %python_attribute2(dem, dem_ptr, boost::shared_ptr<Dem>)
  MapInfo map_info_bounding() const;
  void map_info_bounding_update();
  %python_attribute2(raw_image, raw_image_ptr, boost::shared_ptr<RasterImage>)
  virtual int operator()(int Line, int Sample) const;
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;
  virtual void write(int Line, int Sample, int Val);
  void fit(const RasterImage& Ref_img,
	   double Max_diff = 1000,
	   const Mask& M = CombinedMask(),
	   const FeatureDetector& Fd = ForstnerFeatureDetector(), 
	   const ImageMatcher& Coarse_im = CcorrMatcher(),
	   const ImageMatcher& Fine_im = CcorrLsmMatcher(),
	   double Match_search = 15,
	   int Feature_grid_space = 15,
	   double Blunder_threshold = 3,
	   double Chisq_threshold = 0.8,
	   bool Blunder_detect = true);
  const std::vector<boost::shared_ptr<GroundCoordinate> >& 
  reference_image_ground_point() const;
  const std::vector<boost::shared_ptr<GroundCoordinate> >&
  new_image_ground_point() const;
  const std::vector<double>& distance_new_and_reference() const;
  const std::set<int> blunder() const;
  const std::vector<boost::shared_ptr<GroundCoordinate> >& 
  failed_ground_point() const;
  const std::vector<int>&
  image_match_diagnostic() const;
};
}
