/*===========================================================================
=                                                                           =
=                                L1B2FieldMap                               =
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

   This does unit testing for the class L1B2FieldMap.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "hdf4_l1b2_field_map.h"   // Definition of HDF4::L1B2FieldMap
#include "MSPI-Shared/File/hdf4/L1B2File/src/l1b2_file.h"
				// Definition of L1B2File
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <string.h>
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check
#include "MSPI-Shared/File/FieldMap/src/configured_field_map.h"
				// Definition of ConfiguredFieldMap

using std::cerr;
using MSPI::Shared::HDF4::L1B2FieldMap;
using MSPI::Shared::HDF4::L1B2File;
using MSPI::Shared::Exception;
using MSPI::Shared::FieldMap;
using MSPI::Shared::ProjectionInformation;
using MSPI::Shared::MapInformation;
using MSPI::Shared::vector_check;
using MSPI::Shared::ConfiguredFieldMap;


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

    std::string filename = "File/FieldMap/tmp/hdf4_l1b2_field_map_test.hdf";
    {
      ConfiguredFieldMap 
	simple_field_map("File/FieldMap/src/hdf4_l1b2_field_map_test.field_map");

      L1B2File file1(filename, MSPI::Shared::HDF4::L1B2File::CREATE,
		     simple_field_map);

      float wavelength = 500;
      double e0 = 5001.1;
      std::vector<double> proj_param(ProjectionInformation::number_proj_param,0.0);
      int proj_code = 1;
      int zonecode = 11;
      int sphere_code = 8;
      ProjectionInformation proj_info(proj_code, sphere_code, zonecode, proj_param);
      
      MapInformation map_info(1.1, 1.2, 1.3, 1.4, 2*64, 3*64, 
			      MapInformation::ORIGIN_LR, 
			      MapInformation::PIX_REG_CORNER); 
      
      L1B2File::DimensionOrder dimension_order = L1B2File::YMAJOR;
      file1.add_band(63,wavelength, e0, proj_info, map_info, dimension_order);
      file1.add_band(57,wavelength+1, e0, proj_info, map_info, dimension_order);
      file1.add_band(52,wavelength+2, e0, proj_info, map_info, dimension_order);
    }

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    L1B2FieldMap field_map(filename);
    
//-----------------------------------------------------------------------
// Test grid_numbers()
//-----------------------------------------------------------------------

    test_number++;

    {
      const int number_grid = 3;
      int grid_numbers_expect0[number_grid] = { 52, 57, 63 };
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
	{ {"A","R","C"},
	  {"E","",""},
	  {"F","H",""}
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
// Test type(int, size_t)
//-----------------------------------------------------------------------

    {
      std::string type = field_map.type(63, 1);

      if (type != "int") {
	cerr << "type = " << type << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test fill(int, size_t)
//-----------------------------------------------------------------------

    {
      float fill = field_map.fill<int>(63, 1);

      if (fill != 5555) {
	cerr << "fill = " << fill << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
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
	 << "test_number = " << test_number <<"\n"
	 << "Stack trace:\n" << e.stack_trace() << "\n";
  }
}