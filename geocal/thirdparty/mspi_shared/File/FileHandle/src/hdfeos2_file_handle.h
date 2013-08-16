//==========================================================================
//
//                       Hdfeos2FileHandle
//
//==========================================================================
//
//           Copyright 2012, California Institute of Technology.
//                             ALL RIGHTS RESERVED.
//                  U.S. Government Sponsorship acknowledged.
//
//==========================================================================

#ifndef HDFEOS2_FILE_HANDLE_H
#define HDFEOS2_FILE_HANDLE_H

#include "hdf.h"                  // Definition of int32
#include <string>		// Definition of std::string

namespace MSPI {
  namespace Shared {
    class Hdfeos2FileHandle;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Manages file handles for accessing MSPI files.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::Hdfeos2FileHandle {
public:
  Hdfeos2FileHandle(const std::string& Filename, 
		    uintn Access_mode);
  const std::string& filename() const;
  int32 sid() const;
  int32 hdfeos_fid() const;
  int32 hdf_fid() const;
  bool read_only() const;
  int32 file_attr_group_id() const;
  void clear_meta();
  void close();
  ~Hdfeos2FileHandle();
  
protected:

private:
  static int32 create_group(int32 Fid, const std::string& Name);
  std::string filename_;
  int32 hdfeos_fid_;
  int32 hdf_fid_;
  int32 sid_;
  int32 file_attr_group_id_;
  bool read_only_;

  // Disallow copy and assignment
  Hdfeos2FileHandle(const Hdfeos2FileHandle&);
  Hdfeos2FileHandle& operator=(const Hdfeos2FileHandle&);
};

/////////////////////////////////////////////////////////////////////////////
/// Return vgroup identifier for file attributes.
/////////////////////////////////////////////////////////////////////////////

inline int32
MSPI::Shared::Hdfeos2FileHandle::file_attr_group_id() const
{
  return file_attr_group_id_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return read-only indicator.
/////////////////////////////////////////////////////////////////////////////

inline bool
MSPI::Shared::Hdfeos2FileHandle::read_only() const
{
  return read_only_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return filename.
/////////////////////////////////////////////////////////////////////////////

inline const std::string&
MSPI::Shared::Hdfeos2FileHandle::filename() const
{
  return filename_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns HDF-EOS group identifier.
/////////////////////////////////////////////////////////////////////////////

inline int32
MSPI::Shared::Hdfeos2FileHandle::sid() const
{
  return sid_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns HDF-EOS file identifier.
/////////////////////////////////////////////////////////////////////////////

inline int32
MSPI::Shared::Hdfeos2FileHandle::hdfeos_fid() const
{
  return hdfeos_fid_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns native HDF5 identifier for group containing HDF-EOS file attributes.
/////////////////////////////////////////////////////////////////////////////

inline int32
MSPI::Shared::Hdfeos2FileHandle::hdf_fid() const
{
  return hdf_fid_;
}

/////////////////////////////////////////////////////////////////////////////
/// Destructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::Hdfeos2FileHandle::~Hdfeos2FileHandle()
{
  close();
}

#endif
