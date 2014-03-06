#include "unit_test_support.h"
#include "vicar_file.h"
#include "vicar_tiled_file.h"
#include "vicar_raster_image.h"
#include "vicar_argument.h"
#include "ibis_file.h"
#include <boost/foreach.hpp>
#include <iostream>

using namespace GeoCal;

// All the VICAR tests that use the PDF file parsing need to be done
// here, because we can only initialize the VICAR interface once.

// Structure to initialize everything before each test. Ensures
// zvzinit which initializes the PDF parsing only gets called once.

struct VicarFixture : public GlobalFixture {
  std::string fname;
  std::string fname2;
  std::string fname3;
  std::string fname4;
  std::string fname5;
  static bool done;
  VicarFixture()
  {
    if(!VicarFile::vicar_available())
      return;
    fname = test_data_dir() + "vicar.img";
    fname2 = test_data_dir() + "rpc.ntf";
    fname3 = "test_out/vicar_write.img";
    fname4 = "test_out/vicar_write2.img";
    fname5 = "test_data/vicar_copy.img";
    
    if(done)
      return;
    std::string t = "INP=" + fname + "," + fname;
    std::string t2 = "OUT=" + fname3;
    std::string str = "STR=test1";
    char* argv[9];
    argv[0] = const_cast<char*>("dummy");
    argv[1] = const_cast<char*>(t.c_str());
    argv[2] = const_cast<char*>(t2.c_str());
    argv[3] = const_cast<char*>("STR=test1");
    argv[4] = const_cast<char*>("STRV=test2,test3,test4");
    argv[5] = const_cast<char*>("RL=1.23456789011");
    argv[6] = const_cast<char*>("RLV=1.1,2.2,3.3");
    argv[7] = const_cast<char*>("IN=10");
    argv[8] = const_cast<char*>("INV=11,12,13");
    VicarFile::zvzinitw(9, argv);
    done = true;
  }
};

bool VicarFixture::done = false;

BOOST_FIXTURE_TEST_SUITE(vicar, VicarFixture)


BOOST_AUTO_TEST_CASE(vicar_argument)
{
  if(!VicarFile::vicar_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use VICAR library, so skipping Vicar tests.");
    return;
  }
  std::string tp;
  int count;
  VicarArgument::type("STR", tp, count);
  BOOST_CHECK_EQUAL(tp, std::string("STRING"));
  BOOST_CHECK_EQUAL(count,1);
  VicarArgument::type("STRV", tp, count);
  BOOST_CHECK_EQUAL(tp, std::string("STRING"));
  BOOST_CHECK_EQUAL(count,3);
  VicarArgument::type("RL", tp, count);
  BOOST_CHECK_EQUAL(tp, std::string("REAL"));
  BOOST_CHECK_EQUAL(count,1);
  VicarArgument::type("RLV", tp, count);
  BOOST_CHECK_EQUAL(tp, std::string("REAL"));
  BOOST_CHECK_EQUAL(count,3);
  VicarArgument::type("IN", tp, count);
  BOOST_CHECK_EQUAL(tp, std::string("INT"));
  BOOST_CHECK_EQUAL(count,1);
  VicarArgument::type("INV", tp, count);
  BOOST_CHECK_EQUAL(tp, std::string("INT"));
  BOOST_CHECK_EQUAL(count,3);
  BOOST_CHECK_EQUAL(VicarArgument::arg<std::string>("STR"), 
		    std::string("test1"));
  BOOST_CHECK_EQUAL(VicarArgument::arg<std::vector<std::string> >("STRV")[0], 
		    std::string("test2"));
  BOOST_CHECK_EQUAL(VicarArgument::arg<std::vector<std::string> >("STRV")[1], 
		    std::string("test3"));
  BOOST_CHECK_EQUAL(VicarArgument::arg<std::vector<std::string> >("STRV")[2], 
		    std::string("test4"));
  BOOST_CHECK_CLOSE(VicarArgument::arg<double>("RL"), 1.23456789011, 1e-12);
  BOOST_CHECK_CLOSE(VicarArgument::arg<std::vector<double> >("RLV")[0], 
		    1.1, 1e-4);
  BOOST_CHECK_CLOSE(VicarArgument::arg<std::vector<double> >("RLV")[1], 
		    2.2, 1e-4);
  BOOST_CHECK_CLOSE(VicarArgument::arg<std::vector<double> >("RLV")[2], 
		    3.3, 1e-4);
  BOOST_CHECK_EQUAL(VicarArgument::arg<int>("IN"), 10);
  BOOST_CHECK_EQUAL(VicarArgument::arg<std::vector<int> >("INV")[0], 11);
  BOOST_CHECK_EQUAL(VicarArgument::arg<std::vector<int> >("INV")[1], 12);
  BOOST_CHECK_EQUAL(VicarArgument::arg<std::vector<int> >("INV")[2], 13);
}

BOOST_AUTO_TEST_CASE(vicar_file)
{
  if(!VicarFile::vicar_available())
    return;
  VicarFile f(fname);
  VicarFile f2(2);
  BOOST_CHECK_EQUAL(f.file_name(), fname);
  BOOST_CHECK_EQUAL(f2.file_name(), "INP Instance: 2");
  BOOST_CHECK(f.unit() != -1);
  BOOST_CHECK(f2.unit() != -1);
  BOOST_CHECK_EQUAL(f.label<int>("NL"), 10);
  BOOST_CHECK_EQUAL(f2.label<int>("NL"), 10);
  std::vector<int> nl_expect;
  nl_expect.push_back(10);
  BOOST_CHECK(f.label<std::vector<int> >("NL") == nl_expect);
  BOOST_CHECK_CLOSE((double) f.label<float>("BINC"), 1.0, 1e-4);
  BOOST_CHECK_CLOSE((double) f.label<std::vector<float> >("BINC").front(), 
		    1.0, 1e-4);
  BOOST_CHECK_EQUAL(f.label<std::string>("USER"), "pkim");
  BOOST_CHECK_EQUAL(f.label<std::vector<std::string> >("USER").front(), 
		    "pkim");
  BOOST_CHECK(f2.label<std::vector<int> >("NL") == nl_expect);
  BOOST_CHECK_CLOSE((double) f2.label<float>("BINC"), 1.0, 1e-4);
  BOOST_CHECK_CLOSE((double) f2.label<std::vector<float> >("BINC").front(), 
		    1.0, 1e-4);
  BOOST_CHECK_EQUAL(f2.label<std::string>("USER"), "pkim");
  BOOST_CHECK_EQUAL(f2.label<std::vector<std::string> >("USER").front(), 
		    "pkim");
  BOOST_CHECK(VicarFile::is_vicar_file(fname));
  BOOST_CHECK(!VicarFile::is_vicar_file(fname2));

  {				// This will close the file by calling
				// the destructor
    VicarFile f3(1, 10, 20);
    VicarFile f4(fname4, 4, 5, "HALF");
  }
  VicarFile f3(fname3);
  VicarFile f4(fname4);
  BOOST_CHECK_EQUAL(f3.label<int>("NL"), 10);
  BOOST_CHECK_EQUAL(f3.label<int>("NS"), 20);
  BOOST_CHECK_EQUAL(f3.label<std::string>("FORMAT"), "BYTE");
  BOOST_CHECK_EQUAL(f4.label<int>("NL"), 4);
  BOOST_CHECK_EQUAL(f4.label<int>("NS"), 5);
  BOOST_CHECK_EQUAL(f4.label<std::string>("FORMAT"), "HALF");
}

BOOST_AUTO_TEST_CASE(ibis_file_create)
{
  if(!VicarFile::vicar_available())
    return;
  std::vector<std::string> fmt(5);
  fmt[0] = "BYTE";
  fmt[1] = "HALF";
  fmt[2] = "FULL";
  fmt[3] = "REAL";
  fmt[4] = "DOUB";
  IbisFile f("test_out/ibis.img", 10, fmt);
  BOOST_CHECK_EQUAL(f.column_data_type(0), IbisFile::VICAR_BYTE);
  BOOST_CHECK_EQUAL(f.column_data_type(1), IbisFile::VICAR_HALF);
  BOOST_CHECK_EQUAL(f.column_data_type(2), IbisFile::VICAR_FULL);
  BOOST_CHECK_EQUAL(f.column_data_type(3), IbisFile::VICAR_FLOAT);
  BOOST_CHECK_EQUAL(f.column_data_type(4), IbisFile::VICAR_DOUBLE);
  f.column<VicarByte>(0).data[0] = '\1';
  f.column<VicarHalf>(1).data[2] = (VicarHalf) 2;
  f.column<VicarFull>(2).data[4] = (VicarFull) 3;
  f.column<VicarFloat>(3).data[6] = (VicarFloat) 4.0;
  f.column<VicarDouble>(4).data[8] = 5.0;
  f.flush();
  IbisFile f2("test_out/ibis.img");
  BOOST_CHECK_EQUAL(f2.number_row(), 10);
  BOOST_CHECK_EQUAL(f2.number_col(), 5);
  BOOST_CHECK_EQUAL(f2.column_data_type(0), IbisFile::VICAR_BYTE);
  BOOST_CHECK_EQUAL(f2.column_data_type(1), IbisFile::VICAR_HALF);
  BOOST_CHECK_EQUAL(f2.column_data_type(2), IbisFile::VICAR_FULL);
  BOOST_CHECK_EQUAL(f2.column_data_type(3), IbisFile::VICAR_FLOAT);
  BOOST_CHECK_EQUAL(f2.column_data_type(4), IbisFile::VICAR_DOUBLE);
  BOOST_CHECK_EQUAL(f2.column<VicarByte>(0).data[0], '\1');
  BOOST_CHECK_EQUAL(f2.column<VicarHalf>(1).data[2], (VicarHalf) 2);
  BOOST_CHECK_EQUAL(f2.column<VicarFull>(2).data[4], (VicarFull) 3);
  BOOST_CHECK_CLOSE(f.column<VicarFloat>(3).data[6], (VicarFloat) 4.0,
		    1e-4);
  BOOST_CHECK_CLOSE(f.column<VicarDouble>(4).data[8], 5.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

