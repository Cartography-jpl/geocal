#include "unit_test_support.h"
#include "geocal_thread_pool.h"
#include "geocal_exception.h"
#include <atomic>
#include <thread>
#include <vector>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_thread_pool, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  ThreadPool& tpool = ThreadPool::thread_pool();
  BOOST_CHECK_EQUAL(tpool.number_thread(), ThreadPool::default_number_thread);
  tpool.number_thread(20);
  BOOST_CHECK_EQUAL(tpool.number_thread(), 20);
  tpool.number_thread(ThreadPool::default_number_thread);

  double res[3];
  auto add_func = [&res](int ind, auto& i1, auto& i2) { res[ind] = i1 + i2; };
  tpool.add_task(add_func, 0, 1, 2);
  tpool.add_task(add_func, 1, 3.0, 4.0);
  tpool.add_task(add_func, 2, 5, 6.0);
  tpool.wait_tasks_finish();
  BOOST_CHECK_EQUAL(res[0], 1 + 2);
  BOOST_CHECK_EQUAL(res[1], 3.0 + 4.0);
  BOOST_CHECK_EQUAL(res[2], 5 + 6.0);
}

BOOST_AUTO_TEST_CASE(loop_args_test)
{
  // Pass the arguments from loop variables, the way
  // misc_array_function does. Do this twice, to check that the
  // ThreadPool can be used again after wait_tasks_finish.
  ThreadPool& tpool = ThreadPool::thread_pool();
  for(int pass = 0; pass < 2; ++pass) {
    std::vector<int> res(100, -1);
    auto set_func = [&res](int istart, int iend) {
      for(int i = istart; i < iend; ++i)
        res[i] = i;
    };
    for(int istart = 0; istart < 100; istart += 10) {
      int iend = istart + 10;
      tpool.add_task(set_func, istart, iend);
    }
    tpool.wait_tasks_finish();
    for(int i = 0; i < 100; ++i)
      BOOST_CHECK_EQUAL(res[i], i);
  }
}

BOOST_AUTO_TEST_CASE(task_exception)
{
  // An exception thrown by a task is rethrown by wait_tasks_finish,
  // and the pool can be used normally afterwards.
  ThreadPool& tpool = ThreadPool::thread_pool();
  auto throw_func = [](int i) {
    if(i == 3)
      throw Exception("Task failed");
  };
  for(int i = 0; i < 10; ++i)
    tpool.add_task(throw_func, i);
  try {
    tpool.wait_tasks_finish();
    BOOST_FAIL("wait_tasks_finish should have thrown an exception");
  } catch(const Exception& e) {
    BOOST_CHECK(std::string(e.what()).find("Task failed") != std::string::npos);
  }
  std::vector<int> res(10, -1);
  auto set_func = [&res](int i) { res[i] = i; };
  for(int i = 0; i < 10; ++i)
    tpool.add_task(set_func, i);
  tpool.wait_tasks_finish();
  for(int i = 0; i < 10; ++i)
    BOOST_CHECK_EQUAL(res[i], i);
}

BOOST_AUTO_TEST_CASE(nested_use)
{
  // Using the pool from inside a task isn't supported. We should get
  // an exception, rather than hanging or crashing.
  ThreadPool& tpool = ThreadPool::thread_pool();
  auto inner_func = [](int i) {};
  auto outer_func = [&tpool, &inner_func](int i) {
    tpool.add_task(inner_func, i);
    tpool.wait_tasks_finish();
  };
  for(int i = 0; i < 4; ++i)
    tpool.add_task(outer_func, i);
  try {
    tpool.wait_tasks_finish();
    BOOST_FAIL("wait_tasks_finish should have thrown an exception");
  } catch(const Exception& e) {
    BOOST_CHECK(std::string(e.what()).find("inside a ThreadPool task") !=
                std::string::npos);
  }
  // Same thing through parallel_row_blocks
  auto outer_block = [](int, int, int) {
    parallel_row_blocks(10, 2, [](int, int, int) {});
  };
  BOOST_CHECK_THROW(parallel_row_blocks(10, 4, outer_block), Exception);
}

BOOST_AUTO_TEST_CASE(concurrent_use)
{
  // Using the pool from a second thread while the first thread has
  // tasks that it hasn't waited for isn't supported, and should throw.
  ThreadPool& tpool = ThreadPool::thread_pool();
  std::atomic<bool> go(false);
  auto wait_func = [&go](int i) {
    while(!go)
      std::this_thread::yield();
  };
  tpool.add_task(wait_func, 0);
  bool other_thread_threw = false;
  std::thread t2([&tpool, &other_thread_threw]() {
    auto f = [](int i) {};
    try {
      tpool.add_task(f, 1);
      tpool.wait_tasks_finish();
    } catch(const Exception& e) {
      other_thread_threw =
          (std::string(e.what()).find("another thread") != std::string::npos);
    }
  });
  t2.join();
  // Changing the number of threads with tasks pending also isn't
  // allowed.
  BOOST_CHECK_THROW(tpool.number_thread(5), Exception);
  go = true;
  tpool.wait_tasks_finish();
  BOOST_CHECK(other_thread_threw);
  BOOST_CHECK_EQUAL(tpool.number_thread(), ThreadPool::default_number_thread);
  // Once we have waited, another thread can use the pool.
  bool other_thread_ok = false;
  std::thread t3([&tpool, &other_thread_ok]() {
    std::vector<int> res(10, -1);
    parallel_row_blocks(10, 5, [&res](int, int istart, int iend) {
      for(int i = istart; i < iend; ++i)
        res[i] = i;
    });
    other_thread_ok = (res[9] == 9);
  });
  t3.join();
  BOOST_CHECK(other_thread_ok);
}

BOOST_AUTO_TEST_CASE(parallel_row_blocks_test)
{
  // Check that we cover all the rows exactly once, for various
  // numbers of tasks (including more tasks than rows), and the
  // single threaded case.
  ThreadPool& tpool = ThreadPool::thread_pool();
  for(int nthread : {1, ThreadPool::default_number_thread}) {
    tpool.number_thread(nthread);
    for(int ntask : {1, 3, 7, 100, 1000}) {
      std::vector<int> cnt(100, 0);
      std::vector<int> task_seen(std::min(ntask, 100), 0);
      parallel_row_blocks(100, ntask,
                          [&cnt, &task_seen](int k, int istart, int iend) {
                            task_seen[k] += 1;
                            for(int i = istart; i < iend; ++i)
                              cnt[i] += 1;
                          });
      for(int i = 0; i < 100; ++i)
        BOOST_CHECK_EQUAL(cnt[i], 1);
      // Each task index is used once, unless we did this serially
      // in which case we have one call for everything.
      if(nthread == 1 || ntask == 1)
        BOOST_CHECK_EQUAL(task_seen[0], 1);
      else
        for(int v : task_seen)
          BOOST_CHECK_EQUAL(v, 1);
    }
  }
  tpool.number_thread(ThreadPool::default_number_thread);
  // Zero rows does nothing
  bool called = false;
  parallel_row_blocks(0, 10, [&called](int, int, int) { called = true; });
  BOOST_CHECK(!called);
}

BOOST_AUTO_TEST_CASE(helper_cleanup)
{
  // An exception from a task in parallel_row_blocks is passed on,
  // and leaves the pool usable, including from another thread.
  auto throw_func = [](int k, int, int) {
    if(k == 2)
      throw Exception("Block failed");
  };
  BOOST_CHECK_THROW(parallel_row_blocks(100, 10, throw_func), Exception);
  bool other_thread_ok = false;
  std::thread t2([&other_thread_ok]() {
    std::vector<int> res(100, -1);
    parallel_row_blocks(100, 10, [&res](int, int istart, int iend) {
      for(int i = istart; i < iend; ++i)
        res[i] = i;
    });
    other_thread_ok = (res[99] == 99);
  });
  t2.join();
  BOOST_CHECK(other_thread_ok);
}

BOOST_AUTO_TEST_SUITE_END()
