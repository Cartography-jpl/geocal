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
  AirMspiL1b1File(const std::string& Fname);
  virtual ~AirMspiL1b1File() {}
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
  {
    throw Exception("AirMspiL1b1File is read only, can't write");
  }
private:
  boost::shared_ptr<MSPI::Shared::L1B1Reader> l1b1_reader;
};

class AirMspiL1b1: public RasterImageTiledFile {
public:
  AirMspiL1b1(const std::string& Fname);
  virtual ~AirMspiL1b1() {}
};
}
#endif
