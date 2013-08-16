/*===========================================================================
=                                                                           =
=                                BoxRegionCoor                              =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: box_region_coor_test.cc

!ABSTRACT:

   This does unit testing for the class BoxRegionCoor.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "box_region_coor.h"           
				   // Definition of class to be tested.
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.

using std::cerr;
using MSPI::Shared::BoxRegionCoor;
using MSPI::Shared::Exception;


int main()
{
//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// 1. Normal call to all member functions.
//-----------------------------------------------------------------------

    bool error = false;		// Flag indicating test failure.

    double min_x = 2.0;
    double min_y = 2.1;
    double max_x = 2.2;
    double max_y = 2.3;

    BoxRegionCoor box(max_x, max_y, min_x, min_y);

    if (box.min_x() != min_x ||
	box.min_y() != min_y ||
	box.max_x() != max_x ||
	box.max_y() != max_y) {
      cerr << "min_x = " << box.min_x() << "(expected " << min_x << ")\n";
      cerr << "min_y = " << box.min_y() << "(expected " << min_y << ")\n";
      cerr << "max_x = " << box.max_x() << "(expected " << max_x << ")\n";
      cerr << "max_y = " << box.max_y() << "(expected " << max_y << ")\n";
      cerr << "Unexpected_result for constructor\n";
      error = true;
    }

    double min_x2 = 1.0;
    double min_y2 = 1.1;
    double max_x2 = 2.0;
    double max_y2 = 2.1;

    BoxRegionCoor box2(min_x2, max_y2, max_x2, min_y2);

    BoxRegionCoor box_n_box2 = box.intersect(box2);
    if (box_n_box2.min_x() != 0.0 ||
	box_n_box2.min_y() != 0.0 ||
	box_n_box2.max_x() != 0.0 ||
	box_n_box2.max_y() != 0.0) {
      cerr << "min_x = " << box_n_box2.min_x() 
	   << "(expected " << 0.0 << ")\n";
      cerr << "min_y = " << box_n_box2.min_y() 
	   << "(expected " << 0.0 << ")\n";
      cerr << "max_x = " << box_n_box2.max_x() 
	   << "(expected " << 0.0 << ")\n";
      cerr << "max_y = " << box_n_box2.max_y() 
	   << "(expected " << 0.0 << ")\n";
      cerr << "Unexpected_result for intersect (1)\n";
      error = true;
    }

    double min_x3 = 1.0;
    double min_y3 = 1.1;
    double max_x3 = 2.05;
    double max_y3 = 2.15;

    BoxRegionCoor box3(min_x3, max_y3, max_x3, min_y3);

    BoxRegionCoor box_n_box3 = box.intersect(box3);
    if (box_n_box3.min_x() != min_x ||
	box_n_box3.min_y() != min_y ||
	box_n_box3.max_x() != max_x3 ||
	box_n_box3.max_y() != max_y3) {
      cerr << "min_x = " << box_n_box3.min_x() 
	   << "(expected " << min_x << ")\n";
      cerr << "min_y = " << box_n_box3.min_y() 
	   << "(expected " << min_y << ")\n";
      cerr << "max_x = " << box_n_box3.max_x() 
	   << "(expected " << max_x3 << ")\n";
      cerr << "max_y = " << box_n_box3.max_y() 
	   << "(expected " << max_y3 << ")\n";
      cerr << "Unexpected_result for intersect (2)\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test operator== and operator!=
//-----------------------------------------------------------------------

    BoxRegionCoor box4(1.0, 2.0, 3.0, 4.0);
    BoxRegionCoor box5(1.1, 2.0, 3.0, 4.0);
    BoxRegionCoor box6(1.0, 2.1, 3.0, 4.0);
    BoxRegionCoor box7(1.0, 2.0, 3.1, 4.0);
    BoxRegionCoor box8(1.0, 2.0, 3.0, 4.1);
    BoxRegionCoor box9(1.0 + 5 * std::numeric_limits<double>::epsilon(), 
		       2.0 - 5 * std::numeric_limits<double>::epsilon(),
		       3.0 + 5 * std::numeric_limits<double>::epsilon(), 
		       4.0 + 5 * std::numeric_limits<double>::epsilon());

    if (box4 == box5 ||
	box4 == box6 ||
	box4 == box7 ||
	box4 == box8 ||
	box4 != box9) {
      cerr << "unexpected result for operator ==\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test is_empty
//-----------------------------------------------------------------------

    BoxRegionCoor box10(1, 0, 0, 0); 
				// Not empty by current
				// implementation.
    BoxRegionCoor box11(0,0,0,0); 
				// Should be empty.

    if (!box_n_box2.is_empty() ||
	box10.is_empty() ||
	!box11.is_empty()) {
      cerr << "unexpected result for is_empty\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test point_intersect
//-----------------------------------------------------------------------

    double min_x12 = 1; 
    double min_y12 = 2;
    double max_x12 = 3;
    double max_y12 = 4;

    BoxRegionCoor box12(min_x12, min_y12, max_x12, max_y12); 

    if (!box12.point_intersect(min_x12 + 0.01, min_y12 + 0.01) ||
	box12.point_intersect(min_x12 - 0.01, min_y12 + 0.01) || 
	box12.point_intersect(min_x12 + 0.01, min_y12 - 0.01) || 
	box12.point_intersect(max_x12 - 0.01, max_y12 + 0.01) || 
	box12.point_intersect(max_x12 + 0.01, max_y12 - 0.01) ||
	box12.point_intersect(max_x12 + 0.01, max_y12 + 0.01) ||
	!box12.point_intersect(max_x12 - 0.01, max_y12 - 0.01) ||
	box12.point_intersect(min_x12 - 0.01, min_y12 - 0.01)) {
      cerr << "unexpected result for is_point_intersect(1)\n";
      error = true;
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
