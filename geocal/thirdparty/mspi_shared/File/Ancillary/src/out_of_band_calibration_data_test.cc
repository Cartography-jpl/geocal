//===========================================================================
//                                                                      
//                           OutOfBandCalibrationData                   
//
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "out_of_band_calibration_data.h"
				// Definition of OutOfBandCalibrationData
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/float_comparator.h" 
				// Definition of FloatComparator
#include "MSPI-Shared/UnitTest/src/check.h"
				// Definition of array_check
#include "H5Cpp.h"		// Definition of H5::H5File
#include "hdf5_hl.h" 		// Definition of H5DSattach
#include <string.h> 		// Definition of strcmp
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::path

using std::cerr;
using MSPI::Shared::OutOfBandCalibrationData;
using MSPI::Shared::Exception;
using MSPI::Shared::FloatComparator;
using MSPI::Shared::array_check;
using MSPI::Shared::vector_check;
using MSPI::Shared::Hdf5Meta;
using boost::filesystem::path;


//-----------------------------------------------------------------------
// Generate expected output.
//-----------------------------------------------------------------------

float data_init(int Pixel, int Row, int Col) 
{
  return Pixel * 100.0 + Row * 10.0 + Col;
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

    std::string filename = "File/Ancillary/tmp/out_of_band_calibration_data_test.hdf5";
    std::string dataset_name = "OutOfBandCalibration";
    std::string row_name = "RowNumber";
    const size_t number_row = 3;
    const size_t number_col = 4;
    const size_t number_pixel = 2;

    std::vector<int> row_scale(number_row);
    for (size_t irow = 0 ; irow < number_row ; irow++) {
      row_scale[irow] = irow + 100;
    }

    boost::multi_array<float,3> 
      data(boost::extents[number_pixel][number_row][number_col]);
    for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
      for (size_t irow = 0 ; irow < number_row ; irow++) {
	for (size_t icol = 0 ; icol < number_col ; icol++) {
	  data[ipixel][irow][icol] = data_init(ipixel, irow, icol);
	}
      }
    }

    {
      H5::H5File file(filename, H5F_ACC_TRUNC);

      hsize_t dims[3];
      dims[0] = number_pixel;
      dims[1] = number_row;
      dims[2] = number_col;
      
      H5::DataSpace row_space(1, &dims[1]);
      H5::DataSet row_set = file.createDataSet(row_name, H5::PredType::NATIVE_INT, row_space);

      H5::DataSpace dataspace(3, dims);
      H5::DataSet dataset = file.createDataSet(dataset_name, H5::PredType::NATIVE_FLOAT, dataspace);

      row_set.write(&(row_scale[0]), H5::PredType::NATIVE_INT);
      dataset.write(data.data(), H5::PredType::NATIVE_FLOAT);

      herr_t status;
      status = H5DSattach_scale(dataset.getId(), row_set.getId(), 1);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }

      status = H5DSattach_scale(dataset.getId(), row_set.getId(), 2);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }
      
      Hdf5Meta meta;
      meta.add<std::string>("granule_id",path(filename).filename());
      meta.hdf5_export(file.openGroup("/").getLocId());
    }

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    OutOfBandCalibrationData file(filename);

//-----------------------------------------------------------------------
// Test granule_id()
//-----------------------------------------------------------------------

    test_number++;
    
    if (file.granule_id() != "out_of_band_calibration_data_test.hdf5") {
      cerr << "granule_id = " << file.granule_id() << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test number_row()
// Test number_col()
// Test number_pixel()
//-----------------------------------------------------------------------

    test_number++;
    if (file.number_row() != number_row ||
	file.number_col() != number_col ||
	file.number_pixel() != number_pixel) {
      cerr << "number_row = " << file.number_row() << "\n";
      cerr << "number_col = " << file.number_col() << "\n";
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

      boost::multi_array<float,2> data = file.data(row_number);

      boost::multi_array<float,2> data_expect(boost::extents[number_col][number_pixel]);
      for (size_t icol = 0 ; icol < number_col ; icol++) {
	for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
	  data_expect[icol][ipixel] = data_init(ipixel, irow, icol);
	}
      }
      array_check(data, data_expect, cmp, __LINE__, error);
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
	if (strcmp(e.what(), "OutOfBandCalibrationData: row number not available: 1000")) {
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
// Create sample calibration file for AirMSPI testing.
//-----------------------------------------------------------------------

    {
      std::string filename = "File/Ancillary/tmp/out_of_band_calibration_data_test2.hdf5";
      const size_t number_row = 13;
      const size_t number_col = 13;
      const size_t number_pixel = 1536;

      int row_number_init[number_row] = 
	{0, 6, 12, 17, 22, 27, 32, 37, 42, 47, 52, 57, 62};
      
      std::vector<int> row_scale(&row_number_init[0],&row_number_init[number_row]);
      
      boost::multi_array<float,3> 
	data1(boost::extents[number_pixel][number_row][number_col]);
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
	for (size_t irow = 0 ; irow < number_row ; irow++) {
	  int row_number = row_scale[irow];
	  for (size_t icol = 0 ; icol < number_col ; icol++) {
	    data1[ipixel][irow][icol] = ((irow == icol && (int)ipixel != row_number) ? 1 : 0);
	  }
	}
      }

      boost::multi_array<float,2> 
	data2(boost::extents[number_row][number_pixel]);
      for (size_t irow = 0 ; irow < number_row ; irow++) {
	int row_number = row_scale[irow];
	for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
	  data2[irow][ipixel] = ((int)ipixel == row_number ? 1 : 0);
	}
      }

      H5::H5File file(filename, H5F_ACC_TRUNC);
	
      hsize_t dims1[3];
      dims1[0] = number_pixel;
      dims1[1] = number_row;
      dims1[2] = number_col;

      hsize_t dims2[3];
      dims2[0] = number_row;
      dims2[1] = number_pixel;
	
      H5::DataSet row_set = file.createDataSet("RowNumber", H5::PredType::NATIVE_INT,
					       H5::DataSpace(1, &dims1[1]));

      row_set.write(&(row_scale[0]), H5::PredType::NATIVE_INT);
      
      H5::DataSet dataset1 = file.createDataSet("OutOfBandCalibration", H5::PredType::NATIVE_FLOAT, 
						H5::DataSpace(3, dims1));
      dataset1.write(data1.data(), H5::PredType::NATIVE_FLOAT);

      
      H5::DataSet dataset2 = file.createDataSet("RDQI", H5::PredType::NATIVE_FLOAT,
						H5::DataSpace(2, dims2));
      dataset2.write(data2.data(), H5::PredType::NATIVE_FLOAT);
      

      herr_t status;
      status = H5DSattach_scale(dataset1.getId(), row_set.getId(), 1);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }
      
      status = H5DSattach_scale(dataset1.getId(), row_set.getId(), 2);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }
      
      status = H5DSattach_scale(dataset2.getId(), row_set.getId(), 0);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }

      Hdf5Meta meta;
      meta.add<std::string>("granule_id",path(filename).filename());
      meta.hdf5_export(file.openGroup("/").getLocId());
    }

//-----------------------------------------------------------------------
// Create identity calibration file for AirMSPI testing.
//-----------------------------------------------------------------------

    {
      std::string filename = "File/Ancillary/tmp/identity_out_of_band_rdqi.hdf5";
      const size_t number_row = 13;
      const size_t number_col = 13;
      const size_t number_pixel = 1536;

      int row_number_init[number_row] = 
	{0, 6, 12, 17, 22, 27, 32, 37, 42, 47, 52, 57, 62};
      
      std::vector<int> row_scale(&row_number_init[0],&row_number_init[number_row]);
      
      boost::multi_array<float,3> 
	data1(boost::extents[number_pixel][number_row][number_col]);
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
	for (size_t irow = 0 ; irow < number_row ; irow++) {
	  for (size_t icol = 0 ; icol < number_col ; icol++) {
	    data1[ipixel][irow][icol] = (irow == icol ? 1 : 0);
	  }
	}
      }

      boost::multi_array<float,2> 
	data2(boost::extents[number_row][number_pixel]);
      for (size_t irow = 0 ; irow < number_row ; irow++) {
	for (size_t ipixel = 0 ; ipixel < number_pixel ; ipixel++) {
	  data2[irow][ipixel] = 0;
	}
      }

      H5::H5File file(filename, H5F_ACC_TRUNC);
	
      hsize_t dims1[3];
      dims1[0] = number_pixel;
      dims1[1] = number_row;
      dims1[2] = number_col;

      hsize_t dims2[3];
      dims2[0] = number_row;
      dims2[1] = number_pixel;
	
      H5::DataSet row_set = file.createDataSet("RowNumber", H5::PredType::NATIVE_INT,
					       H5::DataSpace(1, &dims1[1]));

      row_set.write(&(row_scale[0]), H5::PredType::NATIVE_INT);
      
      H5::DataSet dataset1 = file.createDataSet("OutOfBandCalibration", H5::PredType::NATIVE_FLOAT, 
						H5::DataSpace(3, dims1));
      dataset1.write(data1.data(), H5::PredType::NATIVE_FLOAT);

      
      H5::DataSet dataset2 = file.createDataSet("RDQI", H5::PredType::NATIVE_FLOAT,
						H5::DataSpace(2, dims2));
      dataset2.write(data2.data(), H5::PredType::NATIVE_FLOAT);
      

      herr_t status;
      status = H5DSattach_scale(dataset1.getId(), row_set.getId(), 1);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }
      
      status = H5DSattach_scale(dataset1.getId(), row_set.getId(), 2);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }
      
      status = H5DSattach_scale(dataset2.getId(), row_set.getId(), 0);
      if (status) {
	cerr << "Trouble with H5DSattach\n";
      }

      Hdf5Meta meta;
      meta.add<std::string>("granule_id",path(filename).filename());
      meta.hdf5_export(file.openGroup("/").getLocId());
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
