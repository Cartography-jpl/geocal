/*===========================================================================
=                                                                           =
=                             Rdqi                                          =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "rdqi.h"
				// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "H5Cpp.h"		// Definition of H5::H5File

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Rdqi::Rdqi(
  const std::vector<int>& Row_number,
  const boost::multi_array<float,2>& Data
  ///< (pixel, row)
)
{
  init(Row_number, Data);
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Rdqi::Rdqi(
  const std::string& Filename
)
{
  //---------------------------------------------------------------
  // Open file
  //---------------------------------------------------------------

  H5::H5File file(Filename, H5F_ACC_RDONLY);

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
  // Read data quality indicator.
  //---------------------------------------------------------------

  boost::multi_array<float,2> data;

  {
    H5::DataSet dataset = file.openDataSet("RDQI");
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
  
    if (rank != 2) {
      throw MSPI_EXCEPTIONm("Unexpected dataset rank, filename: "+Filename);
    }
    
    hsize_t hdims[rank];
    int ndims = dataspace.getSimpleExtentDims(hdims, NULL);

    if (ndims != rank) {
      throw MSPI_EXCEPTIONm("Unexpected dataspace ndims, filename: "+Filename);
    }

    number_pixel_ = hdims[1];

    data.resize(std::vector<size_t>(&hdims[0],&hdims[rank]));
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
MSPI::Shared::Rdqi::init(
  const std::vector<int>& Row_number,
  const boost::multi_array<float,2>& Data
  ///< (pixel, row)
)
{
  //---------------------------------------------------------------
  // Initialize private attributes.
  //---------------------------------------------------------------

  row_number_ = Row_number;

  //---------------------------------------------------------------
  // Get the dimension sizes.
  //---------------------------------------------------------------

  number_row_ = Data.shape()[0];
  number_pixel_ = Data.shape()[1];
  
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
  // Reorganize data
  //---------------------------------------------------------------
    
  data_.resize(number_row_);

  for (size_t irow = 0 ; irow < number_row_ ; irow++) {
    data_[irow].resize(number_pixel_);

    for (size_t ipixel = 0 ; ipixel < number_pixel_ ; ipixel++) {
      data_[irow][ipixel] = Data[irow][ipixel];
    }
  }
}
