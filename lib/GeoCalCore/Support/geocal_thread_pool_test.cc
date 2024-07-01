#include "unit_test_support.h"
#include "geocal_thread_pool.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_thread_pool, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  BOOST_CHECK_EQUAL(ThreadPool::thread_pool().number_thread(),
		    ThreadPool::default_number_thread);
  ThreadPool::thread_pool().number_thread(20);
  BOOST_CHECK_EQUAL(ThreadPool::thread_pool().number_thread(), 20);
  ThreadPool::thread_pool().number_thread(ThreadPool::default_number_thread);
}

BOOST_AUTO_TEST_SUITE_END()


