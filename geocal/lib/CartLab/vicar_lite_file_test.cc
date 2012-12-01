#include "unit_test_support.h"
#include "vicar_lite_file.h"
#include <boost/foreach.hpp>

using namespace GeoCal;
// Initialize before each test
struct VicarLiteFixture : public GlobalFixture {
  std::string fname;
  std::string fname2;
  VicarLiteFixture() {
    fname = test_data_dir() + "vicar.img";
    fname2 = test_data_dir() + "rpc.ntf";
  }
};
BOOST_FIXTURE_TEST_SUITE(vicar_lite, VicarLiteFixture)

// We derive from VicarLiteFile because there are some protected
// members we want to test.
class VicarLiteFileTest : public VicarLiteFile {
public:
  VicarLiteFileTest(const std::string& Fname, access_type Access = READ)
    : VicarLiteFile(Fname, Access) {}
void do_test()
{
  char c = 0x01;
  short int si = 0x0102;
  int i = 0x01020304;
  int c2 = to_int<unsigned char, false>(&c);
  int si2 = to_int<short int, false>((char*) &si);
  int i2 = to_int<int, false>((char*) &i);
  BOOST_CHECK_EQUAL(c2, (int) c);
  BOOST_CHECK_EQUAL(si2, (int) si);
  BOOST_CHECK_EQUAL(i2, (int) i);
  c2 = (int) to_double<unsigned char, false>(&c);
  si2 = (int) to_double<short int, false>((char*) &si);
  i2 = (int) to_double<int, false>((char*) &i);
  BOOST_CHECK_EQUAL(c2, (int) c);
  BOOST_CHECK_EQUAL(si2, (int) si);
  BOOST_CHECK_EQUAL(i2, (int) i);
  char cb;
  short int sib;
  int ib;
  from_int<unsigned char, false>(c2, reinterpret_cast<char*>(&cb));
  from_int<short int, false>(si2, reinterpret_cast<char*>(&sib));
  from_int<int, false>(i2, reinterpret_cast<char*>(&ib));
  BOOST_CHECK_EQUAL(cb, c);
  BOOST_CHECK_EQUAL(sib, si);
  BOOST_CHECK_EQUAL(ib, i);

  c2 = to_int<unsigned char, true>(&c);
  si2 = to_int<short int, true>((char*) &si);
  i2 = to_int<int, true>((char*) &i);
  BOOST_CHECK_EQUAL(c2, (int) 0x01);
  BOOST_CHECK_EQUAL(si2, (int) 0x0201);
  BOOST_CHECK_EQUAL(i2, (int) 0x04030201);
  c2 = (int) to_double<unsigned char, true>(&c);
  si2 = (int) to_double<short int, true>((char*) &si);
  i2 = (int) to_double<int, true>((char*) &i);
  BOOST_CHECK_EQUAL(c2, (int) 0x01);
  BOOST_CHECK_EQUAL(si2, (int) 0x0201);
  BOOST_CHECK_EQUAL(i2, (int) 0x04030201);
  from_int<unsigned char, true>(c2, reinterpret_cast<char*>(&cb));
  from_int<short int, true>(si2, reinterpret_cast<char*>(&sib));
  from_int<int, true>(i2, reinterpret_cast<char*>(&ib));
  BOOST_CHECK_EQUAL(cb, c);
  BOOST_CHECK_EQUAL(sib, si);
  BOOST_CHECK_EQUAL(ib, i);
}
};

BOOST_AUTO_TEST_CASE(vicar_lite_file)
{
  VicarLiteFileTest f(fname);
  typedef std::pair<std::string, std::string> mt;
  BOOST_CHECK(VicarLiteFile::is_vicar_file(fname));
  BOOST_CHECK(!VicarLiteFile::is_vicar_file(fname2));
  BOOST_CHECK_EQUAL(f.number_band(), 1);
  BOOST_CHECK_EQUAL(f.number_line(), 10);
  BOOST_CHECK_EQUAL(f.number_sample(), 10);
  BOOST_CHECK_CLOSE(f.label<double>("SINC"), 1.0, 1e-4);
  BOOST_CHECK_EQUAL(f.label<int>("RECSIZE"), 10);
  BOOST_CHECK_EQUAL(f.label<std::string>("BINTFMT"), "LOW");
  BOOST_CHECK_EQUAL(f.map_info().number_x_pixel(), 10);
  BOOST_CHECK_EQUAL(f.map_info().number_y_pixel(), 10);
  BOOST_CHECK_CLOSE(f.map_info().ulc_x(), 44.799913294865902, 1e-4);
  BOOST_CHECK_CLOSE(f.map_info().ulc_y(), 35.900027777778142, 1e-4);
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j)
      BOOST_CHECK_EQUAL(f.read_int(0, i, j), i + j);
  boost::multi_array<int, 2> r(boost::extents[10][10]);
  f.read_int(0, 0, 0, 1, 10, 10, &r[0][0]);
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j)
      BOOST_CHECK_EQUAL(r[i][j], i + j);
  Rpc rpc = f.rpc();
  ImageCoordinate ic_expect(5729.22, 27561.36);
  Geodetic g(35.8399968, 45.0770183, 1017);
  BOOST_CHECK_EQUAL(rpc.image_coordinate(g), ic_expect);
  f.do_test();			// Test protected members.
}


BOOST_AUTO_TEST_CASE(vicar_lite_raster_image)
{
  VicarLiteRasterImage f(fname);

  BOOST_CHECK_EQUAL(f.number_line(), 10);
  BOOST_CHECK_EQUAL(f.number_sample(), 10);
  BOOST_CHECK_EQUAL(f.number_tile_line(), 10);
  BOOST_CHECK_EQUAL(f.number_tile_sample(), 10);
  BOOST_CHECK(f.has_map_info());
  BOOST_CHECK_EQUAL(f.map_info().number_x_pixel(), 10);
  BOOST_CHECK_EQUAL(f.map_info().number_y_pixel(), 10);
  BOOST_CHECK_CLOSE(f.map_info().ulc_x(), 44.799913294865902, 1e-4);
  BOOST_CHECK_CLOSE(f.map_info().ulc_y(), 35.900027777778142, 1e-4);
  
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j)
      BOOST_CHECK_EQUAL(f(i, j), (i + j));

  BOOST_CHECK(f.has_rpc());
  Rpc rpc = f.rpc();
  ImageCoordinate ic_expect(5729.22, 27561.36);
  Geodetic g(35.8399968, 45.0770183, 1017);
  BOOST_CHECK_EQUAL(rpc.image_coordinate(g), ic_expect);
}

BOOST_AUTO_TEST_CASE(vicar_lite_dem)
{
  VicarLiteDem d(fname);
  Geodetic g1(35.9, 44.800, 100);
  BOOST_CHECK_CLOSE(d.height_reference_surface(g1), 1.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
