/*===========================================================================
=                                                                           =
=                                AutoRemoveTempDir                          =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: auto_remove_temp_dir_test.cc

!ABSTRACT:

   This does unit testing for the class AutoRemoveTempDir.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "auto_remove_temp_dir.h"           
				   // Definition of class to be tested.
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.
#include <fstream> 		// Definition of std::ofstream
#include <sys/stat.h>		// Definition of stat
#include <stdlib.h>

using std::cerr;
using MSPI::Shared::AutoRemoveTempDir;
using MSPI::Shared::Exception;


int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constuctor
// Test dirname()
//-----------------------------------------------------------------------

    test_number++;

    std::string tmp_path = "Support/tmp";

    AutoRemoveTempDir temp(tmp_path,"auto_remove_temp_dir_test");

    {
      struct stat sb;
      if (stat(temp.dirname().c_str(), &sb) != 0) {
	cerr << "trouble with stat()\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    std::string filename = temp.dirname() + "/testfile";
    std::ofstream file(filename.c_str());
    if (file.fail()) {
      cerr << "trouble opening file " << filename << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test destructor.
//-----------------------------------------------------------------------

    test_number++;

    std::string temp_dirname;
    std::string temp_filename;

    {
      std::string tmp_path = "Support/tmp";

      AutoRemoveTempDir temp2(tmp_path,"auto_remove_temp_dir_test2");

      temp_dirname = temp2.dirname();
      temp_filename = temp2.dirname() + "/testfile2";
      std::ofstream file(temp_filename.c_str());
      if (file.fail()) {
	cerr << "trouble opening file " << filename << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {
      struct stat sb;
      if (stat(temp_filename.c_str(), &sb) == 0) {
	cerr << temp_filename << " file not removed\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      if (stat(temp_dirname.c_str(), &sb) == 0) {
	cerr << temp_dirname << " directory not removed\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test set_persistent()
//-----------------------------------------------------------------------

    test_number++;

    {
      std::string temp_dirname;
      std::string temp_filename;

      {
	std::string tmp_path = "Support/tmp";
	
	AutoRemoveTempDir temp(tmp_path,"auto_remove_temp_dir_test3");
	temp.set_persistent();
	
	temp_dirname = temp.dirname();
	temp_filename = temp.dirname() + "/testfile3";
	std::ofstream file(temp_filename.c_str());
	if (file.fail()) {
	  cerr << "trouble opening file " << filename << "\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      
      {
	struct stat sb;
	if (stat(temp_filename.c_str(), &sb) != 0) {
	  cerr << temp_filename << " file removed\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	
	if (stat(temp_dirname.c_str(), &sb) != 0) {
	  cerr << temp_dirname << " directory removed\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test constructor with persistent = true
//-----------------------------------------------------------------------

    test_number++;

    std::string persistent_temp_dirname;

    {
      std::string temp_filename;
      std::string temp_dirname;

      {
	std::string tmp_path = "Support/tmp";
	
	AutoRemoveTempDir temp(tmp_path,"auto_remove_temp_dir_test4", 1);
	
	temp_dirname = temp.dirname();
	persistent_temp_dirname = temp_dirname;
	temp_filename = temp.dirname() + "/testfile4";
	std::ofstream file(temp_filename.c_str());
	if (file.fail()) {
	  cerr << "trouble opening file " << filename << "\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      
      {
	struct stat sb;
	if (stat(temp_filename.c_str(), &sb) != 0) {
	  cerr << temp_filename << " file removed\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	
	if (stat(temp_dirname.c_str(), &sb) != 0) {
	  cerr << temp_dirname << " directory removed\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test constructor(dirname,1)
//-----------------------------------------------------------------------

    test_number++;

    {
      std::string temp_dirname;

      {
	AutoRemoveTempDir temp(persistent_temp_dirname,1);
	
	temp_dirname = temp.dirname();

	if (temp_dirname != persistent_temp_dirname) {
	  cerr << "dirname = " << temp_dirname
	       << " (expected " << persistent_temp_dirname << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      {
	struct stat sb;
	if (stat(temp_dirname.c_str(), &sb) != 0) {
	  cerr << temp_dirname << " directory removed\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test constructor(dirname)
//-----------------------------------------------------------------------

    test_number++;

    {
      std::string temp_dirname;

      {
	AutoRemoveTempDir temp(persistent_temp_dirname);
	
	temp_dirname = temp.dirname();

	if (temp_dirname != persistent_temp_dirname) {
	  cerr << "dirname = " << temp_dirname
	       << " (expected " << persistent_temp_dirname << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      {
	struct stat sb;
	if (stat(temp_dirname.c_str(), &sb) == 0) {
	  cerr << temp_dirname << " directory not removed\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Check for test failures.
//-----------------------------------------------------------------------

    if (error) {
      cerr << "Error! One or more tests failed.\n";
    } else {
      cerr << "All tests succeeded.\n";
    }

//-----------------------------------------------------------------------
// End try block for main.
//-----------------------------------------------------------------------
    
  }

//-----------------------------------------------------------------------
// Catch exceptions.
//-----------------------------------------------------------------------

  catch(const Exception& e) {
    cerr << "Caught Exception: " << e.what() << "\n";
  }
}
