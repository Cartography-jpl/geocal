#include "igc_msp.h"
#include "geocal_internal_config.h"
#include "geocal_serialize_support.h"
#include "simple_dem.h"
#include "ecr.h"
#include <cstdlib>
#include <dlfcn.h>
#ifdef HAVE_MSP
#include "Plugin.h"
#include "GroundPoint.h"
#include "ImagePoint.h"
#include "PointExtractionService.h"
#include "SensorModelService.h"
#include "SupportDataService.h"
#include "TerrainService.h"
#include "TerrainModel.h"
#endif
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcMsp::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnectionCopy);
}

GEOCAL_IMPLEMENT(IgcMsp);
#endif

// We use the PIMPL design pattern so our header file doesn't include
// the internal configuration file. This is a good idea, since we
// don't want to expose the internal configuration file to outside users.
#ifdef HAVE_MSP
namespace GeoCal {
class IgcMspImp: public virtual ImageGroundConnection {
public:
  IgcMspImp(const std::string& Fname, const boost::shared_ptr<Dem>& Dem,
	    const std::string& Plugin_name = "",
	    const std::string& Model_name = "")
    : fname_(Fname), plugin_name_(Plugin_name), model_name_(Model_name)
  { dem_ = Dem; init(); }
  virtual ~IgcMspImp() {}
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, 
			const Dem& D) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const
  { throw Exception("Not Implemented"); }
private:
  std::string fname_, plugin_name_, model_name_;
  boost::shared_ptr<MSP::SMS::SensorModelService> sms;
  boost::shared_ptr<csm::RasterGM> model;
  boost::shared_ptr<MSP::TS::TerrainModel> terrain_model;
  mutable MSP::PES::PointExtractionService pes;
  void init();
  IgcMspImp() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcMspImp);

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcMspImp::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnection)
    & GEOCAL_NVP_(fname) & GEOCAL_NVP_(plugin_name)
    & GEOCAL_NVP_(model_name);
  boost::serialization::split_member(ar, *this, version);
}

template<class Archive>
void IgcMspImp::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void IgcMspImp::load(Archive & ar, const unsigned int version)
{
  init();
}

GEOCAL_IMPLEMENT(IgcMspImp);
#endif

void IgcMspImp::init()
{
try {
  IgcMsp::msp_init();
  MSP::SDS::SupportDataService sds;
  sms = boost::make_shared<MSP::SMS::SensorModelService>();
  boost::shared_ptr<csm::Isd> isd(sds.createIsdFromFile(fname_.c_str()));
  if(sds.getNumImages(*isd) > 1)
    throw Exception("Only handle 1 image for now");
  MSP::WarningListType msg;
  boost::shared_ptr<csm::Model> model_raw;
  if(plugin_name_ == "")
    model_raw.reset(sms->createModelFromFile(fname_.c_str(),0,0,&msg));
  else {
    const csm::Plugin* t = csm::Plugin::findPlugin(plugin_name_);
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
  // Create constant height terrain. Not sure if this is the same as
  // our SimpleDem or not.
  MSP::TS::TerrainService terrain_service;
  MSP::IntersectionUncertainty uncertainty;
  MSP::TS::TerrainModel *tm;
  boost::shared_ptr<SimpleDem> sd = boost::dynamic_pointer_cast<SimpleDem>(dem_);
  if(!sd)
    throw Exception("Right now, IgcMsp only works with constant DEMs of SimpleDem");
  terrain_service.createConstantHeightTerrainModel(sd->h(), uncertainty, tm);
  terrain_model.reset(tm);
} catch(const MSP::Error& error) {
  // Translate MSP error to Geocal error, just so we don't need
  // additional logic to handle this
  Exception e;
  e << "MSP error:\n"
    << "Message: " << error.getMessage() << "\n"
    << "Function: " << error.getFunction() << "\n";
  throw e;
}
}

boost::shared_ptr<GroundCoordinate> IgcMspImp::ground_coordinate_dem
(const ImageCoordinate& Ic, const Dem& D) const
{
try { 
  const SimpleDem* sd = dynamic_cast<const SimpleDem*>(&D);
  if(!sd)
    throw Exception("Right now, IgcMsp only works with constant DEMs of SimpleDem");
  boost::shared_ptr<MSP::TS::TerrainModel> tm_to_use = terrain_model;
  boost::shared_ptr<SimpleDem> curr_dem = boost::dynamic_pointer_cast<SimpleDem>(dem_);
  if(sd->h() != curr_dem->h()) {
    MSP::TS::TerrainService terrain_service;
    MSP::IntersectionUncertainty uncertainty;
    MSP::TS::TerrainModel *tm;
    terrain_service.createConstantHeightTerrainModel(sd->h(), uncertainty, tm);
    tm_to_use.reset(tm);
  }
  MSP::GroundPoint gp;
  std::string terrain_type_used;
  MSP::ImagePoint ipoint(Ic.line,Ic.sample);
  ipoint.setImageID(model->getImageIdentifier());
  pes.intersectTerrain(model.get(), ipoint, *tm_to_use, gp,
		       terrain_type_used);
  return boost::make_shared<Ecr>(gp.getX(), gp.getY(), gp.getZ());
} catch(const MSP::Error& error) {
  // Translate MSP error to Geocal error, just so we don't need
  // additional logic to handle this
  Exception e;
  e << "MSP error:\n"
    << "Message: " << error.getMessage() << "\n"
    << "Function: " << error.getFunction() << "\n";
  throw e;
}
}

#endif // HAVE_MSP
//-----------------------------------------------------------------------
/// Return true if we were built with MSP support, false
/// otherwise. 
//-----------------------------------------------------------------------

bool GeoCal::have_msp_supported()
{
#ifdef HAVE_MSP
  return true;
#else
  return false;
#endif
}

//-----------------------------------------------------------------------
/// Constructor.
///
/// Note to use the MSP library, you should make sure the proper
/// environment variables are set. This happens in the normal GeoCal
/// setup, but the variables are MSP_DATA_DIR, MSPCCS_DATA, and
/// CSM_PLUGIN_DIR. Note the directory should end in "/", just as a
/// convention - so /foo/bar/plugins/.
//-----------------------------------------------------------------------

IgcMsp::IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& Dem)
{
#ifdef HAVE_MSP
  igc = boost::make_shared<IgcMspImp>(Fname, Dem);
  initialize(igc->dem_ptr(),
	     igc->image(), igc->image_multi_band(),
	     igc->title(), igc->image_mask(), igc->ground_mask());
#else
  throw MspNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Constructor.
///
/// This version forces the use of the given model name form the given
/// plugin. This can be useful when diagnosing problems where you
/// expect a particular plugin to handle a file, but it doesn't - or
/// if the wrong plugin in is processing the file.
//-----------------------------------------------------------------------

IgcMsp::IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& Dem,
	       const std::string& Plugin_name, const std::string& Model_name)
{
#ifdef HAVE_MSP
  igc = boost::make_shared<IgcMspImp>(Fname, Dem, Plugin_name, Model_name);
  initialize(igc->dem_ptr(),
	     igc->image(), igc->image_multi_band(),
	     igc->title(), igc->image_mask(), igc->ground_mask());
#else
  throw MspNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Print a list of all plugins.
//-----------------------------------------------------------------------

void IgcMsp::msp_print_plugin_list()
{
#ifdef HAVE_MSP
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
} catch(const MSP::Error& error) {
  // Translate MSP error to Geocal error, just so we don't need
  // additional logic to handle this
  Exception e;
  e << "MSP error:\n"
    << "Message: " << error.getMessage() << "\n"
    << "Function: " << error.getFunction() << "\n";
  throw e;
}
#else
  throw MspNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Return list of registered plugins
//-----------------------------------------------------------------------

std::vector<std::string> IgcMsp::msp_plugin_list()
{
#ifdef HAVE_MSP
try {
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
} catch(const MSP::Error& error) {
  // Translate MSP error to Geocal error, just so we don't need
  // additional logic to handle this
  Exception e;
  e << "MSP error:\n"
    << "Message: " << error.getMessage() << "\n"
    << "Function: " << error.getFunction() << "\n";
  throw e;
}
#else
  throw MspNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Fro a given plugin, return the list of models it supports. Some
/// plugins may support more than one sensor model.
//-----------------------------------------------------------------------

std::vector<std::string> IgcMsp::msp_model_list(const std::string& Plugin)
{
#ifdef HAVE_MSP
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
} catch(const MSP::Error& error) {
  // Translate MSP error to Geocal error, just so we don't need
  // additional logic to handle this
  Exception e;
  e << "MSP error:\n"
    << "Message: " << error.getMessage() << "\n"
    << "Function: " << error.getFunction() << "\n";
  throw e;
}
#else
  throw MspNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Register the given plugin. Note that we already register all the
/// plugins at CSM_PLUGIN_DIR, so you don't usually need to use this
/// function.
//-----------------------------------------------------------------------

void IgcMsp::msp_register_plugin(const std::string& Plugin_name)
{
#ifdef HAVE_MSP
try {
  msp_init();
  MSP::SDS::SupportDataService sds;
  boost::shared_ptr<MSP::SMS::SensorModelService> sms =
    boost::make_shared<MSP::SMS::SensorModelService>();
  sms->registerPlugin(Plugin_name);
} catch(const MSP::Error& error) {
  // Translate MSP error to Geocal error, just so we don't need
  // additional logic to handle this
  Exception e;
  e << "MSP error:\n"
    << "Message: " << error.getMessage() << "\n"
    << "Function: " << error.getFunction() << "\n";
  throw e;
}
#else
  throw MspNotAvailableException();
#endif
}

void* IgcMsp::lib_ptr = 0;

//-----------------------------------------------------------------------
///  An important note for using this in Python. The SensorModelService
///  code automatically loads all the plugins found at
///  CSM_PLUGIN_DIR. However, it will silently fail when it tries to
///  load them. You can see this by running with LD_DEBUG=files to get
///  debugging information from ld.so. This does not happen in C++.
///
///  Turns out that the plugins depend on the library libMSPcsm.so,
///  although they don't list this as a dependency. The plugins probably
///  should, but since we don't have the source we can't fix this. For
///  C++, the library get loaded as a dependency of geocal. The same
///  happens in python, but the difference is that geocal loads this
///  with RTLD_GLOBAL and python with RTD_LOCAL (see man page on dlopen
///  for description of these). This means in C++ the symbols can be
///  resolved when SensorModelService loads a plugin. For python, this
///  can't be used.
///
///  The solution is to preload the library. You can either define
///  LD_PRELOAD=/data/smyth/MSP/install/lib/libMSPcsm.so when starting
///  python, or alternatively explicitly load the library in python with
///  RTLD_GLOBAL: 
///
///      ctypes.CDLL(os.environ["CSM_PLUGIN_DIR"] +
///         "../lib/libMSPcsm.so", ctypes.RTLD_GLOBAL)
///
/// The C++ code is set up to do this automatically when you use
/// IgcMsp. But this is a fairly obscure thing that took a good
/// while to track down initially, so we want to document this.
/// Also, this might change in
/// a future version of the MSP library (either adding or remove
/// libraries 
//-----------------------------------------------------------------------

void IgcMsp::msp_init()
{
#ifdef HAVE_MSP
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
#else
  // Nothing to do if don't have MSP
#endif
}
