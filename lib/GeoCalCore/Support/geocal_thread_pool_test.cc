#include "unit_test_support.h"
#include "geocal_thread_pool.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_thread_pool, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  ThreadPool& tpool = ThreadPool::thread_pool();
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

BOOST_AUTO_TEST_CASE(loop_args_test)
{
  // Pass the arguments from loop variables, the way
  // misc_array_function does. Do this twice, to check that the
  // ThreadPool can be used again after wait_tasks_finish.
  ThreadPool& tpool = ThreadPool::thread_pool();
  for(int pass = 0; pass < 2; ++pass) {
    std::vector<int> res(100, -1);
    auto set_func = [&res](int istart, int iend)
      { for(int i = istart; i < iend; ++i) res[i] = i; };
    for(int istart = 0; istart < 100; istart += 10) {
      int iend = istart + 10;
      tpool.add_task(set_func, istart, iend);
    }
    tpool.wait_tasks_finish();
    for(int i = 0; i < 100; ++i)
      BOOST_CHECK_EQUAL(res[i], i);
  }
}

BOOST_AUTO_TEST_SUITE_END()


