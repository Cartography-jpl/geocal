#ifndef TILE_H
#define TILE_H
#include "boost/multi_array.hpp"

namespace GeoCal {

/****************************************************************//**
  This is used by TiledFile to maintain a tile of data. 

  This class has two jobs:
  -# Maintain the actual tile data. 
  -# Keep track of if the data has been changed and needs to be
  written to disk.

  We make use of boost::multi_array ability to change the index
  base, so you index data the same way you would a file (e.g., to 
  read (100, 200) in a file, you read data[100][200] for the
  appropriate tile).
*******************************************************************/

template<class T, std::size_t D> class Tile {
public:

//-----------------------------------------------------------------------
/// Data type for index (e.g., int).
//-----------------------------------------------------------------------

  typedef boost::multi_array_types::index index;

//-----------------------------------------------------------------------
/// Indicates if the data has been changed and needs to be written out
/// to disk.
//-----------------------------------------------------------------------

  bool changed;

//-----------------------------------------------------------------------
/// The data. The index_bases of this is set so this matches the
/// indexing of the file.
//-----------------------------------------------------------------------

  boost::multi_array<T, D> data;

//-----------------------------------------------------------------------
/// Test the given index to see if it is in this tile or not.
//-----------------------------------------------------------------------

  bool in_tile(const boost::array<index, D>& Index) const
  {
    for(std::size_t i = 0; i < D; ++i)
      if(Index[i] < static_cast<index>(data.index_bases()[i]) ||
	 Index[i] >= static_cast<index>(data.index_bases()[i] + data.shape()[i]))
	return false;
    return true;
  }

//-----------------------------------------------------------------------
/// Swap range of data.
//-----------------------------------------------------------------------
  
  void swap(const boost::array<index, D>& Min_index,
	    const boost::array<index, D>& Max_index) {
    boost::array<boost::multi_array_types::size_type, D> r;
    boost::array<index, D> r2;
    for(std::size_t i = 0; i < D; ++i) {
      r[i] = Max_index[i] - Min_index[i];
      r2[i] = 0;
    }
    data.reindex(r2);
    data.resize(r);
    data.reindex(Min_index);
    changed = false;
  }

//-----------------------------------------------------------------------
/// Return minimum index.
//-----------------------------------------------------------------------

  boost::array<index, D> min_index() const
  {
    boost::array<index, D> res;
    for(std::size_t i = 0; i < D; ++i)
      res[i] = data.index_bases()[i];
    return res;
  }

//-----------------------------------------------------------------------
/// Return maximum index.
//-----------------------------------------------------------------------

  boost::array<index, D> max_index() const
  {
    boost::array<index, D> res;
    for(std::size_t i = 0; i < D; ++i)
      res[i] = data.shape()[i] + data.index_bases()[i];
    return res;
  }

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  Tile() :changed(false) {}
};
}
#endif
