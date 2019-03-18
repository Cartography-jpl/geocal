#include "geocal_internal_config.h"
#include "msp_support.h"
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
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

//-----------------------------------------------------------------------
/// This uses an image in a NITF file and the sensor attached to it
/// (e.g. RPC, RSM, SENSRB) to find the ground location for a
/// particular image point. This is meant for comparison with our own
/// GeoCal calculation, to make sure we are meeting whatever
/// assumptions BAE has in in MSP software.
///
/// An important note for using this in Python. The
/// SensorModelService code automatically loads all the plugins found
/// at CSM_PLUGIN_DIR. However, it will silently fail when it tries to
/// load them. You can see this by running with LD_DEBUG=files to get
/// debugging information from ld.so. This does not happen in C++.
///
/// Turns out that the plugins depend on the library libMSPcsm.so,
/// although they don't list this as a dependency. The plugins
/// probably should, but since we don't have the source we can't fix
/// this. For C++, the library get loaded as a dependency of
/// geocal. The same happens in python, but the difference is that
/// geocal loads this with RTLD_GLOBAL and python with RTD_LOCAL (see
/// man page on dlopen for description of these). This means in C++
/// the symbols can be resolved when SensorModelService loads a
/// plugin. For python, this can't be used.
///
/// The solution is to preload the library. You can either define
/// LD_PRELOAD=/data/smyth/MSP/install/lib/libMSPcsm.so when starting
/// python, or alternatively explicitly load the library in python
/// with RTLD_GLOBAL:
///    ctypes.CDLL(os.environ["CSM_PLUGIN_DIR"] + "../lib/libMSPcsm.so",
///                ctypes.RTLD_GLOBAL)
//-----------------------------------------------------------------------

Ecr GeoCal::msp_terrain_point(const std::string& Fname,
			      const ImageCoordinate& Ic)
{
#ifdef HAVE_MSP
try {
  MSP::SDS::SupportDataService sds;
  boost::shared_ptr<MSP::SMS::SensorModelService> sms =
    boost::make_shared<MSP::SMS::SensorModelService>();
  boost::shared_ptr<csm::Isd> isd(sds.createIsdFromFile(Fname.c_str()));
  if(sds.getNumImages(*isd) > 1)
    throw Exception("Only handle 1 image for now");
  MSP::WarningListType msg;
  boost::shared_ptr<csm::Model>
    model_raw(sms->createModelFromFile(Fname.c_str(),0,0,&msg));
  BOOST_FOREACH(const MSP::Warning& w, msg) {
    std::cout << "Warning:\n" << w.getMessage() << "\n"
	      << w.getFunction() << "\n";
  }
  boost::shared_ptr<csm::RasterGM> model =
    boost::dynamic_pointer_cast<csm::RasterGM>(model_raw);
  if(!model)
    throw Exception("Model needs to be a RasterGM model");
  MSP::ImagePoint ipoint(Ic.line,Ic.sample);
  ipoint.setImageID(model->getImageIdentifier());
  // Create constant height terrain. Not sure if this is the same as
  // our SimpleDem or not.
  MSP::TS::TerrainService terrain_service;
  MSP::IntersectionUncertainty uncertainty;
  MSP::TS::TerrainModel *tm;
  terrain_service.createConstantHeightTerrainModel(0, uncertainty, tm);
  boost::shared_ptr<MSP::TS::TerrainModel> terrain_model(tm);
  MSP::PES::PointExtractionService pes;
  MSP::GroundPoint gp;
  std::string terrain_type_used;
  pes.intersectTerrain(model.get(), ipoint, *terrain_model, gp,
		       terrain_type_used);

  if(false)
    std::cerr << "Ground Point coordinates: \n"
	      << "X: " << gp.getX() << "\n"
	      << "Y: " << gp.getY() << "\n"
	      << "Z: " << gp.getZ() << "\n"
	      << "terrain is " << terrain_type_used << "\n"
	      << "Ground Point Covariance\n"
	      << gp.getCovariance();
  return Ecr(gp.getX(), gp.getY(), gp.getZ());
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



