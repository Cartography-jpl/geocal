#ifndef RSM_INDIRECT_COVARIANCE_B_H
#define RSM_INDIRECT_COVARIANCE_B_H
#include "rsm_indirect_covariance.h"
#include "rsm_adjustable_parameter_b.h"

namespace GeoCal {
class RsmId;  // Forward declaration

/****************************************************************//**
  This is used to handle the RSM indirect covariance. This 
  supports RSMECB.
*******************************************************************/
class RsmIndirectCovarianceB : public RsmIndirectCovariance,
			       public RsmBParameterDesc {
public:
  RsmIndirectCovarianceB(const std::string& Image_identifier="",
			const std::string& Rsm_support_data_edition="fake-1",
			const std::string& Triangulation_id="")
    : RsmIndirectCovariance(Image_identifier, Rsm_support_data_edition,
			    Triangulation_id)
  { }
  RsmIndirectCovarianceB(const ImageGroundConnection& Igc,
			  double H_min, double H_max,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="");
  virtual ~RsmIndirectCovarianceB() {}
  
  std::string tre_string() const;
  static boost::shared_ptr<RsmIndirectCovarianceB>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
  virtual bool has_ground_coordinate_parameter() const
  { return RsmBParameterDesc::has_ground_coordinate_parameter(); }
  virtual int naif_code() const { return RsmBParameterDesc::naif_code(); }
  virtual void naif_code(int Naif_code)
  { RsmBParameterDesc::naif_code(Naif_code); }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmIndirectCovarianceB);
#endif
