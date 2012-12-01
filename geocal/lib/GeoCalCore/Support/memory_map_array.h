#ifndef MEMORY_MAP_ARRAY_H
#define MEMORY_MAP_ARRAY_H
#include "geocal_exception.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <boost/multi_array.hpp>
#include <boost/scoped_ptr.hpp>

namespace GeoCal {

/****************************************************************//**
  Access a binary file as an array. This memory maps a file and 
  supplies a boost::multi_array_ref interface to the file. Note 
  that in general binary files aren't portable across different machines
  because the endian and/or size of types aren't portable. Never the 
  less, these binary files can be useful.
*******************************************************************/

template<class T, int D> class MemoryMapArray {
public:
  typedef off_t offset_type;	///< Offset type.
  enum Mode {READ,		///< Read only file
	     UPDATE,		///< Update existing file
	     CREATE		///< Create a file
  };
  
//-----------------------------------------------------------------------
/// Open an existing file for reading or update, or if the mode is
/// CREATE then create a new file. The extents to use for the data is
/// passed in. An offset to the first byte to read/write can be given,
/// in bytes (e.g., there is a header that should be skipped).
//-----------------------------------------------------------------------

  template <class ExtentList>
    MemoryMapArray(const std::string& Fname, ExtentList const& Extents,
		   Mode M = READ, offset_type Offset = 0,
		   const boost::general_storage_order<D>& So = 
		   boost::c_storage_order())
      : offset_(Offset), fname(Fname)
  {
//-----------------------------------------------------------------------
// Note that data is actually invalid at this point. We initialize it
// so we can query size information from it, rather than parsing the
// Extents ourself.
//-----------------------------------------------------------------------

    data_.reset(new boost::multi_array_ref<T, D>(0, Extents));

    range_min_check(Offset, (offset_type) 0);

//-----------------------------------------------------------------------
// Open file.
//-----------------------------------------------------------------------

    int flags = (M ==UPDATE || M ==CREATE ? O_RDWR : O_RDONLY);
    if(M ==CREATE)
      flags |= O_CREAT | O_TRUNC;
    fd_ = open(Fname.c_str(), flags, 0666);
    if(fd_ ==-1)
      throw Exception("Open failed for file " + Fname);

//-----------------------------------------------------------------------
// If we are creating file, then fill file.
//-----------------------------------------------------------------------
    if(M ==CREATE) {
      lseek(fd_, data().num_elements() * sizeof(T) + Offset - 1, SEEK_SET);
      char zero = 0;
      write(fd_, &zero, 1);
    } else {

//-----------------------------------------------------------------------
// Otherwise, find length of file and make sure file is large enough.
//-----------------------------------------------------------------------

      off_t file_size = lseek(fd_, 0, SEEK_END);
      if(data().num_elements() * sizeof(T) + Offset > (size_t) file_size) {
	Exception e("File to too small to fit requested size\n");
	e << "  File:        " << Fname << "\n"
	  << "  Size:        " << file_size << "\n"
	  << "  Offset:      " << Offset << "\n"
	  << "  Sizeof(T):   " << sizeof(T) << "\n"
	  << "  Request #Ts: " << data().num_elements() << "\n";
	throw e;
      }
    }

//-----------------------------------------------------------------------
// Memory map file.
//-----------------------------------------------------------------------

    int prot = PROT_READ;
    if(M ==UPDATE ||
       M ==CREATE)
      prot |= PROT_WRITE;
    data_void_ = mmap(0, sizeof(T) * data().num_elements() + offset_, 
		      prot, MAP_SHARED, fd_, 0);
    if(data_void_ ==MAP_FAILED) {
      Exception e("Mmap call failed.");
      e << " Error number: " << errno << "\n";
      throw e;
    }
    data_.reset(new 
		boost::multi_array_ref<T, D>(
   	        reinterpret_cast<T*>(reinterpret_cast<char*>(data_void_) + 
				     offset_),
		Extents, So));
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MemoryMapArray()
  {
    munmap(data_void_, data().num_elements() * sizeof(T) + offset_);
    close(fd_);
  }

//-----------------------------------------------------------------------
/// Synchronize the file with the memory map.
//-----------------------------------------------------------------------

  void flush() const
  {
    int status = msync(data_void_, 
		       data_->num_elements() * sizeof(T) + offset_, MS_SYNC);
    if(status)
      throw Exception("Msync call failed");
  }

//-----------------------------------------------------------------------
/// File data that is memory mapped to binary file.
//-----------------------------------------------------------------------

  const boost::multi_array_ref<T, D>& data() const {return *data_;}
  
//-----------------------------------------------------------------------
/// File data that is memory mapped to binary file.
//-----------------------------------------------------------------------

  boost::multi_array_ref<T, D>& data() {return *data_;}

  const std::string& file_name() const {return fname;}
 private:
  int fd_;			///< File descriptor of opened file.
  mutable boost::scoped_ptr<boost::multi_array_ref<T, D> > data_;
                                ///< Data.
  offset_type offset_;		// Offset from data_void_ to data_.
  void* data_void_;		// Underlying data.
  std::string fname;
};
}
#endif
