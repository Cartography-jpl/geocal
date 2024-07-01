#ifndef GEOCAL_THREAD_POOL_H
#define GEOCAL_THREAD_POOL_H
#include "printable.h"
#include <boost/asio/thread_pool.hpp>
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
private:
  ThreadPool(int Number_thread = default_number_thread);
  static ThreadPool the_thread_pool;
  boost::shared_ptr<boost::asio::thread_pool> tpool;
  int number_thread_;
};
}
#endif
