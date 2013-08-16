/*===========================================================================
=                                                                           =
=                               AutoRemoveTempDir                            =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: AutoRemoveTempDir

!ABSTRACT:

   This contains support routines for the class AutoRemoveTempDir 

!PARENT(S):

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "auto_remove_temp_dir.h"	// Definition of class.
#include <stdio.h>  		// Definition of remove
#include <stdlib.h>  		// Definition of mkdtemp
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <boost/filesystem.hpp> // Definition of boost::filesystem

/////////////////////////////////////////////////////////////////////////////
/// Constuctor that takes an existing directory path.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::AutoRemoveTempDir::AutoRemoveTempDir(
  const std::string& Dirname,
  int Persistent
) 
  : persistent_(Persistent != 0),
    dirname_(Dirname)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Constuctor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::AutoRemoveTempDir::AutoRemoveTempDir(
  const std::string& Tmp_path,
  ///< This directory path will be created if it doesn't already exist.
  const std::string& Prefix,
  int Persistent
) 
  : persistent_(Persistent != 0)
{
  boost::filesystem::create_directories(Tmp_path);
  dirname_ = Tmp_path+"/"+Prefix+"_XXXXXX";
  mkdtemp(&dirname_[0]);
}
