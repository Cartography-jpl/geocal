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
{
  number_thread(Number_thread);
}

//-----------------------------------------------------------------------
/// Set the number of workers in the thread pool.
//-----------------------------------------------------------------------

void ThreadPool::number_thread(int V)
{
  number_thread_ = V;
  tpool = boost::make_shared<boost::asio::thread_pool>(V);
}

void ThreadPool::print(std::ostream& Os) const
{
  Os << "ThreadPool\n"
     << "  number_thread: " << number_thread() << "\n";
}
