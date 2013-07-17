//===========================================================================
//                                                                         
//                               FieldMap                                  
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "field_map.h"          // Definition of FieldMap
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <string.h>
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check

using std::cerr;
using MSPI::Shared::FieldMap;
using MSPI::Shared::Exception;
using MSPI::Shared::vector_check;


//-----------------------------------------------------------------------
// Generate unit-test expected results.
//-----------------------------------------------------------------------

std::vector<std::string> field_names_init(const std::string& Name)
{
  std::vector<std::string> result;

  if (Name == "Grid10") {
    result.push_back("A");
    result.push_back("B");
    result.push_back("C");
  }

  if (Name == "Grid20") {
    result.push_back("E");
    result.push_back("F");
  }

  if (Name == "GridA") {
    result.push_back("G");
    result.push_back("H");
    result.push_back("I");
  }

  if (Name == "GridB") {
    result.push_back("J");
    result.push_back("K");
  }

  if (Name == "GridC") {
    result.push_back("L");
    result.push_back("M");
    result.push_back("N");
  }

  return result;
}

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
//-----------------------------------------------------------------------

    test_number++;
    FieldMap field_map;
    FieldMap field_map2;

//-----------------------------------------------------------------------
// Test init_grid()
//-----------------------------------------------------------------------

    field_map.init_grid(10, "Grid10", 3, "A","B","C");
    field_map.init_grid(20, "Grid20", 2, "E","F");

    field_map2.init_grid(30,"GridA",field_names_init("GridA"));
    field_map2.init_grid(40,"GridB",field_names_init("GridB"));

    {
      std::vector<std::string> type;
      std::vector<boost::any> fill;

      type.push_back("int");
      fill.push_back(555);

      type.push_back("double");
      fill.push_back(555.1D);

      type.push_back("uint16");
      fill.push_back((unsigned short)6666);

      field_map2.init_grid(50,"GridC",field_names_init("GridC"),type,fill);
    }

//-----------------------------------------------------------------------
// Test grid_numbers()
//-----------------------------------------------------------------------

    test_number++;

    {
      std::vector<int> grid_numbers_expect;
      grid_numbers_expect.push_back(10);
      grid_numbers_expect.push_back(20);

      std::vector<int> grid_numbers = field_map.grid_numbers();

      vector_check(grid_numbers, grid_numbers_expect, __LINE__, error);
    }


    {
      std::vector<int> grid_numbers_expect;
      grid_numbers_expect.push_back(30);
      grid_numbers_expect.push_back(40);
      grid_numbers_expect.push_back(50);

      std::vector<int> grid_numbers = field_map2.grid_numbers();
      
      vector_check(grid_numbers, grid_numbers_expect, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test grid_names()
//-----------------------------------------------------------------------

    test_number++;

    {
      std::vector<std::string> grid_names_expect;
      grid_names_expect.push_back("Grid10");
      grid_names_expect.push_back("Grid20");

      std::vector<std::string> grid_names = field_map.grid_names();

      vector_check(grid_names, grid_names_expect, __LINE__, error);
    }


    {
      std::vector<std::string> grid_names_expect;
      grid_names_expect.push_back("GridA");
      grid_names_expect.push_back("GridB");
      grid_names_expect.push_back("GridC");

      std::vector<std::string> grid_names = field_map2.grid_names();
      
      vector_check(grid_names, grid_names_expect, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test field_names(int)
// Test field_name(int,int)
// Test field_names(std::string)
// Test field_name(std::string,int)
//-----------------------------------------------------------------------

    test_number++;


    {
      std::vector<std::string> field_names10_expect(field_names_init("Grid10"));
      std::vector<std::string> field_names20_expect(field_names_init("Grid20"));

      std::vector<std::string> field_names10 = field_map.field_names(10);
      std::vector<std::string> field_names20 = field_map.field_names(20);

      std::vector<std::string> field_names_grid10 = field_map.field_names("Grid10");
      std::vector<std::string> field_names_grid20 = field_map.field_names("Grid20");
      
      vector_check(field_names10, field_names10_expect,  __LINE__, error);
      vector_check(field_names20, field_names20_expect,  __LINE__, error);

      vector_check(field_names_grid10, field_names10_expect,  __LINE__, error);
      vector_check(field_names_grid20, field_names20_expect,  __LINE__, error);
    }

    {
      std::vector<std::string> field_names30_expect(field_names_init("GridA"));
      std::vector<std::string> field_names40_expect(field_names_init("GridB"));

      std::vector<std::string> field_names30 = field_map2.field_names(30);
      std::vector<std::string> field_names40 = field_map2.field_names(40);

      std::vector<std::string> field_names_gridA = field_map2.field_names("GridA");
      std::vector<std::string> field_names_gridB = field_map2.field_names("GridB");
    
      vector_check(field_names30, field_names30_expect,  __LINE__, error);
      vector_check(field_names40, field_names40_expect,  __LINE__, error);

      vector_check(field_names_gridA, field_names30_expect,  __LINE__, error);
      vector_check(field_names_gridB, field_names40_expect,  __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test grid_name()
//-----------------------------------------------------------------------

    test_number++;

    {
      const char *grid_name10_expect = "Grid10";
      const char *grid_name20_expect = "Grid20";

      std::string grid_name10 = field_map.grid_name(10);
      std::string grid_name20 = field_map.grid_name(20);

      if (strcmp(grid_name10.c_str(), grid_name10_expect) ||
	  strcmp(grid_name20.c_str(), grid_name20_expect)) {
	cerr << "grid_name10 = "<<grid_name10.c_str()
	     << " (expected "<<grid_name10_expect<<")\n";
	cerr << "grid_name20 = "<<grid_name20.c_str()
	     << " (expected "<<grid_name20_expect<<")\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      const char *grid_name30_expect = "GridA";
      const char *grid_name40_expect = "GridB";

      std::string grid_name30 = field_map2.grid_name(30);
      std::string grid_name40 = field_map2.grid_name(40);

      if (strcmp(grid_name30.c_str(), grid_name30_expect) ||
	  strcmp(grid_name40.c_str(), grid_name40_expect)) {
	cerr << "grid_name30 = "<<grid_name30.c_str()
	     << " (expected "<<grid_name30_expect<<")\n";
	cerr << "grid_name40 = "<<grid_name40.c_str()
	     << " (expected "<<grid_name40_expect<<")\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test type(int, size_t)
// Test type(std::string, size_t)
//-----------------------------------------------------------------------

    {
      std::string type1 = field_map.type(20, 0);
      std::string type2 = field_map.type("Grid20", 0);

      if (type1 != "float" ||
          type2 != "float") {
	cerr << "type1 = " << type1 << "\n";
	cerr << "type2 = " << type2 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      std::string type1 = field_map2.type(30, 1);
      std::string type2 = field_map2.type("GridA", 1);

      if (type1 != "float" ||
	  type2 != "float") {
	cerr << "type1 = " << type1 << "\n";
	cerr << "type2 = " << type2 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      std::string type1 = field_map2.type(50, 0);
      std::string type2 = field_map2.type("GridC", 0);

      if (type1 != "int" ||
	  type2 != "int") {
	cerr << "type1 = " << type1 << "\n";
	cerr << "type2 = " << type2 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      std::string type1 = field_map2.type(50, 1);
      std::string type2 = field_map2.type("GridC", 1);
      if (type1 != "double" ||
	  type2 != "double") {
	cerr << "type1 = " << type1 << "\n";
	cerr << "type2 = " << type2 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      std::string type1 = field_map2.type(50, 2);
      std::string type2 = field_map2.type("GridC", 2);
      if (type1 != "uint16" ||
	  type2 != "uint16") {
	cerr << "type1 = " << type1 << "\n";
	cerr << "type2 = " << type2 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test fill(int, size_t)
// Test fill(std::string, size_t)
// Test fill(int, std::string)
// Test fill(std::string, std::string)
//-----------------------------------------------------------------------

    {
      float fill1 = field_map.fill<float>(20, 0);
      float fill2 = field_map.fill<float>("Grid20", 0);
      float fill3 = field_map.fill<float>(20, "E");
      float fill4 = field_map.fill<float>("Grid20", "E");

      if (fill1 != -9999.0 ||
	  fill2 != -9999.0 ||
	  fill3 != -9999.0 ||
	  fill4 != -9999.0) {
	cerr << "fill1 = " << fill1 << "\n";
	cerr << "fill2 = " << fill2 << "\n";
	cerr << "fill3 = " << fill3 << "\n";
	cerr << "fill4 = " << fill4 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      float fill1 = field_map2.fill<float>(30, 1);
      float fill2 = field_map2.fill<float>("GridA", 1);
      float fill3 = field_map2.fill<float>(30, "H");
      float fill4 = field_map2.fill<float>("GridA", "H");

      if (fill1 != -9999.0 ||
	  fill2 != -9999.0 ||
	  fill3 != -9999.0 ||
	  fill4 != -9999.0) {
	cerr << "fill1 = " << fill1 << "\n";
	cerr << "fill2 = " << fill2 << "\n";
	cerr << "fill3 = " << fill3 << "\n";
	cerr << "fill4 = " << fill4 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      int fill1 = field_map2.fill<int>(50, 0);
      int fill2 = field_map2.fill<int>("GridC", 0);
      int fill3 = field_map2.fill<int>(50, "L");
      int fill4 = field_map2.fill<int>("GridC", "L");
      if (fill1 != 555 ||
	  fill2 != 555 ||
	  fill3 != 555 ||
	  fill4 != 555) {
	cerr << "fill1 = " << fill1 << "\n";
	cerr << "fill2 = " << fill2 << "\n";
	cerr << "fill3 = " << fill3 << "\n";
	cerr << "fill4 = " << fill4 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      double fill1 = field_map2.fill<double>(50, 1);
      double fill2 = field_map2.fill<double>("GridC", 1);
      double fill3 = field_map2.fill<double>(50, "M");
      double fill4 = field_map2.fill<double>("GridC", "M");
      if (fill1 != 555.1 ||
	  fill2 != 555.1 ||
	  fill3 != 555.1 ||
	  fill4 != 555.1) {
	cerr << "fill1 = " << fill1 << "\n";
	cerr << "fill2 = " << fill2 << "\n";
	cerr << "fill3 = " << fill3 << "\n";
	cerr << "fill4 = " << fill4 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    {
      unsigned short fill1 = field_map2.fill<unsigned short>(50, 2);
      unsigned short fill2 = field_map2.fill<unsigned short>("GridC", 2);
      unsigned short fill3 = field_map2.fill<unsigned short>(50, "N");
      unsigned short fill4 = field_map2.fill<unsigned short>("GridC", "N");
      if (fill1 != 6666 ||
	  fill2 != 6666 ||
	  fill3 != 6666 ||
	  fill4 != 6666) {
	cerr << "fill1 = " << fill1 << "\n";
	cerr << "fill2 = " << fill2 << "\n";
	cerr << "fill3 = " << fill3 << "\n";
	cerr << "fill4 = " << fill4 << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test grid_name() with invalid grid number.
//-----------------------------------------------------------------------

    test_number++;
    bool caught_exception = false;
    try {
      std::string grid_name = field_map.grid_name(3);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "FieldMap : unexpected grid number: 3")) {
	cerr << "grid_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "grid_name() exception not caught\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Argument check: Field_index < 0
//                 Field_index >= field_names1.size()
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      try {
	std::string grid_name = field_map.field_name(10,-1);
	
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Field index 18446744073709551615 not valid for grid number 10")) {
	  cerr << "grid_name() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "grid_name() exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    test_number++;

    {
      bool caught_exception = false;
      try {
	std::string grid_name = field_map.field_name(10,3);
	
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Field index 3 not valid for grid number 10")) {
	  cerr << "grid_name() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "grid_name() exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Argument check: Field_index < 0
//                 Field_index >= field_names1.size()
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      try {
	std::string grid_name = field_map.field_name("Grid10",-1);
	
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Field index 18446744073709551615 not valid for grid name Grid10")) {
	  cerr << "grid_name() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "grid_name() exception not caught\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

    test_number++;

    {
      bool caught_exception = false;
      try {
	std::string grid_name = field_map.field_name("Grid10",3);
	
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Field index 3 not valid for grid name Grid10")) {
	  cerr << "grid_name() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<< __LINE__ << "\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "grid_name() exception not caught\n";
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
	 << "test_number = " << test_number <<"\n";
  }
}
