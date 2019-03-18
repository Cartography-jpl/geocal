#ifndef IGC_MSP_H
#define IGC_MSP_H
#include "image_ground_connection.h"
#include "msp_support.h"

namespace GeoCal {
bool have_msp_supported();
  
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

  An important note for using this in Python. The SensorModelService
  code automatically loads all the plugins found at
  CSM_PLUGIN_DIR. However, it will silently fail when it tries to
  load them. You can see this by running with LD_DEBUG=files to get
  debugging information from ld.so. This does not happen in C++.

  Turns out that the plugins depend on the library libMSPcsm.so,
  although they don't list this as a dependency. The plugins probably
  should, but since we don't have the source we can't fix this. For
  C++, the library get loaded as a dependency of geocal. The same
  happens in python, but the difference is that geocal loads this
  with RTLD_GLOBAL and python with RTD_LOCAL (see man page on dlopen
  for description of these). This means in C++ the symbols can be
  resolved when SensorModelService loads a plugin. For python, this
  can't be used.

  The solution is to preload the library. You can either define
  LD_PRELOAD=/data/smyth/MSP/install/lib/libMSPcsm.so when starting
  python, or alternatively explicitly load the library in python with
  RTLD_GLOBAL: 

      ctypes.CDLL(os.environ["CSM_PLUGIN_DIR"] +
         "../lib/libMSPcsm.so", ctypes.RTLD_GLOBAL)

  The python code is set up to do this automatically. But this is
  a fairly obscure thing that took a good while to track down
  initially, so we want to document this. Also, this might change in
  a future version of the MSP library (either adding or remove
  libraries 

*******************************************************************/

class IgcMsp: public virtual ImageGroundConnectionCopy {
public:
  IgcMsp(const std::string& Fname);
  virtual ~IgcMsp() {}
  static void msp_print_plugin_list();
  static void msp_register_plugin(const std::string& Plugin_name);
private:
  IgcMsp() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcMsp);
#endif
