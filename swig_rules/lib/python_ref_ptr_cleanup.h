#ifndef PYTHON_REF_PTR_CLEANUP_H
#define PYTHON_REF_PTR_CLEANUP_H

//#define PTR_DEBUG 1
#ifdef PTR_DEBUG
#include <iostream>
#endif

/****************************************************************//**
  This handles the lifetime to a python object, incrementing the
  reference counter on creation and decrementing when we call
  operator() as a shared_ptr deleter.
  
  The lifetime of python classes that derive from C++ director objects is a bit
  confusing, since we have both python and C++ pieces it isn't immediately
  clear who owns things, and how lifetime/cleanup is handled.

  After spending some time with this, we have adopted python *always*
  owns the objects and manages the lifetimes. A python director object
  is given a "this" member function by SWIG when we create the
  director object. The "this" member points to a boost::shared_ptr of
  the underlying C++ code (so in our skeleton
  https://github.jpl.nasa.gov/Cartography/swig-rules-skeleton
  DirectorExample with is a C++ class SwigDirector_DirectorExample
  object).

  When the python object gets a zero reference count, it is deleted
  and it in turns deletes the C++ SWIG director object.

  The complication that arises is when we pass this python object to
  C++ where it appears as the same SwigDirector_DirectorExample class,
  which contains a pointer to the python class (as swig_self member
  variable, available by the C++ accessor code swig_get_self()), which
  in turns contains the python level "this" that points back to the
  same SwigDirector_DirectorExample.

  The way we handle this is that we handle the passed C++ class in
  SWIG as a *separate* shared_ptr - so a different shared_ptr than the
  one used in the "this" python object. This shared_ptr used a feature
  of std::shared_ptr where you can supply a deleter object. The
  default shared_ptr deletes the data it points to when its count gets
  to zero, but this can behavior can be changed by supplying the
  deleter. We use PythonRefPtrCleanup class, which instead of deleting
  the SwigDirector_DirectorExample object instead decrements the
  python object reference count.

  If we happen to only have one reference left to the python director
  class as a shared_ptr to the C++ SWIG object, then when the SWIG
  object isn't directly deleted. Instead, the python reference is
  decremented, results in a count of 0, which then results in the
  python object cleaning of "this" and finally deleting the actual C++
  SWIG object.

  We modify both the swig code (so my_shared_ptr.i) and the boost
  serialization code of directors to handle this.

  See the https://github.jpl.nasa.gov/Cartography/swig-rules-skeleton
  for an example of this in use, and in particular py-tests to check
  that the object lifetimes are handled correctly.
*******************************************************************/

class PythonRefPtrCleanup {
public:
  PythonRefPtrCleanup(PyObject* Obj)
  : obj(Obj)
  {
    Py_INCREF((PyObject*) obj);
#ifdef 	PTR_DEBUG
    std::cerr << "In PythonRefPtrCleanup constructor " << obj << " has count "
	      << Py_REFCNT((PyObject*) obj) << "\n";
#endif	
  }
    
  PythonRefPtrCleanup(const PythonRefPtrCleanup& V)
    : obj(V.obj)
  {
  // Note we really don't want to increase reference count here. The
  // object doesn't get decremented in the ~PythonRefPtrCleanup,
  // rather just in operator(). So we don't really have an extra
  // reference here.
#ifdef 	PTR_DEBUG
    std::cerr << "In PythonRefPtrCleanup copy " << obj << " has count "
	      << Py_REFCNT((PyObject*) obj) << "\n";
#endif
  }
  
//-----------------------------------------------------------------------
/// Call by std::shared_ptr when its count gets to 0.
/// p here is the pointer contained in the shared_ptr
//-----------------------------------------------------------------------
  void operator()(void* p)
  {
#ifdef 	PTR_DEBUG
    std::cerr << "Start PythonRefPtrCleanup operator() " << obj << " has count "
	      << Py_REFCNT((PyObject*) obj) << "\n";
#endif    
    Py_DECREF(obj);
  }
private:
  PyObject* obj;
};
#endif
