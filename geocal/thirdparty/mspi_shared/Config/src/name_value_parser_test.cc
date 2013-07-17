/*===========================================================================
=                                                                           =
=                                NameValueParser                            =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                  Config File

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: name_value_parser_test.cc

!ABSTRACT:

   This does unit testing for the class NameValueParser.

!DESCRIPTION:

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "name_value_parser.h"	// Definition of NameValueParser
#include <iostream>		// Definition of cerr.
#include <iomanip> 		// Definition of std::setprecision
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include <limits> 		// Definition of std::numeric_limits
#include <string.h> 		// Definition of strcmp

using std::cerr;
using MSPI::Shared::NameValueParser;
using MSPI::Shared::Exception;


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

    NameValueParser name_value_parser;

//-----------------------------------------------------------------------
// Test done()
//-----------------------------------------------------------------------

    test_number++;

    {
      bool done_expect = false;

      if (name_value_parser.done() != done_expect) {
	cerr << "done = " << name_value_parser.done()
	     << " (expect " << done_expect << ")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add_token(std::string) with scalar value
// Test done()
// Test name()
// Test value()
//-----------------------------------------------------------------------

    test_number++;

    {   
      const int number_token = 4;
      const char *tokens[number_token] = {
	"100",":","var_a","extra"
      };

      bool done_expect[number_token] = { false, false, true, true };
      bool success_expect[number_token] = {true, true, true, false};

      for (int i = 0 ; i < number_token ; i++) {
	bool success = name_value_parser.add_token(tokens[i]);

	if (success != success_expect[i]) {
	  cerr << "Unexpected return status from add_token\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}

	if (name_value_parser.done() != done_expect[i]) {
	  cerr << "done = " << name_value_parser.done()
	       << " (expect " << done_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      std::string name_expect = "var_a";
      const size_t value_size_expect = 1;
      const char *value_expect[value_size_expect] = { "100" };

      std::vector<std::string> value = name_value_parser.value();
      if (value.size() != value_size_expect) {
	cerr << "value.size() = " << value.size()
	     << " (expect " << value_size_expect << ")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      
      for (size_t i = 0 ; i < value_size_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = " << value[i]
	       << " (expect " << value_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test reset()
//-----------------------------------------------------------------------

    test_number++;

    {   
      name_value_parser.reset();

      bool done_expect = false;
      
      if (name_value_parser.done() != done_expect) {
	cerr << "done = " << name_value_parser.done()
	     << " (expect " << done_expect << ")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add_token(std::string) with array value
// Test done()
// Test name()
// Test value()
//-----------------------------------------------------------------------

    test_number++;

    {   
      const int number_token = 5;
      const char *tokens[number_token] = {
	"200","300","400",":","var_b"
      };

      bool done_expect[number_token] = { false, false, false, false, true };

      for (int i = 0 ; i < number_token ; i++) {
	bool success = name_value_parser.add_token(tokens[i]);

	if (!success) {
	  cerr << "Unexpected return status from add_token\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}

	if (name_value_parser.done() != done_expect[i]) {
	  cerr << "done = " << name_value_parser.done()
	       << " (expect " << done_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      std::string name_expect = "var_b";
      const size_t value_size_expect = 3;
      const char *value_expect[value_size_expect] = { "200", "300", "400" };

      std::vector<std::string> value = name_value_parser.value();
      if (value.size() != value_size_expect) {
	cerr << "value.size() = " << value.size()
	     << " (expect " << value_size_expect << ")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      
      for (size_t i = 0 ; i < value_size_expect ; i++) {
	if (value[i] != value_expect[i]) {
	  cerr << "value["<<i<<"] = " << value[i]
	       << " (expect " << value_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test name() with incomplete parsing
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/badfilename";
      NameValueParser name_value_parser;

      try {
	std::string name = name_value_parser.name();
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != "Parsing not done (name)") {
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
// Test value() with incomplete parsing
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      std::string filename = "Config/src/badfilename";
      NameValueParser name_value_parser;

      try {
	std::vector<std::string> value = name_value_parser.value();
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != "Parsing not done (value)") {
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
// Test add_token() with invalid token sequence.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;
      NameValueParser name_value_parser;

      try {
	name_value_parser.add_token(":");
      } catch (Exception& e) {
	caught_exception = true;
	if (std::string(e.what()) != "Unexpected token: ':'") {
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

