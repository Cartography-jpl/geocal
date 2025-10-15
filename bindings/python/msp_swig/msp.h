#ifndef MSP_SWIG_WRAP_H
#define MSP_SWIG_WRAP_H

// Lots of MSP headers. Not sure we need all of these in the header
// vs. the .cc file, but for now include everything. We can sort out
// if needed.

#include "Plugin.h"
#include "GroundPoint.h"
#include "CovarianceService.h"
#include "ImagePoint.h"
#include "PointExtractionService.h"
#include "SensorModelService.h"
#include "SupportDataService.h"
#include "ImagingGeometryService.h"
#include "MSPTime.h"
#include "IWS_WarningTracker.h"
#include "CsmSensorModelList.h"
// Note these two include files were *not* part of MSP 1.6, although
// the RsmGeneratorService library still is present. I'm assuming they
// have just dropped the headers for now. We copied this over from MSP
// 1.5. We can come back to this if the RSM service gets completely
// dropped in the future, but for now just include the header files
// and assume they have been copied into place.
#include "RsmGeneratorService.h"
#include "RGSConfig.h"

#include <sstream>		// Definition of ostringstream.

/****************************************************************//**
  Copy of exception we use in geocal. Note we can't link in geocal,
  because this library must use the much older gcc 4.85 CXX ABI,
  which means we can't link to geocal.
*******************************************************************/

class Exception: public std::exception {
public:
  //-----------------------------------------------------------------------
  /// Default constructor. Can give an optional string describing
  /// the error.
  //-----------------------------------------------------------------------

  Exception(const std::string& W = "") 
  { 
    // This reserve shouldn't really be necessary, but on a Mac
    // 10.4.11 using gcc 4.0.1, there is some kind of bug where we get
    // a "Double free" error when printing in Ruby. I never tracked
    // exactly where this occurred, but it was somewhere in the
    // iostream library when the buffer of os was resized. We just
    // reserve enough space up front so this isn't an issue. Since
    // this only gets called when an exception occurs, there shouldn't
    // be much of a performance issue with this.
    std::string buf("blah");
    buf.reserve(1000);
    s_.str(buf);
    s_ << W;  
  }

  //-----------------------------------------------------------------------
  /// Copy constructor.
  //-----------------------------------------------------------------------

  Exception(const Exception& E)
  {
    try {
      std::string cp(E.s_.str());
      s_.str(cp);
    } catch(...) {		// Ignore all errors.
    }
  }
  
  //-----------------------------------------------------------------------
  /// Destructor.
  //-----------------------------------------------------------------------
    
  virtual ~Exception() throw() {}

  //-----------------------------------------------------------------------
  /// Write to exception what() string.
  //-----------------------------------------------------------------------
    
  template<class T> inline Exception& operator<<(const T& V)
  {
    s_ << V;
    return *this;
  }

  //-----------------------------------------------------------------------
  /// Print out description of object.
  //-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  {
    Os << "GeoCal Exception:\n"
       << "=========================\n" 
       << what() << "\n"
       << "=========================\n" ;
  }

  //-----------------------------------------------------------------------
  /// Description of what the error is.
  //-----------------------------------------------------------------------

  virtual const char* what() const throw()
  {
    scratch = s_.str();
    return scratch.c_str();
  }
private:
  mutable std::string scratch;
  std::ostringstream s_;
};

/****************************************************************//**
  This is close to a ImageGroundConnection that uses the MSP (Mensuration
  Service Program) library to implement an ImageGroundConnection. This
  uses a set of plugins for the CSM (Community Sensor Model, see 
  https://github.com/sminster/csm) to support various sensor models.
  In particular, this supports various NITF formats such as RPC, RSM,
  SENSRB, GLAS/GFM.

  This is primarily used for testing, to compare our
  ImageGroundConnection implementation against the MSP library, which
  is somewhat of a standard library.

  Note that the MSP library uses the very old gcc 4.8.5 compiler, and
  is closed source. So we don't want to directly link to this in
  geocal. Instead, we have a simple class that we can call from swig,
  and we wrap this in python into a full ImageGroundConnection.

  This library isn't intended to be used as an actual
  ImageGroundConnection, it is not written to be overly efficient.
  But it should be sufficient for comparing things.
*******************************************************************/

class Msp {
public:
  Msp() { }
  virtual ~Msp() {}
  void msp_print_plugin_list();
  std::vector<std::string> msp_plugin_list();
  std::vector<std::string> IgcMsp::msp_model_list(const std::string& Plugin);
  void msp_register_plugin(const std::string& Plugin_name);
private:
  void msp_init();
  static void* lib_ptr;
};

#endif
