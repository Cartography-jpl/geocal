#include "geocal_thread_pool.h"
#include "geocal_exception.h"
#include <boost/make_shared.hpp>

using namespace GeoCal;

ThreadPool ThreadPool::the_thread_pool{};
thread_local bool ThreadPool::in_task = false;

//-----------------------------------------------------------------------
/// Return the ThreadPool we are using.
///
/// We have this as a singleton, we want one ThreadPool for handling
/// all our threading. Note that there isn't any intrinsic reason why
/// we couldn't have multiple ThreadPool, it just seems natural to use
/// one set of workers. We could reevaluate this in the future if
/// there are places where we want multiple ThreadPool.
//-----------------------------------------------------------------------

ThreadPool& ThreadPool::thread_pool()
{
  return the_thread_pool;
}

//-----------------------------------------------------------------------
/// Constructor, create a thread pool with the given number of workers
//-----------------------------------------------------------------------

ThreadPool::ThreadPool(int Number_thread)
  : number_thread_(Number_thread)
{
}

//-----------------------------------------------------------------------
/// Set the number of workers in the thread pool. This can't be done
/// while there are tasks we haven't waited for.
//-----------------------------------------------------------------------

void ThreadPool::number_thread(int V)
{
  if(in_task)
    throw Exception("ThreadPool::number_thread can't be changed from inside a ThreadPool task");
  range_min_check(V, 1);
  std::lock_guard<std::mutex> lock(tpool_mutex);
  if(owner != std::thread::id())
    throw Exception("ThreadPool::number_thread can't be changed while the ThreadPool has tasks that haven't been waited for");
  number_thread_ = V;
  // Any existing pool is replaced with one of the new size when it is
  // next needed.
  tpool.reset();
}

//-----------------------------------------------------------------------
/// Check that we aren't being called from inside a task, or from a
/// different thread than the one that added the current tasks. If the
/// pool isn't being used, this thread becomes the owner.
//-----------------------------------------------------------------------

void ThreadPool::check_caller()
{
  if(in_task)
    throw Exception("ThreadPool used from inside a ThreadPool task, which isn't supported");
  std::lock_guard<std::mutex> lock(tpool_mutex);
  if(owner == std::thread::id())
    owner = std::this_thread::get_id();
  else if(owner != std::this_thread::get_id())
    throw Exception("ThreadPool is already being used by another thread, which isn't supported");
}

//-----------------------------------------------------------------------
/// Save the first exception thrown by a task.
//-----------------------------------------------------------------------

void ThreadPool::save_exception(const std::exception_ptr& E)
{
  std::lock_guard<std::mutex> lock(tpool_mutex);
  if(!task_exception)
    task_exception = E;
}

//-----------------------------------------------------------------------
/// Return the boost thread pool, creating it if needed.
//-----------------------------------------------------------------------

boost::asio::thread_pool& ThreadPool::pool()
{
  std::lock_guard<std::mutex> lock(tpool_mutex);
  if(!tpool)
    tpool = boost::make_shared<boost::asio::thread_pool>(number_thread_);
  return *tpool;
}

//-----------------------------------------------------------------------
/// Wait for all the tasks to finish. If any of the tasks threw an
/// exception, we rethrow the first one here.
//-----------------------------------------------------------------------

void ThreadPool::wait_tasks_finish()
{
  check_caller();
  boost::shared_ptr<boost::asio::thread_pool> t;
  {
    std::lock_guard<std::mutex> lock(tpool_mutex);
    t.swap(tpool);
  }
  if(t)
    t->join();
  std::exception_ptr e;
  {
    std::lock_guard<std::mutex> lock(tpool_mutex);
    e = task_exception;
    task_exception = nullptr;
    owner = std::thread::id();
  }
  if(e)
    std::rethrow_exception(e);
}

void ThreadPool::print(std::ostream& Os) const
{
  Os << "ThreadPool\n"
     << "  number_thread: " << number_thread() << "\n";
}
