//===========================================================================
//                                                                          
//                                 L1AFile                                  
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "l1a_file.h"		// Definition of L1AFile
#include <iostream>		// Definition of cerr.
#include <vector>		// Definition of std::vector.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.
#include "MSPI-Shared/UnitTest/src/float_comparator.h" // Definition of FloatComparator
#include "MSPI-Shared/UnitTest/src/check.h" // Definition of array_check
#include "MSPI-Shared/File/FieldMap/src/configured_field_map.h"
				// Definition of ConfiguredFieldMap


using std::cerr;
using MSPI::Shared::L1AFile;
using MSPI::Shared::FieldMap;
using MSPI::Shared::FrameInfo;
using MSPI::Shared::Exception;
using MSPI::Shared::FloatComparator;
using MSPI::Shared::ConfiguredFieldMap;
using MSPI::Shared::Meta;
using MSPI::Shared::vector_check;
using MSPI::Shared::array_check;


/////////////////////////////////////////////////////////////////////////
// Generate unit-test input
/////////////////////////////////////////////////////////////////////////

const size_t max_number_subframe = 35;
const size_t number_pixel = 1536;

//-----------------------------------------------------------------------
class SimpleFieldMap2 : public FieldMap {
public:
  SimpleFieldMap2() {
    init_grid(63, "355-I", 4, "p1", "p2", "xn","dn");
    init_grid(47, "470-Z", 4, "p1", "p2", "x0n","d0n");
  };
};

//-----------------------------------------------------------------------
class SimpleFieldMap3 : public FieldMap {
public:
  SimpleFieldMap3() {
    init_grid(63, "355-I", 4, "p1", "p2", "xn","dn");
    init_grid(47, "470-Q", 4, "p1", "p2", "ugh","d0n");
  };
};

//-----------------------------------------------------------------------
boost::multi_array<float,2> p1_init(size_t Number_frame)
{
  boost::multi_array<float,2> data(boost::extents[Number_frame][max_number_subframe]);
  for (size_t i = 0 ; i < Number_frame ; i++) {
    for (size_t j = 0 ; j < max_number_subframe ; j++) {
      data[i][j] = i * 7 + j * 0.01;
    }
  }
  return data;
}

//-----------------------------------------------------------------------
boost::multi_array<float,2> p2_init(size_t Number_frame)
{
  boost::multi_array<float,2> data(boost::extents[Number_frame][max_number_subframe]);
  for (size_t i = 0 ; i < Number_frame ; i++) {
    for (size_t j = 0 ; j < max_number_subframe ; j++) {
      data[i][j] = i * 13 + j * 0.01;
    }
  }
  return data;
}

//-----------------------------------------------------------------------
boost::multi_array<float,2> xn_init(size_t Number_frame)
{
  boost::multi_array<float,2> data(boost::extents[Number_frame][max_number_subframe]);
  for (size_t i = 0 ; i < Number_frame ; i++) {
    for (size_t j = 0 ; j < max_number_subframe ; j++) {
      data[i][j] = i * 17 + j * 0.01;
    }
  }
  return data;
}

//-----------------------------------------------------------------------
boost::multi_array<unsigned short,3> dn_init(size_t Number_frame)
{
  boost::multi_array<unsigned short,3> data(boost::extents[Number_frame][max_number_subframe][number_pixel]);
  int count = 0;
  for (size_t i = 0 ; i < Number_frame ; i++) {
    for (size_t j = 0 ; j < max_number_subframe ; j++) {
      for (size_t k = 0 ; k < number_pixel ; k++) {
	data[i][j][k] = count++;
      }
    }
  }
  return data;
}

//-----------------------------------------------------------------------
std::vector<unsigned> subframe_count_init(size_t Number_frame)
{
  std::vector<unsigned> data(Number_frame, max_number_subframe);
  return data;
}

/////////////////////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////////////////////

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  FloatComparator cmp(10.0);	// Float comparator

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test constructor.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename = "File/L1AFile/tmp/l1a_file_test.hdf5";
    ConfiguredFieldMap simple_field_map("File/L1AFile/src/l1a_file_test.simple_field_map");
    int max_number_frame = 10000;
    L1AFile file1(filename, MSPI::Shared::L1AFile::CREATE, simple_field_map,
		  max_number_frame, max_number_subframe);

//-----------------------------------------------------------------------
// Test epoch()
//-----------------------------------------------------------------------
    
    test_number++;
    
    std::string epoch_expect = "1993-01-01T00:00:00.000000Z";
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
    
    if (file1.granule_id() != "l1a_file_test.hdf5") {
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
      if (meta_out.number_element() != 5) {
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
      field_names1_expect = simple_field_map.field_names(63);
    std::vector<std::string> 
      field_names2_expect = simple_field_map.field_names(47);

    std::vector<std::string> field_names1 = file1.field_names(63);
    std::vector<std::string> field_names2 = file1.field_names(47);

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
    int number_pixel_expect = 1536;
    int number_pixel = file1.number_pixel();
    if (number_pixel != number_pixel_expect) {
      cerr << "number_pixel = "<<number_pixel
	   << " (expected "<<number_pixel_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test max_number_subframes()
//-----------------------------------------------------------------------

    test_number++;
    if (file1.max_number_subframe() != max_number_subframe) {
      cerr << "max_number_subframe = "<<file1.max_number_subframe()
	   << " (expected "<<max_number_subframe<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test row_numbers(), for initial empty file
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
    int number_frame_expect = 0;
    int number_frame = file1.number_frame(1);
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
    float wavelength3_expect = 700.9;
    float polarization_angle1_expect = 1.1;
    float polarization_angle2_expect = 1.2;
    float polarization_angle3_expect = 1.3;

    file1.add_row(63,wavelength1_expect, polarization_angle1_expect);
    file1.add_row(47,wavelength2_expect, polarization_angle2_expect);
    file1.add_row(42,wavelength3_expect, polarization_angle3_expect);

//-----------------------------------------------------------------------
// Test number_frame(), for initial empty file
//-----------------------------------------------------------------------

    test_number++;
    int number_frame1_expect = 0;
    int number_frame2_expect = 0;
    int number_frame3_expect = 0;

    int number_frame1 = file1.number_frame(63);
    int number_frame2 = file1.number_frame(47);
    int number_frame3 = file1.number_frame(42);

    if (number_frame1 != number_frame1_expect ||
	number_frame2 != number_frame2_expect ||
	number_frame3 != number_frame3_expect) {
      cerr << "number_frame1 = "<<number_frame1
	   << " (expected "<<number_frame1_expect<<")\n";
      cerr << "number_frame2 = "<<number_frame2
	   << " (expected "<<number_frame2_expect<<")\n";
      cerr << "number_frame3 = "<<number_frame3
	   << " (expected "<<number_frame3_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test wavelength()
//-----------------------------------------------------------------------

    test_number++;
    float wavelength1 = file1.wavelength(63);
    float wavelength2 = file1.wavelength(47);
    float wavelength3 = file1.wavelength(42);
    if (wavelength1 != wavelength1_expect ||
	wavelength2 != wavelength2_expect ||
	wavelength3 != wavelength3_expect) {
      cerr << "wavelength1 = "<<wavelength1
	   << " (expected "<<wavelength1_expect<<")\n";
      cerr << "wavelength2 = "<<wavelength2
	   << " (expected "<<wavelength2_expect<<")\n";
      cerr << "wavelength3 = "<<wavelength3
	   << " (expected "<<wavelength3_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test polarization_angle()
//-----------------------------------------------------------------------
      
    test_number++;
    float polarization_angle1 = file1.polarization_angle(63);
    float polarization_angle2 = file1.polarization_angle(47);
    float polarization_angle3 = file1.polarization_angle(42);
    if (polarization_angle1 != polarization_angle1_expect ||
	polarization_angle2 != polarization_angle2_expect ||
	polarization_angle3 != polarization_angle3_expect) {
      cerr << "polarization_angle1 = '"<<polarization_angle1
	   << "' (expected '"<<polarization_angle1_expect<<"')\n";
      cerr << "polarization_angle2 = '"<<polarization_angle2
	   << "' (expected '"<<polarization_angle2_expect<<"')\n";
      cerr << "polarization_angle3 = '"<<polarization_angle3
	   << "' (expected '"<<polarization_angle3_expect<<"')\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test swath_name()
//-----------------------------------------------------------------------
      
    test_number++;
    std::string swath_name1_expect = "355-I";
    std::string swath_name2_expect = "470-Q";
    std::string swath_name3_expect = "470-U";

    std::string swath_name1 = file1.swath_name(63);
    std::string swath_name2 = file1.swath_name(47);
    std::string swath_name3 = file1.swath_name(42);
    if (swath_name1 != swath_name1_expect ||
	swath_name2 != swath_name2_expect ||
	swath_name3 != swath_name3_expect) {
      cerr << "swath_name1 = '"<<swath_name1
	   << "' (expected '"<<swath_name1_expect<<"')\n";
      cerr << "swath_name2 = '"<<swath_name2
	   << "' (expected '"<<swath_name2_expect<<"')\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      cerr << "swath_name3 = '"<<swath_name3
	   << "' (expected '"<<swath_name3_expect<<"')\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test read_time(), for uninitialized frames returns zero size vector.
//-----------------------------------------------------------------------

    test_number++;
    int row_number = 47;
    int frame_number = 500;

    std::vector<float> time = file1.read_time(row_number, frame_number);
    
    if (time.size()) {
      cerr << "time.size = '"<<time.size()
	   << "'  (expected 0)'\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test read_data(), for uninitialized frames returns zero size array.
//-----------------------------------------------------------------------

    test_number++;
    row_number = 47;
    frame_number = 500;

    boost::multi_array<unsigned short, 2> data = file1.read_data(row_number, frame_number);
    
    if (data.size()) {
      cerr << "data.size = '"<<data.size()
	   << "'  (expected 0)'\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test write_frame()
// Test read_time()
// Test read_data()
//-----------------------------------------------------------------------

    test_number++;
    number_frame = 5;
    int number_subframe = 6;
    number_pixel = 1536;

    std::vector<float> p1_expect(number_subframe);
    std::vector<float> p2_expect(number_subframe);
    std::vector<float> xn_expect(number_subframe);
    boost::multi_array<unsigned short, 2>
      dn_expect(boost::extents[number_subframe][number_pixel]);

    std::vector<int> row_numbers = file1.row_numbers();

    for (std::vector<int>::iterator irow = row_numbers.begin();
	 irow != row_numbers.end(); ++irow) {

      for (int iframe = 0; iframe < number_frame; ++iframe) {

	for (int isubframe = 0 ; isubframe < number_subframe ; ++isubframe) {
	  p1_expect[isubframe] = (*irow) * number_frame + iframe * number_subframe + isubframe + 19;
	  p2_expect[isubframe] = (*irow) * number_frame + iframe * number_subframe + isubframe + 17;
	  xn_expect[isubframe] = (*irow) * number_frame + iframe * number_subframe + isubframe;
	  for (int ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	    dn_expect[isubframe][ipixel] = 
	      7 + (*irow) * number_frame + iframe * number_subframe + 
	      isubframe * number_pixel + ipixel;
	  }
	}

	file1.write_frame(*irow, iframe, p1_expect, p2_expect, xn_expect, dn_expect);
      }
    }

    for (std::vector<int>::iterator irow = row_numbers.begin();
	 irow != row_numbers.end(); ++irow) {
      for (int iframe = 0; iframe < number_frame; ++iframe) {

	std::vector<float> xn = file1.read_time(*irow, iframe);
	boost::multi_array<unsigned short, 2> dn = file1.read_data(*irow, iframe);

	for (int isubframe = 0; isubframe < number_subframe; ++isubframe) {
	  xn_expect[isubframe] = (*irow) * number_frame + iframe * number_subframe + isubframe;
	  if (xn[isubframe] != xn_expect[isubframe]) {
	    cerr << "xn["<<isubframe<<"]="<<xn[isubframe]
	         << " (expected "<<xn_expect[isubframe]<<")\n";
	    cerr << "Unexpected result at line "<<__LINE__<<"\n";
	    error = true;
	  }
	  for (int ipixel = 0; ipixel < number_pixel; ++ipixel) {
	    dn_expect[isubframe][ipixel] = 
	      7 + (*irow) * number_frame + iframe * number_subframe + 
	      isubframe * number_pixel + ipixel;
	    if (dn[isubframe][ipixel] != dn_expect[isubframe][ipixel]) {
	      cerr << "dn["<<isubframe<<"]["<<ipixel<<"]="<<dn[isubframe][ipixel]
		   << " (expected "<<dn_expect[isubframe][ipixel]<<")\n";
	      cerr << "Unexpected result at line "<<__LINE__<<"\n";
	      error = true;
	    }
	  }
	}
      }
    }

//-----------------------------------------------------------------------
// Test write_frames()
// Test read_frames()
//-----------------------------------------------------------------------

    {
      test_number++;

      size_t number_frame = 10;
      boost::multi_array<float,2> p1_in(p1_init(number_frame));
      boost::multi_array<float,2> p2_in(p2_init(number_frame));
      boost::multi_array<float,2> xn_in(xn_init(number_frame));
      boost::multi_array<unsigned short,3> dn_in(dn_init(number_frame));
      std::vector<unsigned> subframe_count_in(number_frame);
      
      int row_number = 47;
      size_t start_frame_number = 400;
      file1.write_frames(row_number, start_frame_number,
			 p1_in, p2_in, xn_in, dn_in, subframe_count_in);
      
      boost::multi_array<float,2> p1;
      boost::multi_array<float,2> p2;
      boost::multi_array<float,2> xn;
      boost::multi_array<unsigned short,3> dn;
      std::vector<unsigned> subframe_count;

      file1.read_frames(row_number, start_frame_number, number_frame,
			p1, p2, xn, dn, subframe_count);

      FloatComparator cmp;
      array_check(p1_in, p1, cmp, __LINE__, error);
      array_check(p2_in, p2, cmp, __LINE__, error);
      array_check(xn_in, xn, cmp, __LINE__, error);
      array_check(dn_in, dn, __LINE__, error);
      vector_check(subframe_count_in, subframe_count, __LINE__, error);

      if (file1.number_frame(row_number) != 410) {
	cerr << "number_frame = " << file1.number_frame(row_number) << "\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test read_time(), for uninitialized frames returns zero size vector.
//-----------------------------------------------------------------------

    test_number++;
    row_number = 47;
    frame_number = 500;

    time = file1.read_time(row_number, frame_number);
    
    if (time.size()) {
      cerr << "time.size = '"<<time.size()
	   << "'  (expected 0)'\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test read_data(), for uninitialized frames returns zero size array.
//-----------------------------------------------------------------------

    test_number++;
    row_number = 47;
    frame_number = 500;

    data = file1.read_data(row_number, frame_number);
    
    if (data.size()) {
      cerr << "data.size = '"<<data.size()
	   << "'  (expected 0)'\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Argument check: write_frame() with invalid row_number.
//-----------------------------------------------------------------------


    test_number++;
    row_number = 197;
    frame_number = 0;
    bool caught_exception = false;
    try {
      file1.write_frame(row_number, frame_number, p1_expect, p2_expect, 
			xn_expect, dn_expect);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "FieldMap : unexpected grid number: 197")) {
	cerr << "write_frame() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "write_frame() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Argument check: write_frame() with invalid frame_number (too high).
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    row_number = 63;
    frame_number = 20000;
    try {
      file1.write_frame(row_number, frame_number, p1_expect, p2_expect, 
			xn_expect, dn_expect);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Start_frame_number + number_frame > max_number_frame()")) {
	cerr << "write_frame() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "write_frame() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Argument check: write_frame() with invalid frame_number (already exists).
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    row_number = 63;
    frame_number = 1;
    try {
      file1.write_frame(row_number, frame_number, p1_expect, p2_expect,
			xn_expect, dn_expect);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Start_frame_number < number_frame(Row_number) = 5")) {
	cerr << "write_frame() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "write_frame() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Argument check: P2.shape()[0] != number_frame
//                 P2.shape()[1] != number_subframe
//-----------------------------------------------------------------------

    test_number++;

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[2][max_number_subframe]);
      boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe]);
      boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe][number_pixel]);
      std::vector<unsigned> subframe_count(1,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "P2.shape()[0] != number_frame")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe+1]);
      boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe]);
      boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe][number_pixel]);
      std::vector<unsigned> subframe_count(1,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "P2.shape()[1] != number_subframe")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Argument check: Xn.shape()[0] != number_frame
//                 Xn.shape()[1] != number_subframe
//-----------------------------------------------------------------------

    test_number++;

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> xn(boost::extents[2][max_number_subframe]);
      boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe][number_pixel]);
      std::vector<unsigned> subframe_count(1,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Xn.shape()[0] != number_frame")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe+1]);
      boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe][number_pixel]);
      std::vector<unsigned> subframe_count(1,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Xn.shape()[1] != number_subframe")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Argument check: Dn.shape()[0] != number_frame
//                 Dn.shape()[1] != number_subframe
//-----------------------------------------------------------------------

    test_number++;

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe]);
      boost::multi_array<unsigned short,3> dn(boost::extents[2][max_number_subframe][number_pixel]);
      std::vector<unsigned> subframe_count(1,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Dn.shape()[0] != number_frame")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe]);
      boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe+1][number_pixel]);
      std::vector<unsigned> subframe_count(1,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Dn.shape()[1] != number_subframe")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe]);
      boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe][number_pixel+1]);
      std::vector<unsigned> subframe_count(1,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Dn.shape()[2] != number_pixel()")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Argument check: Subframe_count.size() != number_frame
//-----------------------------------------------------------------------

    {
      caught_exception = false;
      row_number = 63;
      frame_number = 5;
      boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe]);
      boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe]);
      boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe][number_pixel]);
      std::vector<unsigned> subframe_count(2,max_number_subframe-1);

      try {
	file1.write_frames(row_number, frame_number, p1, p2, xn, dn, subframe_count);
      } catch (Exception& e) {
	caught_exception = true;
	if (strcmp(e.what(), "Subframe_count.size() != number_frame")) {
	  cerr << "write_frame() exception = "<<e.what()<<"\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
      if (!caught_exception) {
	cerr << "write_frame() exception not caught\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }

//-----------------------------------------------------------------------
// Test frame_info() on a file with empty frame info.
//-----------------------------------------------------------------------

    test_number++;
    FrameInfo frame_info = file1.frame_info(500);

    float T_expect = -9999.0;
    float tframe_nominal_expect = -9999.0;
    float omega_bar_expect = -9999.0;
    float omega_beat_expect = -9999.0;
    float tframe_measured_expect = -9999.0;
    float timetag_expect = -9999.0;
    if (frame_info.T() != -9999.0 ||
	frame_info.tframe_nominal() != -9999.0 ||
	frame_info.omega_bar() != -9999.0 ||
	frame_info.omega_beat() != -9999.0 ||
	frame_info.tframe_measured() != -9999.0 ||
	frame_info.timetag() != -9999.0) {
      cerr << "T = "<<frame_info.T()
	   << " (expected "<<T_expect<<")\n";
      cerr << "tframe_nominal = "<<frame_info.tframe_nominal()
	   << " (expected "<<tframe_nominal_expect<<")\n";
      cerr << "omega_bar = "<<frame_info.omega_bar()
	   << " (expected "<<omega_bar_expect<<")\n";
      cerr << "omega_beat = "<<frame_info.omega_beat()
	   << " (expected "<<omega_beat_expect<<")\n";
      cerr << "tframe_measured = "<<frame_info.tframe_measured()
	   << " (expected "<<tframe_measured_expect<<")\n";
      cerr << "timetag = "<<frame_info.timetag()
	   << " (expected "<<timetag_expect<<")\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test write_frame_info().
// Test frame_info().
//-----------------------------------------------------------------------

    {
      test_number++;
      int number_frame_expect = 4;
      std::vector<FrameInfo> frame_info_expect(number_frame_expect);
      frame_info_expect[0] = FrameInfo(100.4, 100.3, 100.2, 100.1, 200.1, 200.2);
      frame_info_expect[1] = FrameInfo(95.0, 34.0, 35.0, 36.0);
      frame_info_expect[2] = FrameInfo();
      frame_info_expect[3] = FrameInfo(1.1, 2.2, 3.3, 4.4, 5.5, 6.6);

      for (int iframe = 0; iframe < number_frame_expect; ++iframe) {
	file1.write_frame_info(iframe,frame_info_expect[iframe]);
      }

      int number_frame = file1.number_frame(FRAME_INFO_IDX);
      if (number_frame != number_frame_expect) {
	cerr << "number_frame = "<<number_frame
	     << " (expected "<<number_frame_expect<<")\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }

      for (int iframe = 0; iframe < number_frame; ++iframe) {
	FrameInfo frame_info = file1.frame_info(iframe);
	if (cmp.neq(frame_info.T(),frame_info_expect[iframe].T()) ||
	    cmp.neq(frame_info.tframe_nominal(),frame_info_expect[iframe].tframe_nominal()) ||
	    cmp.neq(frame_info.omega_bar(),frame_info_expect[iframe].omega_bar()) ||
	    cmp.neq(frame_info.omega_beat(),frame_info_expect[iframe].omega_beat()) ||
	    cmp.neq(frame_info.tframe_measured(),frame_info_expect[iframe].tframe_measured()) ||
	    cmp.neq(frame_info.timetag(),frame_info_expect[iframe].timetag())) {
	  cerr << "frame_number = "<<iframe<<"\n";
	  cerr << "T = "<<frame_info.T()
	       << " (expected "<<frame_info_expect[iframe].T()<<")\n";
	  cerr << "tframe_nominal = "<<frame_info.tframe_nominal()
	       << " (expected "<<frame_info_expect[iframe].tframe_nominal()<<")\n";
	  cerr << "omega_bar = "<<frame_info.omega_bar()
	       << " (expected "<<frame_info_expect[iframe].omega_bar()<<")\n";
	  cerr << "omega_beat = "<<frame_info.omega_beat()
	       << " (expected "<<frame_info_expect[iframe].omega_beat()<<")\n";
	  cerr << "tframe_measured = "<<frame_info.tframe_measured()
	       << " (expected "<<frame_info_expect[iframe].tframe_measured()<<")\n";
	  cerr << "timetag = "<<frame_info.timetag()
	       << " (expected "<<frame_info_expect[iframe].timetag()<<")\n";
	  cerr << "Unexpected result at line "<<__LINE__<<"\n";
	  error = true;
	}
      }
    }

//-----------------------------------------------------------------------
// Test write_frame_info() invalid Frame_number argument (too high).
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      file1.write_frame_info(20,frame_info);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Argument check: Frame_number != number_frame(FRAME_INFO_IDX), Frame_number = 20, number_frame(FRAME_INFO_IDX) = 4")) {
	cerr << "write_frame_info() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "write_frame_info() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }
    
//-----------------------------------------------------------------------
// Test write_frame_info() when frame already exists.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      file1.write_frame_info(2,frame_info);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Argument check: Frame_number != number_frame(FRAME_INFO_IDX), Frame_number = 2, number_frame(FRAME_INFO_IDX) = 4")) {
	cerr << "write_frame_info() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "write_frame_info() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Test add_row() with a row that's already created.
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    try {
      file1.add_row(63,wavelength2_expect, polarization_angle2_expect);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Row number already in table: 63")) {
	cerr << "add_row() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "add_row() exception not caught\n";
      cerr << "Unexpected result at line "<<__LINE__<<"\n";
      error = true;
    }

//-----------------------------------------------------------------------
// Try creating the same file twice.  This seems to be an effective
// to test that the destructor has closed all open file handles.
//-----------------------------------------------------------------------

    test_number++;
    std::string filename2 = "File/L1AFile/tmp/l1a_file_test2.hdf5";
    {
      L1AFile file2(filename2, MSPI::Shared::L1AFile::CREATE, simple_field_map,
		    max_number_frame, max_number_subframe);

      float wavelength1_expect = 500.1;
      float wavelength2_expect = 600.5;
      float polarization_angle1_expect = 1.1;
      float polarization_angle2_expect = 1.2;
      
      file2.add_row(63,wavelength1_expect, polarization_angle1_expect);
      file2.add_row(47,wavelength2_expect, polarization_angle2_expect);
    }  // end scope should close the file.

    test_number++;
    {
      L1AFile file2(filename2, MSPI::Shared::L1AFile::CREATE, simple_field_map,
		    max_number_frame, max_number_subframe);

      float wavelength1_expect = 500.1;
      float wavelength2_expect = 600.5;
      float polarization_angle1_expect = 1.1;
      float polarization_angle2_expect = 1.2;
      
      file2.add_row(63,wavelength1_expect, polarization_angle1_expect);
      file2.add_row(47,wavelength2_expect, polarization_angle2_expect);
    }  // end scope should close the file.

//-----------------------------------------------------------------------
// Test constructor with incompatible field map, due to swath name
//-----------------------------------------------------------------------

    test_number++;
    caught_exception = false;
    SimpleFieldMap2 simple_field_map2;
    try {
      L1AFile file2(filename2, MSPI::Shared::L1AFile::READ, simple_field_map2,
		    max_number_frame, max_number_subframe);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Swath name '470-Q', in file, does not match row name '470-Z', in field map.  Row_number: 47   Filename: File/L1AFile/tmp/l1a_file_test2.hdf5")) {
	cerr << "L1AFile() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "L1AFile() exception not caught\n";
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
      L1AFile file2(filename2, MSPI::Shared::L1AFile::READ, simple_field_map3,
		    max_number_frame, max_number_subframe);
    } catch (Exception& e) {
      caught_exception = true;
      if (strcmp(e.what(), "Trouble with SWfieldinfo for required field name: ugh,  swath: 470-Q,  Filename: File/L1AFile/tmp/l1a_file_test2.hdf5")) {
	cerr << "L1AFile() exception = "<<e.what()<<"\n";
	cerr << "Unexpected result at line "<<__LINE__<<"\n";
	error = true;
      }
    }
    if (!caught_exception) {
      cerr << "L1AFile() exception not caught\n";
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
    std::string filename3 = "File/L1AFile/tmp/l1a_file_test3.hdf5";
    {
      L1AFile file3(filename3, MSPI::Shared::L1AFile::CREATE, simple_field_map,
		    max_number_frame, max_number_subframe);

      float wavelength1_expect = 500.1;
      float wavelength2_expect = 600.5;
      float polarization_angle1_expect = 1.1;
      float polarization_angle2_expect = 1.2;
      
      file3.add_row(63,wavelength1_expect, polarization_angle1_expect);
      file3.add_row(47,wavelength2_expect, polarization_angle2_expect);

      int row_number = 47;
      int number_frame = 8;
      int number_subframe = 6;
      int number_pixel = 1536;
      std::vector<float> xn_expect(number_subframe);
      boost::multi_array<unsigned short, 2>
	dn_expect(boost::extents[number_subframe][number_pixel]);

      for (int isubframe = 0; isubframe < number_subframe ; ++isubframe) {
	xn_expect[isubframe] = 18.1 + isubframe;
	for (int ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	  dn_expect[isubframe][ipixel] = isubframe * number_pixel + ipixel;
	}
      }

      for (int iframe = 0; iframe < number_frame ; ++iframe) {
	file3.write_frame(row_number, iframe, p1_expect, p2_expect,
			  xn_expect, dn_expect);
      }
    }  // end scope should close the file.

//-----------------------------------------------------------------------
// Test constructor with an existing file containing some data, read-only
//-----------------------------------------------------------------------

    test_number++;
    L1AFile file3(filename3, MSPI::Shared::L1AFile::READ, simple_field_map,
		  max_number_frame, max_number_subframe);

//-----------------------------------------------------------------------
// Test number_frame()
//-----------------------------------------------------------------------

    test_number++;
    number_frame1_expect = 0;
    number_frame2_expect = 8;

    number_frame1 = file3.number_frame(63);
    number_frame2 = file3.number_frame(47);

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
