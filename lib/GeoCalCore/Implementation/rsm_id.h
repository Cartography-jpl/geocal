#ifndef RSM_ID_H
#define RSM_ID_H
#include "rsm_base.h"

namespace GeoCal {
/****************************************************************//**
  This provides additional information about the Rsm, it is what is
  used to populate 
*******************************************************************/

class RsmId : public Printable<RsmId> {
public:
  RsmId(const boost::shared_ptr<RsmBase>& Base,
	const boost::shared_ptr<CoordinateConverter> &Cconv)
    : image_identifier_(Base->image_identifier()),
      rsm_suport_data_edition_(Base->rsm_suport_data_edition())
  {
  }
  virtual ~RsmId() {}
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
  
  std::string tre_string() const;
  static boost::shared_ptr<RsmId>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
private:
  std::string image_identifier_, rsm_suport_data_edition_;
  RsmId() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmId);
#endif
