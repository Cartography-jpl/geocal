#ifndef RSM_ADJUSTABLE_PARAMETER_H
#define RSM_ADJUSTABLE_PARAMETER_H
#include "printable.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This is used to handle the RSM adjustable parameters. Right now,
  we only support RSMAPA. Could add RSMAPB if we end up needing it, 
  (perhaps as a new subclass?)

*******************************************************************/

class RsmAdjustableParameter : public boost::noncopyable,
			       public Printable<RsmAdjustableParameter> {
  // Could make copyable, we just haven't bothered. But put this in
  // place so we don't accidentally try to copy
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

  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameter>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
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
