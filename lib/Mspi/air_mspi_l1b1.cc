#include "geocal_internal_config.h"
#include "air_mspi_l1b1.h"
#include "geocal_serialize_support.h"
#ifdef HAVE_MSPI_SHARED
#include "MSPI-Shared/File/L1B1File/src/l1b1_reader.h"
#endif

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiL1b1File::AirMspiL1b1File(const std::string& Fname)
{
#ifdef HAVE_MSPI_SHARED
  l1b1_reader.reset(new MSPI::Shared::L1B1Reader(Fname));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}
