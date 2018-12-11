// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

// Stuff to include everywhere for using std::ostream. We also have 
// swig_iostream.i which includes stuff compiled in one spot.

%{
#include <ios>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/categories.hpp>

struct closable_seekable_device_tag :
  virtual boost::iostreams::device_tag, boost::iostreams::seekable,
  boost::iostreams::closable_tag { };
 
// Make a python FileHandle look like a device
class python_fh_device {
public:
  typedef char  char_type;
  typedef closable_seekable_device_tag category;
  python_fh_device(PyObject* Fh, boost::iostreams::filtering_istream* Fis = 0) :
    fh(Fh), fis(Fis) {}
  std::streamsize read(char* s, std::streamsize n)
  {
    PyObject* res = PyObject_CallMethod(fh, "read", "(i)", (int) n);
    if(res == NULL) {
      throw GeoCal::Exception("Call to FileHandle read failed");
    }
    char *rescp = PyBytes_AsString(res);
    std::copy(rescp, rescp + n, s);
    Py_DECREF(res);
    return n;
  }
  std::streamoff seek(std::streamoff off, std::ios_base::seekdir way)
  {
    throw GeoCal::Exception("Not implemented");
  }
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
  void close()
  {
    std::cerr << "Close is being called\n";
    if(fis)
      std::cerr << "File handle tell: " << fis->tellg() << "\n";
  }
  PyObject* fh;
  boost::iostreams::filtering_istream* fis;
};

 inline python_fh_device python_fh_inserter(PyObject* Fh, boost::iostreams::filtering_istream* Fis = 0)
{
  return python_fh_device(Fh, Fis);
}
 
%}
