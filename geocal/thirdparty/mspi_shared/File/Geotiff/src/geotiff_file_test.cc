/*===========================================================================
=                                                                           =
=                                GeotiffFile                                =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "geotiff_file.h"          // Definition of GeotiffFile
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.
#include <iomanip>		// Definition of std::setprecision
#include <fstream> 		// Definition of std::ofstream
#include <arpa/inet.h>          // Definition of htons

using std::cerr;
using MSPI::Shared::GeotiffFile;
using MSPI::Shared::Exception;
using MSPI::Shared::MapInformation;


/////////////////////////////////////////////////////////////////////////////
// Write ppm file.
/////////////////////////////////////////////////////////////////////////////

void write_ppm(
  const std::string& Filename,
  const boost::multi_array<unsigned char,2>& Data
)
{
  std::ofstream of(Filename.c_str());
  of << "P6\n" << Data.shape()[1] << " " << Data.shape()[0] << "\n";
  of << 255 << "\n";
  for (size_t iline = 0 ; iline < Data.shape()[0] ; iline++) {
    for (size_t isample = 0 ; isample < Data.shape()[1] ; isample++) {
      unsigned char value = Data[iline][isample];
      of.write((char *)&value, 1);
      of.write((char *)&value, 1);
      of.write((char *)&value, 1);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////////////////////////

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename = "File/Geotiff/src/geotiff_file_test.tif";
    GeotiffFile file(filename);

//-----------------------------------------------------------------------
// Test projection_info()
//-----------------------------------------------------------------------
    
    test_number++;

    {
    
      std::string wkt_expect = "PROJCS[\"WGS 84 / UTM zone 11N\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-117],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],UNIT[\"metre\",1,AUTHORITY[\"EPSG\",\"9001\"]],AUTHORITY[\"EPSG\",\"32611\"]]";

      std::string wkt = file.projection_info().wkt();

      if (wkt != wkt_expect) {
	cerr << "wkt = " << file.projection_info().wkt() << "\n";
	cerr << "pretty wkt:\n" << file.projection_info().pretty_wkt() << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test map_information()
//-----------------------------------------------------------------------

    test_number++;

    {
      MapInformation map_info = file.map_info();

      double min_x_expect = 229861.64680036710342;
      double min_y_expect = 4044278.384250250645;
      double max_x_expect = 276061.64680036710342;
      double max_y_expect = 4100978.384250250645;
      double res_x_expect = 300;
      double res_y_expect = 300;
      double size_x_expect = 154;
      double size_y_expect = 189;

      if (map_info.min_x() != min_x_expect ||
	  map_info.min_y() != min_y_expect ||
	  map_info.max_x() != max_x_expect ||
	  map_info.max_y() != max_y_expect ||
	  map_info.resolution_x() != res_x_expect ||
	  map_info.resolution_y() != res_y_expect ||
	  map_info.grid_size_x() != size_x_expect ||
	  map_info.grid_size_y() != size_y_expect) {
	cerr << std::setprecision(20);
	cerr << "min_x = " << map_info.min_x() 
	     << " (expected " << min_x_expect << ")\n";
	cerr << "min_y = " << map_info.min_y() 
	     << " (expected " << min_y_expect << ")\n";
	cerr << "max_x = " << map_info.max_x() 
	     << " (expected " << max_x_expect << ")\n";
	cerr << "max_y = " << map_info.max_y() 
	     << " (expected " << max_y_expect << ")\n";
	cerr << "res_x = " << map_info.resolution_x() 
	     << " (expected " << res_x_expect << ")\n";
	cerr << "res_y = " << map_info.resolution_y() 
	     << " (expected " << res_y_expect << ")\n";
	cerr << "size_x = " << map_info.grid_size_x() 
	     << " (expected " << size_x_expect << ")\n";
	cerr << "size_y = " << map_info.grid_size_y() 
	     << " (expected " << size_y_expect << ")\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test number_line()
// Test number_sample()
// Test number_band()
//-----------------------------------------------------------------------

    
    test_number++;

    {
      if (file.number_band() != 1 ||
	  file.number_line() != 189 ||
	  file.number_sample() != 154) {
	cerr << "number_band = " << file.number_band() << "\n";
	cerr << "number_line = " << file.number_line() << "\n";
	cerr << "number_sample = " << file.number_sample() << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test raster()
//-----------------------------------------------------------------------
    
    test_number++;

    {
      int band = 0;
      size_t number_line_expect = 189;
      size_t number_sample_expect = 154;
      boost::multi_array<unsigned char,2> raster = file.raster<unsigned char>(band);
      if (raster.shape()[0] != number_line_expect ||
	  raster.shape()[1] != number_sample_expect) {
	cerr << "raster_number_line = " << raster.shape()[0] << "\n";
	cerr << "raster_number_sample = " << raster.shape()[1] << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
      write_ppm("File/Geotiff/tmp/geotiff_file_test.ppm",raster);
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
