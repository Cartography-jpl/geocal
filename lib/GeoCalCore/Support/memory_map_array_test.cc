#include "unit_test_support.h"
#include "memory_map_array.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(memory_map_array, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::string fname = "test_out/memory_map_array.bin";
  MemoryMapArray<int, 2> m(fname, boost::extents[5][4], 
			   MemoryMapArray<int, 2>::CREATE);
  BOOST_CHECK(m.data().shape()[0] ==5);
  BOOST_CHECK(m.data().shape()[1] ==4);
  int val = 0;
  for(boost::multi_array_types::index i = 0; i < 5; ++i)
    for(boost::multi_array_types::index j = 0; j < 4; 
	++j, ++val)
      m.data()[i][j] = val;
  m.flush();
  MemoryMapArray<int, 2> m2(fname, boost::extents[5][4]);
  val = 0;
  for(boost::multi_array_types::index i = 0; i < 5; ++i)
    for(boost::multi_array_types::index j = 0; j < 4; 
	++j, ++val)
      BOOST_CHECK(m2.data()[i][j] ==val);
}

BOOST_AUTO_TEST_SUITE_END()
