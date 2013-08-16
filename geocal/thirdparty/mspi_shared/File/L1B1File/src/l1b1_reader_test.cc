/*===========================================================================
=                                                                           =
=                                L1B1Reader                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "l1b1_reader.h"  	// Definition of L1B1Reader
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h" // Definition of Exception.


using std::cerr;
using MSPI::Shared::L1B1Reader;
using MSPI::Shared::Exception;
using MSPI::Shared::FieldMap;


//-----------------------------------------------------------------------
// Field map for generating test input.
//-----------------------------------------------------------------------

class SimpleFieldMap : public FieldMap {
public:
  SimpleFieldMap() {
    init_grid(2, "Row2", 1, "D");
  };
};

//-----------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

    //-------------------------------------------------------------------
    // Generate test data.
    //-------------------------------------------------------------------

    int number_pixel = 1536;  // Hard coded pixel dimension
    int max_number_frame = 10000;
    int row_number = 2;
    int number_frame = 123;
    float wavelength = 500.1;
    float polarization_angle = 1.1;
    std::vector<double> time(number_frame);
    std::vector<std::vector<float> > intensity(number_frame);
    
    std::vector<std::vector<float> > param_vector(1);

    for (int iframe = 0; iframe < number_frame ; ++iframe) {
      time[iframe] = 18.1 + iframe;
      intensity[iframe].resize(number_pixel);
      for (int ipixel = 0 ; ipixel < number_pixel ; ++ipixel) {
	intensity[iframe][ipixel] = iframe * number_pixel + ipixel;
      }
      param_vector[0] = intensity[iframe];
    }

    //-------------------------------------------------------------------
    // Create HDF4 file for testing.
    //-------------------------------------------------------------------


    std::string filename_h4 = "File/L1B1File/tmp/l1b1_reader_test.hdf4";
    {
      SimpleFieldMap simple_field_map;
      MSPI::Shared::HDF4::L1B1File 
	file(filename_h4, MSPI::Shared::HDF4::L1B1File::CREATE,
	     simple_field_map);

      
      file.add_row(row_number,wavelength, polarization_angle, 
		   max_number_frame);

      for (int iframe = 0; iframe < number_frame ; ++iframe) {
	file.append(row_number, time[iframe], param_vector);
      }

      file.epoch("1994-01-01T00:00:00.123456Z");
    }

    //-------------------------------------------------------------------
    // Create HDF5 file for testing.
    //-------------------------------------------------------------------

    std::string filename_h5 = "File/L1B1File/tmp/l1b1_reader_test.hdf5";
    {
      SimpleFieldMap simple_field_map;
      MSPI::Shared::L1B1File
	file(filename_h5, MSPI::Shared::L1B1File::CREATE, 
	     simple_field_map);

      file.add_row(row_number,wavelength, polarization_angle, 
		   max_number_frame);

      for (int iframe = 0; iframe < number_frame ; ++iframe) {
	file.append(row_number, time[iframe], param_vector);
      }
      
      file.epoch("1994-01-01T00:00:00.123456Z");
    }

    //-------------------------------------------------------------------
    // Test constructor.
    //-------------------------------------------------------------------
    
    test_number++;

    L1B1Reader reader1(filename_h4);
    L1B1Reader reader2(filename_h5);

    //-------------------------------------------------------------------
    // Test meta()
    //-------------------------------------------------------------------

    reader1.meta();
    reader2.meta();

    //-------------------------------------------------------------------
    // Test granule_id()
    //-------------------------------------------------------------------

    reader1.granule_id();
    reader2.granule_id();

    //-------------------------------------------------------------------
    // Test std::string epoch()
    //-------------------------------------------------------------------

    reader1.epoch();
    reader2.epoch();

    //-------------------------------------------------------------------
    // Test number_frame(int Row_number)
    //-------------------------------------------------------------------

    reader1.number_frame(row_number);
    reader2.number_frame(row_number);

    //-------------------------------------------------------------------
    // Test size_t number_pixel()
    //-------------------------------------------------------------------
    
    reader1.number_pixel();
    reader2.number_pixel();

    //-------------------------------------------------------------------
    // Test std::set<int> row_numbers()
    //-------------------------------------------------------------------

    reader1.row_numbers();
    reader2.row_numbers();

    //-------------------------------------------------------------------
    // Test std::vector<std::string> field_names(int Row_number)
    //-------------------------------------------------------------------

    reader1.field_names(row_number);
    reader2.field_names(row_number);

    //-------------------------------------------------------------------
    // Test std::vector<double> read_time(...)
    //-------------------------------------------------------------------

    reader1.read_time(row_number, 0, 1);
    reader2.read_time(row_number, 0, 1);

    //-------------------------------------------------------------------
    // Test boost::multi_array<float, 2> read_data(...)
    //-------------------------------------------------------------------

    std::string field_name = "D";
    reader1.read_data(row_number, field_name, 0, 1);
    reader2.read_data(row_number, field_name, 0, 1);

    //-------------------------------------------------------------------
    // Test wavelength(int Row_number)
    //-------------------------------------------------------------------
    
    reader1.wavelength(row_number);
    reader2.wavelength(row_number);

    //-------------------------------------------------------------------
    // Test float polarization_angle(int Row_number)
    //-------------------------------------------------------------------

    reader1.polarization_angle(row_number);
    reader2.polarization_angle(row_number);

    //-------------------------------------------------------------------
    // Test std::string swath_name(int Row_number) const
    //-------------------------------------------------------------------

    reader1.swath_name(row_number);
    reader2.swath_name(row_number);

    //-------------------------------------------------------------------
    // Check for test failures.
    //-------------------------------------------------------------------

    if (error) {
      cerr << "Error! One or more tests failed.\n";
    } else {
      cerr << "All tests succeeded.\n";
    }

  //---------------------------------------------------------------------
  // End try block for main.
  //---------------------------------------------------------------------
    
  }

  //---------------------------------------------------------------------
  // Catch exceptions.
  //---------------------------------------------------------------------

  catch(const Exception& e) {
    cerr << "catch(const Exception& e): " << e.what() << "\n"
	 << "Stack trace:\n" << e.stack_trace() << "\n"
	 << "test_number = " << test_number <<"\n";
  }
  catch(...) {
    cerr << "catch (...)\n"
	 << "test_number = " << test_number <<"\n";
  }
}
