#include "igc_msp.h"
#include "geocal_internal_config.h"
#include "geocal_serialize_support.h"
#include "simple_dem.h"
#include "ostream_pad.h"
#include "ecr.h"
#include <cstdlib>
#include <dlfcn.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#ifdef HAVE_MSP
#include "Plugin.h"
#include "GroundPoint.h"
#include "ImagePoint.h"
#include "PointExtractionService.h"
#include "SensorModelService.h"
#include "SupportDataService.h"
#include "ImagingGeometryService.h"
#include "MSPTime.h"
#include "IWS_WarningTracker.h"
// Note these two include files were *not* part of MSP 1.6, although
// the RsmGeneratorService library still is present. I'm assuming they
// have just dropped the headers for now. We copied this over from MSP
// 1.5. We can come back to this if the RSM service gets completely
// dropped in the future, but for now just include the header files
// and assume they have been copied into place.
#include "RsmGeneratorService.h"
#include "RGSConfig.h"
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
  IgcMspImp(const std::string& Fname, const boost::shared_ptr<Dem>& D,
	    int Image_index = 0,
	    const std::string& Plugin_name = "",
	    const std::string& Model_name = "")
    : fname_(Fname), plugin_name_(Plugin_name), model_name_(Model_name),
      image_index_(Image_index)
  { dem_ = D; init(); }
  virtual ~IgcMspImp() {}
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, 
			const Dem& D) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, 
				  double H) const;
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const;
  virtual void cf_look_vector(const ImageCoordinate& Ic, 
			      CartesianFixedLookVector& Lv,
			      boost::shared_ptr<CartesianFixed>& P) const;
  virtual bool has_time() const { return true; }
  virtual Time pixel_time(const ImageCoordinate& Ic) const;
  std::string family() const { return model->getFamily(); }
  std::string version() const { return model->getVersion().version(); }
  std::string model_name() const { return model->getModelName(); }
  std::string pedigree() const { return model->getPedigree(); }
  std::string image_identifer() const { return model->getImageIdentifier(); }
  std::string sensor_identifer() const { return model->getSensorIdentifier(); }
  std::string platform_identifer() const { return model->getPlatformIdentifier(); }
  std::string collection_identifer() const { return model->getCollectionIdentifier(); }
  std::string trajectory_identifer() const { return model->getTrajectoryIdentifier(); }
  std::string sensor_type() const { return model->getSensorType(); }
  std::string sensor_mode() const { return model->getSensorMode(); }
  std::string reference_date_time() const { return model->getReferenceDateAndTime(); }
  std::string file_name() const { return fname_;}
  int image_index() const { return image_index_;}
  blitz::Array<double, 1> sensor_velocity(const ImageCoordinate& Ic) const;
  std::string generate_rsm_tre(const std::string& Report = "",
			       const std::string& Rsm_config = "") const;
private:
  std::string fname_, plugin_name_, model_name_;
  int image_index_;
  boost::shared_ptr<MSP::SMS::SensorModelService> sms;
  boost::shared_ptr<csm::RasterGM> model;
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
    & GEOCAL_NVP_(model_name)
    & GEOCAL_NVP_(image_index);
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
  sds.setImageInIsd(image_index_, *isd);
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

blitz::Array<double, 1> IgcMspImp::sensor_velocity(const ImageCoordinate& Ic)
  const
{
try {
  csm::EcefVector v = model->getSensorVelocity(csm::ImageCoord(Ic.line,
							       Ic.sample));
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

ImageCoordinate IgcMspImp::image_coordinate(const GroundCoordinate& Gc) const
{
try {
  boost::shared_ptr<CartesianFixed> cf = Gc.convert_to_cf();
  csm::ImageCoord ic = model->groundToImage(csm::EcefCoord(cf->position[0],
			cf->position[1], cf->position[2]));
  return ImageCoordinate(ic.line, ic.samp);
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

void IgcMspImp::cf_look_vector(const ImageCoordinate& Ic, 
			       CartesianFixedLookVector& Lv,
			       boost::shared_ptr<CartesianFixed>& P) const
{
try {
  csm::EcefLocus lc = model->imageToRemoteImagingLocus(csm::ImageCoord(Ic.line, Ic.sample));
  P = boost::make_shared<Ecr>(lc.point.x, lc.point.y, lc.point.z);
  Lv.look_vector[0] = lc.direction.x;
  Lv.look_vector[1] = lc.direction.y;
  Lv.look_vector[2] = lc.direction.z;
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


boost::shared_ptr<GroundCoordinate> 
IgcMspImp::ground_coordinate_approx_height(const ImageCoordinate& Ic, 
					double H) const
{
try {
  csm::EcefCoord gp = model->imageToGround(csm::ImageCoord(Ic.line, Ic.sample),
					    H);
  return boost::make_shared<Ecr>(gp.x, gp.y, gp.z);
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

boost::shared_ptr<GroundCoordinate> IgcMspImp::ground_coordinate_dem
(const ImageCoordinate& Ic, const Dem& D) const
{
  const SimpleDem* sd = dynamic_cast<const SimpleDem*>(&D);
  if(!sd)
    throw Exception("Right now, IgcMsp only works with constant DEMs of SimpleDem");
  return ground_coordinate_approx_height(Ic, sd->h());
}

Time IgcMspImp::pixel_time(const ImageCoordinate& Ic) const
{
  using namespace boost::posix_time;
  using namespace boost::gregorian;
try {
  MSP::IGS::ImagingGeometryService is;
  MSP::Time tm;
  double second_from_midnight, second_from_start_of_imaging;
  is.getImageTime(model.get(), MSP::ImagePoint(Ic.line, Ic.sample),
		  tm, second_from_midnight, second_from_start_of_imaging);
  date d(tm.getYear(), Jan, 1);
  d += date_duration(tm.getDate()-1);
  Time t = Time::parse_time(to_iso_extended_string(d) + "T00:00:00Z") +
    second_from_midnight;
  return t;
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

std::string IgcMspImp::generate_rsm_tre
(const std::string& Report,
 const std::string& Rsm_config) const
{
try {
  std::string cfname(Rsm_config);
  if(cfname == "") {
    char *t = getenv("MSP_DATA_DIR");
    if(!t)
      throw Exception("Need to either pass in the RSM configuration file, or set the environment variable MSP_DATA_DIR");
    cfname = std::string(t) + "/rsm/database_settings.strat";
  }
  MSP::RGS::RGSConfig config;
  config.read_config_file(cfname);
  MSP::RGS::RsmGeneratorService generator(&config);
  std::vector<csm::RasterGM *> mlist;
  mlist.push_back(model.get());
  int n_orig_params = 0;
  double *orig_covariance = NULL;
  std::vector<std::string> image_ids;
  std::vector<std::vector<int> > orig_param_numbers_by_image;
  std::vector<csm::Isd *> isd;
  IWS_WarningTracker warning;
  warning = generator.generateRsmWithJointCov(mlist, n_orig_params,
	      orig_covariance, image_ids, orig_param_numbers_by_image,
	      isd, Report);
  warning.printAllWarnings();
  // Not sure if we should change this or not
  const char* sensor_model_type = "IWS_UNKNOWN_SENSOR_MODEL";
  boost::shared_ptr<csm::RasterGM>
    rsm_sm((csm::RasterGM *) sms->createModelFromISD(*isd[0],
						     sensor_model_type));
  if(!rsm_sm)
    throw Exception("Trouble creating rsm_sm");
  return rsm_sm->getModelState();
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

IgcMsp::IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& D,
	       int Image_index)
{
#ifdef HAVE_MSP
  igc = boost::make_shared<IgcMspImp>(Fname, D, Image_index);
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
/// if the wrong plugin is processing the file.
//-----------------------------------------------------------------------

IgcMsp::IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& D,
	       int Image_index,
	       const std::string& Plugin_name, const std::string& Model_name)
{
#ifdef HAVE_MSP
  igc = boost::make_shared<IgcMspImp>(Fname, D, Image_index, Plugin_name,
				      Model_name);
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
} catch(const csm::Error& error) {
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
} catch(const csm::Error& error) {
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
/// For a given plugin, return the list of models it supports. Some
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
} catch(const csm::Error& error) {
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
/// Return the list of image IDS for the given NITF file. If the file
/// has only one 
//-----------------------------------------------------------------------

std::vector<std::string> IgcMsp::image_ids(const std::string& Fname)
{
#ifdef HAVE_MSP
try {
  msp_init();
  // This registers stuff
  MSP::SDS::SupportDataService sds;
  boost::shared_ptr<MSP::SMS::SensorModelService> sms =
    boost::make_shared<MSP::SMS::SensorModelService>();
  boost::shared_ptr<csm::Isd> isd(sds.createIsdFromFile(Fname.c_str()));
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
} catch(const csm::Error& error) {
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

std::string IgcMsp::family() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->family();
#else
  throw MspNotAvailableException();
#endif
}

std::string IgcMsp::version() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->version();
#else
  throw MspNotAvailableException();
#endif
}

std::string IgcMsp::pedigree() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->pedigree();
#else
  throw MspNotAvailableException();
#endif
}

std::string IgcMsp::model_name() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->model_name();
#else
  throw MspNotAvailableException();
#endif
}

std::string IgcMsp::image_identifer() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->image_identifer();
#else
  throw MspNotAvailableException();
#endif
}
  
std::string IgcMsp::sensor_identifer() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->sensor_identifer();
#else
  throw MspNotAvailableException();
#endif
}
  
std::string IgcMsp::platform_identifer() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->platform_identifer();
#else
  throw MspNotAvailableException();
#endif
}
  
std::string IgcMsp::collection_identifer() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->collection_identifer();
#else
  throw MspNotAvailableException();
#endif
}
  
std::string IgcMsp::trajectory_identifer() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->trajectory_identifer();
#else
  throw MspNotAvailableException();
#endif
}
  
std::string IgcMsp::sensor_type() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->sensor_type();
#else
  throw MspNotAvailableException();
#endif
}
  
std::string IgcMsp::sensor_mode() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->sensor_mode();
#else
  throw MspNotAvailableException();
#endif
}
  
std::string IgcMsp::reference_date_time() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->reference_date_time();
#else
  throw MspNotAvailableException();
#endif
}

std::string IgcMsp::file_name() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->file_name();
#else
  throw MspNotAvailableException();
#endif
}

blitz::Array<double, 1> IgcMsp::sensor_velocity(const ImageCoordinate& Ic)
  const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->sensor_velocity(Ic);
#else
  throw MspNotAvailableException();
#endif
}

int IgcMsp::image_index() const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->image_index();
#else
  throw MspNotAvailableException();
#endif
}


//-----------------------------------------------------------------------
/// Use the MSP RSM generation code to create a TRE string (it doesn't
/// directly create a NITF file).
///
/// You can optionally supply a report file to write to and/or specify
/// the RSM configuration file to use. The default is
/// "$MSP_DATA_DIR/rsm/database_settings.strat".
///
/// Note the TRE string can be converted to an actual RSM. We add this
/// functionality in the python interface, it isn't all that easy to
/// do in C++ and we really only need this at the python level anyways.
//-----------------------------------------------------------------------

std::string IgcMsp::generate_rsm_tre
(const std::string& Report,
 const std::string& Rsm_config) const
{
#ifdef HAVE_MSP
  return boost::dynamic_pointer_cast<IgcMspImp>(igc)->generate_rsm_tre(Report, Rsm_config);
#else
  throw MspNotAvailableException();
#endif
}

void IgcMsp::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "IgcMsp:\n"
     << "  File name:             " << file_name() << "\n"
     << "  Image index:           " << image_index() << "\n"
     << "  Image identifier:      " << image_identifer() << "\n"
     << "  Trajectory identifier: " << trajectory_identifer() << "\n"
     << "  MSP model name:        " << model_name() << "\n"
     << "  MSP family:            " << family() << "\n"
     << "  MSP version:           " << version() << "\n"
     << "  MSP Pedigree:          " << pedigree() << "\n"
     << "  Sensor type:           " << sensor_type() << "\n"
     << "  Sensor mode:           " << sensor_mode() << "\n"
     << "  Platform identifier:   " << platform_identifer() << "\n"
     << "  Sensor identifier:     " << sensor_identifer() << "\n"
     << "  Collection:            " << collection_identifer() << "\n";
  opad.strict_sync();
  Os << "  Dem:\n";
  opad << dem() << "\n";
  opad.strict_sync();
  if(image()) {
    Os << "  Image:\n";
    opad << *image() << "\n";
    opad.strict_sync();
  }
  if(image_multi_band()) {
    Os << "  Image multi-band:\n";
    opad << *image_multi_band() << "\n";
    opad.strict_sync();
  }
  Os << "  Title: " << title() << "\n"
     << "  Image mask:\n";
  opad << *image_mask() << "\n";
  opad.strict_sync();
  Os << "  Ground mask:\n";
  opad << *ground_mask() << "\n";
  opad.strict_sync();
}
