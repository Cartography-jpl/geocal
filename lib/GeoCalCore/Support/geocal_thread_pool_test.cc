#include "unit_test_support.h"
#include "geocal_thread_pool.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_thread_pool, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  auto tpool = ThreadPool::thread_pool();
  BOOST_CHECK_EQUAL(tpool.number_thread(),
		    ThreadPool::default_number_thread);
  tpool.number_thread(20);
  BOOST_CHECK_EQUAL(tpool.number_thread(), 20);
  tpool.number_thread(ThreadPool::default_number_thread);

  double res[3];
  auto add_func = [&res](int ind, auto &i1, auto &i2) {res[ind] = i1+i2; };
  tpool.add_task(add_func, 0, 1, 2);
  tpool.add_task(add_func, 1, 3.0, 4.0);
  tpool.add_task(add_func, 2, 5, 6.0);
  tpool.wait_tasks_finish();
  BOOST_CHECK_EQUAL(res[0], 1+2);
  BOOST_CHECK_EQUAL(res[1], 3.0+4.0);
  BOOST_CHECK_EQUAL(res[2], 5+6.0);
}

BOOST_AUTO_TEST_SUITE_END()


