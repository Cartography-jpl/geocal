/*===========================================================================
=                                                                           =
=                                MapInformation                             =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: map_information_test.cc

!ABSTRACT:

   This does unit testing for the class MapInformation.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "map_information.h"           
				   // Definition of class to be tested.
#include <iostream>		// Definition of cerr.
#include <iomanip> 		// Definition of setprecision.
#include <cmath> 		// Definition of fabs
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of Exception.
#include "MSPI-Shared/UnitTest/src/double_comparator.h"
				// Definition of DoubleComparator
#include <fstream> 		// Definition of std::ofstream

using std::cerr;
using MSPI::Shared::MapInformation;
using MSPI::Shared::BoxRegionCoor;
using MSPI::Shared::Exception;
using MSPI::Shared::DoubleComparator;


int main()
{
  bool error = false;		// Flag indicating test failure.
  DoubleComparator cmp(10.0);	// Double comparator
  int test_number = 0;		// Number identifying this test. 

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constuctor.
//-----------------------------------------------------------------------

    test_number++;
    double min_x = -1.9;
    double min_y = 2.8;
    double max_x = 3.7;
    double max_y = 4.6;
    int grid_size_x = 5;
    int grid_size_y = 6;

    MapInformation map_info(max_x, max_y, min_x, min_y, grid_size_x, 
			    grid_size_y, MapInformation::ORIGIN_LR, 
			    MapInformation::PIX_REG_CORNER); 

    double resolution_x_expect = (max_x - min_x) / grid_size_x;
    double resolution_y_expect = (max_y - min_y) / grid_size_y;
    
    if (map_info.min_x() != min_x ||
	map_info.min_y() != min_y ||
	map_info.max_x() != max_x ||
	map_info.max_y() != max_y ||
	map_info.grid_size_x() != grid_size_x ||
	map_info.grid_size_y() != grid_size_y ||
	map_info.resolution_x() != resolution_x_expect ||
	map_info.resolution_y() != resolution_y_expect) {
      cerr << "min_x = " << map_info.min_x() 
	   << " (expected " << min_x << ")\n";
      cerr << "min_y = " << map_info.min_y() 
	   << " (expected " << min_y << ")\n";
      cerr << "max_x = " << map_info.max_x() 
	   << " (expected " << max_x << ")\n";
      cerr << "max_y = " << map_info.max_y() 
	   << " (expected " << max_y << ")\n";
      cerr << "grid_size_x = " << map_info.grid_size_x() 
	   << " (expected " << grid_size_x << ")\n";
      cerr << "grid_size_y = " << map_info.grid_size_y() 
	   << " (expected " << grid_size_y << ")\n";
      cerr << "resolution_x = " << map_info.resolution_x() 
	   << " (expected " << resolution_x_expect << ")\n";
      cerr << "resolution_y = " << map_info.resolution_y() 
	   << " (expected " << resolution_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test config()
//-----------------------------------------------------------------------

    {
      test_number++;
      std::string config_expect = 
	"-1.9 : MapInformation.min_x\n"
	"3.7 : MapInformation.max_x\n"
	"2.8 : MapInformation.min_y\n"
	"4.6 : MapInformation.max_y\n"
	"1.12 : MapInformation.res_x\n"
	"0.3 : MapInformation.res_y\n"
	"3 : MapInformation.origin_code\n"
	"1 : MapInformation.pix_reg_code\n";

      std::string config = map_info.config();

      if (config != config_expect) {
	cerr << "config:\n" << config << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      std::string filename = "Support/tmp/map_information_test.config2";
      {
	std::ofstream file(filename.c_str(),std::ofstream::trunc);
	file << config;
      }

      MSPI::Shared::ConfigFile config_file(filename);
      MapInformation map_info_new(config_file);
      
      std::string config_new = map_info_new.config();
      if (config_new != config_expect) {
	cerr << "config new:\n" << config_new << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test constuctor(MSPI::Shared::ConfigFile&)
//-----------------------------------------------------------------------

    {
      test_number++;
      DoubleComparator cmp;	// Double comparator

      std::string config_filename = 
	"Support/src/map_information_test.config";
      MSPI::Shared::ConfigFile config(config_filename);

      MapInformation map_info(config);
    
      if (cmp.neq(map_info.min_x(), -10.9) ||
	  cmp.neq(map_info.min_y(), 20.8) ||
	  cmp.neq(map_info.max_x(), 30.7) ||
	  cmp.neq(map_info.max_y(), 40.6) ||
	  map_info.grid_size_x() != 208 ||
	  map_info.grid_size_y() != 198 ||
	  cmp.neq(map_info.resolution_x(),0.2) ||
	  cmp.neq(map_info.resolution_y(),0.1)) {
	cerr << "min_x = " << map_info.min_x() << "\n";
	cerr << "min_y = " << map_info.min_y() << "\n";
	cerr << "max_x = " << map_info.max_x() << "\n";
	cerr << "max_y = " << map_info.max_y() << "\n";
	cerr << "grid_size_x = " << map_info.grid_size_x() << "\n";
	cerr << "grid_size_y = " << map_info.grid_size_y() << "\n";
	cerr << "resolution_x = " << map_info.resolution_x() << "\n";
	cerr << "resolution_y = " << map_info.resolution_y() << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }

      if (map_info.origin_code() != MapInformation::ORIGIN_LL ||
	  map_info.pix_reg_code() != MapInformation::PIX_REG_CORNER) {
	cerr << "origin_code = " << map_info.origin_code() << "\n";
	cerr << "pix_reg_code = " << map_info.pix_reg_code() << "\n";
	cerr << "Unexpected result at line "<< __LINE__ << "\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test map_coor_to_pixel_coor  (origin = UL; pix reg = CORNER)
//-----------------------------------------------------------------------

    test_number++;
    double pixel_coor_x;
    double pixel_coor_y;
    double map_coor_x = 7.7;
    double map_coor_y = 8.8;

    double pixel_coor_x_expect;	/* Expected result. */
    double pixel_coor_y_expect;	/* Expected result. */

    MapInformation map_info2(max_x, max_y, min_x, min_y, grid_size_x, 
			    grid_size_y, MapInformation::ORIGIN_UL, 
			    MapInformation::PIX_REG_CORNER);

    pixel_coor_x_expect =
      grid_size_x * (map_coor_x - min_x) / (max_x - min_x) ;
    pixel_coor_y_expect =
      grid_size_y * (max_y - map_coor_y) / (max_y - min_y) ;
    
    map_info2.map_coor_to_pixel_coor(map_coor_x, map_coor_y,
				     pixel_coor_x, pixel_coor_y);
    if (cmp.neq(pixel_coor_x, pixel_coor_x_expect) ||
	cmp.neq(pixel_coor_y, pixel_coor_y_expect)) {
      cerr << "pixel_coor_x = " << pixel_coor_x 
	   << " (expected " << pixel_coor_x_expect << ")\n";
      cerr << "pixel_coor_y = " << pixel_coor_y 
	   << " (expected " << pixel_coor_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test map_coor_to_pixel_coor  (origin = UR; pix reg = CORNER)
//-----------------------------------------------------------------------

    test_number++;
    MapInformation map_info3(max_x, max_y, min_x, min_y, grid_size_x, 
			    grid_size_y, MapInformation::ORIGIN_UR, 
			    MapInformation::PIX_REG_CORNER);

    pixel_coor_x_expect = 
      grid_size_x * (max_x - map_coor_x) / (max_x - min_x) ;
    pixel_coor_y_expect = 
      grid_size_y * (max_y - map_coor_y) / (max_y - min_y) ;
    
    map_info3.map_coor_to_pixel_coor(map_coor_x, map_coor_y,
				     pixel_coor_x, pixel_coor_y);
    if (cmp.neq(pixel_coor_x, pixel_coor_x_expect) ||
	cmp.neq(pixel_coor_y, pixel_coor_y_expect)) {
      cerr << "pixel_coor_x = " << pixel_coor_x 
	   << " (expected " << pixel_coor_x_expect << ")\n";
      cerr << "pixel_coor_y = " << pixel_coor_y 
	   << " (expected " << pixel_coor_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test map_coor_to_pixel_coor  (origin = LL; pix reg = CORNER)
//-----------------------------------------------------------------------

    test_number++;
    MapInformation map_info4(max_x, max_y, min_x, min_y, grid_size_x, 
			    grid_size_y, MapInformation::ORIGIN_LL, 
			    MapInformation::PIX_REG_CORNER);

    pixel_coor_x_expect = 
      grid_size_x * (map_coor_x - min_x) / (max_x - min_x) ;
    pixel_coor_y_expect = 
      grid_size_y * (map_coor_y - min_y) / (max_y - min_y) ;
    
    map_info4.map_coor_to_pixel_coor(map_coor_x, map_coor_y,
				     pixel_coor_x, pixel_coor_y);
    if (cmp.neq(pixel_coor_x, pixel_coor_x_expect) ||
	cmp.neq(pixel_coor_y, pixel_coor_y_expect)) {
      cerr << "pixel_coor_x = " << pixel_coor_x 
	   << " (expected " << pixel_coor_x_expect << ")\n";
      cerr << "pixel_coor_y = " << pixel_coor_y 
	   << " (expected " << pixel_coor_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test map_coor_to_pixel_coor  (origin = LR; pix reg = CORNER)
//-----------------------------------------------------------------------

    test_number++;
    MapInformation map_info5(max_x, max_y, min_x, min_y, grid_size_x, 
			    grid_size_y, MapInformation::ORIGIN_LR, 
			    MapInformation::PIX_REG_CORNER);

    pixel_coor_x_expect = 
      grid_size_x * (max_x - map_coor_x) / (max_x - min_x) ;
    pixel_coor_y_expect = 
      grid_size_y * (map_coor_y - min_y) / (max_y - min_y) ;
    
    map_info5.map_coor_to_pixel_coor(map_coor_x, map_coor_y,
				     pixel_coor_x, pixel_coor_y);
    if (cmp.neq(pixel_coor_x, pixel_coor_x_expect) ||
	cmp.neq(pixel_coor_y, pixel_coor_y_expect)) {
      cerr << "pixel_coor_x = " << pixel_coor_x 
	   << " (expected " << pixel_coor_x_expect << ")\n";
      cerr << "pixel_coor_y = " << pixel_coor_y 
	   << " (expected " << pixel_coor_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test map_coor_to_pixel_coor  (origin = LR; pix reg = CENTER)
//-----------------------------------------------------------------------

    test_number++;
    MapInformation map_info6(max_x, max_y, min_x, min_y, grid_size_x, 
			    grid_size_y, MapInformation::ORIGIN_LR, 
			    MapInformation::PIX_REG_CENTER);

    pixel_coor_x_expect = 
      grid_size_x * (max_x - map_coor_x) / (max_x - min_x) - 0.5;
    pixel_coor_y_expect = 
      grid_size_y * (map_coor_y - min_y) / (max_y - min_y) - 0.5;
    
    map_info6.map_coor_to_pixel_coor(map_coor_x, map_coor_y,
				     pixel_coor_x, pixel_coor_y);
    if (cmp.neq(pixel_coor_x, pixel_coor_x_expect) ||
	cmp.neq(pixel_coor_y, pixel_coor_y_expect)) {
      cerr << "pixel_coor_x = " << pixel_coor_x 
	   << " (expected " << pixel_coor_x_expect << ")\n";
      cerr << "pixel_coor_y = " << pixel_coor_y 
	   << " (expected " << pixel_coor_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test pixel_coor_to_map_coor  (origin = LR; pix reg = CENTER)
//-----------------------------------------------------------------------

    test_number++;
    double map_coor_x_expect;	/* Expected result. */
    double map_coor_y_expect;	/* Expected result. */

    MapInformation map_info7(max_x, max_y, min_x, min_y, grid_size_x, 
			    grid_size_y, MapInformation::ORIGIN_LR, 
			    MapInformation::PIX_REG_CENTER);

    pixel_coor_x = 10.1;
    pixel_coor_y = 20.2;
      
    map_coor_x_expect = 
      max_x - (pixel_coor_x + 0.5) * resolution_x_expect;
    map_coor_y_expect = 
      (pixel_coor_y + 0.5) * resolution_y_expect + min_y;
    
    map_info7.pixel_coor_to_map_coor(pixel_coor_x, pixel_coor_y,
				     map_coor_x, map_coor_y);
    if (cmp.neq(map_coor_x, map_coor_x_expect) ||
	cmp.neq(map_coor_y, map_coor_y_expect)) {
      cerr << "map_coor_x = " << map_coor_x 
	   << " (expected " << map_coor_x_expect << ")\n";
      cerr << "map_coor_y = " << map_coor_y 
	   << " (expected " << map_coor_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test map_coor_to_pixel_coor(region)
//-----------------------------------------------------------------------

    test_number++;
    MapInformation map_info8(max_x, max_y, min_x, min_y, grid_size_x, 
			     grid_size_y, MapInformation::ORIGIN_LR,
			     MapInformation::PIX_REG_CORNER);

    BoxRegionCoor map_coor(7.7, 8.8, 7.8, 8.9);
    BoxRegionCoor pixel_coor_expect(
       grid_size_x * (max_x - map_coor.min_x()) / (max_x - min_x),
       grid_size_y * (map_coor.min_y() - min_y) / (max_y - min_y),
       grid_size_x * (max_x - map_coor.max_x()) / (max_x - min_x),
       grid_size_y * (map_coor.max_y() - min_y) / (max_y - min_y));
    
    BoxRegionCoor pixel_coor = map_info8.map_coor_to_pixel_coor(map_coor);
    
    if (pixel_coor != pixel_coor_expect) {
      cerr << "pixel_coor.min_x = " << pixel_coor.min_x()
	   << " (expected " << pixel_coor_expect.min_x() << ")\n";
      cerr << "pixel_coor.min_y = " << pixel_coor.min_y()
	   << " (expected " << pixel_coor_expect.min_y() << ")\n";
      cerr << "pixel_coor.max_x = " << pixel_coor.max_x()
	   << " (expected " << pixel_coor_expect.max_x() << ")\n";
      cerr << "pixel_coor.max_y = " << pixel_coor.max_y()
	   << " (expected " << pixel_coor_expect.max_y() << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test pixel_coor_to_map_coor(region)
//-----------------------------------------------------------------------

    test_number++;
    MapInformation map_info9(max_x, max_y, min_x, min_y, grid_size_x, 
			     grid_size_y, MapInformation::ORIGIN_LR,
			     MapInformation::PIX_REG_CENTER);

    pixel_coor = BoxRegionCoor(10.1, 10.2, 11.2, 11.3);
    BoxRegionCoor map_coor_expect(
       max_x - (pixel_coor.min_x() + 0.5) * resolution_x_expect,
       (pixel_coor.min_y() + 0.5) * resolution_y_expect + min_y,
       max_x - (pixel_coor.max_x() + 0.5) * resolution_x_expect,
       (pixel_coor.max_y() + 0.5) * resolution_y_expect + min_y);

    map_coor = map_info9.pixel_coor_to_map_coor(pixel_coor);
    
    if (map_coor != map_coor_expect) {
      cerr << "map_coor.min_x = " << map_coor.min_x()
	   << " (expected " << map_coor_expect.min_x() << ")\n";
      cerr << "map_coor.min_y = " << map_coor.min_y()
	   << " (expected " << map_coor_expect.min_y() << ")\n";
      cerr << "map_coor.max_x = " << map_coor.max_x()
	   << " (expected " << map_coor_expect.max_x() << ")\n";
      cerr << "map_coor.max_y = " << map_coor.max_y()
	   << " (expected " << map_coor_expect.max_y() << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test origin_code()
// Test pix_reg_code()
//-----------------------------------------------------------------------

    test_number++;
    MapInformation::OriginCode origin_code_expect = 
      MapInformation::ORIGIN_LR;
    MapInformation::PixRegCode pix_reg_code_expect = 
      MapInformation::PIX_REG_CENTER;

    if (map_info9.origin_code() != origin_code_expect ||
	map_info9.pix_reg_code() != pix_reg_code_expect) {
      cerr << "origin_code = " << map_info9.origin_code()
	   << " (expected " << origin_code_expect << ")\n";
      cerr << "pix_reg_code = " << map_info9.pix_reg_code()
	   << " (expected " << pix_reg_code_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test alternate constructor.
//-----------------------------------------------------------------------

    test_number++;
    double resolution_x = (max_x - min_x) / 49.999;
    double resolution_y = (max_y - min_y) / 60.001;

    MapInformation map_info10(max_x, max_y, min_x, min_y,
			     resolution_x, resolution_y,
			     origin_code_expect,
			     pix_reg_code_expect);

    int grid_size_x_expect = 50;
    int grid_size_y_expect = 61;
    double max_x_expect = min_x + grid_size_x_expect * resolution_x;
    double max_y_expect = min_y + grid_size_y_expect * resolution_y;
    
    if (map_info10.min_x() != min_x ||
	map_info10.min_y() != min_y ||
	cmp.neq(map_info10.max_x(), max_x_expect) ||
	cmp.neq(map_info10.max_y(), max_y_expect) ||
	map_info10.grid_size_x() != grid_size_x_expect ||
	map_info10.grid_size_y() != grid_size_y_expect ||
	cmp.neq(map_info10.resolution_x(), resolution_x) ||
	cmp.neq(map_info10.resolution_y(), resolution_y) ||
	map_info10.origin_code() != origin_code_expect ||
	map_info10.pix_reg_code() != pix_reg_code_expect) {
      cerr << std::setprecision(20);
      cerr << "min_x = " << map_info10.min_x() 
	   << " (expected " << min_x << ")\n";
      cerr << "min_y = " << map_info10.min_y() 
	   << " (expected " << min_y << ")\n";
      cerr << "max_x = " << map_info10.max_x() 
	   << " (expected " << max_x_expect << ")\n";
      cerr << "max_y = " << map_info10.max_y() 
	   << " (expected " << max_y_expect << ")\n";
      cerr << "grid_size_x = " << map_info10.grid_size_x() 
	   << " (expected " << grid_size_x_expect << ")\n";
      cerr << "grid_size_y = " << map_info10.grid_size_y() 
	   << " (expected " << grid_size_y_expect << ")\n";
      cerr << "resolution_x = " << map_info10.resolution_x() 
	   << " (expected " << resolution_x << ")\n";
      cerr << "resolution_y = " << map_info10.resolution_y() 
	   << " (expected " << resolution_y << ")\n";
      cerr << "origin_code = " << map_info10.origin_code()
	   << " (expected " << origin_code_expect << ")\n";
      cerr << "pix_reg_code = " << map_info10.pix_reg_code()
	   << " (expected " << pix_reg_code_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test map_coor_to_pixel_coor(const BoxRegionCoor&, int&, int&, int&, int&)
//-----------------------------------------------------------------------

    test_number++;
    MapInformation map_info11(max_x, max_y, min_x, min_y, grid_size_x,
			      grid_size_y, MapInformation::ORIGIN_LL,
			      MapInformation::PIX_REG_CENTER);

    BoxRegionCoor map_coor11(min_x + map_info11.resolution_x() * 0.5,
			     min_y + map_info11.resolution_y() * 1.5,
			     min_x + map_info11.resolution_x() * 3.5,
			     min_y + map_info11.resolution_y() * 5.5);

    int imin_x_expect = 1;
    int imin_y_expect = 2;
    int isize_x_expect = 3;
    int isize_y_expect = 4;

    int imin_x;
    int imin_y;
    int isize_x;
    int isize_y;

    map_info11.map_coor_to_pixel_coor(map_coor11, imin_x, imin_y, 
				      isize_x, isize_y);

    if (imin_x != imin_x_expect ||
	imin_y != imin_y_expect ||
	isize_x != isize_x_expect ||
	isize_y != isize_y_expect) {
      cerr << "min_x = " << imin_x 
	   << " (expected " << imin_x_expect << ")\n";
      cerr << "min_y = " << imin_y 
	   << " (expected " << imin_y_expect << ")\n";
      cerr << "size_x = " << isize_x 
	   << " (expected " << isize_x_expect << ")\n";
      cerr << "size_y = " << isize_y 
	   << " (expected " << isize_y_expect << ")\n";
      cerr << "Unexpected result at line "<< __LINE__ << "\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test constuctor(std::vector<double>,int,int)
//-----------------------------------------------------------------------

    {
      test_number++;
      std::vector<double> t(6);
      double resolution_x = 10;
      double resolution_y = 5;
      t[0] = 100000;
      t[3] = 300000;
      t[1] = resolution_x;
      t[4] = 0;
      t[2] = 0;
      t[5] = -resolution_y;
      int number_sample = 4000;
      int number_line = 5000;
      
      MapInformation map_info(t, number_sample, number_line);

      double min_x_expect = 100000;
      double min_y_expect = 275000;
      double max_x_expect = 140000;
      double max_y_expect = 300000;

      MapInformation::OriginCode origin_code_expect = 
	MapInformation::ORIGIN_UL;
      MapInformation::PixRegCode pix_reg_code_expect = 
	MapInformation::PIX_REG_CENTER;
      
      if (map_info.min_x() != min_x_expect ||
	  map_info.min_y() != min_y_expect ||
	  map_info.max_x() != max_x_expect ||
	  map_info.max_y() != max_y_expect ||
	  map_info.grid_size_x() != number_sample ||
	  map_info.grid_size_y() != number_line ||
	  map_info.resolution_x() != resolution_x ||
	  map_info.resolution_y() != resolution_y ||
	  map_info.origin_code() != origin_code_expect ||
	  map_info.pix_reg_code() != pix_reg_code_expect) {
	cerr << "min_x = " << map_info.min_x() 
	     << " (expected " << min_x_expect << ")\n";
	cerr << "min_y = " << map_info.min_y() 
	     << " (expected " << min_y_expect << ")\n";
	cerr << "max_x = " << map_info.max_x() 
	     << " (expected " << max_x_expect << ")\n";
	cerr << "max_y = " << map_info.max_y() 
	     << " (expected " << max_y_expect << ")\n";
	cerr << "grid_size_x = " << map_info.grid_size_x() 
	     << " (expected " << number_sample << ")\n";
	cerr << "grid_size_y = " << map_info.grid_size_y() 
	     << " (expected " << number_line << ")\n";
	cerr << "resolution_x = " << map_info.resolution_x() 
	     << " (expected " << resolution_x_expect << ")\n";
	cerr << "resolution_y = " << map_info.resolution_y() 
	     << " (expected " << resolution_y_expect << ")\n";
	cerr << "origin_code = " << map_info.origin_code()
	     << " (expected " << origin_code_expect << ")\n";
	cerr << "pix_reg_code = " << map_info.pix_reg_code()
	     << " (expected " << pix_reg_code_expect << ")\n";
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
  catch(...) {
    cerr << "catch (...)\n"
	 << "test_number = " << test_number <<"\n";
  }
}
