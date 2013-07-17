//===========================================================================
//                                                                         
//                            OutOfBandCalibrationData                     
//                                                                         
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "out_of_band_calibration_data.h"
				// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "H5Cpp.h"		// Definition of H5::H5File

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::OutOfBandCalibrationData::OutOfBandCalibrationData(
  const std::vector<int>& Row_number,
  const boost::multi_array<float,3>& Data
  ///< (pixel, row, col)
)
{
  init(Row_number, Data);
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::OutOfBandCalibrationData::OutOfBandCalibrationData(
  const std::string& Filename
)
{
  //---------------------------------------------------------------
  // Open file
  //---------------------------------------------------------------

  H5::H5File file(Filename, H5F_ACC_RDONLY);

  //---------------------------------------------------------------
  // Initialize metadata.
  //---------------------------------------------------------------

  meta_ = Hdf5Meta::hdf5_import(file.openGroup("/").getLocId());

  //---------------------------------------------------------------
  // Read row numbers.
  // Construct mapping from row number to row index.
  // Get the number of rows.
  //---------------------------------------------------------------

  std::vector<int> row_number;

  { 
    H5::DataSet dataset = file.openDataSet("RowNumber");
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    
    if (rank != 1) {
      throw MSPI_EXCEPTIONm("Unexpected dataset rank, filename: "+Filename);
    }
    
    hsize_t hdims[rank];
    int ndims = dataspace.getSimpleExtentDims(hdims, NULL);
    
    if (ndims != rank) {
      throw MSPI_EXCEPTIONm("Unexpected dataspace ndims, filename: "+Filename);
    }

    row_number.resize(hdims[0]);
    dataset.read(&(row_number[0]), H5::PredType::NATIVE_INT);
  }

  //---------------------------------------------------------------
  // Read coefficients.
  //---------------------------------------------------------------

  boost::multi_array<float,3> data;

  { 
    H5::DataSet dataset = file.openDataSet("OutOfBandCalibration");
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    
    if (rank != 3) {
      throw MSPI_EXCEPTIONm("Unexpected dataset rank, filename: "+Filename);
    }
    
    hsize_t hdims[rank];
    int ndims = dataspace.getSimpleExtentDims(hdims, NULL);
    
    if (ndims != rank) {
      throw MSPI_EXCEPTIONm("Unexpected dataspace ndims, filename: "+Filename);
    }

    number_pixel_ = hdims[0];
    number_col_ = hdims[2];

    data.resize(std::vector<size_t>(&hdims[0],&hdims[3]));
    dataset.read(data.data(), H5::PredType::NATIVE_FLOAT);
  }
  
  //---------------------------------------------------------------
  // Initialize class.
  //---------------------------------------------------------------

  init(row_number, data);

}

/////////////////////////////////////////////////////////////////////////////
/// Initializer used by constructors.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::OutOfBandCalibrationData::init(
  const std::vector<int>& Row_number,
  const boost::multi_array<float,3>& Data
  ///< (pixel, row, col)
)
{
  //---------------------------------------------------------------
  // Initialize private attributes.
  //---------------------------------------------------------------

  row_number_ = Row_number;

  //---------------------------------------------------------------
  // Get the dimension sizes.
  //---------------------------------------------------------------

  number_pixel_ = Data.shape()[0];
  number_row_ = Data.shape()[1];
  number_col_ = Data.shape()[2];
  
  //---------------------------------------------------------------
  // Argument check: Row_number.size() == number_row_
  //---------------------------------------------------------------

  if (Row_number.size() != number_row_) {
    throw MSPI_EXCEPTIONm("Row_number.size() != number_row_");
  }

  //---------------------------------------------------------------
  // Construct mapping from row number to row index.
  //---------------------------------------------------------------

  for (size_t irow = 0 ; irow < number_row_ ; irow++) {
    row_index_[Row_number[irow]] = irow;
  }

  //---------------------------------------------------------------
  // Reorganize file data storage order from (pixel,row,col) to 
  // (row,col,pixel) for more efficient application access.
  //---------------------------------------------------------------
    
  data_.resize(number_row_);

  std::vector<size_t> dims(2);
  dims[0] = number_col_;
  dims[1] = number_pixel_;

  for (size_t irow = 0 ; irow < number_row_ ; irow++) {
    data_[irow].resize(dims);

    for (size_t icol = 0 ; icol < number_col_ ; icol++) {
      for (size_t ipixel = 0 ; ipixel < number_pixel_ ; ipixel++) {
	data_[irow][icol][ipixel] = Data[ipixel][irow][icol];
      }
    }
  }
}
