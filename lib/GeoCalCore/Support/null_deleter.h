#ifndef NULL_DELETER_H
#define NULL_DELETER_H
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This is a class that can be used to create a boost pointer to a
  value that does not actually claim ownership of the object. This
  won't delete anything when the pointer goes out of scope.
*******************************************************************/

struct null_deleter
{
  void operator()(void const *) const
  {
  }
};

//-----------------------------------------------------------------------
/// Helper routine to get a shared_ptr from a reference. The reference
/// is not deleted when the shared_ptr is.
//-----------------------------------------------------------------------

template<class T> boost::shared_ptr<T> const_pointer(const T& t)
{ return boost::shared_ptr<T>(const_cast<T*>(&t), null_deleter()); }

//-----------------------------------------------------------------------
/// Helper routine to get a shared_ptr from a reference. The reference
/// is not deleted when the shared_ptr is.
//-----------------------------------------------------------------------

template<class T> boost::shared_ptr<T> to_ptr(T& t)
{ return boost::shared_ptr<T>(&t, null_deleter()); }

}
#endif
