//===========================================================================
//                                                                         
//                               L1B1File                                  
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "l1b1_file.h"          // Definition of L1B1File
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/check.h" // Definition of array_check
#include "MSPI-Shared/UnitTest/src/float_comparator.h" // Definition of FloatComparator
#include "MSPI-Shared/UnitTest/src/double_comparator.h" // Definition of DoubleComparator

using std::cerr;
using MSPI::Shared::L1B1File;
using MSPI::Shared::FieldMap;
using MSPI::Shared::Exception;
using MSPI::Shared::Meta;
using MSPI::Shared::vector_check;
using MSPI::Shared::array_check;
using MSPI::Shared::FloatComparator;
using MSPI::Shared::DoubleComparator;


//-----------------------------------------------------------------------
// Generate unit-test input.
//-----------------------------------------------------------------------

const size_t number_pixel = 1536;

//-----------------------------------------------------------------------
boost::multi_array<float,2> data_init(size_t Number_frame)
{
  boost::multi_array<float,2> data(boost::extents[Number_frame][number_pixel]);
  for (size_t i = 0 ; i < Number_frame ; i++) {
    for (size_t j = 0 ; j < number_pixel ; j++) {
      data[i][j] = i + j * 0.001;
    }
  }
  return data;
}

//-----------------------------------------------------------------------
std::vector<double> time_init(size_t Number_frame)
{
  std::vector<double> data(Number_frame);
  for (size_t i = 0 ; i < Number_frame ; i++) {
    data[i] = i * 3.17;
  }
  return data;
}

//-----------------------------------------------------------------------
// Field Maps for testing.
//-----------------------------------------------------------------------

class SimpleFieldMap : public FieldMap {
public:
  SimpleFieldMap() {
    init_grid(1, "Row1", 3, "A","B","C");
    init_grid(2, "Row2", 1, "D");
  };
};

class SimpleFieldMap2 : public FieldMap {
public:
  SimpleFieldMap2() {
    init_grid(1, "Row1", 3, "A","B","C");
    init_grid(2, "Row4", 1, "D");
  };
};

class SimpleFieldMap3 : public FieldMap {
public:
  SimpleFieldMap3() {
    init_grid(1, "Row1", 3, "A","B","C");
    init_grid(2, "Row2", 2, "D","E");
  };
};

/////////////////////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////////////////////

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  FloatComparator cmpf;		// Float comparator
  DoubleComparator cmpd;		// Double comparator

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename = "File/L1B1File/tmp/l1b1_file_test.hdf5";
    SimpleFieldMap simple_field_map;
    L1B1File file1(filename, MSPI::Shared::L1B1File::CREATE, simple_field_map);

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
// Test granule_id()
//-----------------------------------------------------------------------

    test_number++;
    
    if (file1.granule_id() != "l1b1_file_test.hdf5") {
      cerr << "granule_id = " << file1.granule_id() << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test add_meta()
// Test meta()
//-----------------------------------------------------------------------

    test_number++;
    {
      Meta meta_in;
      meta_in.add<std::string>("testparam","testvalue");
      file1.add_meta(meta_in);

      Meta meta_out = file1.meta();
      if (meta_out.number_element() != 3) {
	cerr << "number_element = " << meta_out.number_element() << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
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
// Test number_pixel()
//-----------------------------------------------------------------------

    test_number++;
    if (file1.number_pixel() != number_pixel) {
      cerr << "number_pixel = "<<file1.number_pixel() << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test row_numbers()
//-----------------------------------------------------------------------

    test_number++;
    std::vector<int> row_set = file1.row_numbers();
    if (row_set.size() != 0) {
      cerr << "row_set.size() = "<<row_set.size() << "\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test number_frame(), for initial empty file
//-----------------------------------------------------------------------

    test_number++;
    size_t number_frame_expect = 0;

    size_t number_frame = file1.number_frame(1);

    if (number_frame != number_frame_expect) {
      cerr << "number_frame = "<<number_frame
	   << " (expected "<<number_frame_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test add_row()
//-----------------------------------------------------------------------

    test_number++;
    float wavelength1_expect = 500.1;
    float wavelength2_expect = 600.5;
    float polarization_angle1_expect = 1.1;
    float polarization_angle2_expect = 1.2;
    size_t max_number_frame = 10000;

    file1.add_row(1,wavelength1_expect, polarization_angle1_expect, max_number_frame);

    file1.add_row(2,wavelength2_expect, polarization_angle2_expect, max_number_frame);

//-----------------------------------------------------------------------
// Test number_frame()
//-----------------------------------------------------------------------

    test_number++;
    size_t number_frame1_expect = 0;
    size_t number_frame2_expect = 0;

    size_t number_frame1 = file1.number_frame(1);
    size_t number_frame2 = file1.number_frame(2);

    if (number_frame1 != number_frame1_expect ||
	number_frame2 != number_frame2_expect) {
      cerr << "number_frame1 = "<<number_frame1
	   << " (expected "<<number_frame1_expect<<")\n";
      cerr << "number_frame2 = "<<number_frame2
	   << " (expected "<<number_frame2_expect<<")\n";
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
// Test polarization_angle()
//-----------------------------------------------------------------------
      
    test_number++;
    float polarization_angle1 = file1.polarization_angle(1);
    float polarization_angle2 = file1.polarization_angle(2);
    if (polarization_angle1 != polarization_angle1_expect ||
	polarization_angle2 != polarization_angle2_expect) {
      cerr << "polarization_angle1 = '"<<polarization_angle1
	   << "' (expected '"<<polarization_angle1_expect<<"')\n";
      cerr << "polarization_angle2 = '"<<polarization_angle2
	   << "' (expected '"<<polarization_angle2_expect<<"')\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test swath_name()
//-----------------------------------------------------------------------
      
    test_number++;
    std::string swath_name1_expect = "Row1";
    std::string swath_name2_expect = "Row2";

    std::string swath_name1 = file1.swath_name(1);
    std::string swath_name2 = file1.swath_name(2);
    if (swath_name1 != swath_name1_expect ||
	swath_name2 != swath_name2_expect) {
      cerr << "swath_name1 = '"<<swath_name1
	   << "' (expected '"<<swath_name1_expect<<"')\n";
      cerr << "swath_name2 = '"<<swath_name2
	   << "' (expected '"<<swath_name2_expect<<"')\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test read_time(), for uninitialized frames returns fill value.
//-----------------------------------------------------------------------

    test_number++;
    int row_number = 2;
    number_frame = 16;
    std::vector<double> time_expect(number_frame,-9999.0);

    int start_frame = 0;
    std::vector<double> time = 
      file1.read_time(row_number, start_frame, number_frame);
    
    bool any_fail = false;
    for (size_t iframe = 0 ; iframe < number_frame ; ++iframe) {
      if (time[iframe] != time_expect[iframe]) {
	cerr << "time["<<iframe<<"]="<<time[iframe]
	     << " (expected "<<time_expect[iframe]<<")\n";
	error = true;
	any_fail = true;
      }
    }
    if (any_fail) {
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test read_data(), for uninitialized frames returns fill value.
//-----------------------------------------------------------------------

    test_number++;
    row_number = 2;
    start_frame = 0;
    number_frame = 7;
    std::string field_name = "D";

    boost::multi_array<float,2> 
      data_expect(boost::extents[number_frame][number_pixel]);

    for (size_t iframe = 0 ; iframe < number_frame ; ++iframe) {
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	data_expect[iframe][ipixel] = -9999.0;
      }
    }

    boost::multi_array<float,2> data = 
      file1.read_data(row_number, field_name, start_frame, number_frame);
    
    any_fail = false;
    for (size_t iframe = 0 ; iframe < number_frame ; ++iframe) {
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	if (data[iframe][ipixel] != data_expect[iframe][ipixel]) {
	  cerr << "data["<<iframe<<"]["<<ipixel<<"]="<<data[iframe][ipixel]
	       << " (expected "<<data_expect[iframe][ipixel]<<")\n";
	  error = true;
	  any_fail = true;
	}
      }
    }
    if (any_fail) {
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test append()
// Test read_time()
// Test read_data()
//-----------------------------------------------------------------------

    test_number++;
    row_number = 2;
    number_frame = 116;
    std::vector<double> timeB_expect(number_frame);
    std::vector<std::vector<float> > intensityB_expect(number_frame);
    
    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      timeB_expect[iframe] = 18.1 + iframe;
      intensityB_expect[iframe].resize(number_pixel);
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	intensityB_expect[iframe][ipixel] = iframe * number_pixel + ipixel;
      }
    }
    
    std::vector<std::vector<float> > param_vector(1);

    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      param_vector[0] = intensityB_expect[iframe];
      file1.append(row_number, timeB_expect[iframe], param_vector);
    }

    start_frame = 0;
    std::vector<double> timeB = 
      file1.read_time(row_number, start_frame, number_frame);

    field_name = "D";
    boost::multi_array<float,2> dataB = 
      file1.read_data(row_number, field_name, start_frame, number_frame);

    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      if (timeB[iframe] != timeB_expect[iframe]) {
	cerr << "timeB["<<iframe<<"]="<<timeB[iframe]
	     << " (expected "<<timeB_expect[iframe]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	if (dataB[iframe][ipixel] != intensityB_expect[iframe][ipixel]) {
	  cerr << "dataB["<<iframe<<"]["<<ipixel<<"]="<<dataB[iframe][ipixel]
	       << " (expected "<<intensityB_expect[iframe][ipixel]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test append() for a row with more than one field.
// Test read_time()
// Test read_data()
//-----------------------------------------------------------------------

    test_number++;
    row_number = 1;
    number_frame = 11;
    std::vector<double> time1_expect(number_frame);
    std::vector<std::vector<float> > paramA_expect(number_frame);
    std::vector<std::vector<float> > paramB_expect(number_frame);
    std::vector<std::vector<float> > paramC_expect(number_frame);
    
    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      time1_expect[iframe] = 190.3 + iframe;
      paramA_expect[iframe].resize(number_pixel);
      paramB_expect[iframe].resize(number_pixel);
      paramC_expect[iframe].resize(number_pixel);
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	paramA_expect[iframe][ipixel] = 5000.6 + iframe * number_pixel + ipixel;
	paramB_expect[iframe][ipixel] = 6000.5 + iframe * number_pixel + ipixel;
	paramC_expect[iframe][ipixel] = 7000.4 + iframe * number_pixel + ipixel;
      }
    }
    
    param_vector.resize(3);

    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      param_vector[0] = paramA_expect[iframe];
      param_vector[1] = paramB_expect[iframe];
      param_vector[2] = paramC_expect[iframe];
      file1.append(row_number, time1_expect[iframe], param_vector);
    }

    start_frame = 0;
    std::vector<double> time1 = 
      file1.read_time(row_number, start_frame, number_frame);

    boost::multi_array<float,2> paramA = 
      file1.read_data(row_number, "A", start_frame, number_frame);
    boost::multi_array<float,2> paramB = 
      file1.read_data(row_number, "B", start_frame, number_frame);
    boost::multi_array<float,2> paramC = 
      file1.read_data(row_number, "C", start_frame, number_frame);

    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      if (time1[iframe] != time1_expect[iframe]) {
	cerr << "time1["<<iframe<<"]="<<time1[iframe]
	     << " (expected "<<time1_expect[iframe]<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
      for (size_t ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	if (paramA[iframe][ipixel] != paramA_expect[iframe][ipixel] ||
	    paramB[iframe][ipixel] != paramB_expect[iframe][ipixel] ||
	    paramC[iframe][ipixel] != paramC_expect[iframe][ipixel]) {
	  cerr << "paramA["<<iframe<<"]["<<ipixel<<"]="<<paramA[iframe][ipixel]
	       << " (expected "<<paramA_expect[iframe][ipixel]<<")\n";
	  cerr << "paramB["<<iframe<<"]["<<ipixel<<"]="<<paramB[iframe][ipixel]
	       << " (expected "<<paramB_expect[iframe][ipixel]<<")\n";
	  cerr << "paramC["<<iframe<<"]["<<ipixel<<"]="<<paramC[iframe][ipixel]
	       << " (expected "<<paramC_expect[iframe][ipixel]<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test write_data()
// Test read_data()
//-----------------------------------------------------------------------

    {
      test_number++;

      size_t number_frame = 10;
      boost::multi_array<float,2> data_in(data_init(number_frame));
      
      int row_number = 1;
      size_t start_frame = 400;
      file1.write_data(row_number, "A", start_frame, data_in);

      boost::multi_array<float,2> data_out =
	file1.read_data(row_number, "A", start_frame, number_frame);
      
      array_check(data_in, data_out, cmpf, __LINE__, error);

      if (file1.number_frame(row_number) != 410) {
	cerr << "number_frame = " << file1.number_frame(row_number) << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test write_time()
// Test read_time()
//-----------------------------------------------------------------------

    {
      test_number++;

      size_t number_frame = 10;
      std::vector<double> time_in(time_init(number_frame));
      
      int row_number = 1;
      size_t start_frame = 400;
      file1.write_time(row_number, start_frame, time_in);

      std::vector<double> time_out =
	file1.read_time(row_number, start_frame, number_frame);
      
      vector_check(time_in, time_out, cmpd, __LINE__, error);
    }

//-----------------------------------------------------------------------
// Test number_frame()
//-----------------------------------------------------------------------

    test_number++;
    number_frame1_expect = 410;
    number_frame2_expect = 116;

    number_frame1 = file1.number_frame(1);
    number_frame2 = file1.number_frame(2);

    if (number_frame1 != number_frame1_expect ||
	number_frame2 != number_frame2_expect) {
      cerr << "number_frame1 = "<<number_frame1
	   << " (expected "<<number_frame1_expect<<")\n";
      cerr << "number_frame2 = "<<number_frame2
	   << " (expected "<<number_frame2_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test append() with a data vector containing the wrong number of fields.
//-----------------------------------------------------------------------

    test_number++;
    bool caught_exception = false;
    try {
      file1.append(2, time1_expect[0], param_vector);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Argument check: Data.size() != number_field, Data.size() = 3, number_field = 1")) {
	cerr << "add_row() exception = "<<e.what()<<"\n";
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
// Test append() with a data vector containing the wrong number of pixels.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    param_vector[0].resize(number_pixel - 1);
    try {
      file1.append(1, time1_expect[0], param_vector);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Argument check: Data[x].size() != number_pixel()")) {
	cerr << "add_row() exception = "<<e.what()<<"\n";
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
// Test add_row() with a row that's already created.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      file1.add_row(2,wavelength2_expect, polarization_angle2_expect, max_number_frame);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Row number already in table: 2")) {
	cerr << "add_row() exception = "<<e.what()<<"\n";
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
// Try creating the same file twice.  This seems to be an effective
// to test that the destructor has closed all open file handles.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename2 = "File/L1B1File/tmp/l1b1_file_test2.hdf5";
    {
      L1B1File file2(filename2, MSPI::Shared::L1B1File::CREATE, simple_field_map);

      float wavelength1_expect = 500.1;
      float wavelength2_expect = 600.5;
      float polarization_angle1_expect = 1.1;
      float polarization_angle2_expect = 1.2;
      
      file2.add_row(1,wavelength1_expect, polarization_angle1_expect, max_number_frame);
      file2.add_row(2,wavelength2_expect, polarization_angle2_expect, max_number_frame);
    }  // end scope should close the file.

    test_number++;
    {
      L1B1File file2(filename2, MSPI::Shared::L1B1File::CREATE, simple_field_map);

      float wavelength1_expect = 500.1;
      float wavelength2_expect = 600.5;
      float polarization_angle1_expect = 1.1;
      float polarization_angle2_expect = 1.2;
      
      file2.add_row(1,wavelength1_expect, polarization_angle1_expect, max_number_frame);
      file2.add_row(2,wavelength2_expect, polarization_angle2_expect, max_number_frame);
    }  // end scope should close the file.

//-----------------------------------------------------------------------
// Test constructor with incompatible field map, due to swath name
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    SimpleFieldMap2 simple_field_map2;
    try {
      L1B1File file2(filename2, MSPI::Shared::L1B1File::READ, simple_field_map2);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Swath name 'Row2', in file, does not match row name 'Row4', in field map.  Row_number: 2   Filename: File/L1B1File/tmp/l1b1_file_test2.hdf5")) {
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
// Disable HDF-EOS error printing for subsequent tests.
//-----------------------------------------------------------------------

    HE5_EHset_error_on(2,0);

//-----------------------------------------------------------------------
// Test constructor with incompatible field map, due to field name mismatch.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    SimpleFieldMap3 simple_field_map3;
    try {
      L1B1File file2(filename2, MSPI::Shared::L1B1File::READ, simple_field_map3);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Trouble with SWfieldinfo for required field name: E,  swath: Row2,  Filename: File/L1B1File/tmp/l1b1_file_test2.hdf5")) {
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
// Re-enable HDF-EOS error printing.
//----------------------------------------------------------------------- 

    HE5_EHset_error_on(0,0);

//-----------------------------------------------------------------------
// Create a new file and write some data to it.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename3 = "File/L1B1File/tmp/l1b1_file_test3.hdf5";
    {
      L1B1File file3(filename3, MSPI::Shared::L1B1File::CREATE, simple_field_map);

      float wavelength1_expect = 500.1;
      float wavelength2_expect = 600.5;
      float polarization_angle1_expect = 1.1;
      float polarization_angle2_expect = 1.2;
      
      file3.add_row(1,wavelength1_expect, polarization_angle1_expect, max_number_frame);
      file3.add_row(2,wavelength2_expect, polarization_angle2_expect, max_number_frame);

      int row_number = 2;
      size_t number_frame = 123;
      std::vector<double> timeB_expect(number_frame);
      std::vector<std::vector<float> > intensityB_expect(number_frame);
    
      for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
	timeB_expect[iframe] = 18.1 + iframe;
	intensityB_expect[iframe].resize(number_pixel);
	for (size_t ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	  intensityB_expect[iframe][ipixel] = iframe * number_pixel + ipixel;
	}
      }
    
      std::vector<std::vector<float> > param_vector(1);

      for (size_t iframe = 0; iframe < number_frame ; ++iframe) {
	param_vector[0] = intensityB_expect[iframe];
	file3.append(row_number, timeB_expect[iframe], param_vector);
      }
    }  // end scope should close the file.

//-----------------------------------------------------------------------
// Test constructor with an existing file containing some data, read-only
//-----------------------------------------------------------------------

    test_number++;
    L1B1File file3(filename3);

//-----------------------------------------------------------------------
// Test number_frame()
//-----------------------------------------------------------------------

    test_number++;
    number_frame1_expect = 0;
    number_frame2_expect = 123;

    number_frame1 = file3.number_frame(1);
    number_frame2 = file3.number_frame(2);

    if (number_frame1 != number_frame1_expect ||
	number_frame2 != number_frame2_expect) {
      cerr << "number_frame1 = "<<number_frame1
	   << " (expected "<<number_frame1_expect<<")\n";
      cerr << "number_frame2 = "<<number_frame2
	   << " (expected "<<number_frame2_expect<<")\n";
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

