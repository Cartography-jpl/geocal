//===========================================================================
//                                                                          
//                               polygon_table_test
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "polygon_table.h"          // Definition of polygon_table_write
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check
#include "MSPI-Shared/UnitTest/src/double_comparator.h" 
				// Definition of DoubleComparator

using std::cerr;
using MSPI::Shared::Exception;
using MSPI::Shared::vector_check;
using MSPI::Shared::DoubleComparator;
using MSPI::Shared::HDF4::polygon_table_write;
using MSPI::Shared::HDF4::polygon_table_read;


//////////////////////////////////////////////////////////////////////////
// Initialize unit-test input
//////////////////////////////////////////////////////////////////////////

const int number_record = 10;

//------------------------------------------------------------------------
std::vector<double> longitude_init()
{
  std::vector<double> data(number_record);
  for (size_t i = 0 ; i < data.size(); i++) {
    data[i] = i * 0.51;
  }
  return data;
}

//------------------------------------------------------------------------
std::vector<double> latitude_init()
{
  std::vector<double> data(number_record);
  for (size_t i = 0 ; i < data.size(); i++) {
    data[i] = i * 0.71;
  }
  return data;
}

//////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////

int main()
{

  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  DoubleComparator dcmp;

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test polygon_table_write
//-----------------------------------------------------------------------

    test_number++;
    std::string filename = "File/hdf4/Table/tmp/polygon_table_test.hdf";

    {
      int32 fid = Hopen(filename.c_str(), DFACC_CREATE, DEF_NDDS);

      std::vector<double> latitude(latitude_init());
      std::vector<double> longitude(longitude_init());

      polygon_table_write(fid, latitude, longitude);
      Hclose(fid);
    }
      
//-----------------------------------------------------------------------
// Test polygon_table_read
//-----------------------------------------------------------------------

    test_number++;
    {
      int32 fid = Hopen(filename.c_str(), DFACC_READ, DEF_NDDS);

      std::vector<double> latitude;
      std::vector<double> longitude;
      polygon_table_read(fid, latitude, longitude);

      vector_check(latitude, latitude_init(), dcmp, __LINE__, error);
      vector_check(longitude, longitude_init(), dcmp, __LINE__, error);
      Hclose(fid);
    }

//-----------------------------------------------------------------------
// Argument check: number_record < 1
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;
      std::string filename = "File/hdf4/Table/tmp/polygon_table_test2.hdf";
      int32 fid = Hopen(filename.c_str(), DFACC_CREATE, DEF_NDDS);
      std::vector<double> latitude;
      std::vector<double> longitude;
      
      try {
	polygon_table_write(fid, latitude, longitude);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "number_record < 1")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      Hclose(fid);
    }

//-----------------------------------------------------------------------
// Argument check: Longitude.size() != number_record
//-----------------------------------------------------------------------

    test_number++;
    {
      bool caught_exception = false;
      std::string filename = "File/hdf4/Table/tmp/polygon_table_test2.hdf";
      int32 fid = Hopen(filename.c_str(), DFACC_CREATE, DEF_NDDS);
      std::vector<double> latitude(1);
      std::vector<double> longitude(2);
      
      try {
	polygon_table_write(fid, latitude, longitude);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Longitude.size() != number_record")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

      Hclose(fid);
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