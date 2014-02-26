#include "unit_test_support.h"
#include "tiled_file.h"
#include "unit_test_support.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(tiled_file, GlobalFixture)

class TestClass : public TiledFile<int, 3> {
public:
  typedef TiledFile<int, 3>::index index;
  typedef boost::multi_array<int, 3> atype;
  typedef atype::index_range range;

  TestClass(const boost::array<index, 3>& File_size, 
	    const boost::array<index, 3>& Tile_size, 
	    unsigned int Number_tile = 4) 
    : TiledFile<int, 3>(File_size, Tile_size, Number_tile), 
      data(File_size) {}
  virtual ~TestClass() {}
  virtual void read_tile(const boost::array<index, 3>& Min_index, 
			 const boost::array<index, 3>& Max_index, 
			 int* Res) const
  {
    for(index i1 = Min_index[0]; i1 < Max_index[0]; ++i1)
      for(index i2 = Min_index[1]; i2 < Max_index[1]; ++i2)
	for(index i3 = Min_index[2]; i3 < Max_index[2]; ++i3, ++Res)
	  *Res = data[i1][i2][i3];
  }
  virtual void write_tile(const boost::array<index, 3>& Min_index, 
			  const boost::array<index, 3>& Max_index, 
			  const int* V) const
  {
    for(index i1 = Min_index[0]; i1 < Max_index[0]; ++i1)
      for(index i2 = Min_index[1]; i2 < Max_index[1]; ++i2)
	for(index i3 = Min_index[2]; i3 < Max_index[2]; ++i3, ++V)
	  data[i1][i2][i3] = *V;
  }
  mutable atype data;
};

typedef boost::array<boost::multi_array_types::index, 3> atype;
BOOST_AUTO_TEST_CASE(basic_test)
{
  atype fsize = {{10, 20, 30}};
  atype tsize = {{5, 6, 7}};
  TestClass tc(fsize, tsize);
  BOOST_CHECK(tc.number_swap() ==0);
  BOOST_CHECK(tc.size() == fsize);
  BOOST_CHECK(tc.tile_size() == tsize);
  tc.data[1][2][3] = 0;
  tc(1, 2, 3) = 1;
  BOOST_CHECK(tc.number_swap() ==1);
  BOOST_CHECK(tc(1, 2, 3) ==1);
  BOOST_CHECK(tc.data[1][2][3] ==0);
  tc.flush();
  BOOST_CHECK(tc.number_swap() ==1);
  BOOST_CHECK(tc(1, 2, 3) ==1);
  BOOST_CHECK(tc.data[1][2][3] ==1);
  int val = 0;
  for(TestClass::index i1 = 0; i1 < tc.size()[0]; ++i1)
    for(TestClass::index i2 = 0; i2 < tc.size()[1]; ++i2)
      for(TestClass::index i3 = 0; i3 < tc.size()[2]; ++i3, ++val)
	tc(i1, i2, i3) = val;
  val = 0;
  tc.flush();
  for(TestClass::index i1 = 0; i1 < tc.size()[0]; ++i1)
    for(TestClass::index i2 = 0; i2 < tc.size()[1]; ++i2)
      for(TestClass::index i3 = 0; i3 < tc.size()[2]; ++i3, ++val) {
	BOOST_CHECK(tc.data[i1][i2][i3] ==val);
	BOOST_CHECK(tc(i1, i2, i3) == val);
      }
  tc(1, 2, 3) = 1;
  ++tc(1, 2, 3);
  BOOST_CHECK(tc(1, 2, 3) == 2);
  --tc(1, 2, 3);
  BOOST_CHECK(tc(1, 2, 3) == 1);
  tc(1, 2, 3) += 10;
  BOOST_CHECK(tc(1, 2, 3) == 11);
  tc(1, 2, 3) -= 10;
  BOOST_CHECK(tc(1, 2, 3) == 1);
  tc(1, 2, 3) *= 10;
  BOOST_CHECK(tc(1, 2, 3) == 10);
  tc(1, 2, 3) /= 10;
  BOOST_CHECK(tc(1, 2, 3) == 1);

  atype mini= {{2, 3, 4}};
  atype maxi= {{5, 10, 23}};
  boost::multi_array<int, 3> r = tc.read(mini, maxi);
  BOOST_CHECK(r.shape()[0] == 3);
  BOOST_CHECK(r.shape()[1] == 7);
  BOOST_CHECK(r.shape()[2] == 19);
  for(int i1 = 2; i1 < 5; ++i1)
    for(int i2 = 3; i2 < 10; ++i2)
      for(int i3 = 4; i3 < 23; ++i3)
	BOOST_CHECK_EQUAL(r[i1 - 2][i2 - 3][i3 - 4], tc(i1, i2, i3));
}

BOOST_AUTO_TEST_SUITE_END()
