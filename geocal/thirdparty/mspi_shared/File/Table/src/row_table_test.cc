/*===========================================================================
=                                                                           =
=                                  RowTable                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: row_table_test.cc

!ABSTRACT:

   This does unit testing for the class RowTable.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "row_table.h"          // Definition of RowTable
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check

using std::cerr;
using MSPI::Shared::RowTable;
using MSPI::Shared::Exception;
using MSPI::Shared::vector_check;


int main()
{
//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {
    int test_number = 0;	// Number identifying this test. 
    bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Test constructor for creating a new table.
//-----------------------------------------------------------------------

    test_number++;
    hid_t fid = H5Fcreate("File/Table/tmp/row_table_test.h5",
			  H5F_ACC_TRUNC,H5P_DEFAULT, 
			  H5P_DEFAULT);
    RowTable table(fid);

    {
      std::vector<int> expect_empty;
      vector_check(table.row_numbers(), expect_empty, __LINE__, error);
    }
    
//-----------------------------------------------------------------------
// Test add()
// Test row_numbers()
//-----------------------------------------------------------------------

    test_number++;
    const size_t number_row = 4;
    static const int row_numbers_init[number_row] = {3,2,1,5};
    static const float wavelength_expect[number_row] = {10.1, 10.2, 10.3, 10.4};
    static const float polarization_angle_expect[number_row] = {0.0, 45.0, -9999.0, 45.0};
    static const char *swath_name_expect[number_row] = 
      {"Swath A", "Swath B", "Swath C", "Swath D"};

    for (size_t irow = 0 ; irow < number_row ; irow++) {
      table.add(row_numbers_init[irow], wavelength_expect[irow],
		polarization_angle_expect[irow], swath_name_expect[irow]);
    }

    std::vector<int> row_numbers_expect(&row_numbers_init[0],
					&row_numbers_init[number_row]);
    std::sort(row_numbers_expect.begin(), row_numbers_expect.end());

    vector_check(table.row_numbers(), row_numbers_expect, __LINE__, error);

//-----------------------------------------------------------------------
// Test wavelength()
// Test polarization_angle()
// Test swath_name()
//-----------------------------------------------------------------------

    test_number++;
    for (size_t irow = 0 ; irow < number_row ; irow++) {
      float wavelength = table.wavelength(row_numbers_init[irow]);
      float polarization_angle = table.polarization_angle(row_numbers_init[irow]);
      std::string swath_name = table.swath_name(row_numbers_init[irow]);
      
      if (wavelength != wavelength_expect[irow]) {
	cerr << "wavelength("<<row_numbers_init[irow]<<")="<< wavelength
	     << " (expected "<<wavelength_expect[irow]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      if (polarization_angle != polarization_angle_expect[irow]) {
	cerr << "polarization_angle("<<row_numbers_init[irow]<<")="<< polarization_angle
	     << " (expected "<<polarization_angle_expect[irow]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      if (strcmp(swath_name.c_str(),swath_name_expect[irow])) {
	cerr << "swath_name("<<row_numbers_init[irow]<<")="<< swath_name
	     << " (expected "<<swath_name_expect[irow]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test swath_name() with invalid row.
//-----------------------------------------------------------------------

    test_number++;
    bool caught_exception = false;
    try {
      std::string swath_name3 = table.swath_name(8);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for row: 8")) {
	cerr << "swath_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "swath_name() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test wavelength() with invalid row.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      table.wavelength(8);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for row: 8")) {
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
// Test polarization_angle() with invalid row.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      table.polarization_angle(8);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for row: 8")) {
	cerr << "polarization_angle() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "polarization_angle() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

    H5Fclose(fid);

//-----------------------------------------------------------------------
// Open existing file.
//-----------------------------------------------------------------------

    hid_t fid2 = H5Fopen("File/Table/tmp/row_table_test.h5",
			 H5F_ACC_RDWR,H5P_DEFAULT);

//-----------------------------------------------------------------------
// Test constructor for opening an existing table.
//-----------------------------------------------------------------------

    test_number++;
    RowTable table2(fid2);

    vector_check(table2.row_numbers(), row_numbers_expect, __LINE__, error);
    
//-----------------------------------------------------------------------
// Test wavelength()
// Test polarization_angle()
// Test swath_name()
//-----------------------------------------------------------------------

    test_number++;
    for (size_t irow = 0 ; irow < number_row ; irow++) {
      float wavelength = table2.wavelength(row_numbers_init[irow]);
      float polarization_angle = table2.polarization_angle(row_numbers_init[irow]);
      std::string swath_name = table2.swath_name(row_numbers_init[irow]);
      
      if (wavelength != wavelength_expect[irow] ||
	  polarization_angle != polarization_angle_expect[irow] ||
	  strcmp(swath_name.c_str(),swath_name_expect[irow])) {
	cerr << "swath_name("<<row_numbers_init[irow]<<")="<< swath_name
	     << " (expected "<<swath_name_expect[irow]<<")\n";
	cerr << "wavelength("<<row_numbers_init[irow]<<")="<< wavelength
	     << " (expected "<<wavelength_expect[irow]<<")\n";
	cerr << "polarization_angle("<<row_numbers_init[irow]<<")="<< polarization_angle
	     << " (expected "<<polarization_angle_expect[irow]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add() with a row that already exists.
//-----------------------------------------------------------------------

    caught_exception = false;
    try {
      table2.add(3, 5.0, 45.0, "Swath F");
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Row number already in table: 3")) {
	cerr << "swath_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

    H5Fclose(fid2);

//-----------------------------------------------------------------------
// Open existing file, read-only
//-----------------------------------------------------------------------

    hid_t fid3 = H5Fopen("File/Table/tmp/row_table_test.h5",
			 H5F_ACC_RDONLY,H5P_DEFAULT);

//-----------------------------------------------------------------------
// Test constructor for opening an existing table, read-only
//-----------------------------------------------------------------------

    test_number++;
    RowTable table3(fid3);

    vector_check(table3.row_numbers(), row_numbers_expect, __LINE__, error);
    
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

    caught_exception = false;
    try {
      table3.add(8, 5.0, 45.0, "Swath F");
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Trouble with H5TBappend_records, row number: 8")) {
	cerr << "swath_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Re-enable HDF5 error printing.
//-----------------------------------------------------------------------

    H5Eset_auto(H5E_DEFAULT,func, client_data);

//-----------------------------------------------------------------------
// Close file.
//-----------------------------------------------------------------------

    H5Fclose(fid3);

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
