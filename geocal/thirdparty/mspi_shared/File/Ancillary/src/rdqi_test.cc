/*===========================================================================
=                                                                           =
=                                Rdqi                                       =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "rdqi.h"
				// Definition of Rdqi
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/float_comparator.h" 
				// Definition of FloatComparator
#include "MSPI-Shared/UnitTest/src/check.h"
				// Definition of array_check
#include "H5Cpp.h"		// Definition of H5::H5File
#include "hdf5_hl.h" 		// Definition of H5DSattach
#include <string.h> 		// Definition of strcmp

using std::cerr;
using MSPI::Shared::Rdqi;
using MSPI::Shared::Exception;
using MSPI::Shared::FloatComparator;
using MSPI::Shared::array_check;
using MSPI::Shared::vector_check;


//-----------------------------------------------------------------------
// Generate expected output.
//-----------------------------------------------------------------------

float data_init(int Row, int Pixel)
{
  return Pixel * 100.0 + Row * 10.0;
}

//-----------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  FloatComparator cmp;

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Create test data file.
//-----------------------------------------------------------------------

    std::string filename = "File/Ancillary/tmp/rdqi_test.hdf5";
    std::string dataset_name = "RDQI";
    std::string row_name = "RowNumber";
    const size_t number_row = 3;
    const size_t number_pixel = 20;

    std::vector<int> row_scale(number_row);
    for (size_t irow = 0 ; irow < number_row ; irow++) {
      row_scale[irow] = irow + 100;
    }

    boost::multi_array<float,2> 
      data(boost::extents[number_row][number_pixel]);
    for (size_t irow = 0 ; irow < number_row ; irow++) {
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
	data[irow][ipixel] = data_init(irow,ipixel);
      }
    }

    {
      H5::H5File file(filename, H5F_ACC_TRUNC);

      hsize_t dims[2];
      dims[0] = number_row;
      dims[1] = number_pixel;
      
      H5::DataSpace row_space(1, &dims[0]);
      H5::DataSet row_set = file.createDataSet(row_name, H5::PredType::NATIVE_INT, row_space);

      H5::DataSpace dataspace(2, dims);
      H5::DataSet dataset = file.createDataSet(dataset_name, H5::PredType::NATIVE_FLOAT, dataspace);

      row_set.write(&(row_scale[0]), H5::PredType::NATIVE_INT);
      dataset.write(data.data(), H5::PredType::NATIVE_FLOAT);

      herr_t status;
      status = H5DSattach_scale(dataset.getId(), row_set.getId(), 0);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }
    }

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    Rdqi file(filename);

//-----------------------------------------------------------------------
// Test number_row()
// Test number_pixel()
//-----------------------------------------------------------------------

    test_number++;
    if (file.number_row() != number_row ||
	file.number_pixel() != number_pixel) {
      cerr << "number_row = " << file.number_row() << "\n";
      cerr << "number_pixel = " << file.number_pixel() << "\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test row_numbers()
//-----------------------------------------------------------------------

    test_number++;

    std::vector<int> row_numbers = file.row_numbers();
    vector_check(row_numbers, row_scale, __LINE__, error);

//-----------------------------------------------------------------------
// Test data()
//-----------------------------------------------------------------------
    
    test_number++;

    for (size_t irow = 0 ; irow < number_row ; irow++) {
      int row_number = irow + 100;

      std::vector<float> data = file.data(row_number);

      std::vector<float> data_expect(number_pixel);
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
	data_expect[ipixel] = data_init(irow, ipixel);
      }
      vector_check(data, data_expect, cmp, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test with invalid row number.
//-----------------------------------------------------------------------

    test_number++;

    {
      bool caught_exception = false;

      try {
	file.data(1000);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Rdqi: row number not available: 1000")) {
	  cerr << "exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "L1B1File() exception not caught\n";
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
