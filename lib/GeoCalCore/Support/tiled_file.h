#ifndef TILED_FILE_H
#define TILED_FILE_H
#include "tile.h"
#include "map_info.h"
#include "geocal_exception.h"
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <iostream>

namespace GeoCal {
/****************************************************************//**
  This is a helper class for TiledFile. This allows access to a
  value, which can optionally be updated. If it is updated, we make
  sure that we correctly mark the affected tile as changed.
*******************************************************************/

template<class T> class TiledFileReference {
public:
  TiledFileReference(T& Val, bool& Changed) : v(Val), c(Changed) {}
  operator T() const {return v;}
  TiledFileReference& operator%=(const T& Val) 
  { v %= Val; c = true; return *this; }
  TiledFileReference& operator&=(const T& Val)
  { v &= Val; c = true; return *this; }
  TiledFileReference& operator*=(const T& Val)
  { v *= Val; c = true; return *this; }
  T operator++(int) { c = true; return v++; }
  TiledFileReference& operator++()
  { ++v; c = true; return *this; }
  TiledFileReference& operator+=(const T& Val)
  { v += Val; c = true; return *this; }
  T operator--(int) { c = true; return v--; }
  TiledFileReference& operator--()
  { --v; c = true; return *this; }
  TiledFileReference& operator-=(const T& Val)
  { v -= Val; c = true; return *this; }
  TiledFileReference& operator/=(const T& Val)
  { v /= Val; c = true; return *this; }
  TiledFileReference& operator=(const T& Val)
  { v = Val; c = true; return *this; }
  TiledFileReference operator^=(const T& Val)
  { v ^= Val; c = true; return *this; }
  TiledFileReference& operator|=(const T& Val)
  { v |= Val; c = true; return *this; }
private:
  T& v;
  bool& c;
};

/****************************************************************//**
  This class is the base class of TiledFile<T, D>. It turns out to
  be useful to factor out the part of TiledFile that doesn't depend
  on the type T. For most purposes though, you can ignore this class
  and use TiledFile<T, D> directly.

  For use with some classes, it is useful to be able to change a
  generic type T to some common type. For instance, RasterImage
  converts the types to and from int, and Dem converts to double. We 
  supply conversion routines in this base class for a few common
  types, as get_int, put_double, read_int etc.
*******************************************************************/

template<std::size_t D> class TiledFileBase {
public:
//-----------------------------------------------------------------------
/// Data type for index (e.g., int).
//-----------------------------------------------------------------------

  typedef boost::multi_array_types::index index;

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~TiledFileBase() { }

//-----------------------------------------------------------------------
/// Number of times we have swapped a tile since reset_number_swap called.
//-----------------------------------------------------------------------

  unsigned int number_swap() const {return number_swap_;}

//-----------------------------------------------------------------------
/// Reset number of swap counter to 0.
//-----------------------------------------------------------------------

  void reset_number_swap()  {number_swap_ = 0; }

//-----------------------------------------------------------------------
/// Flush data to disk.
//-----------------------------------------------------------------------

  virtual void flush() const = 0;

//-----------------------------------------------------------------------
/// File size.
//-----------------------------------------------------------------------

  const boost::array<index, D>& size() const {return size_;}

//-----------------------------------------------------------------------
/// Number of tiles
//-----------------------------------------------------------------------

  unsigned int number_tile() const {return number_tile_;}

//-----------------------------------------------------------------------
/// Tile size.
//-----------------------------------------------------------------------

  const boost::array<index, D>& tile_size() const {return tile_size_;}

//-----------------------------------------------------------------------
/// Get data an convert to integer.
//-----------------------------------------------------------------------

  virtual int get_int(const boost::array<index, D>& Index) const = 0;

//-----------------------------------------------------------------------
/// Get data an convert to double.
//-----------------------------------------------------------------------

  virtual double get_double(const boost::array<index, D>& Index) const = 0;

//-----------------------------------------------------------------------
/// Indicate if a file has MapInfo. Default is false, derived classes 
/// can override this.
//-----------------------------------------------------------------------

  virtual bool has_map_info() const { return false; }

//-----------------------------------------------------------------------
/// Indicate if this file type can set MapInfo. Default is false,
/// derived class can override this.
//-----------------------------------------------------------------------

  virtual bool can_set_map_info() const { return false; }

//-----------------------------------------------------------------------
/// Return MapInfo. Default is to throw an exception since this isn't
/// supported, derived classes can override this.
//-----------------------------------------------------------------------

  virtual MapInfo map_info() const 
  { throw Exception("Not implemented"); }

//-----------------------------------------------------------------------
/// Set MapInfo. Default is to throw an exception since this isn't
/// supported, derived classes can override this.
//-----------------------------------------------------------------------

  virtual void map_info(const MapInfo& M)
  { throw Exception("Not implemented"); }

//-----------------------------------------------------------------------
/// Read a subset of the data, and convert to int.
//-----------------------------------------------------------------------

  virtual void read_int(const boost::array<index, D>& Min_index, 
			const boost::array<index, D>& Max_index, 
			int* Res) const = 0;

//-----------------------------------------------------------------------
/// Read a subset of the data, and convert to double.
//-----------------------------------------------------------------------

  virtual void read_double(const boost::array<index, D>& Min_index, 
			   const boost::array<index, D>& Max_index, 
			   double* Res) const = 0;

//-----------------------------------------------------------------------
/// Convert an int to type T, and write to file.
//-----------------------------------------------------------------------

  virtual void write_int(const boost::array<index, D>& Index, int Val) = 0;

//-----------------------------------------------------------------------
/// Convert a double to type T, and write to file.
//-----------------------------------------------------------------------

  virtual void write_double(const boost::array<index, D>& Index, 
			    double Val) = 0;

//-----------------------------------------------------------------------
/// Write a subset of the data, converted from int.
//-----------------------------------------------------------------------

  virtual void write_int_arr(const boost::array<index, D>& Min_index, 
			     const boost::array<index, D>& Max_index, 
			     const int* V) = 0;

//-----------------------------------------------------------------------
/// Read a subset of the data, and convert to double.
//-----------------------------------------------------------------------

  virtual void write_double_arr(const boost::array<index, D>& Min_index, 
				const boost::array<index, D>& Max_index, 
				const double* V) = 0;

protected:
  boost::array<index, D> size_;	///< File size
  mutable unsigned int number_swap_; 
				///< Number of swaps that have
				///occured.
  unsigned int number_tile_;
  boost::array<index, D> tile_size_; 
				///< Nominal size of tile.
};

/****************************************************************//**
  This class is used to handle reading and writing a file using
  tiles. 

  It is common for files to be stored as a set of tiles. This class
  handles all of the book keeping needed to make these files seem like
  they are all in memory at one time. We automatically handle the
  swapping to and from disk as needed.

  This is an abstract based class, a derived class needs to supply
  the actual read and write methods used by this class.

  Note that it is imperative that derived class provide a destructor
  that calls flush. We can not call virtual functions of derived
  classes from a destructor in the base class, because the virtual
  table for the derived class has already been destroyed by the time
  we get to the destructor in this class (see Stroustrup 3rd edition,
  4.3 for details). This means that the derived class needs to handle
  flushing of the data to disk, by a call to flush().

  Note that the tile size is not required to evenly divide the file
  size, in this case we just have tiles on the edges that aren't the
  full size.
*******************************************************************/

template<class T, std::size_t D> class TiledFile : public boost::noncopyable,
  public TiledFileBase<D> {
public:

//-----------------------------------------------------------------------
/// Data type for index (e.g., int).
//-----------------------------------------------------------------------

  typedef typename TiledFileBase<D>::index index;

//-----------------------------------------------------------------------
/// Reference type.
//-----------------------------------------------------------------------
  
  typedef TiledFileReference<T> reference;

//-----------------------------------------------------------------------
/// Const reference type.
//-----------------------------------------------------------------------
  
  typedef const T& const_reference;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  TiledFile(const boost::array<index, D>& File_size, 
	    const boost::array<index, D>& Tile_size, 
	    unsigned int Number_tile = 4)
  { initialize(File_size, Tile_size, Number_tile); }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~TiledFile() { }

//-----------------------------------------------------------------------
/// Flush data to disk.
//-----------------------------------------------------------------------

  virtual void flush() const
  {
    typedef Tile<T, D> Tl;
    BOOST_FOREACH(Tl& t, tile_) {
      if(t.changed) {
	write_tile(t.min_index(), t.max_index(), 
		   &t.data(t.min_index()));
	t.changed = false;
      }
    }
  }

/// Access data at given location. Note that only the operator() with
/// the correct number of arguments should be called.
  reference operator()(index I1)
  { boost::array<index, D> a = {{I1}}; return (*this)(a); }
  const_reference  operator()(index I1) const
  { boost::array<index, D> a = {{I1}}; return (*this)(a); }
  reference operator()(index I1, index I2)
  { boost::array<index, D> a = {{I1, I2}}; return (*this)(a); }
  const_reference  operator()(index I1, index I2) const
  { boost::array<index, D> a = {{I1, I2}}; return (*this)(a); }
  reference operator()(index I1, index I2, index I3)
  { boost::array<index, D> a = {{I1, I2, I3}}; return (*this)(a); }
  const_reference  operator()(index I1, index I2, index I3) const
  { boost::array<index, D> a = {{I1, I2, I3}}; return (*this)(a); }
  reference operator()(index I1, index I2, index I3, index I4)
  { boost::array<index, D> a = {{I1, I2, I3, I4}}; return (*this)(a); }
  const_reference  operator()(index I1, index I2, index I3, index I4) const
  { boost::array<index, D> a = {{I1, I2, I3, I4}}; return (*this)(a); }
  reference operator()(index I1, index I2, index I3, index I4,
		       index I5)
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5}}; return (*this)(a); }
  const_reference  operator()(index I1, index I2, index I3, index I4,
			      index I5) const
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5}}; return (*this)(a); }
  reference operator()(index I1, index I2, index I3, index I4,
		       index I5, index I6)
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5, I6}}; return (*this)(a); }
  const_reference  operator()(index I1, index I2, index I3, index I4,
			      index I5, index I6) const
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5, I6}}; return (*this)(a); }
  reference operator()(index I1, index I2, index I3, index I4,
		       index I5, index I6, index I7)
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5, I6, I7}}; 
    return (*this)(a); }
  const_reference  operator()(index I1, index I2, index I3, index I4,
			      index I5, index I6, index I7) const
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5, I6, I7}};
    return (*this)(a); }
  reference operator()(index I1, index I2, index I3, index I4,
		       index I5, index I6, index I7, index I8)
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5, I6, I7, I8}}; 
    return (*this)(a); }
  const_reference  operator()(index I1, index I2, index I3, index I4,
			      index I5, index I6, index I7, index I8) const
  { boost::array<index, D> a = {{I1, I2, I3, I4, I5, I6, I7, I8}};
    return (*this)(a); }

//-----------------------------------------------------------------------
/// Return data referenced at the given index. This can be assigned to
/// write data.
//-----------------------------------------------------------------------

  reference operator()(const boost::array<index, D>& Index)
  {
    Tile<T, D>& t = swap(Index);
    return TiledFileReference<T>(t.data(Index), t.changed);
  }

//-----------------------------------------------------------------------
/// Return data referenced at the given index.
//-----------------------------------------------------------------------

  const_reference  operator()(const boost::array<index, D>& Index) const
  { return swap(Index).data(Index); }

//-----------------------------------------------------------------------
/// Read a subset of the data.
//-----------------------------------------------------------------------

  virtual void read_ptr(const boost::array<index, D>& Min_index, 
		const boost::array<index, D>& Max_index, 
		T* Res) const
  { 
    for(std::size_t i = 0; i < D; ++i)
      if(Min_index[i] < 0 ||
	 Min_index[i] >= Max_index[i] ||
	 Max_index[i] > TiledFileBase<D>::size()[i]) {
	Exception e("Min_index or Max_index is out of range\n");
	e << "i: " << i << "\n"
	  << "Min_index[i]: " << Min_index[i] << "\n"
	  << "Max_index[i]: " << Max_index[i] << "\n"
	  << "TiledFileBase<D>::size()[i]: " << TiledFileBase<D>::size()[i] << "\n";
	throw e;
      }

//-----------------------------------------------------------------------
/// Check first to see if entire requested data is in a single tile.
/// If not, the we read from disk (rather than trying to stitch
/// together multiple tiles).
//-----------------------------------------------------------------------

//     typedef Tile<T, D> Tl;
//     BOOST_FOREACH(Tl& t, tile_)
//       if(t.in_tile(Min_index) &&
// 	 t.in_tile(Max_index)) {
	
//       }
    read_tile(Min_index, Max_index, Res); 
  }

//-----------------------------------------------------------------------
/// Write a subset of the data.
//-----------------------------------------------------------------------

  virtual void write_ptr(const boost::array<index, D>& Min_index, 
	     const boost::array<index, D>& Max_index, 
	     const T* V) const
  { 
    for(std::size_t i = 0; i < D; ++i)
      if(Min_index[i] < 0 ||
	 Min_index[i] >= Max_index[i] ||
	 Max_index[i] > TiledFileBase<D>::size()[i]) {
	Exception e("Min_index or Max_index is out of range\n");
	e << "i: " << i << "\n"
	  << "Min_index[i]: " << Min_index[i] << "\n"
	  << "Max_index[i]: " << Max_index[i] << "\n"
	  << "TiledFileBase<D>::size()[i]: " << TiledFileBase<D>::size()[i] << "\n";
	throw e;
      }

    write_tile(Min_index, Max_index, V); 
  }

//-----------------------------------------------------------------------
/// Read a subset of the data.
//-----------------------------------------------------------------------

  boost::multi_array<T, D>
  read(const boost::array<index, D>& Min_index, 
       const boost::array<index, D>& Max_index) const
  {
    boost::array<index, D> sz;
    for(std::size_t i = 0; i < D; ++i) {
      if(Min_index[i] < 0 ||
	 Min_index[i] >=  Max_index[i] ||
	 Max_index[i] > TiledFileBase<D>::size()[i]) {
	Exception e("Min_index or Max_index is out of range\n");
	e << "i: " << i << "\n"
	  << "Min_index[i]: " << Min_index[i] << "\n"
	  << "Max_index[i]: " << Max_index[i] << "\n"
	  << "TiledFileBase<D>::size()[i]: " << TiledFileBase<D>::size()[i] << "\n";
	throw e;
      }
      sz[i] = Max_index[i] - Min_index[i];
    }
    boost::multi_array<T, D> res(sz);
    read_ptr(Min_index, Max_index, res.data());
    return res;
  }

//-----------------------------------------------------------------------
/// Write a subset of the data.
//-----------------------------------------------------------------------

  
  void write(const boost::array<index, D>& Min_index, 
	     const boost::multi_array<T, D>& V) const
  {
    boost::array<index, D> max_index;
    for(std::size_t i = 0; i < D; ++i) {
      max_index[i] = Min_index[i] + V.shape()[i];
      if(Min_index[i] < 0 ||
	 Min_index[i] >=  max_index[i] ||
	 max_index[i] > TiledFileBase<D>::size()[i]) {
	Exception e("Min_index or Max_index is out of range\n");
	e << "i: " << i << "\n"
	  << "Min_index[i]: " << Min_index[i] << "\n"
	  << "max_index[i]: " << max_index[i] << "\n"
	  << "TiledFileBase<D>::size()[i]: " << TiledFileBase<D>::size()[i] << "\n";
	throw e;
      }
    }
    write_ptr(Min_index, max_index, V.data());
  }

//-----------------------------------------------------------------------
/// Number of tiles
//-----------------------------------------------------------------------

  int number_tile() const { return static_cast<int>(tile_.size()); }

//-----------------------------------------------------------------------
/// Get data an convert to integer.
//-----------------------------------------------------------------------

  virtual int get_int(const boost::array<index, D>& Index) const
  { return static_cast<int>((*this)(Index));}

//-----------------------------------------------------------------------
/// Get data an convert to double.
//-----------------------------------------------------------------------

  virtual double get_double(const boost::array<index, D>& Index) const
  { return static_cast<double>((*this)(Index));}

//-----------------------------------------------------------------------
/// Read a subset of the data, and convert to int.
//-----------------------------------------------------------------------

  virtual void read_int(const boost::array<index, D>& Min_index, 
			const boost::array<index, D>& Max_index, 
			int* Res) const
  {
    boost::multi_array<T, D> dat = read(Min_index, Max_index);
    for(const T* i = dat.data(); i != dat.data() + dat.num_elements(); ++i, 
	  ++Res)
      *Res = static_cast<int>(*i);
  }

//-----------------------------------------------------------------------
/// Read a subset of the data, and convert to double.
//-----------------------------------------------------------------------

  virtual void read_double(const boost::array<index, D>& Min_index, 
			   const boost::array<index, D>& Max_index, 
			   double* Res) const
  {
    boost::multi_array<T, D> dat = read(Min_index, Max_index);
    for(const T* i = dat.data(); i != dat.data() + dat.num_elements(); ++i, 
	  ++Res)
      *Res = static_cast<double>(*i);
  }

//-----------------------------------------------------------------------
/// Convert an int to type T, and write to file.
//-----------------------------------------------------------------------

  virtual void write_int(const boost::array<index, D>& Index, int Val)
  { (*this)(Index) = static_cast<T>(Val);}

//-----------------------------------------------------------------------
/// Convert an int to type T, and write to file.
//-----------------------------------------------------------------------

  virtual void write_int_arr(const boost::array<index, D>& Min_index, 
			     const boost::array<index, D>& Max_index, 
			     const int* V)
  { 
    boost::array<index, D> sz;
    for(std::size_t i = 0; i < D; ++i)
      sz[i] = Max_index[i] - Min_index[i];
    boost::multi_array<T, D> dat(sz);
    for(T* i = dat.data(); i != dat.data() + dat.num_elements(); ++i, ++V)
      *i = static_cast<T>(*V);
    write(Min_index, dat);
  }

//-----------------------------------------------------------------------
/// Convert a double to type T, and write to file.
//-----------------------------------------------------------------------

  virtual void write_double(const boost::array<index, D>& Index, 
			    double Val)
  { (*this)(Index) = static_cast<T>(Val);}

//-----------------------------------------------------------------------
/// Convert a double to type T, and write to file.
//-----------------------------------------------------------------------

  virtual void write_double_arr(const boost::array<index, D>& Min_index, 
			    const boost::array<index, D>& Max_index, 
			    const double* V) 
  { 
    boost::array<index, D> sz;
    for(std::size_t i = 0; i < D; ++i)
      sz[i] = Max_index[i] - Min_index[i];
    boost::multi_array<T, D> dat(sz);
    for(T* i = dat.data(); i != dat.data() + dat.num_elements(); ++i, ++V)
      *i = static_cast<T>(*V);
    write(Min_index, dat);
  }

protected:

//-----------------------------------------------------------------------
/// Default constructor. Derived classes need to call initialize
/// before they are done with their constructor.
//-----------------------------------------------------------------------

  inline TiledFile() { }

//-----------------------------------------------------------------------
/// Finish initializing the object.
//-----------------------------------------------------------------------

  void initialize(const boost::array<index, D>& File_size, 
		  const boost::array<index, D>& Tile_size, 
		  unsigned int Number_tile = 4)
  {
    for(std::size_t i = 0; i < D; ++i) {
      range_min_check(File_size[i], (index) 1);
      range_min_check(Tile_size[i], (index) 1);
    }
    TiledFileBase<D>::size_ = File_size;
    TiledFileBase<D>::number_tile_ = Number_tile;
    TiledFileBase<D>::tile_size_ = Tile_size;
    tile_.resize(Number_tile);
    next_swap_ = tile_.begin();
    TiledFileBase<D>::reset_number_swap();
  }

//-----------------------------------------------------------------------
/// Read a tile from disk. Derived classes need to supply this function.
//-----------------------------------------------------------------------

  virtual void read_tile(const boost::array<index, D>& Min_index, 
			     const boost::array<index, D>& Max_index, 
			     T* Res) const = 0;

//-----------------------------------------------------------------------
/// Return reference to tile that contains item pointed to by Index,
/// performing a swap if necessary
//-----------------------------------------------------------------------

  Tile<T, D>& swap(const boost::array<index, D>& Index) const
  {
//-----------------------------------------------------------------------
// Check to see if data in in cache, before doing any other checking.
//-----------------------------------------------------------------------

    typedef Tile<T, D> Tl;
    BOOST_FOREACH(Tl& t, tile_) {
      if(t.in_tile(Index))
	return t;
    }

//-----------------------------------------------------------------------
// Now, we have the more complicated case of needed to read in data.
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// REQUIRE: Index[i] < size(i).
//---------------------------------------------------------------------------

    boost::array<index, D> min_index;
    boost::array<index, D> max_index;
    for(std::size_t i = 0; i < D; ++i) {
      range_max_check(Index[i], TiledFileBase<D>::size_[i]);
      min_index[i] = (Index[i] / TiledFileBase<D>::tile_size_[i]) * 
	TiledFileBase<D>::tile_size_[i];
      max_index[i] = min_index[i] + TiledFileBase<D>::tile_size_[i];
      if(max_index[i] > TiledFileBase<D>::size_[i])
	max_index[i] = TiledFileBase<D>::size_[i];
    }

//-----------------------------------------------------------------------
// Write out tile to be swapped, if it has changed.
//-----------------------------------------------------------------------

    if((*next_swap_).changed) {
      write_tile((*next_swap_).min_index(), (*next_swap_).max_index(),
		 &(*next_swap_).data((*next_swap_).min_index()));
      (*next_swap_).changed = false;
    }

//-----------------------------------------------------------------------
// Read in tile. If any problems occur, reset the tile to empty before
// exiting (so it doesn't contain bad data).
//-----------------------------------------------------------------------

    try {
      (*next_swap_).swap(min_index, max_index);
      read_tile(min_index, max_index, &(*next_swap_).data(min_index));
    } catch(...) {
      boost::array<index, D> t;
      for(std::size_t i = 0; i < D; ++i)
	t[i] = 0;
      next_swap_->swap(t, t);
      throw;
    }

//-----------------------------------------------------------------------
// Increment next_swap, and return old value.
//-----------------------------------------------------------------------

    typename std::vector<Tile<T, D> >::iterator result = next_swap_;
    next_swap_++;
    if(next_swap_ ==tile_.end())
      next_swap_ = tile_.begin();
    ++TiledFileBase<D>::number_swap_;
#ifdef DIAGNOSTIC
    if(TiledFileBase<D>::number_swap_ % 100 == 0)
      std::cerr << "Swap count: " << TiledFileBase<D>::number_swap_
		<< " for object at address " << this << "\n";
#endif
    return *result;
  }

//-----------------------------------------------------------------------
/// Write a tile to disk. Derived classes need to supply this function.
//-----------------------------------------------------------------------

  virtual void     write_tile(const boost::array<index, D>& Min_index, 
			      const boost::array<index, D>& Max_index, 
			      const T* V) const = 0;
private:
  mutable typename std::vector<Tile<T, D> >::iterator next_swap_;
				///< Next tile to be swapped.
  mutable std::vector<Tile<T, D> > tile_; 
				///< Tiles
};
}
#endif
