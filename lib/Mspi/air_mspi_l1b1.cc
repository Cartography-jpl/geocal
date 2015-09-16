#include "geocal_internal_config.h"
#include "air_mspi_l1b1.h"
#include "geocal_serialize_support.h"
#include "air_mspi_file.h"
#ifdef HAVE_MSPI_SHARED
#include "MSPI-Shared/File/L1B1File/src/l1b1_reader.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void AirMspiL1b1File::save(Archive & ar, const unsigned int version) const
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiledFile_float_2)
    & GEOCAL_NVP(min_l1b1_line)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP_(row_index_to_use);

}

template<class Archive>
void AirMspiL1b1File::load(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiledFile_float_2)
    & GEOCAL_NVP(min_l1b1_line)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP_(row_index_to_use);
#ifdef HAVE_MSPI_SHARED
  l1b1_reader.reset(new MSPI::Shared::L1B1Reader(air_mspi_true_file_name(fname)));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

template<class Archive>
void AirMspiL1b1::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageTiledFile)
    & GEOCAL_NVP_(l1b1_file);
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
 int Min_l1b1_line,
 int Max_l1b1_line,
 int Tile_number_line,
 int Tile_number_sample,
 unsigned int Number_tile
)
  : fname(Fname),
    min_l1b1_line(Min_l1b1_line)
{
#ifdef HAVE_MSPI_SHARED
  fname = Fname;
  l1b1_reader.reset(new MSPI::Shared::L1B1Reader(air_mspi_true_file_name(Fname)));
  row_index_to_use_ = -1;
  for(int i = 0; i < number_row_index(); ++i)
    if(swath_name(i) == Swath_to_use)
      row_index_to_use_ = i;
  if(row_index_to_use_ < 0) {
    Exception e;
    e << "Could not find swath " << Swath_to_use << " in file " << Fname;
    throw e;
  }
  typedef TiledFile<float, 2>::index index;
  boost::array<index, 2> file_size;
  boost::array<index, 2> tile_size;
  file_size[0] = l1b1_reader->number_frame(row_number_to_use()) - min_l1b1_line;
  if(Max_l1b1_line > -1 &&
     file_size[0] > Max_l1b1_line - min_l1b1_line + 1)
    file_size[0] = Max_l1b1_line - min_l1b1_line + 1;
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
/// Map Row index to row number.
//-----------------------------------------------------------------------

int AirMspiL1b1File::row_index_to_row_number(int Row_index) const
{
  fill_in_row_number();
  range_check(Row_index, 0, (int) row_number_.size());
  return row_number_[Row_index];
}

//-----------------------------------------------------------------------
/// Map row number to row index.
//-----------------------------------------------------------------------

int AirMspiL1b1File::row_number_to_row_index(int Row_number) const
{
  fill_in_row_number();
  std::vector<int>::const_iterator f = 
    std::find(row_number_.begin(), row_number_.end(), Row_number);
  if(f == row_number_.end()) {
    Exception e("Row number not assigned to row index: ");
    e << Row_number;
    throw e;
  }
  return (int)(f - row_number_.begin());
}

void AirMspiL1b1File::fill_in_row_number() const
{
  if(row_number_.size() > 0)
    return;
#ifdef HAVE_MSPI_SHARED
  row_number_ = l1b1_reader->row_numbers();
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
  return l1b1_reader->field_names(row_index_to_row_number(Row_index));
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Return the granule id.
//-----------------------------------------------------------------------

std::string AirMspiL1b1File::granule_id() const
{
#ifdef HAVE_MSPI_SHARED
  return l1b1_reader->granule_id();
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

void AirMspiL1b1File::read_tile(const boost::array<index, 2>& Min_index, 
	       const boost::array<index, 2>& Max_index, 
	       float* Res) const
{
#ifdef HAVE_MSPI_SHARED
  boost::multi_array<float, 2> d = 
    l1b1_reader->read_data(row_number_to_use(), "I", Min_index[0] + min_l1b1_line,
			   Max_index[0] - Min_index[0]);
  for(int i = 0; i < (int) d.shape()[0]; ++i)
    for(int j = Min_index[1]; j < Max_index[1]; ++j, ++Res)
      *Res = d[i][j];
#else
  throw Exception("This class requires that MSPI Shared library be available");
#endif
}

//-----------------------------------------------------------------------
/// Return the time for each line.
//-----------------------------------------------------------------------

std::vector<Time> AirMspiL1b1File::time() const
{
#ifdef HAVE_MSPI_SHARED
  Time tepoch = Time::parse_time(l1b1_reader->epoch());
  std::vector<double> toffset =
    l1b1_reader->read_time(row_number_to_use(), min_l1b1_line, 
			   size()[0]);
  std::vector<Time> tlist;
  BOOST_FOREACH(double toff, toffset)
    tlist.push_back(tepoch + toff);
  return tlist;
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
  return l1b1_reader->wavelength(row_index_to_row_number(Row_index));
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
  return l1b1_reader->polarization_angle(row_index_to_row_number(Row_index));
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
  return l1b1_reader->swath_name(row_index_to_row_number(Row_index));
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
  : l1b1_file_(new AirMspiL1b1File(Fname, Swath_to_use, Tile_number_line, 
				   Tile_number_sample, Number_tile))
{
  initialize(l1b1_file_);
}

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

AirMspiL1b1::AirMspiL1b1
(const boost::shared_ptr<AirMspiL1b1File>& L1b1_file,
 int Tile_number_line,
 int Tile_number_sample,
 unsigned int Number_tile
)
  : l1b1_file_(L1b1_file)
{
  initialize(l1b1_file_);
}
