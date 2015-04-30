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
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiledFile_float_2)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP(row_index_to_use);

}

template<class Archive>
void AirMspiL1b1File::load(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiledFile_float_2)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP(row_index_to_use);
#ifdef HAVE_MSPI_SHARED
  l1b1_reader.reset(new MSPI::Shared::L1B1Reader(fname));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
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

AirMspiL1b1File::AirMspiL1b1File
(const std::string& Fname, 
 const std::string& Swath_to_use,
 int Tile_number_line,
 int Tile_number_sample,
 unsigned int Number_tile
)
  : fname(Fname)
{
#ifdef HAVE_MSPI_SHARED
  fname = Fname;
  l1b1_reader.reset(new MSPI::Shared::L1B1Reader(Fname));
  row_index_to_use = -1;
  for(int i = 0; i < number_row_index(); ++i)
    if(swath_name(i) == Swath_to_use)
      row_index_to_use = i;
  if(row_index_to_use < 0) {
    Exception e;
    e << "Could not find swath " << Swath_to_use << " in file " << Fname;
    throw e;
  }
  typedef TiledFile<float, 2>::index index;
  boost::array<index, 2> file_size;
  boost::array<index, 2> tile_size;
  file_size[0] = l1b1_reader->number_frame(row_index_to_row(row_index_to_use));
  file_size[1] = l1b1_reader->number_pixel();
  if(Tile_number_line < 0)
    tile_size[0] = file_size[0];
  else
    tile_size[0] = Tile_number_line;
  if(Tile_number_sample < 0)
    tile_size[1] = file_size[1];
  else
    tile_size[1] = Tile_number_sample;
  initialize(file_size, tile_size, Number_tile);
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

int AirMspiL1b1File::row_index_to_row(int Row_index) const
{
#ifdef HAVE_MSPI_SHARED
  std::vector<int> rnum = l1b1_reader->row_numbers();
  range_check(Row_index, 0, (int) rnum.size());
  return rnum[Row_index];
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Number of row index. Note that a lot of the MSPI shared stuff is
/// written using "row_number", which is an underlying CCD row I
/// think. Row_index is just an index into the available rows.
//-----------------------------------------------------------------------

int AirMspiL1b1File::number_row_index() const 
{ 
#ifdef HAVE_MSPI_SHARED
  return (int) l1b1_reader->row_numbers().size(); 
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Return the field names for the given row_index.
//-----------------------------------------------------------------------

std::vector<std::string> AirMspiL1b1File::field_names(int Row_index) const
{ 
#ifdef HAVE_MSPI_SHARED
  return l1b1_reader->field_names(row_index_to_row(Row_index));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Return the wavelength for the given row_index.
//-----------------------------------------------------------------------

float AirMspiL1b1File::wavelength(int Row_index) const
{ 
#ifdef HAVE_MSPI_SHARED
  return l1b1_reader->wavelength(row_index_to_row(Row_index));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Return the polarization angle for the given row_index.
//-----------------------------------------------------------------------


float AirMspiL1b1File::polarization_angle(int Row_index) const
{ 
#ifdef HAVE_MSPI_SHARED
  return l1b1_reader->polarization_angle(row_index_to_row(Row_index));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Return the swath name for the given row_index.
//-----------------------------------------------------------------------

std::string AirMspiL1b1File::swath_name(int Row_index) const
{ 
#ifdef HAVE_MSPI_SHARED
  return l1b1_reader->swath_name(row_index_to_row(Row_index));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}


//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiL1b1::AirMspiL1b1
(const std::string& Fname, 
 const std::string& Swath_to_use,
 int Tile_number_line,
 int Tile_number_sample,
 unsigned int Number_tile
)
{
  l1b1.reset(new AirMspiL1b1File(Fname, Swath_to_use, Tile_number_line, 
				 Tile_number_sample, Number_tile));
  initialize(l1b1);
}
