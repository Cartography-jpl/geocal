#include "geocal_internal_config.h"
#include "air_mspi_time_table.h"
#include "mspi_config_file.h"
#include "geocal_serialize_support.h"
#ifdef HAVE_MSPI_SHARED
#include "MSPI-Shared/File/L1B1File/src/l1b1_reader.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void AirMspiTimeTable::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(TimeTable);
  GEOCAL_BASE(AirMspiTimeTable, TimeTable);
  ar & GEOCAL_NVP(l1b1_file);
}

template<class Archive>
void AirMspiTimeTable::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(TimeTable);
  GEOCAL_BASE(AirMspiTimeTable, TimeTable);
  ar & GEOCAL_NVP(l1b1_file);
  read_data();
}

GEOCAL_SPLIT_IMPLEMENT(AirMspiTimeTable);
#endif

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiTimeTable::AirMspiTimeTable
(const std::string& L1b1_file_name, 
 const std::string& Swath_to_use)
  : l1b1_file(new AirMspiL1b1File(L1b1_file_name, Swath_to_use))
{
  read_data();
}

void AirMspiTimeTable::read_data()
{
  min_line_ = 0;
  tlist = l1b1_file->time();
}

void AirMspiTimeTable::print(std::ostream& Os) const 
{
  Os << "AirMspiTimeTable:\n"
     << "  L1b1 file name: " << l1b1_file_name() << "\n"
     << "  Swath to use:   " << l1b1_file->swath_to_use() << "\n";
}

