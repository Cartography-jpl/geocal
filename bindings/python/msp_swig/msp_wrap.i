// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%module msp_wrap

%include "std_string.i"

// Map std::string to and from the native string type
%naturalvar std::string;

%include "exception.i"

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%{
#include <iostream>

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
#include <dlfcn.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

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
  
class Test {
public:
  Test() {
    std::cerr << "Hi there";
  }

  void msp_print_plugin_list() {
    try {
      msp_init();
      MSP::SDS::SupportDataService sds;
      boost::shared_ptr<MSP::SMS::SensorModelService> sms =
	boost::make_shared<MSP::SMS::SensorModelService>();
      MSP::SMS::NameList plugin_list;
      sms->getAllRegisteredPlugins(plugin_list);
      std::cout << "MSP Plugin list:\n";
      BOOST_FOREACH(const std::string& n, plugin_list)
	std::cout<< "  " << n << "\n";
    } catch(const csm::Error& error) {
      // Translate MSP error to Geocal error, just so we don't need
      // additional logic to handle this
      Exception e;
      e << "MSP error:\n"
	<< "Message: " << error.getMessage() << "\n"
	<< "Function: " << error.getFunction() << "\n";
      throw e;
    }
  }
  
  void msp_init()
  {
    if(lib_ptr)
      return;
    char* t = getenv("CSM_PLUGIN_DIR");
    if(!t)
      throw Exception("You need to set the environment variable CSM_PLUGIN_DIR to use the MSP plugins");
    std::string lib = std::string(t) + "/../lib/libMSPcsm.so";
    lib_ptr = dlopen(lib.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if(!lib_ptr) {
      Exception e;
      e << "Trouble loading the library " << lib << "\n"
	<< "  Error: " << dlerror() << "\n";
      throw e;
    }
  }

  void msp_register_plugin(const std::string& Plugin_name)
  {
    try {
      msp_init();
      MSP::SDS::SupportDataService sds;
      boost::shared_ptr<MSP::SMS::SensorModelService> sms =
	boost::make_shared<MSP::SMS::SensorModelService>();
      sms->registerPlugin(Plugin_name);
    } catch(const csm::Error& error) {
      // Translate MSP error to Geocal error, just so we don't need
      // additional logic to handle this
      Exception e;
      e << "MSP error:\n"
	<< "Message: " << error.getMessage() << "\n"
	<< "Function: " << error.getFunction() << "\n";
      throw e;
    }
  }
private:
  static void* lib_ptr;
};

void* Test::lib_ptr = 0;

%}

class Test {
public:
  Test();
  void msp_register_plugin(const std::string& Plugin_name);
  void msp_print_plugin_list();
};

