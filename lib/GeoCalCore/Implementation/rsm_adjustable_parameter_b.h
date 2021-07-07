#ifndef RSM_ADJUSTABLE_PARAMETER_B_H
#define RSM_ADJUSTABLE_PARAMETER_B_H
#include "rsm_adjustable_parameter.h"
#include "local_rectangular_coordinate.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
  class RsmId;			// Forward declaration.

/****************************************************************//**
  It turns out the block of data that described the parameter B
  structure (e.g. which parameters are used) is used in several
  places (RSMAPB, RSMECB, RSMDCA). We pull this piece out so it
  can get reused.
*******************************************************************/

class RsmBParameterDesc : public boost::noncopyable,
			  public virtual WithParameter,
			  public Printable<RsmBParameterDesc> {
  // Could make copyable, we just haven't bothered. But put this in
  // place so we don't accidentally try to copy
public:
  RsmBParameterDesc()
    :  normalization_scale_(3),
       normalization_offset_(3)
  { }
  RsmBParameterDesc(const ImageGroundConnection& Igc,
		    double H_min, double H_max);
  virtual ~RsmBParameterDesc() {}
  const boost::shared_ptr<LocalRcConverter>&
  coordinate_converter() const { return cconv; }
  void coordinate_converter(const
    boost::shared_ptr<LocalRcConverter>& V) 
  { cconv = V; }
  virtual bool has_ground_coordinate_parameter() const;
  virtual void print(std::ostream& Os) const;
  virtual int naif_code() const { return cconv->naif_code(); }
  virtual void naif_code(int Naif_code);
  int number_parameter() const
  {
    if(using_basis())
      return 1;
    if(has_ground_coordinate_parameter())
      return 1;
    else
      return row_power_.rows() + col_power_.rows();
  }

  const blitz::Array<double, 1>& normalization_scale() const
  { return normalization_scale_;}
  blitz::Array<double, 1> normalization_offset() const
  { return normalization_offset_;}
  bool using_basis() const { return false;}
  const blitz::Array<int, 2>& row_power() const { return row_power_;}
  const blitz::Array<int, 2>& col_power() const { return col_power_;}
  void row_power(const blitz::Array<int, 2>& V)
  {row_power_.reference(V.copy());}
  void col_power(const blitz::Array<int, 2>& V)
  {col_power_.reference(V.copy());}
  std::string rsm_b_parm_tre_string() const;
  void rsm_b_parm_read_tre_string(std::istream& In);
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return parameter_; }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { parameter_.reference(Parm.copy()); }
  // Isn't a natural name here, so for now just use the default
  // name. Perhaps we will figure out a better one.
  // virtual std::vector<std::string> parameter_name() const;

protected:  
  void delta_x(double x, double y, double z, double& xdelta,
	       double& ydelta, double& zdelta) const;
  void delta_x(double x, double y, double z, AutoDerivative<double>& xdelta,
	       AutoDerivative<double>& ydelta,
	       AutoDerivative<double>& zdelta) const;
  void delta_ls(double x, double y, double z, double& lndelta,
	       double& smpdelta) const;
  void delta_ls(double x, double y, double z, AutoDerivative<double>& lndelta,
		AutoDerivative<double>& smpdelta) const;
  boost::shared_ptr<GroundCoordinate>
  convert_from_coordinate(double X, double Y, double Z = 0) const;
  void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			     double& Y, double& Z) const;
  ArrayAd<double, 1> convert_to_cf(const AutoDerivative<double>& X,
				   const AutoDerivative<double>& Y,
				   const AutoDerivative<double>& Z) const;
  boost::shared_ptr<LocalRcConverter> cconv;
  blitz::Array<double, 1> normalization_scale_;
  blitz::Array<double, 1> normalization_offset_;
  blitz::Array<int, 2> row_power_;
  blitz::Array<int, 2> col_power_;
  ArrayAd<double, 1> parameter_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  This is used to handle the RSM adjustable parameters. This 
  supports RSMAPB.
*******************************************************************/

class RsmAdjustableParameterB : public RsmBParameterDesc,
				public RsmAdjustableParameter {
public:
  RsmAdjustableParameterB(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="")
    : RsmAdjustableParameter(Image_identifier, Rsm_support_data_edition,
			     Triangulation_id)
  {
  }
  RsmAdjustableParameterB(const ImageGroundConnection& Igc,
			  double H_min, double H_max,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="");
  virtual ~RsmAdjustableParameterB() {}

  virtual void adjustment(const GroundCoordinate& Gc,
			  boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
			  double& Lndelta, double& Smpdelta) const;
  virtual void adjustment_with_derivative(const GroundCoordinate& Gc,
	  ArrayAd<double, 1>& Cf_adjusted, AutoDerivative<double>& Lndelta,
	  AutoDerivative<double>& Smpdelta) const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameterB>
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

GEOCAL_EXPORT_KEY(RsmAdjustableParameterB);
GEOCAL_EXPORT_KEY(RsmBParameterDesc);
#endif
