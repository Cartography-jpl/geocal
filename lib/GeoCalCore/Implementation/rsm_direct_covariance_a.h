#ifndef RSM_DIRECT_COVARIANCE_A_H
#define RSM_DIRECT_COVARIANCE_A_H
#include "rsm_direct_covariance.h"
#include "local_rectangular_coordinate.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
  class RsmId;			// Forward declaration.
  
/****************************************************************//**
  This handles the direct error covariance for and RSM. There are
  two versions of this, RSMDCA and RSMDCB. This is the A version.
*******************************************************************/

class RsmDirectCovarianceA : public RsmDirectCovariance,
			     public boost::noncopyable  {
  // Could make copyable, we just haven't bothered. But put this in
  // place so we don't accidentally try to copy
public:
  RsmDirectCovarianceA(const std::string& Image_identifier="",
		       const std::string& Rsm_support_data_edition="fake-1",
		       const std::string& Triangulation_id="")
    : RsmDirectCovariance(Image_identifier, Rsm_support_data_edition,
			  Triangulation_id),
      parm_index(36)
  {
    for(int i = 0; i < parm_index.rows(); ++i) {
      parm_index(i) = -1;
    }
  }
  RsmDirectCovarianceA(const ImageGroundConnection& Igc,
		       const RsmId& Rsm_id,
		       const std::string& Triangulation_id="",
		       bool Activate_image_correction=false,
		       bool Activate_ground_rotation=false,
		       bool Activate_ground_correction=false);
  virtual ~RsmDirectCovarianceA() {}
  
  const boost::shared_ptr<LocalRcConverter>&
  coordinate_converter() const { return cconv; }
  void coordinate_converter(const
    boost::shared_ptr<LocalRcConverter>& V) 
  { cconv = V; }

//-----------------------------------------------------------------------
/// Covariance data
//-----------------------------------------------------------------------

  const blitz::Array<double, 2> covariance() const { return cov; }
  void covariance(const blitz::Array<double, 2>& V)
  { cov.reference(V.copy()); }
      
//-----------------------------------------------------------------------
/// Number of images associated with this RsmDirectCovarianceA
//-----------------------------------------------------------------------

  int number_image() const { return (int) npari.size(); }
  
//-----------------------------------------------------------------------
/// Image identifier for each image
//-----------------------------------------------------------------------

  const std::string& image_identifier_i(int ind) const
  {
    range_check(ind, 0, number_image());
    return iidi[ind];
  }
  
//-----------------------------------------------------------------------
/// Number of parameters for each image.
//-----------------------------------------------------------------------

  int number_parameter_i(int ind) const
  {
    range_check(ind, 0, number_image());
    return npari[ind];
  }

//-----------------------------------------------------------------------
/// Add a new image to the set associated with this covariance
//-----------------------------------------------------------------------

  void add_image(const std::string& Image_identifier, int Number_parm)
  {
    iidi.push_back(Image_identifier);
    npari.push_back(Number_parm);
  }
  void activate_image_correction();
  void activate_ground_rotation();
  void activate_ground_correction();
  virtual std::vector<std::string> parameter_name() const;
  
  std::string tre_string() const;
  static boost::shared_ptr<RsmDirectCovarianceA>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
  virtual int naif_code() const { return cconv->naif_code(); }
  virtual void naif_code(int Naif_code);
private:
  boost::shared_ptr<LocalRcConverter> cconv;
  // The TRE will in general reorder the parameters
  blitz::Array<int, 1> parm_index;
  std::vector<std::string> iidi;
  std::vector<int> npari;
  blitz::Array<double, 2> cov;
  int num_parameter() const;
  int total_num_parameter() const;
  static const std::vector<std::string>& full_parameter_name();
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmDirectCovarianceA);
#endif
  
