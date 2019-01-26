#include "geocal_internal_config.h"
#include "msp_support.h"
#include <boost/make_shared.hpp>
#ifdef HAVE_MSP
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
/// Return true if we were built with MSP support, false
/// otherwise. 
//-----------------------------------------------------------------------

bool GeoCal::have_serialize_supported()
{
#ifdef HAVE_MSP
  return true;
#else
  return false;
#endif
}

//-----------------------------------------------------------------------
/// This uses an image in a NITF file and the sensor attached to it
/// (e.g. RPC, RSM, SENSRB) to find the ground location for a
/// particular image point. This is meant for comparison with our own
/// GeoCal calculation, to make sure we are meeting whatever
/// assumptions BAE has in in MSP software.
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
  boost::shared_ptr<csm::Model>
    model_raw(sms->createModelFromFile(Fname.c_str()));
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
