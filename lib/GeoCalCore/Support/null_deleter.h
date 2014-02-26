#ifndef NULL_DELETER_H
#define NULL_DELETER_H

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
/// Create a boost pointer to a reference, where we don't actually
/// take ownership of the object. The pointed to object must continue
/// to exist.
//-----------------------------------------------------------------------

  template<class T> boost::shared_ptr<T> const_pointer(const T& t)
  { return boost::shared_ptr<T>(const_cast<T*>(&t), null_deleter()); }

//-----------------------------------------------------------------------
/// Create a boost pointer to a reference, where we don't actually
/// take ownership of the object. The pointed to object must continue
/// to exist.
//-----------------------------------------------------------------------

  template<class T> boost::shared_ptr<T> pointer(T& t)
  { return boost::shared_ptr<T>(&t, null_deleter()); }
}
#endif
