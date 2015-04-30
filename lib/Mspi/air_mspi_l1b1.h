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
*******************************************************************/
class AirMspiL1b1File: public TiledFile<float, 2> {
public:
  AirMspiL1b1File(const std::string& Fname, int Tile_number_line = -1,
		  int Tile_number_sample = -1, 
		  unsigned int Number_tile = 4);
  virtual ~AirMspiL1b1File() {}

//-----------------------------------------------------------------------
/// The file name we are using.
//-----------------------------------------------------------------------

  const std::string& file_name() const { return fname;}
  
protected:
  typedef TiledFile<float, 2>::index index;
  virtual void read_tile(const boost::array<index, 2>& Min_index, 
			 const boost::array<index, 2>& Max_index, 
			 float* Res) const
  {
    //Temp
  }
  virtual void write_tile(const boost::array<index, 2>& Min_index, 
			      const boost::array<index, 2>& Max_index, 
			      const float* V) const
  { throw Exception("AirMspiL1b1File is read only, can't write"); }
private:
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
  AirMspiL1b1(const std::string& Fname);
  virtual ~AirMspiL1b1() {}
private:
  boost::shared_ptr<AirMspiL1b1File> l1b1;
  AirMspiL1b1() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(AirMspiL1b1File);
GEOCAL_EXPORT_KEY(AirMspiL1b1);
#endif
