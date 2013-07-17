/*===========================================================================
=                                                                           =
=                                Exception                                  =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: exception_test.cc

!ABSTRACT:

   This does unit testing for the class Exception.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "exception.h"          // Definition of Exception
#include <iostream>		// Definition of cerr.

using std::cerr;
using MSPI::Shared::Exception;


int main()
{
//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {
    int test_number = 0;	// Number identifying this test. 
    bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Test constructor.
// Test what(), where(), stack_trace()
//-----------------------------------------------------------------------

    test_number++;
    {
      std::string what_expect = "Exception test message 1";
      std::string where_expect = "Location not available";

      Exception exception(what_expect);

      std::string what(exception.what());
      if (what != what_expect) {
	cerr << "what = "<< what << "(expected "<<what_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      std::string where(exception.where());
      if (where != where_expect) {
	cerr << "where = "<< where << " (expected "<<where_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      std::string stack_trace(exception.stack_trace());
      if (stack_trace.substr(0,43) != 
	  "MSPI::Shared::Exception::stack_trace_init()") {
	cerr << "stack_trace = '"<< stack_trace <<"'\n";;
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test Exception(std::string, std::string, int)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::string what_expect = "Exception test message 2";
      std::string where_expect = "Line 100 in This_filename";

      int line_number = 100;
      Exception exception(what_expect, "This_filename", line_number);

      std::string what(exception.what());
      if (what != what_expect) {
	cerr << "what = "<< what << "(expected "<<what_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      
      std::string where(exception.where());
      if (where != where_expect) {
	cerr << "where = "<< where << "(expected "<<where_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      std::string stack_trace(exception.stack_trace());
      if (stack_trace.substr(0,43) != 
	  "MSPI::Shared::Exception::stack_trace_init()") {
	cerr << "stack_trace = '"<< stack_trace <<"'\n";;
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Try catching Exception class.
//-----------------------------------------------------------------------

    test_number++;

    std::string message2_expect = "Exception test message (2)";

    try {
      throw Exception(message2_expect);
    } catch(Exception& e) {

      std::string message2(e.what());
      if (message2 != message2_expect) {
	cerr << "message2 = "<< message2 << "(expected "<<message2_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Try catching std::exception class.
//-----------------------------------------------------------------------

    test_number++;

    std::string message3_expect = "Exception test message (3)";
    try {
      throw Exception(message3_expect);
    } catch(std::exception& e) {

      std::string message3(e.what());
      if (message3 != message3_expect) {
	cerr << "message3 = "<< message3 << "(expected "<<message3_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }


//-----------------------------------------------------------------------
// Test MSPI_EXCEPTIONm(message)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::string what = "Exception test message (4)";

      try {
	throw MSPI_EXCEPTIONm(what);
      } catch(Exception& e) {
	
	std::string what(e.what());
	if (what != "Exception test message (4)") {
	  cerr << "what = '"<< what << "'\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}

	std::string where(e.where());
	if (where != "Line 174 in ErrorHandling/src/exception_test.cc") {
	  cerr << "where = '"<< where << "'\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}

	std::string stack_trace(e.stack_trace());
	if (stack_trace.substr(0,43) != 
	    "MSPI::Shared::Exception::stack_trace_init()") {
	  cerr << "stack_trace = '"<< stack_trace <<"'\n";;
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
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

  catch(int h) {
    cerr << "Exception code = " << h << "\n";
  }
}
