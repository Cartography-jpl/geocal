/*===========================================================================
=                                                                           =
=                                DoubleComparator                            =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI::Shared
                                  Config File

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: diagnostic_reporter_test.cc

!ABSTRACT:

   This does unit testing for the class DoubleComparator.

!DESCRIPTION:

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "double_comparator.h" // Definition of DoubleComparator
#include <iostream>		// Definition of cerr.
#include "ErrorHandling/src/exception.h"
				// Definition of Exceptions.
#include <sstream> 		// Definition of std::ostringstream


using std::cerr;
using MSPI::Shared::DoubleComparator;
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

    DoubleComparator cmp;
    DoubleComparator cmp2(10);
    DoubleComparator cmp3(1,1e-5);

//-----------------------------------------------------------------------
// Test neq(double,double) with equal values.
//-----------------------------------------------------------------------

    test_number++;
    {
      double x = 1.0;
      double y = x;

      if (cmp.neq(x,y)) { // expect equal
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test neq(double,double) with unequal values, exceeding relative limit
//-----------------------------------------------------------------------

    test_number++;
    {
      double x = 1.0;
      double y = x + 2.0 * std::numeric_limits<double>::epsilon();

      std::ostringstream out; 

      if (!cmp.neq(x,y,out)) { // expect not equal
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      if (out.str().substr(0,16) != "exceeded rlimit:") {
	cerr << "out.str().substr(0,16) = '" << out.str().substr(0,16) <<"'\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test neq(double,double) with unequal values, exceeding absolute limit
//-----------------------------------------------------------------------

    test_number++;
    {
      double x = 1.0;
      double y = x + 2.0 * std::numeric_limits<double>::epsilon();

      DoubleComparator cmp(0,1.0);

      std::ostringstream out; 

      if (!cmp.neq(x,y,out)) { // expect not equal
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
      if (out.str().substr(0,16) != "exceeded alimit:") {
	cerr << "out.str().substr(0,16) = '" << out.str().substr(0,16) <<"'\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test neq(double,double) with relaxed relative limit
//-----------------------------------------------------------------------

    test_number++;
    {
      double x = 1.0;
      double y = x + 2.0 * std::numeric_limits<double>::epsilon();

      DoubleComparator cmp(10);

      if (cmp.neq(x,y)) { // expect equal
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test neq(double,double) with relaxed absolute limit
//-----------------------------------------------------------------------

    test_number++;
    {
      double x = 1.0;
      double y = x + 2.0 * std::numeric_limits<double>::epsilon();

      DoubleComparator cmp(0,10);

      if (cmp.neq(x,y)) { // expect equal
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test neq(double,double) with nan 
//-----------------------------------------------------------------------

    test_number++;
    {
      double zero = 0.0;
      double x = 0.0/zero;
      double y = x + 2.0 * std::numeric_limits<double>::epsilon();

      DoubleComparator cmp(0,10);

      {
	std::ostringstream out; 
	if (!cmp.neq(x,y,out)) { // expect not equal (nan,real)
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (out.str() != "exceeded rlimit: abs(x-y)=nan  limit=nan\n") {
	  cerr << "out.str() = '" << out.str() <<"'\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
	
      {
	std::ostringstream out; 
	if (!cmp.neq(y,x,out)) { // expect not equal (real,nan)
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (out.str() != "exceeded rlimit: abs(x-y)=nan  limit=nan\n") {
	  cerr << "out.str() = '" << out.str() <<"'\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      {
	std::ostringstream out; 
	if (!cmp.neq(x,x,out)) { // expect not equal (nan,nan)
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (out.str() != "exceeded rlimit: abs(x-y)=nan  limit=nan\n") {
	  cerr << "out.str() = '" << out.str() <<"'\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test neq(double,double) with inf
//-----------------------------------------------------------------------

    test_number++;
    {
      double x = std::numeric_limits<double>::max();
      double y = x * 20.0;

      DoubleComparator cmp(0,10);

      {
	std::ostringstream out; 
	if (!cmp.neq(x,y,out)) { // expect not equal (inf,real)
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (out.str().substr(0,29) != "exceeded alimit: abs(x-y)=inf") {
	  cerr << "out.str() = '" << out.str().substr(0,29) <<"'\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      {
	std::ostringstream out; 
	if (!cmp.neq(y,x,out)) { // expect not equal (real,inf)
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (out.str().substr(0,29) != "exceeded rlimit: abs(x-y)=inf") {
	  cerr << "out.str() = '" << out.str().substr(0,29) <<"'\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      {
	std::ostringstream out; 
	if (!cmp.neq(y,y,out)) { // expect not equal (inf,inf)
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (out.str().substr(0,29) != "exceeded rlimit: abs(x-y)=nan") {
	  cerr << "out.str() = '" << out.str() <<"'\n";
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

  catch(const Exception& e) {
    cerr << "Caught Exception: " << e.what() << "\n"
	 << "test_number = " << test_number <<"\n";
  }
}

