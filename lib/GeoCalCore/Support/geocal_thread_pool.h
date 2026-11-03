#ifndef GEOCAL_THREAD_POOL_H
#define GEOCAL_THREAD_POOL_H
#include "printable.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <mutex>
#include <thread>
#include <tuple>

namespace GeoCal {
/****************************************************************//**
  The newer C++ 17 has versions of std library functions that
  automatically handle threading. However there isn't support of this
  until a much newer version of gcc then we want to depend on (9 or
  so), and also depends on third party library (Intel TBB).

  For now, we use the boost threadpool class and handle the threading
  ourselves. We can possibly remove this class at some point as the
  compiler move on.

  There is one global pool, and wait_tasks_finish waits for all the
  tasks in it. So the pool can only be used by one thread at a time,
  and a task can't use the pool itself (no nested use). We don't
  support either of these, but we do detect them and throw an
  Exception rather than hanging or crashing. An exception thrown by
  a task is caught, and rethrown by wait_tasks_finish.

  Most code should use parallel_row_blocks rather than calling
  add_task and wait_tasks_finish directly. This makes sure we always
  wait for the tasks, even if an exception is thrown.
*******************************************************************/


class ThreadPool : public Printable<ThreadPool> {
public:
  virtual ~ThreadPool() {}
  inline static const int default_number_thread = 10;
  static ThreadPool& thread_pool();

//-----------------------------------------------------------------------
/// Return the number of workers in the thread pool.
//-----------------------------------------------------------------------

  int number_thread() const { return number_thread_; }
  void number_thread(int V);
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Add a task to the pools of jobs for the workers to work through.
/// Note you can add as many as you like, there is no requirement that
/// you keep to the number_thread() tasks - more or less is
/// fine. number_thread() is the maximum number of tasks that will be
/// processed at one time, but the list of tasks to be done can be
/// larger than that.
//-----------------------------------------------------------------------

  template<typename ExecutionContext, class... Types> void add_task(ExecutionContext & ctx, Types... args)
  {
    check_caller();
    // The task runs after add_task returns, so we need to copy the
    // arguments into the task rather than refer to them.
    boost::asio::post(pool(), [this, &ctx, targs = std::make_tuple(args...)]() mutable
		      {
			in_task = true;
			try {
			  std::apply(ctx, targs);
			} catch(...) {
			  save_exception(std::current_exception());
			}
			in_task = false;
		      });
  }

  void wait_tasks_finish();
private:
  ThreadPool(int Number_thread = default_number_thread);
  boost::asio::thread_pool& pool();
  void check_caller();
  void save_exception(const std::exception_ptr& E);
  static ThreadPool the_thread_pool;
  // True in the worker threads while they are running a task, used
  // to detect nested use of the pool.
  static thread_local bool in_task;
  // Created when first needed, so we don't start threads unless we
  // use them (e.g., this matters if python later forks the process).
  // A boost::asio::thread_pool can't be reused after join, so
  // wait_tasks_finish releases it and we create a new one as needed.
  boost::shared_ptr<boost::asio::thread_pool> tpool;
  // First exception thrown by a task, rethrown by wait_tasks_finish.
  std::exception_ptr task_exception;
  // Thread that added the tasks we are waiting for, or the default id
  // if we aren't being used.
  std::thread::id owner;
  std::mutex tpool_mutex;
  int number_thread_;
};

//-----------------------------------------------------------------------
/// Divide the rows [0, Number_row) into Number_task nearly equal
/// blocks, and call F(Task_index, Row_start, Row_end) for each block
/// using the ThreadPool. We wait for all the tasks to finish before
/// returning, and rethrow the first exception a task threw.
///
/// If Number_task is 1 or the ThreadPool has only 1 thread, we just
/// call F directly in this thread. Number_task is limited to
/// Number_row, so all the blocks have at least one row. If
/// Number_row is 0 we don't call F at all.
///
/// Note that the number of tasks selected is just how we
/// divide up the work, the actual number of threads used is
/// controlled by ThreadPool (e.g., you could have 10 threads working
/// through 100 tasks).
//-----------------------------------------------------------------------

template<class F> void parallel_row_blocks(int Number_row, int Number_task, F f)
{
  if(Number_row <= 0)
    return;
  ThreadPool& tpool = ThreadPool::thread_pool();
  int ntask = std::max(std::min(Number_task, Number_row), 1);
  if(ntask == 1 || tpool.number_thread() == 1) {
    f(0, 0, Number_row);
    return;
  }
  // Make sure we wait for the tasks we have already added, even if
  // add_task throws partway through (e.g., we are being called from
  // inside a task). Otherwise the tasks could run after f goes out of
  // scope, and the pool would stay owned by this thread.
  struct WaitGuard {
    ThreadPool& tpool;
    bool active = true;
    ~WaitGuard()
    {
      if(active) {
	try {
	  tpool.wait_tasks_finish();
	} catch(...) {
	  // Already have an exception propagating, so we can't throw
	  // a second one.
	}
      }
    }
  } guard{tpool};
  int tsize = Number_row / ntask;
  int overflow_num = Number_row % ntask;
  int istart = 0;
  for(int i = 0; i < ntask; ++i) {
    int iend = istart + tsize;
    // Add a overflow of 1 if needed.
    if(overflow_num > 0) {
      ++iend;
      --overflow_num;
    }
    tpool.add_task(f, i, istart, iend);
    istart = iend;
  }
  guard.active = false;
  tpool.wait_tasks_finish();
}
}
#endif
