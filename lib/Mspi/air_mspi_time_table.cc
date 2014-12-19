#include "geocal_internal_config.h"
#include "air_mspi_time_table.h"
#include "mspi_config_file.h"
#include "geocal_serialize_support.h"
#ifdef HAVE_MSPI_SHARED
#include "File/L1B1File/src/l1b1_reader.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
// template<class Archive>
// void AirMspiIgc::serialize(Archive & ar, const unsigned int version)
// {
//   ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IpiImageGroundConnection);
//   ar & GEOCAL_NVP2("base_directory", bdir) 
//     & GEOCAL_NVP2("master_config_file", mconfig);
// }

// GEOCAL_IMPLEMENT(AirMspiIgc);
#endif

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiTimeTable::AirMspiTimeTable
(const std::string& L1b1_file_name, 
 const std::string& Instrument_config_file_name)
{
#ifdef HAVE_MSPI_SHARED
  MSPI::Shared::L1B1Reader l1b1(L1b1_file_name);
  refrow = reference_row(Instrument_config_file_name);
  Time tepoch = Time::parse_time(l1b1.epoch());
  std::vector<double> toffset = 
    l1b1.read_time(refrow, 0, l1b1.number_frame(refrow));
  min_line_ = 0;
  BOOST_FOREACH(double toff, toffset)
    tlist.push_back(tepoch + toff);
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

void AirMspiTimeTable::print(std::ostream& Os) const 
{
  Os << "AirMspiTimeTable:\n"
     << "  L1b1 file name: " << l1b1_file_name << "\n"
     << "  Reference row:  " << refrow << "\n";
}


//-----------------------------------------------------------------------
/// Determine the reference row to use for the time table. This comes
/// from the 660nm I band. 
/// Note that the band number used in the instrument config file is
/// *not* the same as the band number used in the camera. Instead this
/// is a spectral band number.
//-----------------------------------------------------------------------

int AirMspiTimeTable::reference_row
(const std::string& Instrument_config_file_name) const
{
  // Determine mapping from instrument_band, row_type to row_number
  MspiConfigFile iconfig(Instrument_config_file_name);
  std::vector<int> rn = iconfig.value<std::vector<int> >("row_numbers");
  std::vector<std::string> rt = 
    iconfig.value<std::vector<std::string> >("row_types");
  // Note that this is a spectral band, not the camera band.
  std::vector<int> rb = iconfig.value<std::vector<int> >("band");
  std::map<int, std::map<std::string, int> > inst_to_row;
  for(int i = 0 ; i < (int) rn.size(); ++i)
    inst_to_row[rb[i]][rt[i]]=rn[i];
  return inst_to_row[6]["I"];
}
