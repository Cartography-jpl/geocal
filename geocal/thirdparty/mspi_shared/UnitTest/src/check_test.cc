/*===========================================================================
=                                                                           =
=                                Check                                      =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "check.h" // Definition of Check
#include <iostream>		// Definition of cerr.
#include "MSPI-Shared/ErrorHandling/src/exception.h"
				// Definition of Exception.
#include <sstream> 		// Definition of std::ostringstream
#include <fstream> 		// Definition of std::ofstream

using std::cerr;
using MSPI::Shared::file_check;
using MSPI::Shared::array_check;
using MSPI::Shared::vector_check;
using MSPI::Shared::scalar_check_int;
using MSPI::Shared::scalar_check_float;
using MSPI::Shared::scalar_check_double;
using MSPI::Shared::Exception;
using MSPI::Shared::FloatComparator;
using MSPI::Shared::DoubleComparator;

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

namespace {
  void check_negative(bool Error_flag, const std::string& Message,
		      const std::string& Message_expect,
		      int Line, bool& Error) {
    if (Error_flag == false) {
      cerr << "check_negative: Test_error = " << Error_flag << "\n";
      cerr << "check_negative: Unexpected result at line "<< Line <<"\n";
      Error = true;
    }

    if (Message.substr(0,Message_expect.size()) != Message_expect) {
      cerr << "check_negative: Message = " << Message << "\n"
	   << " (expected '" << Message_expect << "'...)\n";
      cerr << "check_negative: Unexpected result at line "<< Line <<"\n";
      Error = true;
    }
  }
  void check_positive(bool Error_flag, const std::string& Message, 
		      int Line, bool& Error) {
    if (Error_flag == true) {
      cerr << "check_positive: Test_error = " << Error_flag << "\n";
      cerr << "check_positive: Unexpected result at line "<< Line <<"\n";
      Error = true;
    }
    
    if (Message != "") {
      cerr << "check_positive: Message = " << Message << "\n";
      cerr << "check_positive: Unexpected result at line "<< Line <<"\n";
      Error = true;
    }
  }
}

//-----------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------

int main()
{
  int test_number = 0;		// Number identifying this test. 
  bool error = false;		// Flag indicating test failure.
  FloatComparator fcmp;
  DoubleComparator dcmp;

//-----------------------------------------------------------------------
// Begin try block for main.
//-----------------------------------------------------------------------

  try {

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<float,2>,...)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::ostringstream out;

      boost::multi_array<float,2> data(boost::extents[3][4]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 1.3;
      }

      boost::multi_array<float,2> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, fcmp, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2] += 0.01;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, fcmp, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<double,2>,...)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::ostringstream out;

      boost::multi_array<double,2> data(boost::extents[3][4]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 1.3;
      }

      boost::multi_array<double,2> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, dcmp, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2] += 0.01;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, dcmp, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<float,3>,...)
//-----------------------------------------------------------------------

    test_number++;
    {

      boost::multi_array<float,3> data(boost::extents[3][4][5]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 1.3;
      }

      boost::multi_array<float,3> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, fcmp, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2][3] += 0.01;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, fcmp, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<double,3>,...)
//-----------------------------------------------------------------------

    test_number++;
    {

      boost::multi_array<double,3> data(boost::extents[3][4][5]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 1.3;
      }

      boost::multi_array<double,3> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, dcmp, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2][3] += 0.01;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, dcmp, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<float,4>,...)
//-----------------------------------------------------------------------

    test_number++;
    {

      boost::multi_array<float,4> data(boost::extents[3][4][5][6]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 1.3;
      }

      boost::multi_array<float,4> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, fcmp, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2][3][4] += 0.01;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, fcmp, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<double,4>,...)
//-----------------------------------------------------------------------

    test_number++;
    {

      boost::multi_array<double,4> data(boost::extents[3][4][5][6]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 1.3;
      }

      boost::multi_array<double,4> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, dcmp, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2][3][4] += 0.01;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, dcmp, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<int,2>,...)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::ostringstream out;

      boost::multi_array<int,2> data(boost::extents[3][4]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 7;
      }

      boost::multi_array<int,2> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2] += 1;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "value[1][2]", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test array_check(boost::multi_array<int,3>,...)
//-----------------------------------------------------------------------

    test_number++;
    {
      std::ostringstream out;

      boost::multi_array<int,3> data(boost::extents[3][4][5]);
      for (size_t i = 0 ; i < data.num_elements() ; i++) {
	data.data()[i] = i * 7;
      }

      boost::multi_array<int,3> data2(data);

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[1][2][3] += 1;

      {
	bool error_flag = false;
	std::ostringstream out;
	array_check(data, data2, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "value[1][2][3]", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test scalar checkers
//-----------------------------------------------------------------------

    test_number++;
    {
      int a_int = 10;
      int b_int = 11;
      float a_float = 10.1;
      float b_float = 11.1;
      double a_double = 10.11;
      double b_double = 11.12;

      {
	bool error_flag = false;
	std::ostringstream out;
	scalar_check_int(a_int,a_int,__LINE__,error_flag,out);
	scalar_check_float(a_float,a_float,fcmp,__LINE__,error_flag,out);
	scalar_check_double(a_double,a_double,dcmp,__LINE__,error_flag,out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }


      {
	bool error_flag = false;
	
	std::ostringstream out1;
	scalar_check_int(a_int,b_int,__LINE__,error_flag, out1);
	check_negative(error_flag, out1.str(), "scalar_check_int", __LINE__, error);

	std::ostringstream out2;
	scalar_check_float(a_float,b_float,fcmp,__LINE__,error_flag, out2);
	check_negative(error_flag, out2.str(), "exceeded", __LINE__, error);

	std::ostringstream out3;
	scalar_check_double(a_double,b_double,dcmp,__LINE__,error_flag, out3);
	check_negative(error_flag, out3.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test int vector_check
//-----------------------------------------------------------------------

    test_number++;
    {

      size_t size = 10;
      std::vector<int> data(size);
      for (size_t i = 0 ; i < size ; i++) {
	data[i] = i + 7;
      }

      std::vector<int> data2(data);

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,__LINE__,error_flag,out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[3] += 1;

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,__LINE__,error_flag,out);
	check_negative(error_flag, out.str(), "value[3]", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test char vector_check
//-----------------------------------------------------------------------

    test_number++;
    {

      size_t size = 10;
      std::vector<char> data(size);
      for (size_t i = 0 ; i < size ; i++) {
	data[i] = i + 9;
      }

      std::vector<char> data2(data);

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,__LINE__,error_flag,out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[5] += 1;

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,__LINE__,error_flag,out);
	check_negative(error_flag, out.str(), "value[5]", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test float vector_check
//-----------------------------------------------------------------------

    test_number++;
    {

      size_t size = 10;
      std::vector<float> data(size);
      for (size_t i = 0 ; i < size ; i++) {
	data[i] = i * 9.53;
      }

      std::vector<float> data2(data);

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,__LINE__,error_flag,out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[5] += 1.1;

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,fcmp,__LINE__,error_flag,out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test double vector_check
//-----------------------------------------------------------------------

    test_number++;
    {

      size_t size = 10;
      std::vector<double> data(size);
      for (size_t i = 0 ; i < size ; i++) {
	data[i] = i * 9.53;
      }

      std::vector<double> data2(data);

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,__LINE__,error_flag,out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      data2[5] += 1.1;

      {
	bool error_flag = false;
	
	std::ostringstream out;
	vector_check(data,data2,dcmp,__LINE__,error_flag,out);
	check_negative(error_flag, out.str(), "exceeded", __LINE__, error);
      }
    }

//-----------------------------------------------------------------------
// Test file_check
//-----------------------------------------------------------------------

    test_number++;
    {
      std::string filename1 = "UnitTest/tmp/file1.txt";
      std::string filename2 = "UnitTest/tmp/file2.txt";
      std::string filename3 = "UnitTest/tmp/file3.txt";
      std::string filename4 = "UnitTest/tmp/file4.txt";
	
      std::ofstream ofs1(filename1.c_str(), std::ios::trunc);
      std::ofstream ofs2(filename2.c_str(), std::ios::trunc);
      std::ofstream ofs3(filename3.c_str(), std::ios::trunc);
      std::ofstream ofs4(filename4.c_str(), std::ios::trunc);
      ofs1 << "Test";
      ofs2 << "Test";
      ofs3 << "Different length";
      ofs4 << "tEst";
      ofs1.close();
      ofs2.close();
      ofs3.close();
      ofs4.close();

      {
	bool error_flag = false;
	std::ostringstream out;
	file_check(filename1, filename2, __LINE__, error_flag, out);
	check_positive(error_flag, out.str(), __LINE__, error);
      }

      {
	bool error_flag = false;
	std::ostringstream out;
	file_check(filename1, filename3, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "Files are different length", __LINE__, error);
      }

      {
	bool error_flag = false;
	std::ostringstream out;
	file_check(filename1, filename4, __LINE__, error_flag, out);
	check_negative(error_flag, out.str(), "Files differ", __LINE__, error);
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
    cerr << "Caught Exception: " << e.what() << "\n"
	 << "test_number = " << test_number <<"\n";
  }
}

