/*===========================================================================
=                                                                           =
=                                ParaxialTransform                          =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                               L1B1 Equation

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: paraxial_transform_test.cc

!ABSTRACT:

   This does unit testing for the class ParaxialTransform.

!DESCRIPTION:

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "unit_test_support.h"
#include "paraxial_transform.h"	// Definition of ParaxialTransform
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/double_comparator.h"
				// Definition of DoubleComparator
#include <cmath>		// Definition of std::fabs
#include <string.h> 		// Definition of strcmp

using std::cerr;
using GeoCal::Mspi::ParaxialTransform;
using MSPI::Shared::Exception;
using MSPI::Shared::DoubleComparator;
using MSPI::Shared::ConfigFile;
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(paraxial_transform, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  DoubleComparator cmp(10.0);	// Double comparator

//-----------------------------------------------------------------------
// Test default constructor.
//-----------------------------------------------------------------------

  test_number++;
  std::string config_filename = 
    mspi_test_data_dir() + "mspi_camera_test.config";
  ConfigFile config(config_filename);
  ParaxialTransform p(config);

//-----------------------------------------------------------------------
// Test paraxial_to_real() for each supported row number.
// (default constructor)
//-----------------------------------------------------------------------

  test_number++;

  const int number_case = 3;

  double paraxial_x[number_case] = {0.1, 0.1, 0.1};
  double paraxial_y[number_case] = {-0.2, -0.2, -0.2};
  
  int row[number_case] = {32, 27, 22};
  
  double real_x_expect[number_case] = {1.05852897147680050094e-01,
				       1.05852897147680050094e-01,
				       1.05852897147680050094e-01};
  
  double real_y_expect[number_case] = {-1.98878552619107251687e-01,
				       -1.98878552619107251687e-01,
				       -1.98878552619107251687e-01};
  
  for (int i = 0 ; i < number_case ; i++) {
    double real_x;
    double real_y;
    
    p.paraxial_to_real(row[i], paraxial_x[i], paraxial_y[i], real_x, real_y);
    
    if (cmp.neq(real_x, real_x_expect[i]) ||
	cmp.neq(real_y, real_y_expect[i])) {
      cerr << "real_x = " << real_x << " (expected "<< real_x_expect[i] << "\n";
      cerr << "real_y = " << real_y << " (expected "<< real_y_expect[i] << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }
  }
  
  //-----------------------------------------------------------------------
  // Test constructor(A,B,C,D)
  //-----------------------------------------------------------------------
  
  test_number++;
  
  const int number_row = 4;
  std::vector<int> row_numbers(number_row);
  row_numbers[0] = 20;
  row_numbers[1] = 30;
  row_numbers[2] = 40;
  row_numbers[3] = 50;
  
  std::map<int,std::vector<double> > a;
  std::map<int,std::vector<double> > b;
  std::map<int,std::vector<double> > c;
  std::map<int,std::vector<double> > d;
  
  for (int irow = 0 ; irow < number_row ; irow++) {
    int row_number = row_numbers[irow];
    std::vector<double> a_vect;
    for (size_t iparam = 0 ; iparam < ParaxialTransform::size_a ; iparam++) {
      a_vect.push_back(irow * (double)10.0 + iparam);
    }
    a[row_number] = a_vect;
    
    std::vector<double> b_vect;
    for (size_t iparam = 0 ; iparam < ParaxialTransform::size_b ; iparam++) {
      b_vect.push_back(irow * (double)20.0 + iparam);
    }
    b[row_number] = b_vect;
    
    std::vector<double> c_vect;
    for (size_t iparam = 0 ; iparam < ParaxialTransform::size_c ; iparam++) {
      c_vect.push_back(irow * (double)30.0 + iparam);
    }
    c[row_number] = c_vect;
    
    std::vector<double> d_vect;
    for (size_t iparam = 0 ; iparam < ParaxialTransform::size_d ; iparam++) {
      d_vect.push_back(irow * (double)40.0 + iparam);
    }
    d[row_number] = d_vect;
  }
  
  ParaxialTransform p2(a,b,c,d);
  
  //-----------------------------------------------------------------------
  // Test paraxial_to_real() for each supported row number.
  // ( constuctor(A,B,C,D) )
  // Expected results are calculated using bc script:
  //    bc -l paraxial_transform_test.bc
  //-----------------------------------------------------------------------
  
  test_number++;
  
  {
    const int number_case = 3;
    double paraxial_x[number_case] = {0.1, 0.0, -0.1};
    double paraxial_y[number_case] = {-0.2, 0.0, 0.2};
    
    double real_x_expect[number_row][number_case] = 
      { {-.009550056, 0, -.032078344},
	{2.535904464, 0, -3.789654064},
	{5.081358984, 0, -7.547229784},
	{7.626813504, 0, -11.304805504}};
    
    double real_y_expect[number_row][number_case] = 
	{ {-.09550056, 0, .32078344},
	  {33.84389304, 40.00000000, 50.42179304},
	  {67.78328664, 80.00000000, 100.52280264},
	  {101.72268024, 120.00000000, 150.62381224}};

    for (int icase = 0 ; icase < number_case ; icase++) {
      for (int irow = 0 ; irow < number_row ; irow++) {
	double real_x;
	double real_y;
	p2.paraxial_to_real(row_numbers[irow], 
			    paraxial_x[icase], paraxial_y[icase], 
			    real_x, real_y);
	
	if (cmp.neq(real_x, real_x_expect[irow][icase]) ||
	    cmp.neq(real_y, real_y_expect[irow][icase])) {
	  cerr << row_numbers[irow] << ":" << "real_x = " << real_x 
	       << " (expected "<< real_x_expect[irow][icase] << "\n";
	  cerr << row_numbers[irow] << ":" << "real_y = " << real_y
	       << " (expected "<< real_y_expect[irow][icase] << "\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }
  }
  
  //-----------------------------------------------------------------------
  // Test real_to_paraxial() for each supported row number.
  // ( constuctor(A,B,C,D) )
  // Expected results are calculated using bc script:
  //    bc -l paraxial_transform_test.bc
  //-----------------------------------------------------------------------
  
  test_number++;
  
  {
    const int number_case = 3;
    double real_x[number_case] = {0.3, 0.0, -0.3};
    double real_y[number_case] = {-0.4, 0.0, 0.4};
    
    double paraxial_x_expect[number_row][number_case] = 
      { {.0325161, 0, -.0325161},
	{3.3290031, 0, -3.3290031},
	{6.6254901, 0, -6.6254901},
	{9.9219771, 0, -9.9219771}};
    
    double paraxial_y_expect[number_row][number_case] = 
      { {-.291613, 0, .508387},
	{21.684967, 20.000000, 22.484967},
	{43.661547, 40.000000, 44.461547},
	{65.638127, 60.000000, 66.438127} };

    for (int icase = 0 ; icase < number_case ; icase++) {
      for (int irow = 0 ; irow < number_row ; irow++) {
	double paraxial_x;
	double paraxial_y;
	
	p2.real_to_paraxial(row_numbers[irow], 
			    real_x[icase], real_y[icase], 
			    paraxial_x, paraxial_y);
	
	if (cmp.neq(paraxial_x, paraxial_x_expect[irow][icase]) ||
	    cmp.neq(paraxial_y, paraxial_y_expect[irow][icase])) {
	  cerr << row_numbers[irow] << ":" << "paraxial_x = " << paraxial_x 
	       << " (expected "<< paraxial_x_expect[irow][icase] << "\n";
	  cerr << row_numbers[irow] << ":" << "paraxial_y = " << paraxial_y
	       << " (expected "<< paraxial_y_expect[irow][icase] << "\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }
  }
  
  //-------------------------------------------------------------------------
  // Test round-trip error.
  //-------------------------------------------------------------------------
  
  test_number++;
  
  {
    ConfigFile camera_model_config(mspi_test_data_dir() + 
				   "paraxial_transform_test.config");
    ParaxialTransform p(camera_model_config);
    double max_error_expect = 0.0012;  // 12% of a pixel
    
    const int number_row = 14;
    int row_number_init[number_row] = {62, 57, 52, 47, 42, 37, 32, 33, 27, 22, 17, 12, 6, 0};
    std::vector<int> row_number(&row_number_init[0],&row_number_init[number_row]);
    
    //      fprintf(stderr,"%3s %6s %6s %6s %6s %6s %6s %6s %6s\n","row","px","py","rx","ry","px'","py'","err(x)","err(y)");
    for (double real_x_in = 0 ; real_x_in < 16 ; real_x_in+=0.1) {
      for (int irow = 0 ; irow < number_row ; irow++) {
	int row = row_number[irow];
	double real_y_in = -0.016 * (row + 0.5 - 64.0/2.0);
	double real_x_out;
	double real_y_out;
	
	double paraxial_x;
	double paraxial_y;
	
	p.real_to_paraxial(row,real_x_in,real_y_in,
			   paraxial_x,paraxial_y);
	
	p.paraxial_to_real(row,paraxial_x,paraxial_y,
			   real_x_out,real_y_out);
	
	if (std::fabs(real_x_out-real_x_in) > max_error_expect) {
	  cerr << row << " : real_x_out-real_x_in " << real_x_out-real_x_in 
	       <<  " (expected less than " << max_error_expect << " millimeters)\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (std::fabs(real_y_out-real_y_in) > max_error_expect) {
	  cerr << row << " : real_y_out-real_y_in " << real_y_out-real_y_in 
	       <<  " (expected less than " << max_error_expect << " millimeters)\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	
	//	  fprintf(stderr,"x: %3d %6.3g %6.3g %6.3g %6.3g %6.3g %6.3g %6.3g %6.3g\n",
	//		  row,  
	//		  real_x_in,    // millimeters
	//		  real_y_in,    // millimeters
	//		  paraxial_x,   // millimeters
	//		  paraxial_y,   // millimeters
	//		  real_x_out,   // millimeters
	//		  real_y_out,   // millimeters
	//		  (real_x_out-real_x_in)/0.001,   // micrometers
	//		  (real_y_out-real_y_in)/0.001);  // micrometers;
      }
    }
  }
  
  //-----------------------------------------------------------------------
  // Argument check: Row_number not supported (real_to_paraxial)
  //-----------------------------------------------------------------------
  
  test_number++;
  
  {
    bool caught_exception = false;
    
    try {
      double paraxial_x;
      double paraxial_y;
      p2.real_to_paraxial(11,
			  0.0, 0.0, 
			  paraxial_x, paraxial_y);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Row_number 11 not supported by paraxial transform (A)")) {
	cerr << "real_to_paraxial() exception = "<<e.what()<<"\n";
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
    
    a[11] = std::vector<double>(1);
    ParaxialTransform p2(a,b,c,d);
    
    try {
      double paraxial_x;
      double paraxial_y;
      p2.real_to_paraxial(11,
			  0.0, 0.0, 
			  paraxial_x, paraxial_y);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Row_number 11 not supported by paraxial transform (B)")) {
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
  // Argument check: Row_number not supported (real_to_paraxial)
  //-----------------------------------------------------------------------
  
  test_number++;
  
  {
    bool caught_exception = false;
    
    try {
      double real_x;
      double real_y;
      p2.paraxial_to_real(11,
			  0.0, 0.0, 
			  real_x, real_y);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Row_number 11 not supported by paraxial transform (C)")) {
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
    
    c[11] = std::vector<double>(1);
    ParaxialTransform p2(a,b,c,d);
    
    try {
      double real_x;
      double real_y;
      p2.paraxial_to_real(11,
			  0.0, 0.0, 
			  real_x, real_y);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Row_number 11 not supported by paraxial transform (D)")) {
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
  // Argument check: a.size()( != size_a
  //-----------------------------------------------------------------------
  
  test_number++;

  {
    bool caught_exception = false;
    
    std::map<int,std::vector<double> > a_bad(a);
    
    a_bad[20].push_back(1);
    ParaxialTransform p2(a_bad,b,c,d);
    
    try {
      double paraxial_x;
      double paraxial_y;
      p2.real_to_paraxial(20,
			  0.0, 0.0, 
			    paraxial_x, paraxial_y);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "a.size() != size_a")) {
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
// Argument check: b.size()( != size_b
//-----------------------------------------------------------------------
  
  test_number++;
  
  {
    bool caught_exception = false;
    
    std::map<int,std::vector<double> > b_bad(b);
    
    b_bad[20].push_back(1);
    ParaxialTransform p2(a,b_bad,c,d);
    
    try {
      double paraxial_x;
      double paraxial_y;
      p2.real_to_paraxial(20,
			  0.0, 0.0, 
			  paraxial_x, paraxial_y);
    } catch (Exception& e) {
      caught_exception = true;
	if (strcmp(e.what(), "b.size() != size_b")) {
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
  // Argument check: c.size()( != size_c
  //-----------------------------------------------------------------------
  
    test_number++;
    
    {
      bool caught_exception = false;
      
      std::map<int,std::vector<double> > c_bad(c);
      
      c_bad[20].push_back(1);
      ParaxialTransform p2(a,b,c_bad,d);

      try {
	double real_x;
	double real_y;
	p2.paraxial_to_real(20,
			    0.0, 0.0, 
			    real_x, real_y);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "c.size() != size_c")) {
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
// Argument check: d.size()( != size_d
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;

      std::map<int,std::vector<double> > d_bad(d);

      d_bad[20].push_back(1);
      ParaxialTransform p2(a,b,c,d_bad);

      try {
	double real_x;
	double real_y;
	p2.paraxial_to_real(20,
			    0.0, 0.0, 
			    real_x, real_y);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "d.size() != size_d")) {
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

    BOOST_CHECK(!error);

}

BOOST_AUTO_TEST_SUITE_END()
