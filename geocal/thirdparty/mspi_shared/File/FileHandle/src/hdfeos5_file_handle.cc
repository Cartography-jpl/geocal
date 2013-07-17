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

#include "hdfeos5_file_handle.h" 	// Definition of class
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Hdfeos5FileHandle::Hdfeos5FileHandle(
  const std::string& Filename,
  uintn Access_mode
  ///< Supported modes are H5F_ACC_TRUNC, H5F_ACC_RDONLY, H5F_ACC_RDWR
)
  : filename_(Filename),
    hdfeos_fid_(FAIL),
    hdfeos_group_id_(FAIL),
    file_attr_group_id_(-1),
    read_only_(Access_mode == H5F_ACC_RDONLY)
{

//---------------------------------------------------------------------------
// Create/open file.
//---------------------------------------------------------------------------

  hdfeos_fid_ = HE5_EHopen(filename_.c_str(), Access_mode, H5P_DEFAULT);
  if (hdfeos_fid_ == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with EHopen, file: " + filename_);
  }

//---------------------------------------------------------------------------
// Get native HDF5 file identifiers.
//---------------------------------------------------------------------------

  hid_t hdf_fid;
  herr_t status = HE5_EHidinfo(hdfeos_fid_, &hdf_fid, &hdfeos_group_id_);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with EHidinfo, file: " + filename_);
  }

//---------------------------------------------------------------------------
// Get native HDF5 group identifier for accessing file attributes.
//---------------------------------------------------------------------------

  file_attr_group_id_ = 
    H5Gopen(hdfeos_group_id_,"ADDITIONAL/FILE_ATTRIBUTES",H5P_DEFAULT);
  if (file_attr_group_id_ < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Gopen, file: " + filename_);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Return swath identifier from H5E_SWattach
/////////////////////////////////////////////////////////////////////////////

hid_t
MSPI::Shared::Hdfeos5FileHandle::swath_id(const std::string& Swath_name) const
{
  
  std::map<std::string, hid_t>::const_iterator i = swath_id_.find(Swath_name);
  if (i != swath_id_.end()) {
    return i->second;
  }

  hid_t sid = HE5_SWattach(hdfeos_fid_, Swath_name.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + Swath_name);
  }

  swath_id_[Swath_name] = sid;
  return sid;
}

/////////////////////////////////////////////////////////////////////////////
/// Close file handles.  No further access to the file is permitted.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::Hdfeos5FileHandle::close()
{

//---------------------------------------------------------------------------
// Close swath handles.
//---------------------------------------------------------------------------

  typedef std::map<std::string, hid_t>::const_iterator it_type;
  
  for (it_type i = swath_id_.begin() ; i != swath_id_.end() ; ++i) {
    herr_t status = HE5_SWdetach(i->second);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + i->first);
    }
  }
  swath_id_.clear();

//---------------------------------------------------------------------------
// Close file attribute group.
//---------------------------------------------------------------------------

  if (file_attr_group_id_ > -1) {
    herr_t status = H5Gclose(file_attr_group_id_);
    if (status < 0) {
      throw MSPI_EXCEPTIONm("Trouble with H5Gclose, file: " + filename_);
    }
  }

//---------------------------------------------------------------------------
// Close file.
//---------------------------------------------------------------------------

  if (hdfeos_fid_ != FAIL) {
    herr_t status = HE5_EHclose(hdfeos_fid_);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with GDclose, file: " + filename_);
    }
  }

//---------------------------------------------------------------------------
// Reset private attributes.
//---------------------------------------------------------------------------

  hdfeos_fid_ = FAIL;
  hdfeos_group_id_ = FAIL;
  file_attr_group_id_ = -1;
}
