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

#include "hdfeos2_file_handle.h" 	// Definition of class
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "HdfEosDef.h"		// Definition of EHopen

/////////////////////////////////////////////////////////////////////////////
/// Create Vgroup
/////////////////////////////////////////////////////////////////////////////

int32
MSPI::Shared::Hdfeos2FileHandle::create_group(
  int32 Fid, 
  const std::string& Name
)
{
  int32 group_id = Vattach(Fid, -1, "w");
  if (group_id == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with Vattach(create)");
  }

  int32 status = Vsetname(group_id, Name.c_str());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with Vsetname");
  }
  return group_id;
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Hdfeos2FileHandle::Hdfeos2FileHandle(
  const std::string& Filename,
  uintn Access_mode
  ///< Supported modes are DFACC_READ, DFACC_RDWR, DFACC_CREATE
)
  : filename_(Filename),
    hdfeos_fid_(FAIL),
    hdf_fid_(FAIL),
    sid_(FAIL),
    file_attr_group_id_(FAIL),
    read_only_(Access_mode == DFACC_READ)
{

//---------------------------------------------------------------------------
// Create/open file.
//---------------------------------------------------------------------------

  hdfeos_fid_ = EHopen((char *)filename_.c_str(), Access_mode);
  if (hdfeos_fid_ == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with EHopen, file: " + filename_);
  }

//---------------------------------------------------------------------------
// Get native HDF5 file identifiers.
//---------------------------------------------------------------------------

  intn status = EHidinfo(hdfeos_fid_, &hdf_fid_, &sid_);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with EHidinfo, file: " + filename_);
  }

//---------------------------------------------------------------------------
// Create new metadata Vgroup
//---------------------------------------------------------------------------

  if (Access_mode == DFACC_CREATE) {
    file_attr_group_id_ = create_group(hdf_fid_, "FILE_ATTRIBUTES");
    
//---------------------------------------------------------------------------
// Open existing metadata Vgroup
//---------------------------------------------------------------------------

  } else {
    int32 vref = Vfind(hdf_fid_,"FILE_ATTRIBUTES");
    if (vref == 0) {
      throw MSPI_EXCEPTIONm("Trouble with Vfind");
    }
    file_attr_group_id_ = Vattach(hdf_fid_, vref, 
				  (Access_mode == DFACC_READ ?  "r" : "w"));
    if (file_attr_group_id_ == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with Vattach(read)");
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Clear metadata.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Hdfeos2FileHandle::clear_meta()
{

  //-------------------------------------------------------------------------
  // Get reference number of metadata Vgroup.
  //-------------------------------------------------------------------------

  int32 vref = VQueryref(file_attr_group_id_);
  if (vref == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VQueryref");
  }

  //-------------------------------------------------------------------------
  // Close metadata Vgroup.
  //-------------------------------------------------------------------------

  {
    int32 status = Vdetach(file_attr_group_id_);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with Vdetach(clear)");
    }
  }
  file_attr_group_id_ = FAIL;

  //-------------------------------------------------------------------------
  // Delete metadata Vgroup.
  //-------------------------------------------------------------------------

  {
    int32 status = Vdelete(hdf_fid_, vref);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with Vdelete");
    }
  }

  //-------------------------------------------------------------------------
  // Create metadata Vgroup.
  //-------------------------------------------------------------------------
  
  file_attr_group_id_ = create_group(hdf_fid_, "FILE_ATTRIBUTES");
}

/////////////////////////////////////////////////////////////////////////////
/// Close file handles.  No further access to the file is permitted.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::Hdfeos2FileHandle::close()
{

//---------------------------------------------------------------------------
// Close vgroup.
//---------------------------------------------------------------------------

  if (file_attr_group_id_ != FAIL) {
    int32 status = Vdetach(file_attr_group_id_);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with Vdetach(close)");
    }
  }

//---------------------------------------------------------------------------
// Close file.
//---------------------------------------------------------------------------

  if (hdfeos_fid_ != FAIL) {
    intn status = EHclose(hdfeos_fid_);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with GDclose, file: " + filename_);
    }
  }

//---------------------------------------------------------------------------
// Reset private attributes.
//---------------------------------------------------------------------------

  hdfeos_fid_ = FAIL;
  hdf_fid_ = -1;
  sid_ = FAIL;
  file_attr_group_id_ = FAIL;
}
