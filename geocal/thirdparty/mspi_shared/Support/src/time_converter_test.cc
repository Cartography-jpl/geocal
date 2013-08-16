/*===========================================================================
=                                                                           =
=                              TimeConverter                                =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "time_converter.h" // Definition of TimeConverter
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h"
				// Definition of Exception.
#include <cstring>		// Definition of strcmp

using std::cerr;
using MSPI::Shared::TimeConverter;
using MSPI::Shared::Exception;
using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::gregorian::Jan;
using boost::posix_time::time_duration;

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
// Test constructor
//-----------------------------------------------------------------------

    ptime epoch(date(1601,Jan,1), time_duration(0,0,0,0));
    signed long long rate = 10000000LL;

    TimeConverter tc(epoch, rate);

//-----------------------------------------------------------------------
// Test file_time_to_utc
//-----------------------------------------------------------------------

    test_number++;
    {

      signed long long hundred_nanosecond = 1; // one tick per hundred nanoseconds.
      signed long long microsecond = hundred_nanosecond * 10;
      signed long long millisecond = microsecond * 1000;
      signed long long second = millisecond * 1000;
      signed long long minute = second * 60;
      signed long long hour = minute * 60;

      signed long long ft0 = 0LL; // 1601-Jan-01
      signed long long ft1 = 0x19fef81c6cc0000LL; // 1972-Jan-01
      signed long long ft2 = ft1 + 
	(1 * hour + 2 * minute + 3 * second + 
	 4 * millisecond + 5 * microsecond + 
	 4 * hundred_nanosecond);

      signed long long ft3 = ft2 + 2 * hundred_nanosecond;

      char utc0[28];
      char utc1[28];
      char utc2[28];
      char utc3[28];

      tc.file_time_to_utc(ft0, utc0);
      tc.file_time_to_utc(ft1, utc1);
      tc.file_time_to_utc(ft2, utc2);
      tc.file_time_to_utc(ft3, utc3);

      if (strcmp(utc0,"1601-01-01T00:00:00Z") ||
	  strcmp(utc1,"1972-01-01T00:00:00Z") ||
	  strcmp(utc2,"1972-01-01T01:02:03.004005Z") ||
	  strcmp(utc3,"1972-01-01T01:02:03.004006Z")) {
	cerr << "utc0 = " << utc0 << "\n";
	cerr << "utc1 = " << utc1 << "\n";
	cerr << "utc2 = " << utc2 << "\n";
	cerr << "utc3 = " << utc3 << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test with 80 Mhz clock rate.
//-----------------------------------------------------------------------

    test_number++;
    {
      signed long long rate = 80000000LL;
      TimeConverter tc(epoch, rate);
      
      signed long long hundred_nanosecond = 8;  // 8 ticks per hundred nanoseconds.
      signed long long microsecond = hundred_nanosecond * 10;
      signed long long millisecond = microsecond * 1000;
      signed long long second = millisecond * 1000;
      signed long long minute = second * 60;
      signed long long hour = minute * 60;

      signed long long ft0 = 0LL; // 1601-Jan-01
      signed long long ft1 = 0xcff7c0e36600000LL; // 1972-Jan-01
      signed long long ft2 = ft1 + 
	(1 * hour + 2 * minute + 3 * second + 
	 4 * millisecond + 5 * microsecond + 
	 4 * hundred_nanosecond);

      signed long long ft3 = ft2 + 2 * hundred_nanosecond;

      signed long long ft4 = ft1 + 
	(3 * hour + 2 * minute + 3 * second + 
	 4 * millisecond + 3 * microsecond + 
	 5 * hundred_nanosecond);

      signed long long ft5 = ft1 + 
	(3 * hour + 2 * minute + 3 * second + 
	 4 * millisecond + 2 * microsecond + 
	 5 * hundred_nanosecond);

      char utc0[28];
      char utc1[28];
      char utc2[28];
      char utc3[28];
      char utc4[28]; 
      char utc5[28]; 

      tc.file_time_to_utc(ft0, utc0);
      tc.file_time_to_utc(ft1, utc1);
      tc.file_time_to_utc(ft2, utc2);
      tc.file_time_to_utc(ft3, utc3);
      tc.file_time_to_utc(ft4, utc4);
      tc.file_time_to_utc(ft5, utc5);

      if (strcmp(utc0,"1601-01-01T00:00:00Z") ||
	  strcmp(utc1,"1972-01-01T00:00:00Z") ||
	  strcmp(utc2,"1972-01-01T01:02:03.004005Z") ||
	  strcmp(utc3,"1972-01-01T01:02:03.004006Z") ||
	  strcmp(utc4,"1972-01-01T03:02:03.004004Z") ||
	  strcmp(utc5,"1972-01-01T03:02:03.004002Z")) {
	cerr << "utc0 = " << utc0 << "\n";
	cerr << "utc1 = " << utc1 << "\n";
	cerr << "utc2 = " << utc2 << "\n";
	cerr << "utc3 = " << utc3 << "\n";
	cerr << "utc4 = " << utc4 << "\n";
	cerr << "utc5 = " << utc5 << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test constructor(std::string, ...)
//-----------------------------------------------------------------------

    test_number++;

    {
      std::string epoch = "1601-01-01T00:00:00.100001Z";
      TimeConverter tc(epoch, rate);

      signed long long ft0 = 0LL;

      char utc0[28];
      tc.file_time_to_utc(ft0, utc0);

      if (strcmp(utc0,"1601-01-01T00:00:00.100001Z")) {
	cerr << "utc0 = " << utc0 << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Argument check: Rate < 1
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;

      try {
	TimeConverter tc(epoch, 0);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Rate < 1")) {
	  cerr << "exception = " << e.what() << "\n";
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
// Argument check: Insufficient clock resolution
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;
      signed long long rate = 100000LL;

      try {
	TimeConverter tc(epoch, rate);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Clock must have at least 1 microsecond resolution.")) {
	  cerr << "exception = " << e.what() << "\n";
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

