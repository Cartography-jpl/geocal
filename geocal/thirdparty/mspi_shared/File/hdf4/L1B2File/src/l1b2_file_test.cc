/*===========================================================================
=                                                                           =
=                                L1B2File                                   =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                             Shared Processing

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!MODULE NAME: l1b2_file_test.cc

!ABSTRACT:

   This does unit testing for the class L1B2File.

!DESCRIPTION:

  The following tests are performed:

  1. Normal call to all member functions.
  2. Test argument checking.
  3. White box testing.

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "l1b2_file.h"          // Definition of L1B2File
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/Support/src/box_region_coor.h" 
				// Definition of BoxRegionCoor.
#include "MSPI-Shared/File/FieldMap/src/configured_field_map.h"
				// Definition of ConfiguredFieldMap
#include "MSPI-Shared/UnitTest/src/double_comparator.h" 
				// Definition of DoubleComparator
#include "MSPI-Shared/UnitTest/src/check.h" 
				// Definition of vector_check

using std::cerr;
using MSPI::Shared::HDF4::L1B2File;
using MSPI::Shared::ProjectionInformation;
using MSPI::Shared::MapInformation;
using MSPI::Shared::Exception;
using MSPI::Shared::BoxRegionCoor;
using MSPI::Shared::FieldMap;
using MSPI::Shared::ConfiguredFieldMap;
using MSPI::Shared::DoubleComparator;
using MSPI::Shared::vector_check;


//////////////////////////////////////////////////////////////////////////
// Initialize unit-test input
//////////////////////////////////////////////////////////////////////////

const int number_record = 10;

//------------------------------------------------------------------------
std::vector<double> longitude_init()
{
  std::vector<double> data(number_record);
  for (size_t i = 0 ; i < data.size(); i++) {
    data[i] = i * 0.51;
  }
  return data;
}

//------------------------------------------------------------------------
std::vector<double> latitude_init()
{
  std::vector<double> data(number_record);
  for (size_t i = 0 ; i < data.size(); i++) {
    data[i] = i * 0.71;
  }
  return data;
}

//------------------------------------------------------------------------
class SimpleFieldMap2B : public FieldMap {
public:
  SimpleFieldMap2B() {
    init_grid(6, "700_nm_band", 4, "Time","I","DOLP","Chi_scat");
    init_grid(7, "Wrong grid name", 4, "Time","I","DOLP","Chi_scat");
  };
};

//------------------------------------------------------------------------
class SimpleFieldMap2C : public FieldMap {
public:
  SimpleFieldMap2C() {
    init_grid(6, "700_nm_band", 4, "Time","I","DOLP","Chi_scat");
    init_grid(7, "801_nm_band", 4, "Time","I","Wrong Field Name","Chi_scat");
  };
};

//////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  DoubleComparator dcmp;

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename = "File/hdf4/L1B2File/tmp/l1b2_file_test.hdf";
    ConfiguredFieldMap simple_field_map("File/hdf4/L1B2File/src/l1b2_file_test.simple_field_map");
    L1B2File file1(filename, MSPI::Shared::HDF4::L1B2File::CREATE,
		   simple_field_map);

//-----------------------------------------------------------------------
// Test polygon_table_write()
//-----------------------------------------------------------------------

    test_number++;
    {
      std::vector<double> latitude(latitude_init());
      std::vector<double> longitude(longitude_init());

      file1.polygon_table_write(latitude, longitude);
    }
      
//-----------------------------------------------------------------------
// Test polygon_table_read()
//-----------------------------------------------------------------------

    test_number++;
    {
      std::vector<double> latitude;
      std::vector<double> longitude;
      file1.polygon_table_read(latitude, longitude);

      vector_check(latitude, latitude_init(), dcmp, __LINE__, error);
      vector_check(longitude, longitude_init(), dcmp, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test granule_id()
//-----------------------------------------------------------------------

    test_number++;
    
    if (file1.granule_id() != "l1b2_file_test.hdf") {
      cerr << "granule_id = " << file1.granule_id() << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test epoch()
//-----------------------------------------------------------------------
    
    test_number++;
    
    std::string epoch_expect = "1994-01-01T00:00:00.123456Z";
    file1.epoch(epoch_expect);

    std::string epoch = file1.epoch();

    if (epoch != epoch_expect) {
      cerr << "epoch = " << epoch << " (expected " << epoch_expect << ")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test field_names()
//-----------------------------------------------------------------------

    test_number++;
    std::vector<std::string> 
      field_names1_expect = simple_field_map.field_names(1);
    std::vector<std::string> 
      field_names2_expect = simple_field_map.field_names(2);

    std::vector<std::string> field_names1 = file1.field_names(1);
    std::vector<std::string> field_names2 = file1.field_names(2);

    if (field_names1 != field_names1_expect ||
	field_names2 != field_names2_expect) {
      cerr << "field_names1 != field_names1_expect\n";
      cerr << "field_names2 != field_names2_expect\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test band_numbers()
//-----------------------------------------------------------------------

    test_number++;
    std::vector<int> band_set = file1.band_numbers();
    if (band_set.size() != 0) {
      cerr << "band_set.size() = "<<band_set.size() << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test add_band()
//-----------------------------------------------------------------------

    test_number++;
    float wavelength1_expect = 500.1;
    float wavelength2_expect = 600.5;
    double e0 = 5001.1;

    std::vector<double> proj_param1_expect(ProjectionInformation::number_proj_param,0.0);
    std::vector<double> proj_param2_expect(ProjectionInformation::number_proj_param,0.0);

    int zonecode = 11;
    ProjectionInformation proj_info1_expect(1, 2, zonecode, proj_param1_expect);
    ProjectionInformation proj_info2_expect(1, 5, zonecode, proj_param2_expect);

    MapInformation map_info1_expect(1.1, 1.2, 1.3, 1.4, 2*64, 3*64, 
				    MapInformation::ORIGIN_LR, 
				    MapInformation::PIX_REG_CORNER); 
    MapInformation map_info2_expect(2.1, 2.2, 2.3, 2.4, 2*64, 3*64, 
				    MapInformation::ORIGIN_LL, 
				    MapInformation::PIX_REG_CENTER); 

    L1B2File::DimensionOrder dimension_order1_expect = L1B2File::YMAJOR;
    file1.add_band(1,wavelength1_expect, e0,
		   proj_info1_expect, map_info1_expect, dimension_order1_expect);

    
    L1B2File::DimensionOrder dimension_order2_expect = L1B2File::XMAJOR;
    file1.add_band(2,wavelength2_expect, e0,
		   proj_info2_expect, map_info2_expect, dimension_order2_expect);

//-----------------------------------------------------------------------
// Test dimension order.
//-----------------------------------------------------------------------

    test_number++;
    std::string field_name1("I");
    std::string field_name2("DOLP");
    L1B2File::DimensionOrder dimension_order1 = file1.dimension_order(1, field_name1);
    L1B2File::DimensionOrder dimension_order2 = file1.dimension_order(2, field_name2);
    if (dimension_order1 != dimension_order1_expect ||
	dimension_order2 != dimension_order2_expect) {
      cerr << "dimension_order1 = " << dimension_order1 
	   << "  (expected "<< dimension_order1_expect << ")\n";
      cerr << "dimension_order2 = " << dimension_order2 
	   << "  (expected "<< dimension_order2_expect << ")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
      
    }

//-----------------------------------------------------------------------
// Test projection_info()
//-----------------------------------------------------------------------

    test_number++;
    ProjectionInformation proj_info1 = file1.projection_info(1);
    ProjectionInformation proj_info2 = file1.projection_info(2);

    if (proj_info1.proj_code() != proj_info1_expect.proj_code() ||
	proj_info2.proj_code() != proj_info2_expect.proj_code() ||
	proj_info1.sphere_code() != proj_info1_expect.sphere_code() ||
	proj_info2.sphere_code() != proj_info2_expect.sphere_code() ||
	proj_info1.zone_code() != proj_info1_expect.zone_code() ||
	proj_info2.zone_code() != proj_info2_expect.zone_code()) {
      cerr << "proj_info1.proj_code() = "<<proj_info1.proj_code() 
	   << " (expected "<<proj_info1_expect.proj_code()<<"\n";
      cerr << "proj_info2.proj_code() = "<<proj_info2.proj_code() 
	   << " (expected "<<proj_info2_expect.proj_code()<<"\n";
      cerr << "proj_info1.sphere_code() = "<<proj_info1.sphere_code() 
	   << " (expected "<<proj_info1_expect.sphere_code()<<"\n";
      cerr << "proj_info2.sphere_code() = "<<proj_info2.sphere_code() 
	   << " (expected "<<proj_info2_expect.sphere_code()<<"\n";
      cerr << "proj_info1.zone_code() = "<<proj_info1.zone_code() 
	   << " (expected "<<proj_info1_expect.zone_code()<<"\n";
      cerr << "proj_info2.zone_code() = "<<proj_info2.zone_code() 
	   << " (expected "<<proj_info2_expect.zone_code()<<"\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

    std::vector<double> proj_param1 = proj_info1.proj_param();
    std::vector<double> proj_param2 = proj_info2.proj_param();
    for (size_t i = 0 ; i < ProjectionInformation::number_proj_param ; i++) {
      if ( proj_param1[i] != proj_param1_expect[i] ||
	   proj_param2[i] != proj_param2_expect[i]) {
	cerr << "proj_param1["<<i<<"] = "<<proj_param1[i]
	     <<  " (expected "<<proj_param1_expect[i]<<")\n";
	cerr << "proj_param2["<<i<<"] = "<<proj_param2[i]
	     << " (expected "<<proj_param2_expect[i]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test map_info()
//-----------------------------------------------------------------------
    
    test_number++;
    MapInformation map_info1 = file1.map_info(1);
    MapInformation map_info2 = file1.map_info(2);
      
    if (map_info1.min_x() != map_info1_expect.min_x() ||
	map_info2.min_x() != map_info2_expect.min_x() ||
	map_info1.max_x() != map_info1_expect.max_x() ||
	map_info2.max_x() != map_info2_expect.max_x() ||
	map_info1.min_y() != map_info1_expect.min_y() ||
	map_info2.min_y() != map_info2_expect.min_y() ||
	map_info1.max_y() != map_info1_expect.max_y() ||
	map_info2.max_y() != map_info2_expect.max_y() ||
	map_info1.grid_size_x() != map_info1_expect.grid_size_x() ||
	map_info2.grid_size_x() != map_info2_expect.grid_size_x() ||
	map_info1.grid_size_y() != map_info1_expect.grid_size_y() ||
	map_info2.grid_size_y() != map_info2_expect.grid_size_y() ||
	map_info1.origin_code() != map_info1_expect.origin_code() ||
	map_info2.origin_code() != map_info2_expect.origin_code() ||
	map_info1.pix_reg_code() != map_info1_expect.pix_reg_code() ||
	map_info2.pix_reg_code() != map_info2_expect.pix_reg_code()) {
      cerr << "map_info1.min_x() = "<<map_info1.min_x() 
	   << " (expected "<<map_info1_expect.min_x()<<")\n";
      cerr << "map_info2.min_x() = "<<map_info2.min_x() 
	   << " (expected "<<map_info2_expect.min_x()<<")\n";
      cerr << "map_info1.max_x() = "<<map_info1.max_x() 
	   << " (expected "<<map_info1_expect.max_x()<<")\n";
      cerr << "map_info2.max_x() = "<<map_info2.max_x() 
	   << " (expected "<<map_info2_expect.max_x()<<")\n";
      cerr << "map_info1.min_y() = "<<map_info1.min_y() 
	   << " (expected "<<map_info1_expect.min_y()<<")\n";
      cerr << "map_info2.min_y() = "<<map_info2.min_y() 
	   << " (expected "<<map_info2_expect.min_y()<<")\n";
      cerr << "map_info1.max_y() = "<<map_info1.max_y() 
	   << " (expected "<<map_info1_expect.max_y()<<")\n";
      cerr << "map_info2.max_y() = "<<map_info2.max_y() 
	   << " (expected "<<map_info2_expect.max_y()<<")\n";
      cerr << "map_info1.grid_size_x() = "<<map_info1.grid_size_x() 
	   << " (expected "<<map_info1_expect.grid_size_x()<<")\n";
      cerr << "map_info2.grid_size_x() = "<<map_info2.grid_size_x() 
	   << " (expected "<<map_info2_expect.grid_size_x()<<")\n";
      cerr << "map_info1.grid_size_y() = "<<map_info1.grid_size_y() 
	   << " (expected "<<map_info1_expect.grid_size_y()<<")\n";
      cerr << "map_info2.grid_size_y() = "<<map_info2.grid_size_y() 
	   << " (expected "<<map_info2_expect.grid_size_y()<<")\n";
      cerr << "map_info1.origin_code() = "<<map_info1.origin_code() 
	   << " (expected "<<map_info1_expect.origin_code()<<")\n";
      cerr << "map_info2.origin_code() = "<<map_info2.origin_code() 
	   << " (expected "<<map_info2_expect.origin_code()<<")\n";
      cerr << "map_info1.pix_reg_code() = "<<map_info1.pix_reg_code() 
	   << " (expected "<<map_info1_expect.pix_reg_code()<<")\n";
      cerr << "map_info2.pix_reg_code() = "<<map_info2.pix_reg_code() 
	   << " (expected "<<map_info2_expect.pix_reg_code()<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test wavelength()
//-----------------------------------------------------------------------

    test_number++;
    float wavelength1 = file1.wavelength(1);
    float wavelength2 = file1.wavelength(2);
    if (wavelength1 != wavelength1_expect ||
	wavelength2 != wavelength2_expect) {
      cerr << "wavelength1 = "<<wavelength1
	   << " (expected "<<wavelength1_expect<<")\n";
      cerr << "wavelength2 = "<<wavelength2
	   << " (expected "<<wavelength2_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test grid_name()
//-----------------------------------------------------------------------
      
    test_number++;
    std::string grid_name1_expect = "500_nm_band";
    std::string grid_name2_expect = "601_nm_band";

    std::string grid_name1 = file1.grid_name(1);
    std::string grid_name2 = file1.grid_name(2);
    if (grid_name1 != grid_name1_expect ||
	grid_name2 != grid_name2_expect) {
      cerr << "grid_name1 = '"<<grid_name1
	   << "' (expected '"<<grid_name1_expect<<"')\n";
      cerr << "grid_name2 = '"<<grid_name2
	   << "' (expected '"<<grid_name2_expect<<"')\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test align_region()
//-----------------------------------------------------------------------

    test_number++;
    int band = 2;
    MapInformation map_info3 = file1.map_info(band);

    double shift_x = map_info3.resolution_x() * 0.25;
    double shift_y = map_info3.resolution_y() * 0.25;

    int start_x_expect = 106;
    int start_y_expect = 12;
    int size_x_expect = 10;
    int size_y_expect = 60;
    BoxRegionCoor region_out_expect(
          map_info3.min_x() + start_x_expect * map_info3.resolution_x(),
	  map_info3.min_y() + start_y_expect * map_info3.resolution_y(),
	  map_info3.min_x() + ((start_x_expect+size_x_expect) * 
			       map_info3.resolution_x()),
	  map_info3.min_y() + ((start_y_expect+size_y_expect) * 
	                       map_info3.resolution_y()));

    BoxRegionCoor region_in = 
      BoxRegionCoor(region_out_expect.min_x() + shift_x,
                    region_out_expect.min_y() + shift_y,
                    region_out_expect.max_x() - shift_x,
                    region_out_expect.max_y() - shift_y);
    
    BoxRegionCoor region_out(0,0,0,0);
    int start_x;
    int start_y;
    int size_x;
    int size_y;
    
    file1.align_region(band, region_in, region_out, 
		       start_x, start_y, size_x, size_y);
    
    if (region_out != region_out_expect) {
      cerr << "region_out.min_x = " << region_out.min_x()
           << " (expected " << region_out_expect.min_x() << ")\n";
      cerr << "region_out.min_y = " << region_out.min_y() 
           << " (expected " << region_out_expect.min_y() << ")\n";
      cerr << "region_out.max_x = " << region_out.max_x() 
           << " (expected " << region_out_expect.max_x() << ")\n";
      cerr << "region_out.max_y = " << region_out.max_y() 
           << " (expected " << region_out_expect.max_y() << ")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

    if (start_x != start_x_expect ||
	start_y != start_y_expect ||
	size_x != size_x_expect ||
	size_y != size_y_expect) {
      cerr << "start_x = " << start_x 
	   << " (expected "<< start_x_expect << ")\n";
      cerr << "start_y = " << start_y 
	   << " (expected "<< start_y_expect << ")\n";
      cerr << "size_x = " << size_x 
	   << " (expected "<< size_x_expect << ")\n";
      cerr << "size_y = " << size_y 
	   << " (expected "<< size_y_expect << ")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }
    
//-----------------------------------------------------------------------
// Test read()
//-----------------------------------------------------------------------

    test_number++;

    { // band 1 is Y-major
      int band = 1;
      std::string field_name = std::string("I");
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;

      boost::multi_array<float,2> data_expect(boost::extents[size_y][size_x]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[iy][ix] = -9.999e-9;
	}
      }
      
      boost::multi_array<float,2> data = 
	file1.read<float>(band, field_name, start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[iy][ix] != data_expect[iy][ix]) {
	    cerr << "data["<<iy<<"]["<<ix<<"]="<<data[iy][ix]
		 << " (expected "<<data_expect[iy][ix]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    { // band 2 is X-major
      int band = 2;
      std::string field_name = std::string("I");
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;

      boost::multi_array<float,2> data_expect(boost::extents[size_x][size_y]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[ix][iy] = -9.999e-9;
	}
      }
      
      boost::multi_array<float,2> data = 
	file1.read<float>(band, field_name, start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[ix][iy] != data_expect[ix][iy]) {
	    cerr << "data["<<ix<<"]["<<iy<<"]="<<data[ix][iy]
		 << " (expected "<<data_expect[ix][iy]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test read<double>()
//-----------------------------------------------------------------------

    test_number++;

    { // band 1 is Y-major
      int band = 1;
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;

      boost::multi_array<double,2> data_expect(boost::extents[size_y][size_x]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[iy][ix] = -9.999e-9;
	}
      }
      
      boost::multi_array<double,2> data = 
	file1.read<double>(band, "Time", start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[iy][ix] != data_expect[iy][ix]) {
	    cerr << "data["<<ix<<"]["<<iy<<"]="<<data[iy][ix]
		 << " (expected "<<data_expect[iy][ix]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    { // band 2 is X-major
      int band = 2;
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;

      boost::multi_array<double,2> data_expect(boost::extents[size_x][size_y]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[ix][iy] = -9.999e-9;
	}
      }
      
      boost::multi_array<double,2> data = 
	file1.read<double>(band, "Time", start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[ix][iy] != data_expect[ix][iy]) {
	    cerr << "data["<<ix<<"]["<<iy<<"]="<<data[ix][iy]
		 << " (expected "<<data_expect[ix][iy]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test write()
//-----------------------------------------------------------------------

    test_number++;

    { // band 1 is Y-major
      int band = 1;
      std::string field_name = std::string("DOLP");
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;

      boost::multi_array<float,2> data_expect(boost::extents[size_y][size_x]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[iy][ix] = ix*size_y + iy;
	}
      }
      
      file1.write(band, field_name, start_x, start_y, data_expect);

      boost::multi_array<float,2> data = 
	file1.read<float>(band, field_name, start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[iy][ix] != data_expect[iy][ix]) {
	    cerr << "data["<<ix<<"]["<<iy<<"]="<<data[iy][ix]
		 << " (expected "<<data_expect[iy][ix]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {  // band 2 is X-major
      int band = 2;
      std::string field_name = std::string("DOLP");
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;

      boost::multi_array<float,2> data_expect(boost::extents[size_x][size_y]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[ix][iy] = ix*size_y + iy;
	}
      }
      
      file1.write(band, field_name, start_x, start_y, data_expect);

      boost::multi_array<float,2> data = 
	file1.read<float>(band, field_name, start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[ix][iy] != data_expect[ix][iy]) {
	    cerr << "data["<<ix<<"]["<<iy<<"]="<<data[ix][iy]
		 << " (expected "<<data_expect[ix][iy]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test write(...,boost::multi_array<double,2>)
//-----------------------------------------------------------------------

    test_number++;

    { // band 1 is Y-major
      int band = 1;
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;
      
      boost::multi_array<double,2> data_expect(boost::extents[size_y][size_x]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[iy][ix] = ix*size_y + iy;
	}
      }
      
      file1.write(band, "Time", start_x, start_y, data_expect);

      boost::multi_array<double,2> data = 
	file1.read<double>(band, "Time", start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[iy][ix] != data_expect[iy][ix]) {
	    cerr << "data["<<ix<<"]["<<iy<<"]="<<data[iy][ix]
		 << " (expected "<<data_expect[iy][ix]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    { // band 2 is X-major
      int band = 2;
      int start_x = 106;
      int start_y = 12;
      int size_x = 10;
      int size_y = 60;
      
      boost::multi_array<double,2> data_expect(boost::extents[size_x][size_y]);
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  data_expect[ix][iy] = ix*size_y + iy;
	}
      }
      
      file1.write(band, "Time", start_x, start_y, data_expect);

      boost::multi_array<double,2> data = 
	file1.read<double>(band, "Time",start_x, start_y, size_x, size_y);
    
      bool any_fail = false;
      for (int ix = 0 ; ix < size_x ; ++ix) {
	for (int iy = 0 ; iy < size_y ; ++iy) {
	  if (data[ix][iy] != data_expect[ix][iy]) {
	    cerr << "data["<<ix<<"]["<<iy<<"]="<<data[ix][iy]
		 << " (expected "<<data_expect[ix][iy]<<")\n";
	    error = true;
	    any_fail = true;
	  }
	}
      }
      if (any_fail) {
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test add_band() with a band thats already created.
//-----------------------------------------------------------------------

    test_number++;
    bool caught_exception = false;
    try {
      file1.add_band(1,wavelength1_expect, e0,
		     proj_info1_expect, map_info1_expect, dimension_order1_expect);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Band number already in table: 1")) {
	cerr << "exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Disable HDF-EOS error printing for subsequent tests.
//-----------------------------------------------------------------------

//    HE5_EHset_error_on(2,0);

//-----------------------------------------------------------------------
// Re-enable HDF-EOS error printing.
//----------------------------------------------------------------------- 

//    HE5_EHset_error_on(0,0);

//-----------------------------------------------------------------------
// Test projection_info() with invalid band.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      ProjectionInformation projection_info3 = file1.projection_info(3);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for band: 3")) {
	cerr << "projection_info() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "projection_info() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test map_info() with invalid band.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      MapInformation map_info3 = file1.map_info(3);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for band: 3")) {
	cerr << "map_info() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "map_info() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test wavelength() with invalid band.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      file1.wavelength(3);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for band: 3")) {
	cerr << "wavelength() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "wavelength() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test grid_name() with invalid band.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      std::string grid_name3 = file1.grid_name(3);
      
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "No record for band: 3")) {
	cerr << "grid_name() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "grid_name() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Create a new file.
//-----------------------------------------------------------------------

    test_number++;
    float wavelength6_expect = 700.1;
    float wavelength7_expect = 800.5;

    std::vector<double> proj_param6_expect(ProjectionInformation::number_proj_param,0.0);
    std::vector<double> proj_param7_expect(ProjectionInformation::number_proj_param,0.0);

    zonecode = 11; 
    ProjectionInformation proj_info6_expect(1, 2, zonecode, proj_param6_expect);
    ProjectionInformation proj_info7_expect(1, 5, zonecode, proj_param7_expect);

    MapInformation map_info6_expect(3.1, 3.2, 3.3, 3.4, 3*64, 4*64, 
				    MapInformation::ORIGIN_UL, 
				    MapInformation::PIX_REG_CORNER); 
    MapInformation map_info7_expect(4.1, 4.2, 4.3, 4.4, 5*64, 6*64, 
				    MapInformation::ORIGIN_UR, 
				    MapInformation::PIX_REG_CENTER); 
      
    std::string filename2 = "File/hdf4/L1B2File/tmp/l1b2_file_test2.hdf";

    ConfiguredFieldMap simple_field_map2("File/hdf4/L1B2File/src/l1b2_file_test.simple_field_map2");
    {
      L1B2File file2(filename2, MSPI::Shared::HDF4::L1B2File::CREATE, simple_field_map2);

      file2.add_band(6,wavelength6_expect, e0,
		     proj_info6_expect, map_info6_expect, dimension_order1_expect);
      
      file2.add_band(7,wavelength7_expect, e0,
		     proj_info7_expect, map_info7_expect, dimension_order1_expect);
    }  // end scope should close the file.

//-----------------------------------------------------------------------
// Test constructor for opening an existing file, read-only
//-----------------------------------------------------------------------

    test_number++;
    L1B2File file3(filename2);
    
//-----------------------------------------------------------------------
// Test projection_info()
//-----------------------------------------------------------------------

    test_number++;
    ProjectionInformation proj_info6 = file3.projection_info(6);
    ProjectionInformation proj_info7 = file3.projection_info(7);

    if (proj_info6.proj_code() != proj_info6_expect.proj_code() ||
	proj_info7.proj_code() != proj_info7_expect.proj_code() ||
	proj_info6.sphere_code() != proj_info6_expect.sphere_code() ||
	proj_info7.sphere_code() != proj_info7_expect.sphere_code() ||
	proj_info6.zone_code() != proj_info6_expect.zone_code() ||
	proj_info7.zone_code() != proj_info7_expect.zone_code()) {
      cerr << "proj_info6.proj_code() = "<<proj_info6.proj_code() 
	   << " (expected "<<proj_info6_expect.proj_code()<<"\n";
      cerr << "proj_info7.proj_code() = "<<proj_info7.proj_code() 
	   << " (expected "<<proj_info7_expect.proj_code()<<"\n";
      cerr << "proj_info6.sphere_code() = "<<proj_info6.sphere_code() 
	   << " (expected "<<proj_info6_expect.sphere_code()<<"\n";
      cerr << "proj_info7.sphere_code() = "<<proj_info7.sphere_code() 
	   << " (expected "<<proj_info7_expect.sphere_code()<<"\n";
      cerr << "proj_info6.zone_code() = "<<proj_info6.zone_code() 
	   << " (expected "<<proj_info6_expect.zone_code()<<"\n";
      cerr << "proj_info7.zone_code() = "<<proj_info7.zone_code() 
	   << " (expected "<<proj_info7_expect.zone_code()<<"\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

    std::vector<double> proj_param6 = proj_info6.proj_param();
    std::vector<double> proj_param7 = proj_info7.proj_param();
    for (size_t i = 0 ; i < ProjectionInformation::number_proj_param ; i++) {
      if ( proj_param6[i] != proj_param6_expect[i] ||
	   proj_param7[i] != proj_param7_expect[i]) {
	cerr << "proj_param6["<<i<<"] = "<<proj_param6[i]
	     <<  " (expected "<<proj_param6_expect[i]<<")\n";
	cerr << "proj_param7["<<i<<"] = "<<proj_param7[i]
	     << " (expected "<<proj_param7_expect[i]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test map_info()
//-----------------------------------------------------------------------
    
    test_number++;
    MapInformation map_info6 = file3.map_info(6);
    MapInformation map_info7 = file3.map_info(7);
      
    if (map_info6.min_x() != map_info6_expect.min_x() ||
	map_info7.min_x() != map_info7_expect.min_x() ||
	map_info6.max_x() != map_info6_expect.max_x() ||
	map_info7.max_x() != map_info7_expect.max_x() ||
	map_info6.min_y() != map_info6_expect.min_y() ||
	map_info7.min_y() != map_info7_expect.min_y() ||
	map_info6.max_y() != map_info6_expect.max_y() ||
	map_info7.max_y() != map_info7_expect.max_y() ||
	map_info6.grid_size_x() != map_info6_expect.grid_size_x() ||
	map_info7.grid_size_x() != map_info7_expect.grid_size_x() ||
	map_info6.grid_size_y() != map_info6_expect.grid_size_y() ||
	map_info7.grid_size_y() != map_info7_expect.grid_size_y() ||
	map_info6.origin_code() != map_info6_expect.origin_code() ||
	map_info7.origin_code() != map_info7_expect.origin_code() ||
	map_info6.pix_reg_code() != map_info6_expect.pix_reg_code() ||
	map_info7.pix_reg_code() != map_info7_expect.pix_reg_code()) {
      cerr << "map_info6.min_x() = "<<map_info6.min_x() 
	   << " (expected "<<map_info6_expect.min_x()<<")\n";
      cerr << "map_info7.min_x() = "<<map_info7.min_x() 
	   << " (expected "<<map_info7_expect.min_x()<<")\n";
      cerr << "map_info6.max_x() = "<<map_info6.max_x() 
	   << " (expected "<<map_info6_expect.max_x()<<")\n";
      cerr << "map_info7.max_x() = "<<map_info7.max_x() 
	   << " (expected "<<map_info7_expect.max_x()<<")\n";
      cerr << "map_info6.min_y() = "<<map_info6.min_y() 
	   << " (expected "<<map_info6_expect.min_y()<<")\n";
      cerr << "map_info7.min_y() = "<<map_info7.min_y() 
	   << " (expected "<<map_info7_expect.min_y()<<")\n";
      cerr << "map_info6.max_y() = "<<map_info6.max_y() 
	   << " (expected "<<map_info6_expect.max_y()<<")\n";
      cerr << "map_info7.max_y() = "<<map_info7.max_y() 
	   << " (expected "<<map_info7_expect.max_y()<<")\n";
      cerr << "map_info6.grid_size_x() = "<<map_info6.grid_size_x() 
	   << " (expected "<<map_info6_expect.grid_size_x()<<")\n";
      cerr << "map_info7.grid_size_x() = "<<map_info7.grid_size_x() 
	   << " (expected "<<map_info7_expect.grid_size_x()<<")\n";
      cerr << "map_info6.grid_size_y() = "<<map_info6.grid_size_y() 
	   << " (expected "<<map_info6_expect.grid_size_y()<<")\n";
      cerr << "map_info7.grid_size_y() = "<<map_info7.grid_size_y() 
	   << " (expected "<<map_info7_expect.grid_size_y()<<")\n";
      cerr << "map_info6.origin_code() = "<<map_info6.origin_code() 
	   << " (expected "<<map_info6_expect.origin_code()<<")\n";
      cerr << "map_info7.origin_code() = "<<map_info7.origin_code() 
	   << " (expected "<<map_info7_expect.origin_code()<<")\n";
      cerr << "map_info6.pix_reg_code() = "<<map_info6.pix_reg_code() 
	   << " (expected "<<map_info6_expect.pix_reg_code()<<")\n";
      cerr << "map_info7.pix_reg_code() = "<<map_info7.pix_reg_code() 
	   << " (expected "<<map_info7_expect.pix_reg_code()<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test wavelength()
//-----------------------------------------------------------------------

    test_number++;
    float wavelength6 = file3.wavelength(6);
    float wavelength7 = file3.wavelength(7);
    if (wavelength6 != wavelength6_expect ||
	wavelength7 != wavelength7_expect) {
      cerr << "wavelength6 = "<<wavelength6
	   << " (expected "<<wavelength6_expect<<")\n";
      cerr << "wavelength7 = "<<wavelength7
	   << " (expected "<<wavelength7_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test grid_name()
//-----------------------------------------------------------------------
      
    test_number++;
    std::string grid_name6_expect = "700_nm_band";
    std::string grid_name7_expect = "801_nm_band";

    std::string grid_name6 = file3.grid_name(6);
    std::string grid_name7 = file3.grid_name(7);
    if (grid_name6 != grid_name6_expect ||
	grid_name7 != grid_name7_expect) {
      cerr << "grid_name6 = '"<<grid_name6
	   << "' (expected '"<<grid_name6_expect<<"')\n";
      cerr << "grid_name7 = '"<<grid_name7
	   << "' (expected '"<<grid_name7_expect<<"')\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Try creating the same file twice.  This seems to be an effective way
// to test that the destructor has closed all open file handles.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename3 = "File/hdf4/L1B2File/tmp/l1b2_file_test3.hdf";
    
    {
      L1B2File file2(filename3, MSPI::Shared::HDF4::L1B2File::CREATE, simple_field_map2);

      file2.add_band(6,wavelength6_expect, e0,
		     proj_info6_expect, map_info6_expect, dimension_order1_expect);
      
      file2.add_band(7,wavelength7_expect, e0,
		     proj_info7_expect, map_info7_expect, dimension_order1_expect);
    }  // end scope should close the file.

    {
      L1B2File file2(filename3, MSPI::Shared::HDF4::L1B2File::CREATE, simple_field_map2);

      file2.add_band(6,wavelength6_expect, e0,
		     proj_info6_expect, map_info6_expect, dimension_order1_expect);
      
      file2.add_band(7,wavelength7_expect, e0,
		     proj_info7_expect, map_info7_expect, dimension_order1_expect);
    }  // end scope should close the file.

//-----------------------------------------------------------------------
// Test constructor with incompatible field map, due to grid name
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    SimpleFieldMap2B simple_field_map2b;
    try {
      L1B2File file2(filename2, MSPI::Shared::HDF4::L1B2File::READ, simple_field_map2b);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Grid name '801_nm_band', in file, does not match band name 'Wrong grid name', in field map.  Band_number: 7   Filename: File/hdf4/L1B2File/tmp/l1b2_file_test2.hdf")) {
	cerr << "L1B1File() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "L1B1File() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test constructor with incompatible field map, due to field name mismatch.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    SimpleFieldMap2C simple_field_map2c;
    try {
      L1B2File file2(filename2, MSPI::Shared::HDF4::L1B2File::READ, simple_field_map2c);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Trouble with GDfieldinfo for required field name: Wrong Field Name,  grid: 801_nm_band,  Filename: File/hdf4/L1B2File/tmp/l1b2_file_test2.hdf")) {
	cerr << "L1B1File() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "L1B1File() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
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
