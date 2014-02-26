#include "unit_test_support.h"
#include "tile.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(tile, GlobalFixture)

typedef boost::array<boost::multi_array_types::index, 3> atype;
BOOST_AUTO_TEST_CASE(tile_constructor)
{
  Tile<int, 3> t;
  atype i = {{0, 0, 0}};
  BOOST_CHECK(!t.changed);
  BOOST_CHECK(!t.in_tile(i));
}

BOOST_AUTO_TEST_CASE(tile_use)
{
  Tile<int, 3> t;
  atype mini = {{10, 20, 30}};
  atype maxi = {{30, 40, 80}};
  t.swap(mini, maxi);
  atype i = {{15, 25, 35}};
  BOOST_CHECK(!t.changed);
  BOOST_CHECK(t.in_tile(i));
  t.data(i) = 5;
  BOOST_CHECK(t.data(i) ==5);
  t.changed = true;
  t.swap(mini, maxi);
  BOOST_CHECK(!t.changed);
  BOOST_CHECK(t.min_index() ==mini);
  BOOST_CHECK(t.max_index() ==maxi);
}

BOOST_AUTO_TEST_SUITE_END()
