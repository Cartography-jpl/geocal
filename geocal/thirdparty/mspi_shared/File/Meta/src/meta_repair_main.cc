//===========================================================================
//                                                                          
//                            meta_repair_main
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "hdf5_meta.h"		// Definition of Hdf5Meta
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.
#include "H5Cpp.h"		// Definition of H5::H5File
#include <cstring>		// Definition of strcmp

using std::cerr;
using std::cout;
using MSPI::Shared::Hdf5Meta;
using MSPI::Shared::Exception;

//////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

    //-------------------------------------------------------------------
    // Parse command-line arguments
    //-------------------------------------------------------------------

    int error = false;
    std::string message;
    std::string filename;

    if (argc >= 2) {
      filename = argv[1];
    } else {
      error = 1;
      message = "Not enough command-line arguments.";
    }

    //------------------------------------------------------------------
    // Print usage message
    //------------------------------------------------------------------

    if (error) {
      cerr << message << "\n";
      cerr << "usage: meta_repair <hdf file>\n";
      return 1;
    }

    //---------------------------------------------------------------------
    // Repair size of Epoch (UTC).
    //---------------------------------------------------------------------

    H5::H5File file(filename,H5F_ACC_RDWR);
    std::string group = "/HDFEOS/ADDITIONAL/FILE_ATTRIBUTES";
    Hdf5Meta meta = Hdf5Meta::hdf5_import(file.openGroup(group).getLocId());

    if (meta.number_element() > 1) {
      cerr << "Unexpected number of metadata elements. No repair attempted.";
      return 1;
    }

    std::string name = "Epoch (UTC)";
    std::string epoch = meta.get_value<std::string>(name);
    if (epoch.size() > 27) {
      std::string epoch_new(&epoch[0]);
      
      cout << "Replacing string " << epoch << " (size "<< epoch.size() 
	   << ") with string " << epoch_new << " (size "<< epoch_new.size() << ")\n";
      Hdf5Meta meta_new;
      meta_new.add<std::string>(name, epoch_new);
      meta_new.hdf5_export(file.openGroup(group).getLocId());
    }

//-----------------------------------------------------------------------
// Return success status
//-----------------------------------------------------------------------
    
    return 0;

//-----------------------------------------------------------------------
// End try block for main.
//-----------------------------------------------------------------------
    
  }

//-----------------------------------------------------------------------
// Catch exceptions.
//-----------------------------------------------------------------------

  catch(const Exception& e) {
    cerr << "Caught Exception: " << e.what() << "\n"
	 << e.stack_trace() << "\n";
  }
  return 1;
}
