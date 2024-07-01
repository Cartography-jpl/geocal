#ifndef GEOCAL_THREAD_POOL_H
#define GEOCAL_THREAD_POOL_H
#include "printable.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/shared_ptr.hpp>

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
  
  template<typename ExecutionContext, class... Types> void add_task(ExecutionContext & ctx, Types... args) { boost::asio::post(*tpool, [ctx, &args...](){ ctx(args...);});}

//-----------------------------------------------------------------------
/// Wait for all the tasks to finish.
//-----------------------------------------------------------------------

  void wait_tasks_finish() { tpool->join(); }
private:
  ThreadPool(int Number_thread = default_number_thread);
  static ThreadPool the_thread_pool;
  boost::shared_ptr<boost::asio::thread_pool> tpool;
  int number_thread_;
};
}
#endif
