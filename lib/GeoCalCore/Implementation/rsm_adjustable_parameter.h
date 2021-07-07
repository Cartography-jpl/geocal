#ifndef RSM_ADJUSTABLE_PARAMETER_H
#define RSM_ADJUSTABLE_PARAMETER_H
#include "printable.h"
#include "with_parameter.h"
#include "ground_coordinate.h"

namespace GeoCal {
/****************************************************************//**
  This is used to handle the RSM adjustable parameters. There are
  currently two version of this, RSMAPA and RSMAPB. We handle the
  specific versions by subclasses, this base class just gives the
  overall interface.
*******************************************************************/

class RsmAdjustableParameter : public virtual WithParameter,
			       public Printable<RsmAdjustableParameter> {
public:
  RsmAdjustableParameter(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="")
    : image_identifier_(Image_identifier),
      rsm_suport_data_edition_(Rsm_support_data_edition),
      triangulation_id_(Triangulation_id)
  {
  }
  virtual ~RsmAdjustableParameter() {}
//-----------------------------------------------------------------------
/// Image identification.
//-----------------------------------------------------------------------

  const std::string& image_identifier() const { return image_identifier_;}
  void image_identifier(const std::string& V) { image_identifier_ = V;}

//-----------------------------------------------------------------------
/// RSM Support Data Edition.
//-----------------------------------------------------------------------

  const std::string& rsm_suport_data_edition() const
  { return rsm_suport_data_edition_;}
  void rsm_suport_data_edition(const std::string& V)
  { rsm_suport_data_edition_ = V; }

//-----------------------------------------------------------------------
/// Triangulation ID
//-----------------------------------------------------------------------

  const std::string& triangulation_id() const { return triangulation_id_;}
  void triangulation_id(const std::string& V) { triangulation_id_ = V;}

//-----------------------------------------------------------------------
/// Adjustment for given ground location
//-----------------------------------------------------------------------

  virtual void adjustment(const GroundCoordinate& Gc,
			  boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
			  double& Lndelta, double& Smpdelta) const = 0;

//-----------------------------------------------------------------------
/// Return the NAIF code for the planet/body we are working with.
//-----------------------------------------------------------------------
  
  virtual int naif_code() const = 0;

//-----------------------------------------------------------------------
/// Set the NAIF code for the planet/body we are working with.
///
/// Note that the NITF TRE structure does not have a place to store
/// the NAIF code, it implicitly assumes earth. So when we read a TRE,
/// even for something like Mars, we have the NAIF code set to
/// earth. We need to update this with other metadata
/// (e.g. TARGET_NAME in PDS label).
///
/// This is not a problem for boost serialization (which keeps the
/// NAIF code), just for NITF TRE.
//-----------------------------------------------------------------------
  
  virtual void naif_code(int Naif_code) = 0;
  
//-----------------------------------------------------------------------
/// Adjustment including sample derivatives.
//-----------------------------------------------------------------------
  virtual void adjustment_with_derivative(const GroundCoordinate& Gc,
	  ArrayAd<double, 1>& Cf_adjusted, AutoDerivative<double>& Lndelta,
	  AutoDerivative<double>& Smpdelta) const = 0;

//-----------------------------------------------------------------------
/// The derivative handling requires different handling for ground
/// coordinate adjusting parameters vs. just image coordinate
/// adjusting parameters. This function lets the Rsm class know if it
/// needs to worry about the ground coordinate adjusting parameters.
//-----------------------------------------------------------------------

  virtual bool has_ground_coordinate_parameter() const = 0;
  std::string base_tre_string() const;
  void base_read_tre_string(std::istream& In);
  virtual void print(std::ostream& Os) const
  { Os << "RsmAdjustableParameter"; }
private:
  std::string image_identifier_, rsm_suport_data_edition_,
    triangulation_id_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmAdjustableParameter);
#endif
