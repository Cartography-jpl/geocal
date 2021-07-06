#ifndef RSM_ADJUSTABLE_PARAMETER_B_H
#define RSM_ADJUSTABLE_PARAMETER_B_H
#include "rsm_adjustable_parameter.h"
#include "local_rectangular_coordinate.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
  class RsmId;			// Forward declaration.
  
/****************************************************************//**
  This is used to handle the RSM adjustable parameters. This 
  supports RSMAPB.
*******************************************************************/

class RsmAdjustableParameterB : public boost::noncopyable,
				public RsmAdjustableParameter {
  // Could make copyable, we just haven't bothered. But put this in
  // place so we don't accidentally try to copy
public:
  RsmAdjustableParameterB(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="")
    : RsmAdjustableParameter(Image_identifier, Rsm_support_data_edition,
			     Triangulation_id)
  {
  }
  RsmAdjustableParameterB(const ImageGroundConnection& Igc,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="");
  virtual ~RsmAdjustableParameterB() {}

  const boost::shared_ptr<LocalRcConverter>&
  coordinate_converter() const { return cconv; }
  void coordinate_converter(const
    boost::shared_ptr<LocalRcConverter>& V) 
  { cconv = V; }

  virtual void adjustment(const GroundCoordinate& Gc,
			  boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
			  double& Lndelta, double& Smpdelta) const;
  virtual void adjustment_with_derivative(const GroundCoordinate& Gc,
	  ArrayAd<double, 1>& Cf_adjusted, AutoDerivative<double>& Lndelta,
	  AutoDerivative<double>& Smpdelta) const;
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual bool has_ground_coordinate_parameter() const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameterB>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
  virtual int naif_code() const { return cconv->naif_code(); }
  virtual void naif_code(int Naif_code);
private:
  boost::shared_ptr<LocalRcConverter> cconv;
  static const std::vector<std::string>& full_parameter_name();
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmAdjustableParameterB);
#endif
