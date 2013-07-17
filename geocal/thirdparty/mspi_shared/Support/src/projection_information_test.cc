/*===========================================================================
=                                                                           =
=                                ProjectionInformation                      =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: projection_information_test.cc

!ABSTRACT:

   This does unit testing for the class ProjectionInformation.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "projection_information.h"           
				   // Definition of class to be tested.
#include <iostream>		// Definition of cerr.
#include <iomanip>		// Definition of setprecision.
#include <cmath> 		// Definition of fabs
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.
#include "MSPI-Shared/UnitTest/src/double_comparator.h"
				// Definition of DoubleComparator
#include <gdal/gdal_priv.h>	// Definition of GDALDataset
#include <fstream> 		// Definition of std::ofstream

using std::cerr;
using MSPI::Shared::ProjectionInformation;
using MSPI::Shared::Exception;
using MSPI::Shared::DoubleComparator;


int main()
{
  int test_number = 0;	// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  DoubleComparator cmp(20);

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor
//-----------------------------------------------------------------------

    test_number++;

    int proj_code_expect = 1;
    int sphere_code_expect = 12;
    int zone_code_expect = 10;
    double deg2rad = std::acos(-1.0) / 180.0;

    std::vector<double> proj_param_expect(ProjectionInformation::number_proj_param,0);

    ProjectionInformation proj_info(proj_code_expect, 
				    sphere_code_expect, 
				    zone_code_expect, 
				    proj_param_expect);

//-----------------------------------------------------------------------
// Test wkt()
//-----------------------------------------------------------------------

    std::string wkt_expect = "PROJCS[\"UTM Zone 10, Northern Hemisphere\",GEOGCS[\"Unknown datum based upon the WGS 84 ellipsoid\",DATUM[\"Not specified (based on WGS 84 spheroid)\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",-123],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],UNIT[\"Meter\",1]]";

    std::string wkt = proj_info.wkt();

    if (wkt != wkt_expect) {
      cerr << "wkt = " << proj_info.wkt() << "\n";
      cerr << "pretty wkt:\n" << proj_info.pretty_wkt() << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test constuctor(MSPI::Shared::ConfigFile&)
//-----------------------------------------------------------------------

    {
      test_number++;

      std::string config_filename = 
	"Support/src/projection_information_test.config";
      MSPI::Shared::ConfigFile config(config_filename);

      ProjectionInformation proj_info(config);
    
      if (proj_info.proj_code() != 1 ||
	  proj_info.zone_code() != 11 ||
	  proj_info.sphere_code() != 8) {
	cerr << "proj_code = " << proj_info.proj_code() << "\n";
	cerr << "zone_code = " << proj_info.zone_code() << "\n";
	cerr << "sphere_code = " << proj_info.sphere_code() << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      const std::vector<double>& proj_param = proj_info.proj_param();

      if (proj_param.size() != ProjectionInformation::number_proj_param) {
	cerr << "proj_param.size = " << proj_param.size() << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      std::vector<double> proj_param_expect(ProjectionInformation::number_proj_param,0);
      
      for (size_t i = 0 ; i < ProjectionInformation::number_proj_param ; ++i) {
	if (proj_param[i] != proj_param_expect[i]) {
	  cerr << std::setprecision(20);
	  cerr << "proj_param[" << i << "] = " << proj_param[i]
	       << " (expected " << proj_param_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test proj_param()
// Test proj_code()
// Test sphere_code()
// Test zone_code()
//-----------------------------------------------------------------------

    test_number++;

    const std::vector<double>& proj_param = proj_info.proj_param();
    
    if (proj_param.size() != proj_param_expect.size()) {
      cerr << "proj_param.size = " << proj_param.size() 
	   << " (expected " << proj_param_expect.size() << ")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

    for (unsigned int i = 0 ; i < proj_param_expect.size() ; ++i) {
      if (proj_param[i] != proj_param_expect[i]) {
	cerr << std::setprecision(20);
	cerr << "proj_param[" << i << "] = " << proj_param[i]
	     << " (expected " << proj_param_expect[i] << ")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    if (proj_info.proj_code() != proj_code_expect ||
	proj_info.sphere_code() != sphere_code_expect ||
	proj_info.zone_code() != zone_code_expect) {
      cerr << "proj_code = " << proj_info.proj_code() 
	   << " (expected " << proj_code_expect << ")\n";
      cerr << "sphere_code = " << proj_info.sphere_code() 
	   << " (expected " << sphere_code_expect << ")\n";
      cerr << "zone_code = " << proj_info.zone_code() 
	   << " (expected " << zone_code_expect << ")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test latlon_to_xy
//-----------------------------------------------------------------------

    test_number++;
    {
      std::vector<double> 
	proj_param(ProjectionInformation::number_proj_param,0.0);
      int projcode = 1; // UTM
      int zonecode = 34; // Zonecode 
      int spherecode = 8; // GRS 1980/WGS 84 (See sphdz.c in GCTP source)
      ProjectionInformation proj_info(projcode, 
				      spherecode, 
				      zonecode, 
				      proj_param);

      double x;
      double y;
      double lat = 10.0 * deg2rad;
      double lon = 20.0 * deg2rad;
      double x_expect = 3.90399227485530660488e+05;
      double y_expect = 1.10557858915649028495e+06;
	
      proj_info.latlon_to_xy(lat, lon, x, y);

      if (cmp.neq(x,x_expect) ||
	  cmp.neq(y,y_expect)) {
	cerr << "x = " << x << " (expected " << x_expect << ")\n";
	cerr << "y = " << y << " (expected " << y_expect << ")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test latlon_to_xy(std::vector<double>...)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::vector<double> 
	proj_param(ProjectionInformation::number_proj_param,0.0);
      int projcode = 1; // UTM
      int zonecode = 34; // Zonecode 
      int spherecode = 8; // GRS 1980/WGS 84 (See sphdz.c in GCTP source)
      ProjectionInformation proj_info(projcode, 
				      spherecode, 
				      zonecode, 
				      proj_param);

      const int number_point = 3;
      double lat_init[number_point] = {	11.0, 12.0, 13.0 }; // degrees
      double lon_init[number_point] = {	21.0, 22.0, 23.0 }; // degrees

      std::vector<double> lat(&lat_init[0], &lat_init[number_point]);
      std::vector<double> lon(&lon_init[0], &lon_init[number_point]);
      for (int i = 0 ; i < number_point ; i++) {
	lat[i] *= deg2rad; // convert to radians
	lon[i] *= deg2rad; // convert to radians
      }

      double x_expect_init[number_point] = { 500000, 
					     6.08864173370232689194e+05,
					     7.16922588177392375655e+05 };
      
      double y_expect_init[number_point] = { 1.21597943334914231673e+06,
					     1.32675117067412845790e+06,
					     1.43798764430226106197e+06 };

      std::vector<double> x_expect(&x_expect_init[0], &x_expect_init[number_point]);
      std::vector<double> y_expect(&y_expect_init[0], &y_expect_init[number_point]);
	
      std::vector<double> x;
      std::vector<double> y;
      
      proj_info.latlon_to_xy(lat, lon, x, y);

      for (int i = 0 ; i < number_point ; i++) {
	if (cmp.neq(x[i],x_expect[i])) {
	  cerr << "x = " << x[i] << " (expected " << x_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (cmp.neq(y[i],y_expect[i])) {
	  cerr << "y = " << y[i] << " (expected " << y_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test xy_to_latlon
//-----------------------------------------------------------------------

    test_number++;
    {
      std::vector<double> 
	proj_param(ProjectionInformation::number_proj_param,0.0);
      int projcode = 1; // UTM
      int zonecode = 34; // Zonecode 
      int spherecode = 8; // GRS 1980/WGS 84 (See sphdz.c in GCTP source)
      ProjectionInformation proj_info(projcode, 
				      spherecode, 
				      zonecode, 
				      proj_param);

      double x = 390400;
      double y = 110500;
      double lat;
      double lon;
      double lat_expect = 1.74459348291534609154e-02; // radians
      double lon_expect = 3.49326818860671073352e-01; // radians
	
      proj_info.xy_to_latlon(x, y, lat, lon);

      if (cmp.neq(lat,lat_expect) ||
	  cmp.neq(lon,lon_expect)) {
	cerr << "lat = " << lat << " (expected " << lat_expect << ")\n";
	cerr << "lon = " << lon << " (expected " << lon_expect << ")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test xy_to_latlon(std::vector<double>...)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::vector<double> 
	proj_param(ProjectionInformation::number_proj_param,0.0);
      int projcode = 1; // UTM
      int zonecode = 34; // Zonecode 
      int spherecode = 8; // GRS 1980/WGS 84 (See sphdz.c in GCTP source)
      ProjectionInformation proj_info(projcode, 
				      spherecode, 
				      zonecode, 
				      proj_param);

      const int number_point = 3;
      double x_init[number_point] = { 500000, 
				      6.08864173370212432928e+05,
				      7.16922588174997013994e+05 };
      
      double y_init[number_point] = { 1.21597943334904522635e+06,
				      1.32675117067404906265e+06,
				      1.43798764430213812739e+06 };

      std::vector<double> x(&x_init[0], &x_init[number_point]);
      std::vector<double> y(&y_init[0], &y_init[number_point]);

      double lat_expect_init[number_point] = {	1.91986217719360890976e-01,
						2.09439510239306875272e-01,
						2.26892802759237649513e-01 }; // radians
      double lon_expect_init[number_point] = {	3.66519142918808937814e-01, 
						3.83972435438745429703e-01,
						4.01425727958199363155e-01 }; // radians

      std::vector<double> lat_expect(&lat_expect_init[0], &lat_expect_init[number_point]);
      std::vector<double> lon_expect(&lon_expect_init[0], &lon_expect_init[number_point]);

      std::vector<double> lat;
      std::vector<double> lon;
      proj_info.xy_to_latlon(x, y, lat, lon);
      
      for (int i = 0 ; i < number_point ; i++) {
	if (cmp.neq(lat[i],lat_expect[i])) {
	  cerr << "lat["<<i<<"] = " << lat[i]
	       << " (expected " << lat_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
	if (cmp.neq(lon[i],lon_expect[i])) {
	  cerr << "lon["<<i<<"] = " << lon[i]
	       << " (expected " << lon_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test constructor(std::string Wkt)
//-----------------------------------------------------------------------

    test_number++;
    {
      GDALAllRegister();
      std::string filename = "Support/src/projection_information_test.tif";
      GDALDataset* dataset = (GDALDataset *)GDALOpen(filename.c_str(), GA_ReadOnly);
      if( dataset == NULL ) {
	cerr << "Trouble with GDALOpen, filename: " << filename << "\n";
	return 1;
      }
      std::string wkt(dataset->GetProjectionRef());
      
      ProjectionInformation proj_info(wkt);

      int proj_code_expect = 1;
      int sphere_code_expect = 12;
      int zone_code_expect = 11;

      if (proj_info.proj_code() != proj_code_expect ||
	  proj_info.sphere_code() != sphere_code_expect ||
	  proj_info.zone_code() != zone_code_expect) {
	cerr << "proj_code = " << proj_info.proj_code() 
	     << " (expected " << proj_code_expect << ")\n";
	cerr << "sphere_code = " << proj_info.sphere_code() 
	     << " (expected " << sphere_code_expect << ")\n";
	cerr << "zone_code = " << proj_info.zone_code() 
	     << " (expected " << zone_code_expect << ")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      std::vector<double> proj_param_expect(ProjectionInformation::number_proj_param,0);
      
      for (size_t i = 0 ; i < ProjectionInformation::number_proj_param ; ++i) {
	if (proj_param[i] != proj_param_expect[i]) {
	  cerr << std::setprecision(20);
	  cerr << "proj_param[" << i << "] = " << proj_param[i]
	       << " (expected " << proj_param_expect[i] << ")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }

    }

//-----------------------------------------------------------------------
// Test config()
//-----------------------------------------------------------------------

    {
      test_number++;

      int proj_code = 4;
      int sphere_code = 12;
      int zone_code = -1;
      
      double proj_param_init[ProjectionInformation::number_proj_param] = 
	{0, 0, 50018013.75, 60038018.98, 10000000.00, 55000000.00, 10, 20, 0, 0, 0, 0, 0, 0, 0};

      std::vector<double> proj_param(&proj_param_init[0],
				     &proj_param_init[ProjectionInformation::number_proj_param]);

      ProjectionInformation proj_info(proj_code, 
				      sphere_code, 
				      zone_code, 
				      proj_param);

      std::string config_expect =
	"0 0 50018013.75 60038018.98 10000000 55000000 10 20 0 0 0 0 0 0 0  : ProjectionInformation.proj_param\n"
	"4 : ProjectionInformation.proj_code\n"
	"0 : ProjectionInformation.zone_code\n"
	"12 : ProjectionInformation.sphere_code\n";

      std::string config = proj_info.config();

      if (config != config_expect) {
	cerr << "config:\n" << config << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      std::string filename = "Support/tmp/projection_information_test.config2";
      {
	std::ofstream file(filename.c_str(),std::ofstream::trunc);
	file << config;
      }

      MSPI::Shared::ConfigFile config_file(filename);
      ProjectionInformation proj_info_new(config_file);
      
      std::string config_new = proj_info_new.config();
      if (config_new != config_expect) {
	cerr << "config new:\n" << config_new << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
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
    cerr << "Caught Exception: " << e.what() << "\n";
  }
}
