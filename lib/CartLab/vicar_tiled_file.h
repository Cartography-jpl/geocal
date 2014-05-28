#ifndef VICAR_TILED_FILE_H
#define VICAR_TILED_FILE_H
#include "vicar_file.h"
#include "tiled_file.h"

namespace GeoCal {
/****************************************************************//**
  This reads and writes a VICAR file, as a TiledFile.
*******************************************************************/

template<class T> class VicarTiledFile : public TiledFile<T, 2> {
public:
//-----------------------------------------------------------------------
/// Data type for index (e.g., int).
//-----------------------------------------------------------------------

  typedef typename TiledFile<T, 2>::index index;
  typedef VicarFile::access_type access_type;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  VicarTiledFile(const boost::shared_ptr<VicarFile>& F, 
		 int Band_id = 1,
		 int Number_line_per_tile = 100, int Number_tile = 4)
    : vicar_file_(F), band_id(Band_id), last_max_index(0)
  {
    boost::array<index, 2> file_size = 
      {{F->number_line(), F->number_sample()}};
    boost::array<index, 2> tile_size = 
      {{std::min(Number_line_per_tile, F->number_line()), F->number_sample()}};
    TiledFile<T, 2>::initialize(file_size, tile_size, Number_tile);
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~VicarTiledFile() {TiledFile<T, 2>::flush();}

//-----------------------------------------------------------------------
/// True if we have map info.
//-----------------------------------------------------------------------

  virtual bool has_map_info() const
  { return vicar_file().has_map_info(); }

//-----------------------------------------------------------------------
/// We can set MapInfo for this kind of file.
//-----------------------------------------------------------------------

  virtual bool can_set_map_info() const { return true; }

//-----------------------------------------------------------------------
/// Return MapInfo. 
//-----------------------------------------------------------------------

  virtual MapInfo map_info() const {return vicar_file().map_info(); }

//-----------------------------------------------------------------------
/// Set MapInfo. 
//-----------------------------------------------------------------------

  virtual void map_info(const MapInfo& M)
  { vicar_file().map_info(M); }

//-----------------------------------------------------------------------
/// VicarFile associated with this class.
//-----------------------------------------------------------------------

  const VicarFile& vicar_file() const {return *vicar_file_;}

//-----------------------------------------------------------------------
/// VicarFile associated with this class.
//-----------------------------------------------------------------------

  VicarFile& vicar_file() {return *vicar_file_;}

protected:
//-----------------------------------------------------------------------
/// Read a tile from disk. Derived classes need to supply this function.
//-----------------------------------------------------------------------

  virtual void read_tile(const boost::array<index, 2>& Min_index, 
			 const boost::array<index, 2>& Max_index, 
			 T* Res) const
  {
    // If we have write access, then reopen to get update access instead.
    if(vicar_file().access() == VicarFile::WRITE)
      vicar_file().reopen_file();
    std::vector<T> buf(this->size()[1]);
// Line is 1 based for VICAR, so add 1 to our 0 based index.

    int status = vicar_file().zvreadw(&buf[0], Min_index[0] + 1, band_id);
    if(status != 1)
      throw VicarException(status,"zvread failed for " + 
			   vicar_file().file_name());
    Res = std::copy(&buf[Min_index[1]], &buf[Max_index[1]], Res);
    for(int i = Min_index[0] + 1; i < Max_index[0]; ++i) {
      status = vicar_file().zvreadw(&buf[0], i + 1, band_id);
      if(status != 1)
	throw VicarException(status,"zvread failed for " + 
			     vicar_file().file_name());
      Res = std::copy(&buf[Min_index[1]], &buf[Max_index[1]], Res);
    }
// Vicar is odd about reading to the end of file, it will give errors
// if we go back and try to read earlier parts of the file.
// If we have read the last line, then close and reopen the file.
    if(Max_index[0] == this->size()[0])
      vicar_file().reopen_file();
  }

//-----------------------------------------------------------------------
/// Write a tile to disk. Derived classes need to supply this function.
//-----------------------------------------------------------------------

  virtual void write_tile(const boost::array<index, 2>& Min_index, 
			  const boost::array<index, 2>& Max_index, 
			  const T* V) const
  {
    if(vicar_file().access() != VicarFile::WRITE)
      throw Exception("As far as I can tell, file access must by WRITE, not UPDATE or READ");
    if(Min_index[1] != 0 ||
       Max_index[1] != TiledFile<T, 2>::size()[1])
      throw Exception("Right now, we require full lines to be written. This could be relaxed by updating the code, but that hasn't happened yet.");
    for(int i = 0; i < Max_index[0] - Min_index[0]; ++i) {
      int status = const_cast<VicarFile&>(vicar_file()).zvwritw(const_cast<T*>(V) + i * TiledFile<T, 2>::size()[1],
				      i + Min_index[0] + 1, band_id);
      if(status != 1)
	throw VicarException(status,"zvwrite failed for " + 
			     vicar_file().file_name());
    }
  }
private:
  boost::shared_ptr<VicarFile> vicar_file_;
  int band_id;
  mutable int last_max_index;
};
}
#endif
