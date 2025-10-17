#include "msp.h"
#include <dlfcn.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

void* Msp::lib_ptr = 0;
boost::shared_ptr<MSP::SDS::SupportDataService> Msp::sds;
boost::shared_ptr<MSP::SMS::SensorModelService> Msp::sms;
boost::shared_ptr<MSP::CS::CovarianceService> Msp::cs;

Msp::Msp(const std::string& Fname, int Image_index,
	 const std::string& Plugin_name, const std::string& Model_name)
  : fname_(Fname), plugin_name_(Plugin_name), model_name_(Model_name),
    image_index_(Image_index)
{
  try{
    msp_init();
    boost::shared_ptr<csm::Isd> isd(sds->createIsdFromFile(fname_.c_str()));
    sds->setImageInIsd(image_index_, *isd);
    MSP::WarningListType msg;
    boost::shared_ptr<csm::Model> model_raw;
    if(plugin_name_ == "")
      model_raw.reset(sms->createModelFromISD(*isd,0,&msg));
    else {
      const csm::Plugin* t = csm::Plugin::findPlugin(plugin_name_);
      if(!t)
	throw Exception("Could not file plugin '" + plugin_name_ + "'");
      std::list<csm::Warning> msg2;
      bool can_do = t->canISDBeConvertedToModelState(*isd, model_name_, &msg2);
      if(can_do)
	model_raw.reset(t->constructModelFromISD(*isd, model_name_, &msg2));
      BOOST_FOREACH(const csm::Warning& w, msg2) {
	std::cout << "Warning:\n" << w.getMessage() << "\n"
		  << w.getFunction() << "\n";
      }
    }
    // We may want to turn this off, but for now it is useful to get all
    // the warning messages when we create the model.
    BOOST_FOREACH(const MSP::Warning& w, msg) {
      std::cout << "Warning:\n" << w.getMessage() << "\n"
		<< w.getFunction() << "\n";
    }
    model = boost::dynamic_pointer_cast<csm::RasterGM>(model_raw);
    if(!model)
      throw Exception("Model needs to be a RasterGM model");
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
  sds = boost::make_shared<MSP::SDS::SupportDataService>();
  sms = boost::make_shared<MSP::SMS::SensorModelService>();
  cs = boost::make_shared<MSP::CS::CovarianceService>();
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

//-----------------------------------------------------------------------
/// Return list of registered plugins
//-----------------------------------------------------------------------

std::vector<std::string> Msp::msp_plugin_list()
{
  try{
    msp_init();
    MSP::SDS::SupportDataService sds;
    boost::shared_ptr<MSP::SMS::SensorModelService> sms =
      boost::make_shared<MSP::SMS::SensorModelService>();
    MSP::SMS::NameList plugin_list;
    sms->getAllRegisteredPlugins(plugin_list);
    std::vector<std::string> res;
    BOOST_FOREACH(const std::string& n, plugin_list)
      res.push_back(n);
    return res;
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
/// For a given plugin, return the list of models it supports. Some
/// plugins may support more than one sensor model.
//-----------------------------------------------------------------------

std::vector<std::string> Msp::msp_model_list(const std::string& Plugin)
{
  try {
    msp_init();
    // This registers stuff
    MSP::SDS::SupportDataService sds;
    boost::shared_ptr<MSP::SMS::SensorModelService> sms =
      boost::make_shared<MSP::SMS::SensorModelService>();

    const csm::Plugin* t = csm::Plugin::findPlugin(Plugin);
    std::vector<std::string> res;
    for(int i = 0; i < (int) t->getNumModels(); ++i)
      res.push_back(t->getModelName(i));
    return res;
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
/// Return the list of image IDS for the given NITF file. 
//-----------------------------------------------------------------------

std::vector<std::string> Msp::image_ids(const std::string& Fname)
{
  try {
    msp_init();
    // This registers stuff
    MSP::SDS::SupportDataService sds;
    boost::shared_ptr<MSP::SMS::SensorModelService> sms =
      boost::make_shared<MSP::SMS::SensorModelService>();
    boost::shared_ptr<csm::Isd> isd(sds.createIsdFromFile(Fname.c_str()));
    int numImages = sds.getNumImages(*isd);
    // For some reason, our test examples don't work with this. Like so
    // much with MSP not clear why, we just get "Unrecognized support
    // data" error. Leave this minor diagnostic in place for now, we can
    // come back to this.
    std::cerr << "nuImages: " << numImages << "\n";
    std::vector<std::string> res;
    sds.getImageIds(*isd, res);
    return res;
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

blitz::Array<double, 1> Msp::sensor_velocity(double Line, double Sample)
{
  try {
    csm::EcefVector v = model->getSensorVelocity(csm::ImageCoord(Line, Sample));
    blitz::Array<double, 1> res(3);
    res = v.x, v.y, v.z;
    return res;
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


blitz::Array<double, 1> Msp::image_coordinate(const blitz::Array<double, 1>& Gc_ecr) const
{
  try {
    csm::ImageCoord ic = model->groundToImage(csm::EcefCoord(Gc_ecr(0), Gc_ecr(1), Gc_ecr(2)));
    blitz::Array<double, 1> res(2);
    res = ic.line, ic.samp;
    return res;
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

blitz::Array<double, 2> Msp::cf_look_vector(double Line, double Sample) const
{
  try {
    csm::EcefLocus lc = model->imageToRemoteImagingLocus(csm::ImageCoord(Line, Sample));
    blitz::Array<double, 2> res(2, 3);
    res = lc.point.x, lc.point.y, lc.point.z,
      lc.direction.x, lc.direction.y, lc.direction.z;
    return res;
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

blitz::Array<double, 1>
Msp::ground_coordinate_approx_height(double Line, double Sample, double H) const
{
  try {
    csm::EcefCoord gp = model->imageToGround(csm::ImageCoord(Line, Sample), H);
    blitz::Array<double, 1> res(3);
    res = gp.x, gp.y, gp.z;
    return res;
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

blitz::Array<int, 1> Msp::image_size() const
{
  try {
    csm::ImageVector sz = model->getImageSize();
    blitz::Array<int, 1> res(2);
    res = sz.line, sz.samp;
    return res;
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

std::string Msp::pixel_time_base(double Line, double Sample) const
{
  using namespace boost::posix_time;
  using namespace boost::gregorian;
  try {
    MSP::IGS::ImagingGeometryService is;
    MSP::Time tm;
    double second_from_midnight, second_from_start_of_imaging;
    is.getImageTime(model.get(), MSP::ImagePoint(Line, Sample),
		    tm, second_from_midnight, second_from_start_of_imaging);
    date d(tm.getYear(), Jan, 1);
    d += date_duration(tm.getDate()-1);
    return to_iso_extended_string(d) + "T00:00:00Z";
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

double Msp::pixel_time_offset(double Line, double Sample) const
{
  try {
    MSP::IGS::ImagingGeometryService is;
    MSP::Time tm;
    double second_from_midnight, second_from_start_of_imaging;
    is.getImageTime(model.get(), MSP::ImagePoint(Line, Sample),
		    tm, second_from_midnight, second_from_start_of_imaging);
    return second_from_midnight;
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

