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
    & GEOCAL_NVP(fname);
}

template<class Archive>
void AirMspiL1b1File::load(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiledFile_float_2)
    & GEOCAL_NVP(fname);
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
 int Tile_number_line,
 int Tile_number_sample,
 unsigned int Number_tile
)
  : fname(Fname)
{
#ifdef HAVE_MSPI_SHARED
  fname = Fname;
  l1b1_reader.reset(new MSPI::Shared::L1B1Reader(Fname));
  typedef TiledFile<float, 2>::index index;
  boost::array<index, 2> file_size;
  boost::array<index, 2> tile_size;
  file_size[0] = l1b1_reader->number_frame(l1b1_reader->row_numbers()[0]);
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


//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiL1b1::AirMspiL1b1(const std::string& Fname)
{
  l1b1.reset(new AirMspiL1b1File(Fname));
  initialize(l1b1);
}
