/*===========================================================================
=                                                                           =
=                                  FrameInfo                                =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: frame_info_test.cc

!ABSTRACT:

   This does unit testing for the class FrameInfo.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "frame_info.h"         // Definition of FrameInfo
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/double_comparator.h" // Definition of DoubleComparator
#include <cmath>                // Definition of pow().

using std::cerr;
using MSPI::Shared::FrameInfo;
using MSPI::Shared::Exception;
using MSPI::Shared::DoubleComparator;


int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  DoubleComparator cmp(10.0);	// Double comparator

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor FrameInfo(f1, f2).
//-----------------------------------------------------------------------

    test_number++;
    {

      double f1 = 95 * (50000000.0 / pow(2,29));
      double f2 = 34 * (50000000.0 / pow(2,29));
      double T_expect = 1.66471600620155035788e-01;
      double tframe_nominal_expect = 1.76023249836065598384e-01;
      double omega_bar_expect = 3.77432864450927212374e+01;
      double omega_beat_expect = 1.78476005670593451669e+01;
      double tframe_measured_expect = 300.1;
      double timetag_expect = 700.2;

      FrameInfo frame_info(f1, f2, tframe_measured_expect, timetag_expect);

      double T = frame_info.T();
      double tframe_nominal = frame_info.tframe_nominal();
      double omega_bar = frame_info.omega_bar();
      double omega_beat = frame_info.omega_beat();
      double tframe_measured = frame_info.tframe_measured();
      double timetag = frame_info.timetag();

      if (cmp.neq(T, T_expect) ||
	  cmp.neq(tframe_nominal, tframe_nominal_expect) ||
	  cmp.neq(omega_bar, omega_bar_expect) ||
	  cmp.neq(omega_beat, omega_beat_expect) ||
	  cmp.neq(tframe_measured, tframe_measured_expect) ||
	  cmp.neq(timetag, timetag_expect)) {
	cerr << "T result:\n"<< T << "\n"
	     << "T result expect:\n"<< T_expect << "\n";
	cerr << "tframe_nominal result:\n"<< tframe_nominal << "\n"
	     << "tframe_nominal result expect:\n"<< tframe_nominal_expect << "\n";
	cerr << "omega_bar result:\n"<< omega_bar << "\n"
	     << "omega_bar result expect:\n"<< omega_bar_expect << "\n";
	cerr << "omega_beat result:\n"<< omega_beat << "\n"
	     << "omega_beat result expect:\n"<< omega_beat_expect << "\n";
	cerr << "tframe_measured result:\n"<< tframe_measured << "\n"
	     << "tframe_measured result expect:\n"<< tframe_measured_expect << "\n";
	cerr << "timetag result:\n"<< timetag << "\n"
	     << "timetag result expect:\n"<< timetag_expect << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test constructor FrameInfo(double,double,double,double,double,double).
//-----------------------------------------------------------------------

    test_number++;
    {

      double T_expect = 100.2;
      double tframe_nominal_expect = 200.3;
      double omega_bar_expect = 300.4;
      double omega_beat_expect = 400.5;
      double tframe_measured_expect = 500.6;
      double timetag_expect = 600.7;

      FrameInfo frame_info(T_expect, tframe_nominal_expect,
			   omega_bar_expect, omega_beat_expect,
			   tframe_measured_expect, timetag_expect);

      double T = frame_info.T();
      double tframe_nominal = frame_info.tframe_nominal();
      double omega_bar = frame_info.omega_bar();
      double omega_beat = frame_info.omega_beat();
      double tframe_measured = frame_info.tframe_measured();
      double timetag = frame_info.timetag();

      if (cmp.neq(T, T_expect) ||
	  cmp.neq(tframe_nominal, tframe_nominal_expect) ||
	  cmp.neq(omega_bar, omega_bar_expect) ||
	  cmp.neq(omega_beat, omega_beat_expect) ||
	  cmp.neq(tframe_measured, tframe_measured_expect) ||
	  cmp.neq(timetag, timetag_expect)) {
	cerr << "T result:\n"<< T << "\n"
	     << "T result expect:\n"<< T_expect << "\n";
	cerr << "tframe_nominal result:\n"<< tframe_nominal << "\n"
	     << "tframe_nominal result expect:\n"<< tframe_nominal_expect << "\n";
	cerr << "omega_bar result:\n"<< omega_bar << "\n"
	     << "omega_bar result expect:\n"<< omega_bar_expect << "\n";
	cerr << "omega_beat result:\n"<< omega_beat << "\n"
	     << "omega_beat result expect:\n"<< omega_beat_expect << "\n";
	cerr << "tframe_measured result:\n"<< tframe_measured << "\n"
	     << "tframe_measured result expect:\n"<< tframe_measured_expect << "\n";
	cerr << "timetag result:\n"<< timetag << "\n"
	     << "timetag result expect:\n"<< timetag_expect << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test FrameInfo(double, double, double, double)
//-----------------------------------------------------------------------

    test_number++;
    {      
      double f1_double = 8.8475644588470458984375;
      double f2_double = 3.1664967536926269531250;
      double T_expect = 1.66471600620155035788e-01;
      double tframe_nominal_expect = 1.76023249836065598384e-01;
      double omega_bar_expect = 3.77432864450927212374e+01;
      double omega_beat_expect = 1.78476005670593451669e+01;
      double tframe_measured_expect = 3.0;
      double timetag_expect = 7.0;

      FrameInfo frame_info(f1_double, f2_double, tframe_measured_expect, 
			   timetag_expect);

      double T = frame_info.T();
      double tframe_nominal = frame_info.tframe_nominal();
      double omega_bar = frame_info.omega_bar();
      double omega_beat = frame_info.omega_beat();
      double tframe_measured = frame_info.tframe_measured();
      double timetag = frame_info.timetag();

      if (cmp.neq(T, T_expect) ||
	  cmp.neq(tframe_nominal, tframe_nominal_expect) ||
	  cmp.neq(omega_bar, omega_bar_expect) ||
	  cmp.neq(omega_beat, omega_beat_expect) ||
	  cmp.neq(tframe_measured, tframe_measured_expect) ||
	  cmp.neq(timetag, timetag_expect)) {
	cerr << "T result:\n"<< T << "\n"
	     << "T result expect:\n"<< T_expect << "\n";
	cerr << "tframe_nominal result:\n"<< tframe_nominal << "\n"
	     << "tframe_nominal result expect:\n"<< tframe_nominal_expect << "\n";
	cerr << "omega_bar result:\n"<< omega_bar << "\n"
	     << "omega_bar result expect:\n"<< omega_bar_expect << "\n";
	cerr << "omega_beat result:\n"<< omega_beat << "\n"
	     << "omega_beat result expect:\n"<< omega_beat_expect << "\n";
	cerr << "tframe_measured result:\n"<< tframe_measured << "\n"
	     << "tframe_measured result expect:\n"<< tframe_measured_expect << "\n";
	cerr << "timetag result:\n"<< timetag << "\n"
	     << "timetag result expect:\n"<< timetag_expect << "\n";
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
