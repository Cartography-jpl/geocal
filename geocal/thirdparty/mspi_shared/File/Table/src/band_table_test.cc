//===========================================================================
//                                                                          
//                               BandTable                                  
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "band_table.h"          // Definition of BandTable
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check
#include <H5Cpp.h>		// Definition of H5::H5File

using std::cerr;
using MSPI::Shared::BandTable;
using MSPI::Shared::Exception;
using MSPI::Shared::vector_check;

//-----------------------------------------------------------------------
// Generate unit-test input
//-----------------------------------------------------------------------

const int number_band = 4;

//-----------------------------------------------------------------------
std::vector<int> band_numbers_init()
{
  int data[number_band] = {3,2,1,5};
  return std::vector<int>(&data[0], &data[number_band]);
}

//-----------------------------------------------------------------------
std::vector<float> wavelength_init()
{
  float data[number_band] = {10.1, 10.2, 10.3, 10.4};
  return std::vector<float>(&data[0], &data[number_band]);
}

//-----------------------------------------------------------------------
std::vector<double> e0_init()
{
  float data[number_band] = {101.1, 101.2, 101.3, 101.4};
  return std::vector<double>(&data[0], &data[number_band]);
}

//-----------------------------------------------------------------------
std::vector<std::string> grid_name_init()
{
  const char *data[number_band] = {"Grid A", "Grid B", "Grid C", "Grid D"};
  return std::vector<std::string>(&data[0], &data[number_band]);
}

//-----------------------------------------------------------------------
std::vector<int> band_numbers_expect()
{
  std::vector<int> band_numbers(band_numbers_init());
  std::sort(band_numbers.begin(), band_numbers.end());
  return band_numbers;
}

//-----------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------

int main()
{
//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {
    int test_number = 0;	// Number identifying this test. 
    bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Create and open file.
//-----------------------------------------------------------------------

    std::string filename = "File/Table/tmp/band_table_test.h5";
    {
      H5::H5File file(filename, H5F_ACC_TRUNC);
      H5::Group group(file.openGroup("/"));

//-----------------------------------------------------------------------
// Test constructor for creating a new table.
//-----------------------------------------------------------------------

      test_number++;

      BandTable table(group.getLocId());

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
      std::vector<int> band_numbers(band_numbers_init());
      std::vector<float> wavelength_expect(wavelength_init());
      std::vector<double> e0_expect(e0_init());
      std::vector<std::string> grid_name_expect(grid_name_init());
      for (int iband = 0 ; iband < number_band ; iband++) {
	table.add(band_numbers[iband], wavelength_expect[iband],
		  e0_expect[iband], grid_name_expect[iband]);
      }

      vector_check(table.band_numbers(), band_numbers_expect(), 
		   __LINE__, error);

//-----------------------------------------------------------------------
// Test wavelength()
// Test E0()
// Test grid_name()
//-----------------------------------------------------------------------

      test_number++;

      for (int iband = 0 ; iband < number_band ; iband++) {
	float wavelength = table.wavelength(band_numbers[iband]);
	double e0 = table.E0(band_numbers[iband]);
	std::string grid_name = table.grid_name(band_numbers[iband]);

	if (wavelength != wavelength_expect[iband]) {
	  cerr << "wavelength("<<band_numbers[iband]<<")="<< wavelength
	       << " (expected "<<wavelength_expect[iband]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (e0 != e0_expect[iband]) {
	  cerr << "E0("<<band_numbers[iband]<<")="<< e0
	       << " (expected "<<e0_expect[iband]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (grid_name != grid_name_expect[iband]) {
	  cerr << "grid_name("<<band_numbers[iband]<<")="<< grid_name
	       << " (expected "<<grid_name_expect[iband]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

//-----------------------------------------------------------------------
// Test grid_name() with invalid band.
//-----------------------------------------------------------------------

      {
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
      }

//-----------------------------------------------------------------------
// Test wavelength() with invalid band.
//-----------------------------------------------------------------------

      {
	test_number++;
	bool caught_exception = false;
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
      }

//-----------------------------------------------------------------------
// Test E0() with invalid band.
//-----------------------------------------------------------------------

      {
	test_number++;
	bool caught_exception = false;
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
      }

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

  }

//-----------------------------------------------------------------------
// Open existing file.
//-----------------------------------------------------------------------

    {
      H5::H5File file(filename, H5F_ACC_RDWR);
      H5::Group group(file.openGroup("/"));

//-----------------------------------------------------------------------
// Test constructor for opening an existing table.
//-----------------------------------------------------------------------

      test_number++;
      BandTable table2(group.getLocId());

      vector_check(table2.band_numbers(), band_numbers_expect(), 
		   __LINE__, error);
    
//-----------------------------------------------------------------------
// Test wavelength()
// Test e0()
// Test grid_name()
//-----------------------------------------------------------------------

      test_number++;
      std::vector<int> band_numbers(band_numbers_init());
      std::vector<float> wavelength_expect(wavelength_init());
      std::vector<double> e0_expect(e0_init());
      std::vector<std::string> grid_name_expect(grid_name_init());

      for (int iband = 0 ; iband < number_band ; iband++) {
	float wavelength = table2.wavelength(band_numbers[iband]);
	double e0 = table2.E0(band_numbers[iband]);
	std::string grid_name = table2.grid_name(band_numbers[iband]);

	if (wavelength != wavelength_expect[iband] ||
	    e0 != e0_expect[iband] ||
	    grid_name != grid_name_expect[iband]) {
	  cerr << "grid_name("<<band_numbers[iband]<<")="<< grid_name
	       << " (expected "<<grid_name_expect[iband]<<")\n";
	  cerr << "wavelength("<<band_numbers[iband]<<")="<< wavelength
	       << " (expected "<<wavelength_expect[iband]<<")\n";
	  cerr << "E0("<<band_numbers[iband]<<")="<< e0
	       << " (expected "<<e0_expect[iband]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

//-----------------------------------------------------------------------
// Test add() with a band that already exists.
//-----------------------------------------------------------------------

      {
	bool caught_exception = false;
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
      }

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

    }

//-----------------------------------------------------------------------
// Open existing file (read-only).
//-----------------------------------------------------------------------

    {
      H5::H5File file(filename, H5F_ACC_RDONLY);
      H5::Group group(file.openGroup("/"));

//-----------------------------------------------------------------------
// Test constructor for opening an existing table, read-only
//-----------------------------------------------------------------------

      test_number++;
      BandTable table3(group.getLocId());

      vector_check(table3.band_numbers(), band_numbers_expect(), 
		   __LINE__, error);
    
//-----------------------------------------------------------------------
// Disable HDF5 error printing for subsequent tests.
//-----------------------------------------------------------------------

      H5E_auto_t func;
      void *client_data;
      H5Eget_auto(H5E_DEFAULT,&func, &client_data);
      H5Eset_auto(H5E_DEFAULT,NULL,NULL);

//-----------------------------------------------------------------------
// Test add() on a read-only file.
//-----------------------------------------------------------------------

      {
	bool caught_exception = false;
	try {
	  table3.add(8, 5.0, 51.0, "Grid F");
	} catch (Exception& e) {
	  caught_exception = true;
	  if (strcmp(e.what(), "Trouble with H5TBappend_records, band number: 8")) {
	    cerr << "grid_name() exception = "<<e.what()<<"\n";
	    cerr << "Unexpected result at line "<<__LINE__<<"\n";
	    error = true;
	  }
	}
      }

//-----------------------------------------------------------------------
// Re-enable HDF5 error printing.
//-----------------------------------------------------------------------

      H5Eset_auto(H5E_DEFAULT,func, client_data);

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

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
