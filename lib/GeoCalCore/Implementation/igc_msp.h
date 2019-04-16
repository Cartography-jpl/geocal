#ifndef IGC_MSP_H
#define IGC_MSP_H
#include "image_ground_connection.h"
#include "simple_dem.h"

namespace GeoCal {
bool have_msp_supported();
  
/****************************************************************//**
  Exception thrown if MSP isn't available.
*******************************************************************/

class MspNotAvailableException : public Exception {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  MspNotAvailableException() : Exception("Geocal library wasn't configured to use MSP library during installation, so can't call MSP functions.")
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MspNotAvailableException() throw() {}
};

/****************************************************************//**
  This is an ImageGroundConnection that uses the MSP (Mensuration
  Service Program) library to implement an ImageGroundConnection. This
  uses a set of plugins for the CSM (Community Sensor Model, see 
  https://github.com/sminster/csm) to support various sensor models.
  In particular, this supports various NITF formats such as RPC, RSM,
  SENSRB, GLAS/GFM.

  Note this goes from the CSM library to our ImageGroundConnection
  interface. We may well write a plugin to go the other way at some
  point, so software that uses the CSM library can use GeoCal
  ImageGroundConnection objects.
*******************************************************************/

class IgcMsp: public virtual ImageGroundConnectionCopy {
public:
  IgcMsp(const std::string& Fname,
   const boost::shared_ptr<Dem>& Dem = boost::shared_ptr<Dem>(new SimpleDem()));
  IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& Dem,
	 const std::string& Plugin_name, const std::string& Model_name);
  virtual ~IgcMsp() {}
  static void msp_print_plugin_list();
  static std::vector<std::string> msp_plugin_list();
  static void msp_register_plugin(const std::string& Plugin_name);
  static void msp_init();
  static std::vector<std::string> msp_model_list(const std::string& Plugin);
private:
  static void* lib_ptr;
  IgcMsp() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcMsp);
#endif
