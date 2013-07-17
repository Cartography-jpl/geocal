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

#ifndef GEOTIFF_FILE_H
#define GEOTIFF_FILE_H

#include <gdal/gdal_priv.h>	// Definition of GDALDataset
#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "MSPI-Shared/Support/src/projection_information.h"
				// Definition of ProjectionInformation
#include "MSPI-Shared/Support/src/map_information.h"
				// Definition of MapInformation
#include "boost/shared_ptr.hpp"	// Definition of boost::shared_ptr

namespace MSPI {
  namespace Shared {
    class GeotiffFile;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief File access interface for geotiff files.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::GeotiffFile {
public:
  GeotiffFile(const std::string& Filename);
  const ProjectionInformation& projection_info() const;
  const MapInformation& map_info() const;
  int number_line() const;
  int number_sample() const;
  int number_band() const;
  template <class T> boost::multi_array<T, 2> raster(int Band) const;
private:
  template <class T> int datatype() const;
  boost::shared_ptr<GDALDataset> dataset_;
  int number_band_;
  int number_line_;
  int number_sample_;
  ProjectionInformation proj_info_;
  MapInformation map_info_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return projection information
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::ProjectionInformation& 
MSPI::Shared::GeotiffFile::projection_info() const
{
  return proj_info_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return map information
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::MapInformation& 
MSPI::Shared::GeotiffFile::map_info() const
{
  return map_info_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return number of lines.
/////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::GeotiffFile::number_line() const
{
  return number_line_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return number of samples per line.
/////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::GeotiffFile::number_sample() const
{
  return number_sample_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return number of bands.
/////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::GeotiffFile::number_band() const
{
  return number_band_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return GDAL datatype identifier
/// Note: template specializations must be inside a namespace block.
/////////////////////////////////////////////////////////////////////////////

namespace MSPI {
  namespace Shared {
    template <> inline int GeotiffFile::datatype<unsigned char>() const { return GDT_Byte; }
    template <> inline int GeotiffFile::datatype<unsigned short>() const { return GDT_UInt16; }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Return raster data for the given band number (0-based).
/////////////////////////////////////////////////////////////////////////////

template <class T>
inline boost::multi_array<T,2>
MSPI::Shared::GeotiffFile::raster(int Band) const
{
  //------------------------------------------------------------------------
  // Argument check: Band > number_band_
  //------------------------------------------------------------------------

  if (Band > number_band_ - 1) {
    throw MSPI_EXCEPTIONm("Out-of-range band number in GeotiffFile::raster()");
  }

  //------------------------------------------------------------------------
  // Get raster.
  //------------------------------------------------------------------------

  GDALRasterBand *rb = dataset_->GetRasterBand(Band+1);

  //------------------------------------------------------------------------
  // Check datatype.
  //------------------------------------------------------------------------

  GDALDataType data_type = rb->GetRasterDataType();
  if (data_type != datatype<T>()) {
    throw MSPI_EXCEPTIONm("Unexpected datatype in GeotiffFile::raster()");
  }

  //------------------------------------------------------------------------
  // Read raster.
  //------------------------------------------------------------------------

  boost::multi_array<T,2> 
    raster(boost::extents[number_line_][number_sample_]);

  CPLErr status = rb->RasterIO(GF_Read, 0, 0, number_sample_, number_line_, 
			       raster.data(), 
			       number_sample_, number_line_, data_type,
			       0, 0);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with RasterIO in GeotiffFile::raster()");
  }

  //------------------------------------------------------------------------
  // Return
  //------------------------------------------------------------------------

  return raster;
}

#endif

