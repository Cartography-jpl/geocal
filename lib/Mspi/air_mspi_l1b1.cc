#include "geocal_internal_config.h"
#include "air_mspi_l1b1.h"
#include "geocal_serialize_support.h"
#ifdef HAVE_MSPI_SHARED
#include "MSPI-Shared/File/L1B1File/src/l1b1_reader.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void AirMspiL1b1File::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(AirMspiL1b1File);
  ar & GEOCAL_NVP(fname);
}

template<class Archive>
void AirMspiL1b1File::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(AirMspiL1b1File);
  ar & GEOCAL_NVP(fname);
  init(fname);
}

template<class Archive>
void AirMspiL1b1::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageTiledFile)
    & GEOCAL_NVP(l1b1);
}

GEOCAL_IMPLEMENT(AirMspiL1b1);
GEOCAL_SPLIT_IMPLEMENT(AirMspiL1b1File);
#endif

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiL1b1File::AirMspiL1b1File(const std::string& Fname)
{
  init(Fname);
}

void AirMspiL1b1File::init(const std::string& Fname)
{
#ifdef HAVE_MSPI_SHARED
  fname = Fname;
  l1b1_reader.reset(new MSPI::Shared::L1B1Reader(Fname));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}


//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiL1b1::AirMspiL1b1(const std::string& Fname)
{
  l1b1.reset(new AirMspiL1b1File(Fname));
}
