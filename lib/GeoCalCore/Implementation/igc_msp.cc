#include "igc_msp.h"
// This will probably go away and get sucked into this class
#include "msp_support.h"
#include "geocal_internal_config.h"
#include "geocal_serialize_support.h"
#include "simple_dem.h"
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
  void init() {}
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



