//===========================================================================
//                                                                      
//                           create_out_of_band_rdqi_main
//
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "out_of_band_calibration_data.h"
				// Definition of OutOfBandCalibrationData
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "H5Cpp.h"		// Definition of H5::H5File
#include "hdf5_hl.h" 		// Definition of H5DSattach
#include <string.h> 		// Definition of strcmp
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::path
#include "MSPI-Shared/Config/src/config_file.h" 
				// Definition of ConfigFile

using std::cerr;
using MSPI::Shared::OutOfBandCalibrationData;
using MSPI::Shared::Exception;
using MSPI::Shared::ConfigFile;
using MSPI::Shared::Hdf5Meta;
using boost::filesystem::path;


//-----------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------

int main(int argc, char **argv)
{
//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

  //--------------------------------------------------------------------
  // Parse command-line arguments.
  //--------------------------------------------------------------------

    int error = false;
    std::string output_filename;
    std::string rdqi_config_filename;
    std::string message;

    if (argc >= 3) {
      output_filename = argv[1];
      rdqi_config_filename = argv[2];
    }
    else {
      error = 1;
      message = "Not enough command-line arguments.";
    }

  //--------------------------------------------------------------------
  // Print usage message
  //--------------------------------------------------------------------

    if (error) {
      cerr << message<< "\n";
      cerr << "create_out_of_band_rdqi_main <output filename> <rdqi config file>\n"
	   << "\n"
	   << "Create out-of-band RDQI file with the given RDQI parameters.\n"
	   << "Out-of-band matrix is set to the identity matrix.\n";
      return 1;
    }

//-----------------------------------------------------------------------
// Initialize constants
//-----------------------------------------------------------------------

    const size_t number_row = 13;
    const size_t number_col = 13;
    const size_t number_pixel = 1536;

//-----------------------------------------------------------------------
// Read RDQI
//-----------------------------------------------------------------------

    ConfigFile rdqi_config(rdqi_config_filename);
    boost::multi_array<int,2> rdqi = rdqi_config.get<int>("rdqi",number_pixel,number_row);

//-----------------------------------------------------------------------
// Create file.
//-----------------------------------------------------------------------

    {

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
	  data2[irow][ipixel] = rdqi[ipixel][irow];
	}
      }

      H5::H5File file(output_filename, H5F_ACC_TRUNC);
	
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
      meta.add<std::string>("granule_id",path(output_filename).filename());
      meta.hdf5_export(file.openGroup("/").getLocId());
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
