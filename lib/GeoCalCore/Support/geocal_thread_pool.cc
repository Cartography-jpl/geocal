#include "geocal_thread_pool.h"
#include <boost/make_shared.hpp>

using namespace GeoCal;

ThreadPool ThreadPool::the_thread_pool{};

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
/// Set the number of workers in the thread pool.
//-----------------------------------------------------------------------

void ThreadPool::number_thread(int V)
{
  std::lock_guard<std::mutex> lock(tpool_mutex);
  number_thread_ = V;
  // Any existing pool is replaced with one of the new size when it is
  // next needed.
  tpool.reset();
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
/// Wait for all the tasks to finish.
//-----------------------------------------------------------------------

void ThreadPool::wait_tasks_finish()
{
  boost::shared_ptr<boost::asio::thread_pool> t;
  {
    std::lock_guard<std::mutex> lock(tpool_mutex);
    t.swap(tpool);
  }
  if(t)
    t->join();
}

void ThreadPool::print(std::ostream& Os) const
{
  Os << "ThreadPool\n"
     << "  number_thread: " << number_thread() << "\n";
}
