//===========================================================================
//                                                                          
//                              ScaledDouble                                
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//=============================================================================

#include "scaled_double.h" 	// Definition of ScaledDouble
#include <iostream> 		// Definition of std::cerr
#include <cmath> 		// Definition of std::pow
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.
#include "MSPI-Shared/UnitTest/src/check.h"
				// Definition of vector_check
#include "MSPI-Shared/UnitTest/src/double_comparator.h"
				// Definition of DoubleComparator
#include <cstring> 		// Definition of strcmp

using std::cerr;
using MSPI::Shared::ScaledDouble;
using MSPI::Shared::Exception;
using MSPI::Shared::DoubleComparator;
using MSPI::Shared::vector_check;

/////////////////////////////////////////////////////////////////////////
// Generate unit-test input
/////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------
std::vector<double> sample_data_init()
{
  std::vector<double> a;
  a.push_back(-1.0);
  for (int i = 0 ; i < 10 ; i++) {
    a.push_back(std::pow(8,i));
  }
  return a;
}

/////////////////////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////////////////////

int main()
{
  int test_number = 0;	// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor
//-----------------------------------------------------------------------

    test_number++;

    
    ScaledDouble s(sample_data_init());

//-----------------------------------------------------------------------
// Test min(), max()
//-----------------------------------------------------------------------

    {
      test_number++;

      if (s.min() != -1.0) {
	cerr << "min = " << s.min() << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      if (s.max() != std::pow(8,9)) {
	cerr << "max = " << s.max() << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test scale(double), unscale(double)
//-----------------------------------------------------------------------

    {
      test_number++;
      DoubleComparator cmp;

      double value = 1552.7;

      if (s.scale(value) != -2147433930) {
	cerr << "scaled = " << s.scale(value) << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      int scaled_value = 60000;
      if (cmp.neq(s.unscale(scaled_value),6.71107385156394094229e+07)) {
	cerr << "unscaled = " << s.unscale(scaled_value) << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      scaled_value = -60000;
      if (cmp.neq(s.unscale(scaled_value),6.71069885156105905771e+07)) {
	cerr << "unscaled = " << s.unscale(scaled_value) << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      scaled_value = s.scale(s.min());
      if (scaled_value != -2147483648) {
	cerr << "scaled = " << s.scale(s.min()) << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      scaled_value = s.scale(s.max());
      if (scaled_value != 2147483647) {
	cerr << "scaled = " << s.scale(s.max()) << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test scale(vector)
//-----------------------------------------------------------------------

    {
      test_number++;
      
      std::vector<int> scaled = s.scale(sample_data_init());

      int expect_init[11] = {
	-2147483648,
	-2147483584,
	-2147483360,
	-2147481568,
	-2147467232,
	-2147352544,
	-2146435040,
	-2139095008,
	-2080374753,
	-1610612708,
	2147483647
      };
      
      std::vector<int> scaled_expect(&expect_init[0], &expect_init[11]);
      vector_check(scaled, scaled_expect, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test unscale(vector)
//-----------------------------------------------------------------------

    {
      test_number++;
      DoubleComparator cmp(1e8);
      
      std::vector<double> unscaled = s.unscale(s.scale(sample_data_init()));
      vector_check(unscaled, sample_data_init(), cmp, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test with values outside the valid range.
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;
 
      try {
	s.scale(s.min()-1.0);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(),"ScaledDouble::scale: exceeded min = -1, value = -2, delta = -1")) {
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

    test_number++;
    {
      bool caught_exception = false;

      try {
	s.scale(s.max()+1.0);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(),"ScaledDouble::scale: exceeded max = 1.34218e+08, value = 1.34218e+08, delta = 1")) {
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
    cerr << "Caught Exception: " << e.what() << "\n";
  }
}
