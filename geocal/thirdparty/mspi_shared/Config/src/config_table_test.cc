/*===========================================================================
=                                                                           =
=                                ConfigTable                                =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "config_table.h"	// Definition of ConfigTable
#include <iostream>		// Definition of cerr.
#include <iomanip> 		// Definition of std::setprecision
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <string.h> 		// Definition of strcmp
#include "MSPI-Shared/UnitTest/src/check.h"
				// Definition of array_check

using std::cerr;
using MSPI::Shared::ConfigTable;
using MSPI::Shared::ConfigFile;
using MSPI::Shared::Exception;
using MSPI::Shared::array_check;


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
    std::string filename = "Config/src/config_table_test.txt";

    const size_t number_row_expect = 3;
    const size_t number_column_expect = 5;

    ConfigFile config_file(filename);
    std::string table_name = "monkeys";
    ConfigTable table(config_file, table_name);

//-----------------------------------------------------------------------
// Test is_defined()
//-----------------------------------------------------------------------

    test_number++;

    if (table.is_defined("undefined parameter")) {
      cerr << "Unexpected result for is_defined\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

    if (!table.is_defined("number_banana")) {
      cerr << "Unexpected result for is_defined\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test number_row()
// Test number_column()
//-----------------------------------------------------------------------

    test_number++;

    {
      size_t number_row = table.number_row();
      
      if (number_row != number_row_expect) {
	cerr << "number_row = "<<number_row<<" (expected "<<number_row_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {
      size_t number_column = table.number_column();
      
      if (number_column != number_column_expect) {
	cerr << "number_column = "<<number_column<<" (expected "<<number_column_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test get_values<float>()
//-----------------------------------------------------------------------

    test_number++;

    {
      float value_expect[number_row_expect] = { 5.1, 4.3, 5.2 };
      std::vector<float> value = table.get_values<float>("height");
      
      if (value.size() != number_row_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<number_row_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < number_row_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get_values<int>()
//-----------------------------------------------------------------------

    test_number++;

    {
      int value_expect[number_row_expect] = { 15, 12, 20 };
      std::vector<int> value = table.get_values<int>("number_banana");
      
      if (value.size() != number_row_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<number_row_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < number_row_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get_values<bool>()
//-----------------------------------------------------------------------

    test_number++;

    {
      bool value_expect[number_row_expect] = { true, false, true };

      std::vector<bool> value = table.get_values<bool>("hungry");
      
      if (value.size() != number_row_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<number_row_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < number_row_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get_values<std::string>()
//-----------------------------------------------------------------------

    test_number++;

    {
      const char *value_expect[number_row_expect] = { "Kanzi", "Bobo", "Koko" };
      std::vector<std::string> value = 
	table.get_values<std::string>("name");
      
      if (value.size() != number_row_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<number_row_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < number_row_expect ; i++) {
	if (value[i] != std::string(value_expect[i])) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get_cell<T>(const std::string&, int)
//-----------------------------------------------------------------------

    test_number++;

    {
      float value_expect = 94.6;
      float value = table.get_cell<float>("weight",1);
      
      if (value != value_expect) {
	cerr << "value = "<<value<<" (expected "<<value_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test get<T>(const std::string&,int)
//-----------------------------------------------------------------------

    test_number++;

    {
      bool value_expect[number_row_expect] = { true, false, true};
      std::vector<bool> value = table.get<bool>("hungry",number_row_expect);
      
      if (value.size() != number_row_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<number_row_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < number_row_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get_values<float>() with undefined parameter.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      try {
	std::vector<float> values = table.get_values<float>("bad_parameter");
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Configuration parameter not found: 'bad_parameter'") {
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
// Test get_values<bool>() with unrecognized boolean value.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      try {
	std::vector<bool> values = table.get_values<bool>("name");
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Boolean value not recognized for string: 'Kanzi'") {
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
// Test with duplicate column name.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/config_table_test2.txt";
      ConfigFile config_file(filename);

      try {
	ConfigTable table(config_file, table_name);
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Duplicate column name name in table monkeys") {
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
