#ifndef RSM_INDIRECT_COVARIANCE_H
#define RSM_INDIRECT_COVARIANCE_H
#include "printable.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This provides additional information about the Rsm, it is what is
  used to populate 
*******************************************************************/

class RsmIndirectCovariance : public Printable<RsmIndirectCovariance> {
public:
  RsmIndirectCovariance()
  {
  }
  virtual ~RsmIndirectCovariance() {}
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
  static boost::shared_ptr<RsmIndirectCovariance>
  read_tre_string(const std::string& Tre_in);
private:
  std::string image_identifier_, rsm_suport_data_edition_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmIndirectCovariance);
#endif
