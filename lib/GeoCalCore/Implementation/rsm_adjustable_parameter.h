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

class RsmAdjustableParameter : public WithParameter,
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
