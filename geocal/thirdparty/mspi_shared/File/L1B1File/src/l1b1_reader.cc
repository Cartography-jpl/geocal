/*===========================================================================
=                                                                           =
=                             L1B1Reader                                    =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                                                        
            Copyright 2012, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

=============================================================================
!END=======================================================================*/

#include "l1b1_reader.h" // Definition of class
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.

/////////////////////////////////////////////////////////////////////////////
/// Constructor.  Create a new file.  Overwrite exisitng file.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::L1B1Reader::L1B1Reader(
  const std::string& Filename
) 
{

  //------------------------------------------------------------------------
  // Determine file type.  If it's not HDF5, assume it is HDF4.
  //------------------------------------------------------------------------

  if (H5Fis_hdf5(Filename.c_str())) {
    is_hdf4_ = false;
  } else {
    is_hdf4_ = true;
  }

  //------------------------------------------------------------------------
  // Open the file.
  //------------------------------------------------------------------------

  if (is_hdf4_) {
    h4_ = boost::shared_ptr<MSPI::Shared::HDF4::L1B1File>
      (new MSPI::Shared::HDF4::L1B1File(Filename));
  } else {
    h5_ = boost::shared_ptr<MSPI::Shared::L1B1File>
      (new MSPI::Shared::L1B1File(Filename));
  }
}
