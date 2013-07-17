//===========================================================================
//                                                                          
//                               Hdfeos5FileHandle                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "hdfeos5_file_handle.h" // Definition of Hdfeos5FileHandle
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.

using std::cerr;
using MSPI::Shared::Hdfeos5FileHandle;


//-----------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor.
// Test read_only()
// Test swath_attach()
//-----------------------------------------------------------------------

    test_number++;

    std::string filename = "File/FileHandle/tmp/hdfeos5_file_handle_test.h5";
    {
      Hdfeos5FileHandle file(filename, H5F_ACC_TRUNC);
      hsize_t count = 1;
      int value = 5;
      herr_t status = HE5_EHwriteglbattr(file.hdfeos_fid(), "number_banana",
					 H5T_NATIVE_INT, &count, &value);
      if (status == FAIL) {
	cerr << "Trouble with HE5_EHwriteglbattr\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      if (file.read_only()) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      std::string swath_name = "TestSwath";
      {
	hid_t sid = HE5_SWcreate(file.hdfeos_fid(), swath_name.c_str());
	if (sid == FAIL) {
	  throw MSPI_EXCEPTIONm("Trouble with SWcreate, swath: " + swath_name);
	}
	herr_t status = HE5_SWdetach(sid);
	if (status) {
	  throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name);
	}
      }

      hid_t swath_id = file.swath_id(swath_name);
      if (swath_id == FAIL) {
	cerr << "Trouble with swath_id\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Save error reporting configuration.
//-----------------------------------------------------------------------

    hid_t error_stack = H5E_DEFAULT;
    herr_t (*old_func)(hid_t, void*);
    void *old_data;
    H5Eget_auto(error_stack, &old_func, &old_data);

//-----------------------------------------------------------------------
// Disable HDF-EOS error printing for subsequent tests.
//-----------------------------------------------------------------------

    HE5_EHset_error_on(2,0);

//-----------------------------------------------------------------------
// Test write on read-only file handle.
// Test read_only()
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdfeos5FileHandle file(filename, H5F_ACC_RDONLY);
      hsize_t count = 1;
      int value = 5;
      herr_t status = HE5_EHwriteglbattr(file.hdfeos_fid(), "number_banana",
					 H5T_NATIVE_INT, &count, &value);
      if (status != FAIL) {
	cerr << "Allowed write request to read-only file.\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      if (!file.read_only()) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Re-enable HDF-EOS error printing.
//----------------------------------------------------------------------- 

    HE5_EHset_error_on(0,0);

//-----------------------------------------------------------------------
// Try creating the same file twice.  This seems to be an effective way
// to test that the destructor has closed all open file handles.
//-----------------------------------------------------------------------

    test_number++;
    
    std::string filename2 = "File/FileHandle/tmp/hdfeos5_file_handle_test2.h5";
    {
      Hdfeos5FileHandle file(filename2, H5F_ACC_TRUNC);
    }
    {
      Hdfeos5FileHandle file(filename2, H5F_ACC_TRUNC);
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

  catch(const MSPI::Shared::Exception& e) {
    cerr << "Caught MSPI::Shared::Exception: " << e.what() << "\n"
	 << "test_number = " << test_number <<"\n";
  }
}
