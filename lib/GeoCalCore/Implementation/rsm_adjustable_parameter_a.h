#ifndef RSM_ADJUSTABLE_PARAMETER_A_H
#define RSM_ADJUSTABLE_PARAMETER_A_H
#include "rsm_adjustable_parameter.h"
#include "local_rectangular_coordinate.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
  class RsmId;			// Forward declaration.
  
/****************************************************************//**
  This is used to handle the RSM adjustable parameters. This 
  supports RSMAPA.
*******************************************************************/

class RsmAdjustableParameterA : public boost::noncopyable,
				public RsmAdjustableParameter {
  // Could make copyable, we just haven't bothered. But put this in
  // place so we don't accidentally try to copy
public:
  RsmAdjustableParameterA(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="")
    : RsmAdjustableParameter(Image_identifier, Rsm_support_data_edition,
			     Triangulation_id),
      parm_index(36),
      full_parameter_(36, 0)
  {
    for(int i = 0; i < full_parameter_.rows(); ++i) {
      full_parameter_(i) = 0.0;
      parm_index(i) = -1;
    }
  }
  RsmAdjustableParameterA(const ImageGroundConnection& Igc,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="",
			  bool Activate_image_correction=false,
			  bool Activate_ground_rotation=false,
			  bool Activate_ground_correction=false);
  virtual ~RsmAdjustableParameterA() {}

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
  void activate_image_correction();
  void activate_ground_rotation();
  void activate_ground_correction();

  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  
  virtual bool has_ground_coordinate_parameter() const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameterA>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
  virtual int naif_code() const { return cconv->naif_code(); }
  virtual void naif_code(int Naif_code);
private:
  boost::shared_ptr<LocalRcConverter> cconv;
  // The TRE will in general reorder the parameters
  blitz::Array<int, 1> parm_index;
  int num_parameter() const;
  void delta_x(double x, double y, double z, double& xdelta,
	       double& ydelta, double& zdelta) const;
  void delta_x(double x, double y, double z, AutoDerivative<double>& xdelta,
	       AutoDerivative<double>& ydelta,
	       AutoDerivative<double>& zdelta) const;
  void delta_ls(double x, double y, double z, double& lndelta,
	       double& smpdelta) const;
  void delta_ls(double x, double y, double z, AutoDerivative<double>& lndelta,
		AutoDerivative<double>& smpdelta) const;
  
  // Internally, we store the parameters in a fixed order. This is the
  // order they appear in the TRE. Values that aren't actually set in
  // the TRE are fixed at 0.
  ArrayAd<double, 1> full_parameter_;
  static const std::vector<std::string>& full_parameter_name();
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmAdjustableParameterA);
#endif
