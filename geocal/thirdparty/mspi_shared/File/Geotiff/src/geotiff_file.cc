/*===========================================================================
=                                                                           =
=                               GeotiffFile                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!END=======================================================================*/

#include "geotiff_file.h"	// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <memory> 		// Definition of std::auto_ptr

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::GeotiffFile::GeotiffFile(
  const std::string& Filename
)
{
  //-------------------------------------------------------------------------
  // Initialize GDAL
  //-------------------------------------------------------------------------

  GDALAllRegister();

  //-------------------------------------------------------------------------
  // Open file.
  //-------------------------------------------------------------------------

  dataset_ = 
    boost::shared_ptr<GDALDataset>((GDALDataset*)GDALOpen(Filename.c_str(), GA_ReadOnly));
  if( dataset_.get() == NULL ) {
    throw MSPI_EXCEPTIONm("Trouble with GDALOpen, filename: " + Filename);
  }

  //-------------------------------------------------------------------------
  // Get the number of bands, lines and samples.
  //-------------------------------------------------------------------------

  number_band_ = dataset_->GetRasterCount();
  number_sample_ = dataset_->GetRasterXSize();
  number_line_ = dataset_->GetRasterYSize();

  //-------------------------------------------------------------------------
  // Get projection information
  //-------------------------------------------------------------------------

  std::string wkt(dataset_->GetProjectionRef());
  proj_info_ = ProjectionInformation(wkt);

  //-------------------------------------------------------------------------
  // Get map information.
  //-------------------------------------------------------------------------

  std::vector<double> map_param(6);
  CPLErr status = dataset_->GetGeoTransform(&map_param[0]);
  if (status != CE_None) {
    throw MSPI_EXCEPTIONm("Trouble with GetGeoTransform, filename: " + Filename);
  }
  map_info_ = MapInformation(map_param, number_sample_, number_line_);

}

