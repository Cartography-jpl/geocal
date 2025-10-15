#include "msp.h"
#include <dlfcn.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

void* Msp::lib_ptr = 0;

//-----------------------------------------------------------------------
/// Print out a list of plugins that have been registered
//-----------------------------------------------------------------------

void Msp::msp_print_plugin_list()
{
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

//-----------------------------------------------------------------------
/// Initialize the MSP library
//-----------------------------------------------------------------------

void Msp::msp_init()
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

//-----------------------------------------------------------------------
/// Register a plugin
//-----------------------------------------------------------------------

void Msp::msp_register_plugin(const std::string& Plugin_name)
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
