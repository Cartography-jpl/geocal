//===========================================================================
//                                                                          
//                               Hdfeos2FileHandle                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "hdfeos2_file_handle.h" // Definition of Hdfeos2FileHandle
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "mfhdf.h"		// Definition of SDcreate
#include <vector> 		// Definition of std::vector
#include <HdfEosDef.h> 		// Definition of SWcreate

using std::cerr;
using MSPI::Shared::Hdfeos2FileHandle;
using MSPI::Shared::Exception;


//-----------------------------------------------------------------------
// Create SDS array
//-----------------------------------------------------------------------

struct array {
  array(int32 Sid, const std::string& Name) : sdid(FAIL) { 
    std::vector<int32> dims(2,2);
    sdid = SDcreate(Sid, Name.c_str(), DFNT_INT32, 2, &dims[0]);
    if (sdid == FAIL) {
      MSPI_EXCEPTIONm("Trouble with SDcreate\n");
    }
  }
  ~array() {
    if (sdid != FAIL) {
      intn status = SDendaccess(sdid);
      if (status) {
	MSPI_EXCEPTIONm("Trouble with SDendaccess\n");
      }
    }
  }
  int32 sdid;
};

//-----------------------------------------------------------------------
// Create HDFEOS swath
//-----------------------------------------------------------------------

struct swath {
  swath(int32 Fid, const std::string& Name) : swid(FAIL) { 
    swid = SWcreate(Fid, (char *)Name.c_str());
    if (swid == FAIL) {
      MSPI_EXCEPTIONm("Trouble with SWcreate");
    }
  }
  ~swath() {
    if (swid != FAIL) {
      intn status = SWdetach(swid);
      if (status) {
	MSPI_EXCEPTIONm("Trouble with SDendaccess");
      }
    }
  }
  int32 swid;
};


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
//-----------------------------------------------------------------------

    test_number++;

    std::string filename = "File/FileHandle/tmp/hdfeos2_file_handle_test.h4";
    {
      Hdfeos2FileHandle file(filename, DFACC_CREATE);
      std::vector<int32> dims(2,2);

      std::vector<char> namebuf(VGNAMELENMAX+1,0);
      int32 status = Vgetname(file.file_attr_group_id(), &namebuf[0]);
      if (status) {
	MSPI_EXCEPTIONm("Trouble with Vgetname");
      }

      std::string name(&namebuf[0]);
      if (name != "FILE_ATTRIBUTES") {
	cerr << "name = " << name;
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      
      if (file.read_only()) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      array(file.sid(),"Array1");
      swath(file.hdfeos_fid(),"Swath1");
    }

    {
      int number_swath = SWinqswath((char *)filename.c_str(), NULL, NULL);
      if (number_swath != 1) {
	cerr << "number_swath = " << number_swath << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test clear_meta()
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdfeos2FileHandle file(filename, DFACC_RDWR);
      file.clear_meta();
    }

//-----------------------------------------------------------------------
// Test read_only()
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdfeos2FileHandle file(filename, DFACC_READ);

      if (!file.read_only()) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test write on read/write file handle.
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdfeos2FileHandle file(filename, DFACC_RDWR);
      array(file.sid(),"Array2");
      swath(file.hdfeos_fid(),"Swath2");
    }

    {    
      int number_swath = SWinqswath((char *)filename.c_str(), NULL, NULL);
      if (number_swath != 2) {
	cerr << "number_swath = " << number_swath << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test write on read-only file handle.
//-----------------------------------------------------------------------

    test_number++;
    {
      Hdfeos2FileHandle file(filename, DFACC_READ);
      array(file.sid(),"Array3");  // HDF silently fails to write
      swath(file.hdfeos_fid(),"Swath3");  // HDFEOS silently fails to write
    }

    {    
      int number_swath = SWinqswath((char *)filename.c_str(), NULL, NULL);
      if (number_swath != 2) {
	cerr << "number_swath = " << number_swath << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Try creating the same file twice.  This seems to be an effective way
// to test that the destructor has closed all open file handles.
//-----------------------------------------------------------------------

    test_number++;
    
    std::string filename2 = "File/FileHandle/tmp/hdfeos2_file_handle_test2.h4";
    {
      Hdfeos2FileHandle file(filename2, DFACC_CREATE);
    }
    {
      Hdfeos2FileHandle file(filename2, DFACC_CREATE);
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
