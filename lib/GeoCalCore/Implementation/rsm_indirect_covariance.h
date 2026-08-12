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
  RsmIndirectCovariance(const std::string& Image_identifier="",
			const std::string& Rsm_support_data_edition="fake-1",
			const std::string& Triangulation_id="")
    : image_identifier_(Image_identifier),
      rsm_support_data_edition_(Rsm_support_data_edition),
      triangulation_id_(Triangulation_id)
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

  const std::string& rsm_support_data_edition() const
  { return rsm_support_data_edition_;}
  void rsm_support_data_edition(const std::string& V)
  { rsm_support_data_edition_ = V; }

//-----------------------------------------------------------------------
/// Triangulation ID
//-----------------------------------------------------------------------

  const std::string& triangulation_id() const { return triangulation_id_;}
  void triangulation_id(const std::string& V) { triangulation_id_ = V;}

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
  
  std::string base_tre_string() const;
  void base_read_tre_string(std::istream& In);
  virtual void print(std::ostream& Os) const
  { Os << "RsmIndirectCovariance\n"; }
private:
  std::string image_identifier_, rsm_support_data_edition_, triangulation_id_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmIndirectCovariance);
#endif
