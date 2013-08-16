//===========================================================================
//                                                                          
//                               BandTable                                  
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "band_table.h"          // Definition of BandTable
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check

using std::cerr;
using MSPI::Shared::HDF4::BandTable;
using MSPI::Shared::Exception;
using MSPI::Shared::vector_check;


int main()
{

  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor for creating a new table.
//-----------------------------------------------------------------------

    test_number++;
    int32 fid = Hopen("File/Table/tmp/band_table_test.hdf",
		      DFACC_CREATE, DEF_NDDS);

    BandTable table(fid);
      
    {
      std::vector<int> expect_empty;
      vector_check(table.band_numbers(), expect_empty, __LINE__, error);
    }
      
//-----------------------------------------------------------------------
// Test add()
// Test band_numbers()
//-----------------------------------------------------------------------

    test_number++;
    const int number_band = 4;
    static const int band_numbers_init[number_band] = {3,2,1,5};
    static const float wavelength_expect[number_band] = {10.1, 10.2, 10.3, 10.4};
    static const float e0_expect[number_band] = {101.1, 101.2, 101.3, 101.4};
    static const char *grid_name_expect[number_band] = 
      {"Grid A", "Grid B", "Grid C", "Grid D"};

    for (int iband = 0 ; iband < number_band ; iband++) {
      table.add(band_numbers_init[iband], wavelength_expect[iband],
		e0_expect[iband], grid_name_expect[iband]);
    }
    
    std::vector<int> band_numbers_expect(&band_numbers_init[0],
					 &band_numbers_init[number_band]);
    std::sort(band_numbers_expect.begin(), band_numbers_expect.end());

    vector_check(table.band_numbers(), band_numbers_expect, __LINE__, error);

//-----------------------------------------------------------------------
// Test wavelength()
// Test E0()
// Test grid_name()
//-----------------------------------------------------------------------

    test_number++;
    for (int iband = 0 ; iband < number_band ; iband++) {
      float wavelength = table.wavelength(band_numbers_init[iband]);
      double e0 = table.E0(band_numbers_init[iband]);
      std::string grid_name = table.grid_name(band_numbers_init[iband]);
      
      if (wavelength != wavelength_expect[iband]) {
	cerr << "wavelength("<<band_numbers_init[iband]<<")="<< wavelength
	     << " (expected "<<wavelength_expect[iband]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      if (e0 != e0_expect[iband]) {
	cerr << "E0("<<band_numbers_init[iband]<<")="<< e0
	     << " (expected "<<e0_expect[iband]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      if (strcmp(grid_name.c_str(),grid_name_expect[iband])) {
	cerr << "grid_name("<<band_numbers_init[iband]<<")="<< grid_name
	     << " (expected "<<grid_name_expect[iband]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test grid_name() with invalid band.
//-----------------------------------------------------------------------

    test_number++;
    bool caught_exception = false;
    try {
      std::string grid_name3 = table.grid_name(8);
	
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for band: 8")) {
	cerr << "grid_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "grid_name() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }
    
//-----------------------------------------------------------------------
// Test wavelength() with invalid band.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      table.wavelength(8);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for band: 8")) {
	cerr << "wavelength() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "wavelength() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }
    
//-----------------------------------------------------------------------
// Test E0() with invalid band.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      table.E0(8);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for band: 8")) {
	cerr << "exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }
    
//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

    Hclose(fid);

//-----------------------------------------------------------------------
// Open existing file.
//-----------------------------------------------------------------------

    int32 fid2 = Hopen("File/Table/tmp/band_table_test.hdf",
		       DFACC_WRITE, DEF_NDDS);

//-----------------------------------------------------------------------
// Test constructor for opening an existing table.
//-----------------------------------------------------------------------

    test_number++;
    BandTable table2(fid2);

    vector_check(table2.band_numbers(), band_numbers_expect, __LINE__, error);
      
//-----------------------------------------------------------------------
// Test wavelength()
// Test E0()
// Test grid_name()
//-----------------------------------------------------------------------

    test_number++;
    for (int iband = 0 ; iband < number_band ; iband++) {
      float wavelength = table2.wavelength(band_numbers_init[iband]);
      double e0 = table2.E0(band_numbers_init[iband]);
      std::string grid_name = table2.grid_name(band_numbers_init[iband]);
	
      if (wavelength != wavelength_expect[iband] ||
	  e0 != e0_expect[iband] ||
	  strcmp(grid_name.c_str(),grid_name_expect[iband])) {
	cerr << "grid_name("<<band_numbers_init[iband]<<")="<< grid_name
	     << " (expected "<<grid_name_expect[iband]<<")\n";
	cerr << "wavelength("<<band_numbers_init[iband]<<")="<< wavelength
	     << " (expected "<<wavelength_expect[iband]<<")\n";
	cerr << "E0("<<band_numbers_init[iband]<<")="<< e0
	     << " (expected "<<e0_expect[iband]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add() with a band that already exists.
//-----------------------------------------------------------------------

    caught_exception = false;
    try {
      table2.add(3, 5.0, 51.0, "Grid F");
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Band number already in table: 3")) {
	cerr << "grid_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------
    
    Hclose(fid2);

//-----------------------------------------------------------------------
// Open existing file, read-only
//-----------------------------------------------------------------------

    int32 fid3 = Hopen("File/Table/tmp/band_table_test.hdf",
		       DFACC_READ, DEF_NDDS);

//-----------------------------------------------------------------------
// Test constructor for opening an existing table, read-only
//-----------------------------------------------------------------------
    
    test_number++;
    BandTable table3(fid3);

    vector_check(table3.band_numbers(), band_numbers_expect, __LINE__, error);
      
//-----------------------------------------------------------------------
// Disable HDF5 error printing for subsequent tests.
//-----------------------------------------------------------------------

//    H5E_auto_t func;
//    void *client_data;
//    H5Eget_auto(&func, &client_data);
//    H5Eset_auto(NULL,NULL);

//-----------------------------------------------------------------------
// Test add() on a read-only file.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      table3.add(8, 5.0, 51.0, "Grid F");
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Trouble with VSattach(write)")) {
	cerr << "grid_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Re-enable HDF5 error printing.
//-----------------------------------------------------------------------

//    H5Eset_auto(func, client_data);

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

    Hclose(fid3);

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
    cerr << "Caught Exception: " << e.what() << "\n"
	 << "test_number = " << test_number <<"\n";
  }
}
