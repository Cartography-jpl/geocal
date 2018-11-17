// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

// Stuff to include everywhere for using std::ostream. We also have 
// swig_iostream.i which includes stuff compiled in one spot.

%{
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/categories.hpp>

// Make a python FileHandle look like a device
class python_fh_device {
public:
  typedef char  char_type;
  typedef boost::iostreams::sink_tag category;
  python_fh_device(PyObject* Fh) : fh(Fh) {}
  std::streamsize write(const char* s, std::streamsize n)
  {
    PyObject* res = PyObject_CallMethod(fh, "write", "(y#)", s, (int) n);
    if(res == NULL) {
      throw GeoCal::Exception("Call to FileHandle write failed");
    } else {
      Py_DECREF(res);
    }
    return n;
  }
  PyObject* fh;
};

inline python_fh_device python_fh_inserter(PyObject* Fh)
{
  return python_fh_device(Fh);
}
 
%}
