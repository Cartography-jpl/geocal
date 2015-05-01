#ifndef AIR_MSPI_L1B1_H
#define AIR_MSPI_L1B1_H
#include "tiled_file.h"
#include "raster_image_tiled_file.h"

namespace MSPI {
  namespace Shared {
    class L1B1Reader;		// Forward declaration.
  }
}

namespace GeoCal {
/****************************************************************//**
  This provides access to a AirMspiL1b1File. 

  Note that somewhat confusingly, row index is *not* the same as the
  MspiCamera band index. In fact, for the current configuration the
  MspiCamera bands run in the *opposite* order from the row index. 
  You should not assume any particular relationship between the 2,
  instead you'll want to relate the *row numbers* to each other, which
  are consistent. AirMspiIgc handles this correctly, but if you are
  directly working with these classes you'll need to do that yourself.
*******************************************************************/
class AirMspiL1b1File: public TiledFile<float, 2> {
public:
  AirMspiL1b1File(const std::string& Fname, 
		  const std::string& Swath_to_use = "660-I",
		  int Tile_number_line = -1,
		  int Tile_number_sample = -1, 
		  unsigned int Number_tile = 4);
  virtual ~AirMspiL1b1File() {}

//-----------------------------------------------------------------------
/// Number of row index. Note that a lot of the MSPI shared stuff is
/// written using "row_number", which is an underlying CCD row I
/// think. Row_index is just an index into the available rows.
//-----------------------------------------------------------------------

  int number_row_index() const 
  {fill_in_row_number(); return (int) row_number_.size();}

  std::vector<std::string> field_names(int Row_index) const;
  float wavelength(int Row_index) const;
  float polarization_angle(int Row_index) const;
  std::string swath_name(int Row_index) const;
  std::string granule_id() const;
  std::vector<Time> time() const;

//-----------------------------------------------------------------------
/// Swath we are using to read data from.
//-----------------------------------------------------------------------

  std::string swath_to_use() const 
  { return swath_name(row_index_to_use_); }

//-----------------------------------------------------------------------
/// Row index to use.
//-----------------------------------------------------------------------

  int row_index_to_use() const {return row_index_to_use_;}

//-----------------------------------------------------------------------
/// Row number to use.
//-----------------------------------------------------------------------

  int row_number_to_use() const 
  {return row_index_to_row_number(row_index_to_use_);}

  void row_index_to_use(int Row_index) 
  { 
    flush();
    clear_tile();
    row_index_to_use_ = Row_index;
  }
    
  int row_index_to_row_number(int Row_index) const;
  int row_number_to_row_index(int Row_number) const;

//-----------------------------------------------------------------------
/// The file name we are using.
//-----------------------------------------------------------------------

  const std::string& file_name() const { return fname;}
protected:
  typedef TiledFile<float, 2>::index index;
  virtual void read_tile(const boost::array<index, 2>& Min_index, 
			 const boost::array<index, 2>& Max_index, 
			 float* Res) const;
  virtual void write_tile(const boost::array<index, 2>& Min_index, 
			      const boost::array<index, 2>& Max_index, 
			      const float* V) const
  { throw Exception("AirMspiL1b1File is read only, can't write"); }
private:
  int row_index_to_use_;
  mutable std::vector<int> row_number_;
  void fill_in_row_number() const;
  std::string fname;
  boost::shared_ptr<MSPI::Shared::L1B1Reader> l1b1_reader;
  AirMspiL1b1File() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};

class AirMspiL1b1: public RasterImageTiledFile {
public:
  AirMspiL1b1(const std::string& Fname, 
	      const std::string& Swath_to_use = "660-I",
	      int Tile_number_line = -1,
	      int Tile_number_sample = -1, 
	      unsigned int Number_tile = 4);
  AirMspiL1b1(const boost::shared_ptr<AirMspiL1b1File>& L1b1_file,
	      int Tile_number_line = -1,
	      int Tile_number_sample = -1, 
	      unsigned int Number_tile = 4);
  virtual ~AirMspiL1b1() {}

//-----------------------------------------------------------------------
/// Underlying L1b1File.
//-----------------------------------------------------------------------

  const boost::shared_ptr<AirMspiL1b1File>& l1b1_file() const 
  { return l1b1_file_; } 
private:
  boost::shared_ptr<AirMspiL1b1File> l1b1_file_;
  AirMspiL1b1() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(AirMspiL1b1File);
GEOCAL_EXPORT_KEY(AirMspiL1b1);
#endif
