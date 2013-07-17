//==========================================================================
//
//                       Hdfeos5FileHandle
//
//==========================================================================
//
//           Copyright 2012, California Institute of Technology.
//                             ALL RIGHTS RESERVED.
//                  U.S. Government Sponsorship acknowledged.
//
//==========================================================================

#ifndef HDFEOS5_FILE_HANDLE_H
#define HDFEOS5_FILE_HANDLE_H

#include <HE5_HdfEosDef.h>	// Definition of hid_t.
#include <string>		// Definition of std::string
#include <map>			// Definition of std::map

namespace MSPI {
  namespace Shared {
    class Hdfeos5FileHandle;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Manages file handles for accessing MSPI files.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::Hdfeos5FileHandle {
public:
  Hdfeos5FileHandle(const std::string& Filename, 
		    uintn Access_mode);
  const std::string& filename() const;
  hid_t file_attr_group_id() const;
  hid_t hdfeos_fid() const;
  hid_t hdfeos_group_id() const;
  bool read_only() const;
  hid_t swath_id(const std::string& Swath_name) const;
  void close();
  ~Hdfeos5FileHandle();
  
protected:

private:
  std::string filename_;
  hid_t hdfeos_fid_;
  hid_t hdfeos_group_id_;
  hid_t file_attr_group_id_;
  bool read_only_;

  // Disallow copy and assignment
  Hdfeos5FileHandle(const Hdfeos5FileHandle&);
  Hdfeos5FileHandle& operator=(const Hdfeos5FileHandle&);
  mutable std::map<std::string, hid_t> swath_id_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return read-only indicator.
/////////////////////////////////////////////////////////////////////////////

inline bool
MSPI::Shared::Hdfeos5FileHandle::read_only() const
{
  return read_only_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return filename.
/////////////////////////////////////////////////////////////////////////////

inline const std::string&
MSPI::Shared::Hdfeos5FileHandle::filename() const
{
  return filename_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns HDF-EOS group identifier.
/////////////////////////////////////////////////////////////////////////////

inline hid_t
MSPI::Shared::Hdfeos5FileHandle::hdfeos_group_id() const
{
  return hdfeos_group_id_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns HDF-EOS file identifier.
/////////////////////////////////////////////////////////////////////////////

inline hid_t
MSPI::Shared::Hdfeos5FileHandle::hdfeos_fid() const
{
  return hdfeos_fid_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns native HDF5 identifier for group containing HDF-EOS file attributes.
/////////////////////////////////////////////////////////////////////////////

inline hid_t
MSPI::Shared::Hdfeos5FileHandle::file_attr_group_id() const
{
  return file_attr_group_id_;
}

/////////////////////////////////////////////////////////////////////////////
/// Destructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::Hdfeos5FileHandle::~Hdfeos5FileHandle()
{
  close();
}

#endif
