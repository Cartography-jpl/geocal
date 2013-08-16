/*===========================================================================
=                                                                           =
=                                L1B1FieldMap                               =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: field_map_test.cc

!ABSTRACT:

   This does unit testing for the class L1B1FieldMap.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "hdf5_l1b1_field_map.h"   // Definition of HDF5::L1B1FieldMap
#include "MSPI-Shared/File/L1B1File/src/l1b1_file.h"
				// Definition of L1B1File
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <string.h>
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check

using std::cerr;
using MSPI::Shared::HDF5::L1B1FieldMap;
using MSPI::Shared::L1B1File;
using MSPI::Shared::Exception;
using MSPI::Shared::FieldMap;
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
// Generate test input file
//-----------------------------------------------------------------------

    std::string filename = "File/FieldMap/tmp/hdf5_l1b1_field_map_test.hdf";
    {
      FieldMap simple_field_map;
      simple_field_map.init_grid(63, "rowC", 3, "A","R","C");
      simple_field_map.init_grid(57, "rowA", 1, "E");
      simple_field_map.init_grid(52, "rowB", 2, "F","H");
      L1B1File file1(filename, MSPI::Shared::L1B1File::CREATE,
		     simple_field_map);

      float wavelength = 500.1;
      float polarization_angle = 1.2;
      int max_number_frame = 10000;
      
      file1.add_row(63,wavelength, polarization_angle, max_number_frame);
      file1.add_row(57,wavelength, polarization_angle, max_number_frame);
      file1.add_row(52,wavelength, polarization_angle, max_number_frame);
    }

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    L1B1FieldMap field_map(filename);
    
//-----------------------------------------------------------------------
// Test grid_numbers()
//-----------------------------------------------------------------------

    test_number++;

    {
      const int number_row = 3;
      int row_numbers_expect0[number_row] = { 52, 57, 63 };
      std::vector<int> row_numbers_expect(&(row_numbers_expect0[0]),
					&(row_numbers_expect0[number_row]));
      
      std::vector<int> row_numbers = field_map.grid_numbers();

      vector_check(row_numbers, row_numbers_expect, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test field_names()
// Test field_names(int,int)
//-----------------------------------------------------------------------

    test_number++;

    { 
      const int number_row = 3;
      const size_t number_field[number_row] = {4,2,3};
      const int row_number[number_row] = {63,57,52};
      const char *field_names_expect[number_row][4] = 
	{ {"A","R","C","Time"},
	  {"E","Time","",""},
	  {"F","H","Time",""}
	};

      for (int irow = 0 ; irow < number_row; ++irow) {
	  std::vector<std::string> field_names = 
	    field_map.field_names(row_number[irow]);
	  if (field_names.size() != number_field[irow]) {
	    cerr << irow << ":field_names.size() =" <<  field_names.size()
		 << " (expected "<< number_field[irow]<<")\n";
	    cerr << "Unexpected result at line "<< __LINE__ << "\n";
	    error = true;
	  }

	for (size_t ifield = 0 ; ifield < number_field[irow]; ++ifield) {
	  std::string field_name = field_map.field_name(row_number[irow],ifield);
	  
	  if (strcmp(field_name.c_str(),field_names_expect[irow][ifield])) {
	    cerr << irow << "," << ifield << ": fieldname = "<<field_name 
		 << " (expected "<<field_names_expect[irow][ifield]<<")\n";
	    cerr << "Unexpected result at line "<< __LINE__ << "\n";
	    error = true;
	  }
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
    cerr << "Caught Exception: " << e.what() << "\n"
	 << "test_number = " << test_number <<"\n";
  }
}
