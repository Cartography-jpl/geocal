#include "material_detect.h"
#include "ibis_file.h"
#include "raster_image_multi_band_variable.h"
#include <boost/foreach.hpp>

using namespace GeoCal;
using namespace blitz;


class RasterImageWrapClosest: public CalcRaster {
public:
  RasterImageWrapClosest(const MaterialDetect& M)
    : CalcRaster(M), m(M) {}
  virtual ~RasterImageWrapClosest() {};
protected:
  virtual void calc(int Lstart, int Sstart) const 
  {  data = m.closest_material_dif(Lstart, Sstart, data.rows(), data.cols()); }
private:
  const MaterialDetect& m;
};

class RasterImageWrapMaterial: public CalcRaster {
public:
  RasterImageWrapMaterial(const MaterialDetect& M, int Class_id)
    : CalcRaster(M), m(M), class_id(Class_id) {}
  virtual ~RasterImageWrapMaterial() {};
protected:
  virtual void calc(int Lstart, int Sstart) const 
  {  data = m.material_dif(Lstart, Sstart, data.rows(), data.cols(), class_id); }
private:
  const MaterialDetect& m;
  int class_id;
};

bool material_class_sort(const MaterialDetect::MaterialClass& M1, 
			 const MaterialDetect::MaterialClass& M2)
{
  return (M1.class_priority < M2.class_priority ||
	  (M1.class_priority == M2.class_priority &&
	   M1.class_id < M2.class_id));
}

//-----------------------------------------------------------------------
/// Constructor. This takes the pan difference band (normally
/// pandif_raster_image from DoughnutAverage), the multispectral
/// bands, the threshold for each material for the pan difference and
/// spectral difference, the class priority for eahc class, and the
/// name of the IBIS file to get the material information.
///
/// The IBIS file should have at least 5 columns (which is all that we
/// read). The first two columns should be full word values, and are
/// the band indices (1 based, rather than the 0 based we use
/// elsewhere). The third should be double (for no good reason, it
/// just is), which is the class id. The fourth and fifth are also
/// double, and are the mean and sigma for that band ratio.
///
/// In generate, the class ID doesn't start from 0. We find the
/// minimum value in the table, and use that as the index into
/// Pan_diff_threshold etc. (so if first id is 8881 then the threshold
/// for 8881 is Pan_diff_threshold[0] and 8885 is
/// Pan_diff_threshold[4]). Obviously we could have just used a map 
/// instead, but this convention fits better with the current way we
/// supply this values in the Shiva scripts.
//-----------------------------------------------------------------------

MaterialDetect::MaterialDetect(
  const boost::shared_ptr<RasterImage>& Pan_data,
  const boost::shared_ptr<RasterImage>& Pan_diff,
  const boost::shared_ptr<RasterImageMultiBand>& Mulspect,
  const blitz::Array<double, 1>& Pan_diff_threshold,
  const blitz::Array<double, 1>& Spectral_diff_threshold,
  const blitz::Array<int, 1>& Class_priority,
  const std::string& Ibis_fname,
  double Pan_shadow_threshold)
: CalcRaster(*Pan_data),
  pan_data(Pan_data), pan_diff(Pan_diff), 
  mulspect(Mulspect), pshadow_thresh(Pan_shadow_threshold)
{
  std::map<int, MaterialClass> mclass;
  IbisFile mfile(Ibis_fname);
  const int band_index1_col = 0;
  const int band_index2_col = 1;
  const int class_id_col = 2;
  const int mean_col = 3;
  const int sigma_col = 4;
  // Find minimum class id
  int min_class_id = (int) mfile.data<double>(0, class_id_col);
  for(int i = 1; i < mfile.number_row(); ++i)
    min_class_id = std::min(min_class_id, (int) mfile.data<double>(i, class_id_col));
  // Allowed index range.
  int max_class_id = min_class_id + 
    ((int) std::max(Pan_diff_threshold.size(), 
		   std::max(Spectral_diff_threshold.size(), 
			    Class_priority.size()))) - 1;
  for(int i = 0; i < mfile.number_row(); ++i) {
    int bind1 = mfile.data<int>(i, band_index1_col);
    int bind2 = mfile.data<int>(i, band_index2_col);
    if(bind1 > bind2)
      std::swap(bind1, bind2);
    int class_id = (int) mfile.data<double>(i, class_id_col);
    double mean = mfile.data<double>(i, mean_col);
    double sigma = mfile.data<double>(i, sigma_col);
    // If first time we've see this class id, fill in basic
    // information for material
    if(mclass.count(class_id) ==0) {
      mclass[class_id].class_id = class_id;
      if(class_id < min_class_id || class_id > max_class_id) {
	Exception e;
	e << "Class ID is out of allowed range (determined by size of class priority,\n"
	  << "pan diff threshold, and spectral distance threshold).\n"
	  << "Class ID:      " << class_id << "\n"
	  << "Allowed range: " << min_class_id << " to " << max_class_id << "\n";
	throw e;
      }
      mclass[class_id].class_priority = Class_priority(class_id - min_class_id);
      mclass[class_id].pdiff_thresh = Pan_diff_threshold(class_id - min_class_id);
      // The 0.01 here is odd, but is the convention used in the old
      // code. We should probably just pass in the right value, but
      // for now this is what is done.
      mclass[class_id].sdiff_thresh = 
	0.01 * Spectral_diff_threshold(class_id - min_class_id);
    }
    mclass[class_id].first_ratio_band.push_back(bind1 - 1);
    mclass[class_id].second_ratio_band.push_back(bind2 - 1);
    mclass[class_id].mean.push_back(mean);
    
    // I'm not sure this makes much sense here, but this is what the
    // old code does;
    const double minsigma = 1.0;
    if(sigma < minsigma)
      sigma = minsigma;
    mclass[class_id].sigma.push_back(sigma);
  }

  // Sort materials so high priority comes first. This lets us short
  // circuit testing against lower priority materials if we already
  // have a match with a higher priority material.
  typedef std::map<int, MaterialClass>::value_type mvaltype;
  BOOST_FOREACH(const mvaltype& m, mclass)
    material.push_back(m.second);
  std::sort(material.begin(), material.end(), material_class_sort);
}

//-----------------------------------------------------------------------
/// This returns an image that gives the distance to the closest
/// material (whose class id will vary from pixel to pixel). By
/// convention we multiple this difference by 100 so it can be viewed
/// more easily in xvd (which prefers integers).
//-----------------------------------------------------------------------

boost::shared_ptr<RasterImage> MaterialDetect::closest_material_raster_image() const
{
  return boost::shared_ptr<RasterImage>(new RasterImageWrapClosest(*this));
}

//-----------------------------------------------------------------------
/// This returns a set of images that gives the distance to each of
/// the materials. This is ordered by class id. By
/// convention we multiple this difference by 100 so it can be viewed
/// more easily in xvd (which prefers integers).
//-----------------------------------------------------------------------

boost::shared_ptr<RasterImageMultiBand> MaterialDetect::material_raster_image() const
{
  boost::shared_ptr<RasterImageMultiBandVariable> res(new RasterImageMultiBandVariable);
  std::set<int> class_id;
  BOOST_FOREACH(const MaterialClass& mc, material)
    class_id.insert(mc.class_id);
  BOOST_FOREACH(int cid, class_id)
    res->add_raster_image(material_raster_image(cid));
  return res;
}

//-----------------------------------------------------------------------
/// This returns a set image that gives the distance to the given
/// material. By convention we multiple this difference by 100 so it can be viewed
/// more easily in xvd (which prefers integers).
//-----------------------------------------------------------------------

boost::shared_ptr<RasterImage> MaterialDetect::material_raster_image(int Class_id) const
{
  return boost::shared_ptr<RasterImage>(new RasterImageWrapMaterial(*this, Class_id));
}

//-----------------------------------------------------------------------
/// Calculate distance for every point from the nearest material. By
/// convention we multiple this by 100.
//-----------------------------------------------------------------------

blitz::Array<double, 2> MaterialDetect::closest_material_dif(int Lstart, int Sstart, 
							     int Number_line,
							     int Number_sample) const
{
  Array<double, 3> msd =
    mulspect->read_double(Lstart, Sstart, Number_line, Number_sample);
  Array<double, 2> res(Number_line, Number_sample);
  for(int ln = 0; ln < data.rows(); ++ln)
    for(int smp = 0; smp < data.cols(); ++smp) {
      int best_match = 0;
      int best_priority = -999;
      double best_sdist = 1e20;
      BOOST_FOREACH(const MaterialClass& mc, material) {
	double sdist = 0;
	for(int i = 0; i < (int) mc.first_ratio_band.size(); ++i) {
	  double v1 = msd(mc.first_ratio_band[i], ln, smp);
	  double v2 = msd(mc.second_ratio_band[i], ln, smp);
	  // Somewhat odd expression, but this matches what
	  // comes from IBIS file.
	  double gratio = 100.0 * ((v1 - v2) / (v1 + v2 + 0.0000037) + 1.0);
	  sdist += fabs(gratio - mc.mean[i]) / mc.sigma[i];
	}
	sdist /= (int) mc.first_ratio_band.size();
	if(sdist < best_sdist) {
	  best_match = mc.class_id;
	  best_priority = mc.class_priority;
	  best_sdist = sdist;
	}
      }
      res(ln, smp) = best_sdist * 100.0;
    }
  return res;
}

//-----------------------------------------------------------------------
/// Calculate the distance for every point from the given material. By
/// convention we multiple this by 100.
//-----------------------------------------------------------------------

blitz::Array<double, 2> MaterialDetect::material_dif(int Lstart, int Sstart, int Number_line,
						     int Number_sample, int Class_id) const
{
  Array<double, 3> msd =
    mulspect->read_double(Lstart, Sstart, Number_line, Number_sample);
  Array<double, 2> res(Number_line, Number_sample);
  for(int ln = 0; ln < data.rows(); ++ln)
    for(int smp = 0; smp < data.cols(); ++smp) {
      double sdist = 0;
      BOOST_FOREACH(const MaterialClass& mc, material) {
	if(mc.class_id == Class_id) { 
	  for(int i = 0; i < (int) mc.first_ratio_band.size(); ++i) {
	    double v1 = msd(mc.first_ratio_band[i], ln, smp);
	    double v2 = msd(mc.second_ratio_band[i], ln, smp);
	    // Somewhat odd expression, but this matches what
	    // comes from IBIS file.
	    double gratio = 100.0 * ((v1 - v2) / (v1 + v2 + 0.0000037) + 1.0);
	    sdist += fabs(gratio - mc.mean[i]) / mc.sigma[i];
	  }
	  sdist /= (int) mc.first_ratio_band.size();
	}
      }
      res(ln, smp) = sdist * 100.0;
    }
  return res;
}

void MaterialDetect::calc(int Lstart, int Sstart) const
{
// We may be able to do something more intelligent in the future, but
// for now just read in all the data at the beginning.
  Array<double, 2> pand =
    pan_data->read_double(Lstart, Sstart, data.rows(), data.cols());
  Array<double, 2> pandiffd = 
    pan_diff->read_double(Lstart, Sstart, data.rows(), data.cols());
  Array<double, 3> msd =
    mulspect->read_double(Lstart, Sstart, data.rows(), data.cols());
  for(int ln = 0; ln < data.rows(); ++ln)
    for(int smp = 0; smp < data.cols(); ++smp) {
      int best_match = 0;
      int best_priority = -999;
      double best_sdist = 1e20;
      BOOST_FOREACH(const MaterialClass& mc, material) {
	// Don't even bother looking at class if we already have a
	// higher priority match, or if we don't pass our pan_diff
	// threshold. 
	if((best_priority < 0 || best_priority == mc.class_priority) &&
	   pand(ln, smp) > pshadow_thresh &&
	   pandiffd(ln, smp) > mc.pdiff_thresh) {
	  // Calculate spectral distance.
	  double sdist = 0;
	  for(int i = 0; i < (int) mc.first_ratio_band.size(); ++i) {
	    double v1 = msd(mc.first_ratio_band[i], ln, smp);
	    double v2 = msd(mc.second_ratio_band[i], ln, smp);
	    // Somewhat odd expression, but this matches what
	    // comes from IBIS file.
	    double gratio = 100.0 * ((v1 - v2) / (v1 + v2 + 0.0000037) + 1.0);
	    sdist += fabs(gratio - mc.mean[i]) / mc.sigma[i];
	  }
	  sdist /= (int) mc.first_ratio_band.size();
	  if(sdist < mc.sdiff_thresh && 
	     sdist < best_sdist) {
	    best_match = mc.class_id;
	    best_priority = mc.class_priority;
	    best_sdist = sdist;
	  }
	}
      }
      data(ln, smp) = best_match;
    }
}

