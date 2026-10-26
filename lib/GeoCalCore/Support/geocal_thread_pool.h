#ifndef GEOCAL_THREAD_POOL_H
#define GEOCAL_THREAD_POOL_H
#include "printable.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/shared_ptr.hpp>
#include <mutex>
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
    // The task runs after add_task returns, so we need to copy the
    // arguments into the task rather than refer to them.
    boost::asio::post(pool(), [&ctx, targs = std::make_tuple(args...)]() mutable
		      { std::apply(ctx, targs); });
  }

//-----------------------------------------------------------------------
/// Wait for all the tasks to finish.
//-----------------------------------------------------------------------

  void wait_tasks_finish();
private:
  ThreadPool(int Number_thread = default_number_thread);
  boost::asio::thread_pool& pool();
  static ThreadPool the_thread_pool;
  // Created when first needed, so we don't start threads unless we
  // use them (e.g., this matters if python later forks the process).
  // A boost::asio::thread_pool can't be reused after join, so
  // wait_tasks_finish releases it and we create a new one as needed.
  boost::shared_ptr<boost::asio::thread_pool> tpool;
  std::mutex tpool_mutex;
  int number_thread_;
};
}
#endif
