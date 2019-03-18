#include "igc_msp.h"
// This will probably go away and get sucked into this class
#include "msp_support.h"
#include "geocal_internal_config.h"
#include "geocal_serialize_support.h"
#include "simple_dem.h"
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
  IgcMspImp(const std::string& Fname) : fname_(Fname) { init(); }
  virtual ~IgcMspImp() {}
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, 
			const Dem& D) const
  { // Temporary, we should have a more efficient version of this that
    // doesn't parse the file for every request.
    // Note this doesn't actually make use the DEM, we'll need to put
    // that in somehow.
    const SimpleDem* sd = dynamic_cast<const SimpleDem*>(&D);
    if(!sd)
      throw Exception("Right now, IgcMsp only works with constant DEMs of SimpleDem");
    if(sd->h() != 0.0)
      throw Exception("Right now, IgcMsp only works with height of 0");
    return boost::make_shared<Ecr>(msp_terrain_point(fname_, Ic));
  }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const
  { throw Exception("Not Implemented"); }
private:
  std::string fname_;
  void init() { IgcMsp::msp_init(); }
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
    & GEOCAL_NVP_(fname);
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

IgcMsp::IgcMsp(const std::string& Fname)
{
#ifdef HAVE_MSP
  igc = boost::make_shared<IgcMspImp>(Fname);
  initialize(boost::make_shared<SimpleDem>(),
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
