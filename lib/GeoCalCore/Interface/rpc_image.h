#ifndef RPC_IMAGE_H
#define RPC_IMAGE_H
#include "geocal_rpc.h"
#include "raster_image_variable.h"
#include "feature_detector.h"
#include "image_matcher.h"
#include "ccorr_lsm_matcher.h"
#include "forstner_feature_detector.h"
#include "ground_mask.h"

namespace GeoCal {
/****************************************************************//**
   \deprecated This class has been almost entirely replaced with 
   RpcImageGroundConnection. We'll leave this code in place for now,
   because this includes blunder detection that is not used by
   RpcImageGroundConnection. But you almost certainly do want to use
   this class.

   This handles both an Rpc, and the RasterImage data the Rpc map
   projects. 

   Some of the data we work with is high resolution (e.g., Quickbird
   at 60 cm). For some purposes, we may want to work with a lower
   resolution version (e.g., image match with CIB data at 5 m). This
   class allows the resolution of the map projection to be given, in
   which case we will average the underlying RasterImage using
   RasterAveraged, before doing the map projection.

   We calculate imagery on demand. This means the imagery is always
   up to date, so changes to the Rpc get reflected in the imagery.
   However, if you are going to be accessing the same data more than
   once w/o updating the Rpc, you'll want to generate a static copy so
   you don't repeatedly calculate the same data over and over
   e.g., use a MemoryRasterImage to copy the data into memory.

   As you update the Rpc parameters, the area covered on the surface
   changes. This class does *not* update the MapInfo describing the
   area covered. However, you can call map_info_bounding_update() to
   update this information if desired.
*******************************************************************/

class RpcImage : public RasterImageVariable {
public:
  RpcImage(const boost::shared_ptr<RasterImage>& Raw_img, const Rpc& R,
	   const boost::shared_ptr<Dem>& D, const MapInfo& Mi, 
	   int Avg_fact = -1);
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------
  
  virtual ~RpcImage() {}

//-----------------------------------------------------------------------
/// DEM.
//-----------------------------------------------------------------------
  
  const Dem& dem() const { return *dem_;}
  boost::shared_ptr<Dem> dem_ptr() const {return dem_;}
  MapInfo map_info_bounding() const;
  void map_info_bounding_update();

//-----------------------------------------------------------------------
/// Image that is being map projected. This is in the instrument space.
//-----------------------------------------------------------------------

  const RasterImage& raw_image() const {return *raw_img_;}
  boost::shared_ptr<RasterImage> raw_image_ptr() const {return raw_img_;}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
///
/// It is ok if the Line, Sample results in a raw image location
/// outside of the image, in that case we return 0.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const
  { return (int) value(Line, Sample); }
  virtual double unchecked_read_double(int Line, int Sample) const
  { return value(Line, Sample); }

  double value(int Line, int Sample) const;
  double value(int Line, int Sample, double Height) const;
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;
  virtual void unchecked_write(int Line, int Sample, int Val);
  virtual void unchecked_write(int Line, int Sample, double Val);
  void fit(const RasterImage& Ref_img,
	   double Max_diff = 1000,
           const GroundMask& M = CombinedGroundMask(),
	   const FeatureDetector& Fd = ForstnerFeatureDetector(), 
	   const ImageMatcher& Coarse_im = CcorrMatcher(),
	   const ImageMatcher& Fine_im = CcorrLsmMatcher(true),
	   double Match_search = 15,
	   int Feature_grid_space = 100,
	   double Blunder_threshold = 3,
	   double Chisq_threshold = 1.0,
	   bool Blunder_detect = true);

//-----------------------------------------------------------------------
/// Return the location in the reference image used in the final
/// iteration of fit, for diagnostic purposes. Note that you should
/// also look at blunder, as some of these points might be marked as
/// blunders. 
//-----------------------------------------------------------------------

  const std::vector<boost::shared_ptr<GroundCoordinate> >& 
  reference_image_ground_point() const { return gref; }

//-----------------------------------------------------------------------
/// Return the location in the new image used in the final
/// iteration of fit, for diagnostic purposes. Note that you should
/// also look at blunder, as some of these points might be marked as
/// blunders. 
//-----------------------------------------------------------------------

  const std::vector<boost::shared_ptr<GroundCoordinate> >&
  new_image_ground_point() const { return gnew; }

//-----------------------------------------------------------------------
/// Return the location in the reference image used in the final
/// iteration of fit, for diagnostic purposes. This point had image
/// matching fail on it, see image_match_diagnostic for code as to why
/// it failed.
//-----------------------------------------------------------------------

  const std::vector<boost::shared_ptr<GroundCoordinate> >& 
  failed_ground_point() const { return failed_gp; }

//-----------------------------------------------------------------------
/// Return the code indicating why an image matching failed.
//-----------------------------------------------------------------------

  const std::vector<int>&
  image_match_diagnostic() const { return im_diagnostic; }

//-----------------------------------------------------------------------
/// Return the code indicating any diagnostics for points that matched
/// successfully 
//-----------------------------------------------------------------------

  const std::vector<int>&
  image_match_succesful_diagnostic() const { return im_success_diagnostic; }

//-----------------------------------------------------------------------
/// Return distance in meters between reference_image_ground_point and
/// new_image_ground_point, for diagnostic purposes. This is a measure
/// of how much the images disagree after the fitting done by fit(). 
/// Note that you should also look at blunder, as some of these points 
/// might be marked as
/// blunders. 
//-----------------------------------------------------------------------

  const std::vector<double>& distance_new_and_reference() const
  { return dist; }

//-----------------------------------------------------------------------
/// Points in the last iteration of fit() that were marked as
/// blunders. 
//-----------------------------------------------------------------------

  const std::set<int> blunder() const {return blunder_;}

  const RasterImage& img_avg() const {return *img_avg_;}
  virtual bool copy_needs_double() const {return raw_img_->copy_needs_double();}
private:
  boost::shared_ptr<Dem> dem_;		   ///< DEM.
  boost::shared_ptr<RasterImage> raw_img_; ///< Raw image
  boost::shared_ptr<RasterImage> img_avg_; 
				///< Image, possibly averaged.
  int line_avg_fact_;		///< Factor we have averaged by.
  int sample_avg_fact_;		///< Factor we have averaged by.
  std::vector<boost::shared_ptr<GroundCoordinate> > gref; 
				///< List of points in reference image
  std::vector<boost::shared_ptr<GroundCoordinate> > gnew;
				///< List of points in new image
  std::vector<boost::shared_ptr<GroundCoordinate> > failed_gp; 
				///< List of points in reference image
				///that failed image matching.
  std::vector<int> im_diagnostic; ///< Code for why point failed image
				  ///matching.
  std::vector<int> im_success_diagnostic; 
				///< Code for why points that
				///succeeded. For CcorrLsmMatcher,
				///this indicates if LsmMatcher was
				///successful or not.
  std::vector<ImageCoordinate> ic;
				///< Location in raw_img_
				/// corresponding to gnew
  std::vector<double> lsigma;	///< Uncertainty in ic[i].line.
  std::vector<double> ssigma;	///< Uncertainty in ic[i].sample.
  std::set<int> blunder_;	///< Points marked as blunders
  std::vector<double> dist;	///< Distance between points.
  std::vector<ImageCoordinate> ip_grid;
				///< Interest points.
  boost::shared_ptr<RasterImage> ref_img;
                                /// Reference image to use during
                                /// matching and interest point.
  void set_ref_img(const RasterImage& Ref_img, 
		   const RasterImage& New_img);
  void interest_point(const FeatureDetector& Fd, const GroundMask& M,
		      int Feature_grid_space);
  double match(const RasterImage& New_img,
	       const ImageMatcher& Im);
  double fit_fine(const ImageMatcher& Im,
		  double Blunder_threshold = 3,
		  double Chisq_threshold = 0.8,
		  bool Blunder_detect = true);
  double fit_coarse(const RasterImage& Ref_img,
		    const FeatureDetector& Fd, const GroundMask& M,
		    const ImageMatcher& Im,
		    double Max_diff,
		    double Match_search,
		    int Feature_grid_space);
};

}
#endif

