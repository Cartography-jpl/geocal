%module geocal
//==============================================================
// This gives support for blitz::Array. For python, we map this
// directly to narray.
//==============================================================

%{
// Don't want to use threads with ruby
#undef _REENTRANT
#include <blitz/array.h>
#include <blitz/range.h>
%}

#ifdef SWIGPYTHON
%{
#include <numpy/arrayobject.h>
#include "geocal_exception.h"

//--------------------------------------------------------------
// Before using numpy, we need to call the numpy supplied 
// function 'import_array' which imports the module and does set up.
// We don't know what numpy related function will be first called,
// so we have a generic function called everywhere that does the
// initialization once.
//--------------------------------------------------------------

static void do_import() {
  static bool already_done = false;
  if(!already_done) {
    import_array();
    already_done = true;
  }
}

//--------------------------------------------------------------
// Helper routines to map a template type to the code numpy uses
// for that type.
//--------------------------------------------------------------

template<class T> int type_to_npy();
template<> int type_to_npy<double>() {return NPY_DOUBLE;}
template<> int type_to_npy<float>() {return NPY_FLOAT32;}
template<> int type_to_npy<int>() {return NPY_INT;}
template<> int type_to_npy<short int>() {return NPY_INT16;}
template<> inline int type_to_npy<bool>() {return NPY_BOOL;}

//--------------------------------------------------------------
// Return numpy module
//--------------------------------------------------------------

static PyObject* numpy_module()
{
  static PyObject* mod = 0;
  if(!mod)
    mod = PyImport_ImportModule("numpy");
  return mod;
}

//--------------------------------------------------------------
// Return numpy.float64
//--------------------------------------------------------------

static PyObject* numpy_dot_float64()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "float64");
  return res;
}

static PyObject* numpy_dot_float32()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "float32");
  return res;
}

static PyObject* numpy_dot_int32()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "int32");
  return res;
}

static PyObject* numpy_dot_bool()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "bool");
  return res;
}

static PyObject* numpy_dot_int16()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "int16");
  return res;
}

//--------------------------------------------------------------
// Use the numpy command "asarray" to convert various python 
// objects to a numpy object. This may return null, if the 
// "asarray" fails. 
//--------------------------------------------------------------

template<class T> PyObject* to_numpy(PyObject* obj);

template<> inline PyObject* to_numpy<double>(PyObject* obj)
{
  PyObject* res = PyObject_CallMethodObjArgs(numpy_module(), 
					     PyString_FromString("asarray"), 
					     obj, numpy_dot_float64(), NULL);
  // Don't worry about errors , since we just return a null
  PyErr_Clear();
  return res;
}

template<> inline PyObject* to_numpy<bool>(PyObject* obj)
{
  PyObject* res = PyObject_CallMethodObjArgs(numpy_module(), 
				    PyString_FromString("asarray"), 
				    obj, numpy_dot_bool(), NULL);
  PyErr_Clear();
  return res;
}

template<> inline PyObject* to_numpy<float>(PyObject* obj)
{
  PyObject* res = PyObject_CallMethodObjArgs(numpy_module(), 
				    PyString_FromString("asarray"), 
				    obj, numpy_dot_float32(), NULL);
  // Don't worry about errors , since we just return a null
  PyErr_Clear();
  return res;
}

template<> inline PyObject* to_numpy<int>(PyObject* obj)
{
  PyObject* res = PyObject_CallMethodObjArgs(numpy_module(), 
				    PyString_FromString("asarray"), 
				    obj, numpy_dot_int32(), NULL);
  PyErr_Clear();
  return res;
}

template<> inline PyObject* to_numpy<short int>(PyObject* obj)
{
  PyObject* res = PyObject_CallMethodObjArgs(numpy_module(), 
				    PyString_FromString("asarray"), 
				    obj, numpy_dot_int16(), NULL);
  PyErr_Clear();
  return res;
}

//--------------------------------------------------------------
// Convert a numpy array to a blitz::Array. The numpy should 
// already be the right data type before calling these (you can
// call to_numpy, if that is convenient). The underlying data is 
// still owned by the numpy object, so you need to make sure that
// the numpy object doesn't get deleted until you are done with
// the blitz::Array.
//
// If this fails, we throw an exception.
//--------------------------------------------------------------

template<class T, int D> inline blitz::Array<T, D> 
  to_blitz_array(PyObject* numpy)
{
  if(PyArray_NDIM(numpy) != D) {
    std::cerr << PyArray_NDIM(numpy) << "\n"
	      << D << "\n";
    throw 
      GeoCal::Exception("Dimension of array is not the expected size");
  }
  if(PyArray_TYPE(numpy) != type_to_npy<T>()) {
    throw 
      GeoCal::Exception("Type of array not the expected type");
  }
  blitz::TinyVector<int, D> shape, stride;
  for(int i = 0; i < D; ++i) {
    shape(i) = PyArray_DIM(numpy, i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride(i) = PyArray_STRIDE(numpy, i) / sizeof(T);
    if((int) (stride(i) * sizeof(T)) != (int) PyArray_STRIDE(numpy, i)) {
      throw 
	GeoCal::Exception("blitz::Array can't handle strides that aren't an even multiple of sizeof(T)");
    }
  }
  return blitz::Array<T, D>((T*)PyArray_DATA(numpy), shape, stride, 
			    blitz::neverDeleteData);
}
%}
#endif

// Allow conversion to a binary String in the target language
%include "cdata.i"

namespace blitz {
// Define blitz::Array for use in Python
template<class T, int D> class Array  {
public:
  // These functions aren't normally used, because typemaps
  // automatically map from blitz::Array to numpy or narry. But leave
  // in place for helping with other languages.
  Array(int e1);
  Array(int e1, int e2, blitz::GeneralArrayStorage<D> storage = 
	blitz::FortranArray<D>());
  Array(int e1, int e2, int e3, blitz::GeneralArrayStorage<D> storage = 
	blitz::FortranArray<D>());
  Array(int e1, int e2, int e3, int e4, blitz::GeneralArrayStorage<D> storage = 
	blitz::FortranArray<D>());
  T* data();
  int size() const;
  %extend {
     T read(int i1) {return (*$self)(i1);}
     T read(int i1,int i2) {return (*$self)(i1,i2);}
     T read(int i1, int i2, int i3) {return (*$self)(i1,i2,i3);}
     T read(int i1, int i2, int i3, int i4) {return (*$self)(i1,i2,i3,i4);}
     void write(int i1, T val) {(*$self)(i1) = val;}
     void write(int i1,int i2, T val) {(*$self)(i1,i2) = val;}
     void write(int i1, int i2, int i3, T val) {(*$self)(i1,i2,i3) = val;}
     void write(int i1, int i2, int i3, int i4, T val) 
     {(*$self)(i1,i2,i3,i4) = val;}
     void* datav() { return (void*) $self->data(); }
     int shape0() { return $self->shape()[0]; }
     int shape1() { return $self->shape()[1]; }
     int shape2() { return $self->shape()[2]; }
     int shape3() { return $self->shape()[3]; }
  }
};

// Define blitz::Range for use in Python
enum { fromStart = Blitz::INT_MIN, toEnd = Blitz::INT_MAX };
class Range {
public:
  Range();
  explicit Range(int slicePosition);
  Range(int first, int last, int stride=1);
  int first(int lowRange = 0) const;
  int last(int highRange = 0) const;
  unsigned length(int =0) const;
  int stride() const;
  bool isAscendingContiguous() const;
  void setRange(int first, int last, int stride=1);
  static Range all();
  bool isUnitStride() const;
};

}

//************************************************************
// Type map to use python type numpy as input and output
//************************************************************

#ifdef SWIGPYTHON
//--------------------------------------------------------------
// Swig doesn't have typemap templates, so we define a macro to
// do this for each type and dimension, and then call the macro
// below to set this up for a range of types and sizes.
//--------------------------------------------------------------

%define %array_template(NAME,TYPE,DIM)

//--------------------------------------------------------------
// Convert to numpy. Note that there is a complication in the 
// lifetime of the pointed to array. numpy can't take ownership
// of the memory in the blitz::Array, since it wasn't allocated
// by python. Instead, numpy just points to the memory. To ensure
// that the blitz::Array memory isn't freeded, we also stash a
// python object wrapping around the blitz::Array that holds onto
// the object. This gets placed in a special area set up by numpy
// exactly for this purpose called "BASE". When the numpy array 
// get deleted, it also deletes the numpy. If this is the only
// reference to the blitz::Array memory, then the memory gets
// cleaned up then.
//--------------------------------------------------------------

%typemap(out) blitz::Array<TYPE, DIM> {
  do_import();
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1.extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1.stride(i) * sizeof(TYPE);
  }
  $result = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE>(), 
			stride, $1.data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>($1);
  PyArray_BASE($result) = SWIG_NewPointerObj(SWIG_as_voidptr(t), 
		     $descriptor(blitz::Array<TYPE, DIM>*), 
	             SWIG_POINTER_NEW | 0 );
}

//--------------------------------------------------------------
// Convert to numpy. See description above for lifetime issues.
//--------------------------------------------------------------

%typemap(out) const blitz::Array<TYPE, DIM>& {
  do_import();
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  $result = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE>(), 
			stride, $1->data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_BASE($result) = SWIG_NewPointerObj(SWIG_as_voidptr(t), 
			     $descriptor(blitz::Array<TYPE, DIM>*), 
			     SWIG_POINTER_NEW | 0 );
}

%typemap(out) blitz::Array<TYPE, DIM>& {
  do_import();
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  $result = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE>(), 
			stride, $1->data(), 0, NPY_WRITEABLE, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_BASE($result) = SWIG_NewPointerObj(SWIG_as_voidptr(t), 
			     $descriptor(blitz::Array<TYPE, DIM>*), 
			     SWIG_POINTER_NEW | 0 );
}

//--------------------------------------------------------------
/// Handle multiple array returns
//--------------------------------------------------------------

%typemap(in, numinputs=0) blitz::Array<TYPE, DIM>& OUTPUT (blitz::Array<TYPE, DIM> temp) {
   $1 = &temp;
}

%typemap(argout) blitz::Array<TYPE, DIM>& OUTPUT {
  do_import();
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  PyObject *res = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE>(), 
			stride, $1->data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_BASE(res) = SWIG_NewPointerObj(SWIG_as_voidptr(t), 
			     $descriptor(blitz::Array<TYPE, DIM>*), 
			     SWIG_POINTER_NEW | 0 );
  $result = SWIG_AppendOutput($result, res);
}

//--------------------------------------------------------------
// Convert any type first to a numpy array (doesn't copy if 
// already a numpy array), and then set blitz array to point to
// this.
//--------------------------------------------------------------

%typemap(in) const blitz::Array<TYPE, DIM>& (blitz::Array<TYPE, DIM> a, PythonObject numpy) 
{
  int res = SWIG_ConvertPtr($input, (void**)(&$1), $descriptor(blitz::Array<TYPE, DIM>*), 
			    %convertptr_flags);
  if(!SWIG_IsOK(res)) {
    numpy.obj = to_numpy<TYPE>($input);
    if(!numpy.obj)
      return NULL;
    a.reference(to_blitz_array<TYPE, DIM>(numpy));
    $1 = &a;
  }
}

//--------------------------------------------------------------
// Version that forces a copy of data
//--------------------------------------------------------------

%typemap(in) const blitz::Array<TYPE, DIM>& FORCE_COPY (blitz::Array<TYPE, DIM> a, PythonObject numpy) 
{
  int res = SWIG_ConvertPtr($input, (void**)(&$1), $descriptor(blitz::Array<TYPE, DIM>*), 
			    %convertptr_flags);
  if(!SWIG_IsOK(res)) {
    numpy.obj = to_numpy<TYPE>($input);
    if(!numpy.obj)
      return NULL;
    a.reference(to_blitz_array<TYPE, DIM>(numpy).copy());
    $1 = &a;
  }
}

//--------------------------------------------------------------
// Convert any type first to a numpy array (doesn't copy if 
// already a numpy array), and then set blitz array to point to
// this.
//--------------------------------------------------------------

%typemap(in) blitz::Array<TYPE, DIM> (PythonObject numpy) 
{
  numpy.obj = to_numpy<TYPE>($input);
  if(!numpy.obj)
    return NULL;
  $1 = to_blitz_array<TYPE, DIM>(numpy);
}

//--------------------------------------------------------------
// Handle conversion in directors
//--------------------------------------------------------------

%typemap(directorout) blitz::Array<TYPE, DIM> (PythonObject numpy) 
{
  $result.reference(to_blitz_array<TYPE, DIM>($input).copy());
}

//--------------------------------------------------------------
// Check if object can be converted to a blitz::Array.
//--------------------------------------------------------------

%typemap(typecheck,precedence=SWIG_TYPECHECK_INTEGER) blitz::Array<TYPE, DIM>, const blitz::Array<TYPE, DIM>& {
  PythonObject t(to_numpy<TYPE>($input));
  $1 = (t.obj && PyArray_NDIM(t.obj) ==DIM ? 1 : 0);
}

%template(NAME) blitz::Array<TYPE, DIM>;

%enddef

%array_template(BlitzArray_double_1, double, 1);
%array_template(BlitzArray_double_2, double, 2);
%array_template(BlitzArray_double_3, double, 3);
%array_template(BlitzArray_double_4, double, 4);
%array_template(BlitzArray_bool_1, bool, 1);
%array_template(BlitzArray_bool_2, bool, 2);
%array_template(BlitzArray_bool_3, bool, 3);
%array_template(BlitzArray_bool_4, bool, 4);
%array_template(BlitzArray_float_1, float, 1);
%array_template(BlitzArray_float_2, float, 2);
%array_template(BlitzArray_float_3, float, 3);
%array_template(BlitzArray_float_4, float, 4);
%array_template(BlitzArray_int_1, int, 1);
%array_template(BlitzArray_int_2, int, 2);
%array_template(BlitzArray_int_3, int, 3);
%array_template(BlitzArray_int_4, int, 4);
%array_template(BlitzArray_short_1, short int, 1);
%array_template(BlitzArray_short_2, short int, 2);
%array_template(BlitzArray_short_3, short int, 3);
%array_template(BlitzArray_short_4, short int, 4);


#endif  // end SWIGPYTHON

//==============================================================
// Support for boost::multi_array.
//==============================================================

%{
#include <boost/shared_ptr.hpp>
#include <boost/multi_array.hpp>
%}

// Allow conversion of multi_array to a binary String in the target language
%include "cdata.i"
namespace boost {
template<class T, int D> class multi_array  {
public:
  T* data();
  int num_elements() const;
  %extend {
     int shape0() { return $self->shape()[0]; }
     int shape1() { return $self->shape()[1]; }
     int shape2() { return $self->shape()[2]; }
     int shape3() { return $self->shape()[3]; }
  }
};
}

%rename(MultiArray_int_2) boost::multi_array<int, 2>;
%template(MultiArray_short_2) boost::multi_array<short int, 2>;
%template(MultiArray_char_2) boost::multi_array<unsigned char, 2>;

// Add conversion to short. This is needed to display raster subsets
// using image magick.
namespace boost {
class multi_array<int, 2>  {
public:
  int* data();
  int num_elements() const;
  %extend {
     int shape0() { return $self->shape()[0]; }
     int shape1() { return $self->shape()[1]; }
     boost::shared_ptr<boost::multi_array<short int, 2> > to_short()
     {
        boost::array<boost::multi_array_types::index, 2> sz = 
	  { {$self->shape()[0], $self->shape()[1]} };
        boost::shared_ptr<boost::multi_array<short int, 2> > res(new
           boost::multi_array<short int, 2>(sz));
        *res = *$self;
        return res;
     }
     boost::shared_ptr<boost::multi_array<unsigned char, 2> > to_char()
     {
        boost::array<boost::multi_array_types::index, 2> sz = 
	  { {$self->shape()[0], $self->shape()[1]} };
        boost::shared_ptr<boost::multi_array<unsigned char, 2> > res(new
           boost::multi_array<unsigned char, 2>(sz));
        *res = *$self;
        return res;
     }
  }
};
}

%cdata(int);
%cdata(double);
%cdata(float);
%cdata(short int, short);
%cdata(unsigned char, char);

// Add support for boost::array.
%{
#include <boost/array.hpp>
#include <boost/foreach.hpp>
%}

namespace boost {
template<class T, int D> class array {
public:
#ifdef SWIGPYTHON
  %rename(_size) size();
  int size();
  %pythoncode %{
    @property
    def size(self):
       return self._size()

    def set(self, v):
       '''Set the value to the given value. The value should be a container
       that we can enumerate over to fill in the value of the array'''
       for i, t in enumerate(v):
          if(i >= self.size):
              raise RuntimeError("Value must be exactly %d in size" % self.size)
          self[i] = t
       if(i != self.size - 1):
           raise RuntimeError("Value must be exactly %d in size" % self.size)

    def __iter__(self):
       for i in range(self.size):
           yield self[i]

    def __reduce__(self):
       return _new_from_set, (self.__class__, list(self))
  %}
  %extend {
    T __getitem__(int i) {return (*$self)[i];}
    void __setitem__(int i, const T& V) {(*$self)[i] = V;}
    std::string print_to_string() const
      { 
	std::ostringstream os;
	BOOST_FOREACH(T i, *$self)
	  os << i << " ";
	return os.str();
      }
   }
#else
   int size();
   %extend {
     T read(int i) {return (*$self)[i];}
     void write(int i, const T& V) {(*$self)[i] = V;}
   }
#endif
};

%template(Array_double_20) array<double, 20>;
%template(Array_double_12) array<double, 12>;
%template(Array_double_14) array<double, 14>;
%template(Array_double_3) array<double, 3>;
%template(Array_bool_20) array<bool, 20>;
}

%template(Vector_Array_double_12) std::vector<boost::array<double, 12> >;
%template(Vector_Array_double_14) std::vector<boost::array<double, 14> >;


