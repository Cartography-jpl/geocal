/*===========================================================================
=                                                                           =
=                                ConfiguredFieldMap                                   =
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

   This does unit testing for the class ConfiguredFieldMap.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "configured_field_map.h"          // Definition of ConfiguredFieldMap
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <string.h>
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check

using std::cerr;
using MSPI::Shared::ConfiguredFieldMap;
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
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    std::string config_filename = "File/FieldMap/src/configured_field_map_test.config";
    ConfiguredFieldMap field_map(config_filename);

//-----------------------------------------------------------------------
// Test granule_id()
//-----------------------------------------------------------------------

    test_number++;
    {
      if (field_map.granule_id() != 
	  "granule_MSPI-Shared/File/FieldMap/src/configured_field_map_test.config") {
	cerr << "granule_id = "<< field_map.granule_id();
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test grid_numbers()
//-----------------------------------------------------------------------

    test_number++;

    {
      const int number_grid = 3;
      int grid_numbers_expect0[number_grid] = { 63, 57, 52 };
      std::vector<int> grid_numbers_expect(&(grid_numbers_expect0[0]),
					   &(grid_numbers_expect0[number_grid]));
      
      std::vector<int> grid_numbers = field_map.grid_numbers();

      vector_check(grid_numbers, grid_numbers_expect, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test field_names()
// Test field_names(int,int)
//-----------------------------------------------------------------------

    test_number++;

    { 
      const int number_grid = 3;
      const size_t number_field[number_grid] = {3,1,2};
      const int grid_number[number_grid] = {63,57,52};
      const char *field_names_expect[number_grid][3] = 
	{ {"A","B","C"},
	  {"E","",""},
	  {"F","G",""}
	};

      for (int igrid = 0 ; igrid < number_grid; ++igrid) {
	  std::vector<std::string> field_names = 
	    field_map.field_names(grid_number[igrid]);
	  if (field_names.size() != number_field[igrid]) {
	    cerr << igrid << ":field_names.size() =" <<  field_names.size()
		 << " (expected "<< number_field[igrid]<<")\n";
	    cerr << "Unexpected result at line "<< __LINE__ << "\n";
	    error = true;
	  }

	for (size_t ifield = 0 ; ifield < number_field[igrid]; ++ifield) {
	  std::string field_name = field_map.field_name(grid_number[igrid],ifield);
	  
	  if (strcmp(field_name.c_str(),field_names_expect[igrid][ifield])) {
	    cerr << igrid << "," << ifield << ": fieldname = "<<field_name 
		 << " (expected "<<field_names_expect[igrid][ifield]<<")\n";
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
