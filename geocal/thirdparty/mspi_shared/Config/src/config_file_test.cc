/*===========================================================================
=                                                                           =
=                                ConfigFile                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MSPI
                                  Config File

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: config_file_test.cc

!ABSTRACT:

   This does unit testing for the class ConfigFile.

!DESCRIPTION:

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "config_file.h"	// Definition of ConfigFile
#include <iostream>		// Definition of cerr.
#include <iomanip> 		// Definition of std::setprecision
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <string.h> 		// Definition of strcmp
#include "MSPI-Shared/UnitTest/src/check.h"
				// Definition of array_check

using std::cerr;
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
    std::string filename = "Config/src/config_file_test.txt";

    ConfigFile config_file(filename);

//-----------------------------------------------------------------------
// Test is_defined()
//-----------------------------------------------------------------------

    test_number++;

    if (config_file.is_defined("undefined parameter")) {
      cerr << "Unexpected result for is_defined\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

    if (!config_file.is_defined("parameter1")) {
      cerr << "Unexpected result for is_defined\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

    if (!config_file.is_defined("parameter2")) {
      cerr << "Unexpected result for is_defined\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test size()
//-----------------------------------------------------------------------

    test_number++;

    {
      int size_expect = 1;
      int size = config_file.size("parameter1");
      
      if (size != size_expect) {
	cerr << "size = "<<size<<" (expected "<<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {
      int size_expect = 2;
      int size = config_file.size("parameter2");
      
      if (size != size_expect) {
	cerr << "size = "<<size<<" (expected "<<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test get_values<float>()
//-----------------------------------------------------------------------

    test_number++;

    {
      const size_t size_expect = 1;
      float value_expect[size_expect] = { 1.1 };
      std::vector<float> value = config_file.get_values<float>("parameter1");
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

    {
      const size_t size_expect = 2;
      float value_expect[size_expect] = { 2.1, -3.1 };
      std::vector<float> value = config_file.get_values<float>("parameter2");
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get_values<double>()
//-----------------------------------------------------------------------

    test_number++;

    {
      const size_t size_expect = 1;
      double value_expect[size_expect] = { 1.00000001 };
      std::vector<double> value = config_file.get_values<double>("parameter6");
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr <<std::setprecision(20) << "value["<<i<<"] = "<<value[i]<<" (expected "
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
      const size_t size_expect = 3;
      int value_expect[size_expect] = { 4,-5,6 };
      std::vector<int> value = config_file.get_values<int>("parameter3");
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get_values<signed long long>()
//-----------------------------------------------------------------------

    test_number++;

    {
      const size_t size_expect = 3;
      signed long long value_expect[size_expect] = { 18446744073709551615ULL, 0, -2 };
      std::vector<signed long long> value = config_file.get_values<signed long long>("signedlonglong");
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
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
      const size_t size_expect = 12;
      bool value_expect[size_expect] = { true,false,true,false,true,false,
					 true,false,true,false,true,false};
      std::vector<bool> value = config_file.get_values<bool>("parameter4");
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
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
      const size_t size_expect = 3;
      const char *value_expect[size_expect] = { "alpha","beta","gamma" };
      std::vector<std::string> value = 
	config_file.get_values<std::string>("parameter7");
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
	if (value[i] != std::string(value_expect[i])) {
	  cerr << "value["<<i<<"] = "<<value[i]<<" (expected "
	       <<value_expect[i]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get<T>(const std::string&)
//-----------------------------------------------------------------------

    test_number++;

    {
      float value_expect = 1.1;
      float value = config_file.get<float>("parameter1");
      
      if (value != value_expect) {
	cerr << "value = "<<value<<" (expected "<<value_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add_replace()
//-----------------------------------------------------------------------

    test_number++;

    {
      std::string filename = "Config/src/config_file_test.txt";

      ConfigFile config_file(filename);

      std::string filename2 = "Config/src/config_file_test4.txt";
      config_file.add_replace(filename2);

      {
	float value_expect = 1.1;
	float value = config_file.get<float>("parameter1");
      
	if (value != value_expect) {
	  cerr << "value = "<<value<<" (expected "<<value_expect<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      {
	float value_expect = 5;
	float value = config_file.get<int>("parameter5");
      
	if (value != value_expect) {
	  cerr << "value = "<<value<<" (expected "<<value_expect<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      {
	float value_expect = 7;
	float value = config_file.get<int>("parameter7");
      
	if (value != value_expect) {
	  cerr << "value = "<<value<<" (expected "<<value_expect<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test get<T>(const std::string&,int)
//-----------------------------------------------------------------------

    test_number++;

    {
      const size_t size_expect = 12;
      bool value_expect[size_expect] = { true,false,true,false,true,false,
					 true,false,true,false,true,false};
      std::vector<bool> value = config_file.get<bool>("parameter4",size_expect);
      
      if (value.size() != size_expect) {
	cerr << "value.size() = "<<value.size()<<" (expected "
	     <<size_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (size_t i = 0 ; i < size_expect ; i++) {
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
	std::vector<float> values = config_file.get_values<float>("bad_parameter");
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Configuration parameter not found: 'bad_parameter', filename=Config/src/config_file_test.txt") {
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
// Test with bad input file
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/config_file_test....";

      try {
	ConfigFile config_file(filename);
	
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Trouble opening input file: Config/src/config_file_test....") {
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
	std::vector<bool> values = config_file.get_values<bool>("parameter3");
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Boolean value not recognized for string: '4', filename=Config/src/config_file_test.txt") {
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
// Test with duplicate parameter name.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/config_file_test2.txt";

      try {
	ConfigFile config_file(filename);
	
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Caught exception, [Duplicate config parameter name: parameter1], at line 10 of Config/src/config_file_test2.txt, line='t f true false .true. .false. : parameter1'") {
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
// Test with empty parameter. (generates unexpected token message)
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/config_file_test3.txt";

      try {
	ConfigFile config_file(filename);
	
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Caught exception, [Unexpected token: ':'], at line 8 of Config/src/config_file_test3.txt, line=': parameter3'") {
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
// Test get<T>(const std::string&) with wrong size.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/config_file_test3.txt";

      try {
	config_file.get<bool>("parameter4");
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Parameter 'parameter4' with size 12 does not match requested size of 1") {
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
// Test get<T>(const std::string&,int) with wrong size.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/config_file_test3.txt";
      const int size_expect = 12;

      try {
	std::vector<bool> value = config_file.get<bool>("parameter4",size_expect-1);
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != 
	    "Parameter 'parameter4' with size 12 does not match requested size of 11") {
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
// Test get<T,2>(const std::string&,int,int)
//-----------------------------------------------------------------------

    test_number++;
    {
      const int number_row = 3;
      const int number_col = 4;
      
      boost::multi_array<int,2> 
	array_expect(boost::extents[number_row][number_col]);
      for (int irow = 0 ; irow < number_row ; irow++) {
	for (int icol = 0 ; icol < number_col ; icol++) {
	  array_expect[irow][icol] = 100 * (irow+1) + icol;
	}
      }

      boost::multi_array<int,2> array = 
	config_file.get<int>("array1",number_row, number_col);

      array_check(array, array_expect,__LINE__,error);
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

