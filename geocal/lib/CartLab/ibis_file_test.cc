#include "unit_test_support.h"
#include "ibis_file.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ibis, GlobalFixture)

BOOST_AUTO_TEST_CASE(ibis_file_create)
{
  if(!VicarFile::vicar_available())
    return;
  std::vector<std::string> fmt(6);
  fmt[0] = "BYTE";
  fmt[1] = "HALF";
  fmt[2] = "FULL";
  fmt[3] = "REAL";
  fmt[4] = "DOUB";
  fmt[5] = "A10";
  IbisFile f("test_out/ibis.img", 10, fmt);
  BOOST_CHECK_EQUAL(f.column_data_type(0), IbisFile::VICAR_BYTE);
  BOOST_CHECK_EQUAL(f.column_data_type(1), IbisFile::VICAR_HALF);
  BOOST_CHECK_EQUAL(f.column_data_type(2), IbisFile::VICAR_FULL);
  BOOST_CHECK_EQUAL(f.column_data_type(3), IbisFile::VICAR_FLOAT);
  BOOST_CHECK_EQUAL(f.column_data_type(4), IbisFile::VICAR_DOUBLE);
  BOOST_CHECK_EQUAL(f.column_data_type(5), IbisFile::VICAR_ASCII);
  f.column<VicarByte>(0).data[0] = '\1';
  f.column<VicarHalf>(1).data[2] = (VicarHalf) 2;
  f.column<VicarFull>(2).data[4] = (VicarFull) 3;
  f.column<VicarFloat>(3).data[6] = (VicarFloat) 4.0;
  f.column<VicarDouble>(4).data[8] = 5.0;
  f.column<std::string>(5).data[0] = "aaa";
  f.column<std::string>(5).data[1] = "abcdefghijxxx"; // Test too long string.
  f.close();
  IbisFile f2("test_out/ibis.img");
  BOOST_CHECK_EQUAL(f2.number_row(), 10);
  BOOST_CHECK_EQUAL(f2.number_col(), 6);
  BOOST_CHECK_EQUAL(f2.column<std::string>(5).size_byte(), 11);
  BOOST_CHECK_EQUAL(f2.column_data_type(0), IbisFile::VICAR_BYTE);
  BOOST_CHECK_EQUAL(f2.column_data_type(1), IbisFile::VICAR_HALF);
  BOOST_CHECK_EQUAL(f2.column_data_type(2), IbisFile::VICAR_FULL);
  BOOST_CHECK_EQUAL(f2.column_data_type(3), IbisFile::VICAR_FLOAT);
  BOOST_CHECK_EQUAL(f2.column_data_type(4), IbisFile::VICAR_DOUBLE);
  BOOST_CHECK_EQUAL(f2.column_data_type(5), IbisFile::VICAR_ASCII);
  BOOST_CHECK_EQUAL(f2.column<VicarByte>(0).data[0], '\1');
  BOOST_CHECK_EQUAL(f2.column<VicarHalf>(1).data[2], (VicarHalf) 2);
  BOOST_CHECK_EQUAL(f2.column<VicarFull>(2).data[4], (VicarFull) 3);
  BOOST_CHECK_CLOSE(f.column<VicarFloat>(3).data[6], (VicarFloat) 4.0,
		    1e-4);
  BOOST_CHECK_CLOSE(f.column<VicarDouble>(4).data[8], 5.0, 1e-4);
  BOOST_CHECK_EQUAL(f2.column<std::string>(5).data[0], "aaa");
  BOOST_CHECK_EQUAL(f2.column<std::string>(5).data[1], "abcdefghij");
}

BOOST_AUTO_TEST_SUITE_END()
