// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#define DO_IMPORT_ARRAY
%}
%include "swig_array_inc.i"

//--------------------------------------------------------------
// Before using numpy, we need to call the numpy supplied 
// function 'import_array' which imports the module and does set up.
//--------------------------------------------------------------

%init {
    import_array();
}

%{

//--------------------------------------------------------------
// Return numpy module
//--------------------------------------------------------------

PyObject* numpy_module()
{
  static PyObject* mod = 0;
  if(!mod)
    mod = PyImport_ImportModule("numpy");
  return mod;
}

//--------------------------------------------------------------
// Return numpy.float64
//--------------------------------------------------------------

PyObject* numpy_dot_float64()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "float64");
  return res;
}

PyObject* numpy_dot_int32()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "int32");
  return res;
}

PyObject* numpy_dot_bool()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "bool");
  return res;
}

PyObject* numpy_dot_object()
{
  static PyObject* res = 0;
  if(!res)
    res = PyObject_GetAttrString(numpy_module(), "object");
  return res;
}

%}

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
enum { fromStart = Blitz::INT_MIN, toEnd = Blitz::INT_MIN };
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
// The PRECEDENCE is the order that we check for a match with
// overloaded functions. The actual value doesn't matter too much,
// just make sure it is different from any of the type check
// in swig, and that it is different for different array types
//--------------------------------------------------------------

%define %array_template(NAME,TYPE,DIM, PRECEDENCE)

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
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1.extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1.stride(i) * sizeof(TYPE);
  }
  $result = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE >(), 
			stride, $1.data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>($1);
  PyArray_SetBaseObject((PyArrayObject*)$result, 
			SWIG_NewPointerObj(SWIG_as_voidptr(t), 
				   $descriptor(blitz::Array<TYPE, DIM>*), 					   SWIG_POINTER_NEW | 0 ));
}

//--------------------------------------------------------------
// Convert to numpy. See description above for lifetime issues.
//--------------------------------------------------------------

%typemap(out) const blitz::Array<TYPE, DIM>& {
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  $result = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE >(), 
			stride, $1->data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_SetBaseObject((PyArrayObject*)$result, 
			SWIG_NewPointerObj(SWIG_as_voidptr(t), 
				   $descriptor(blitz::Array<TYPE, DIM>*), 					   SWIG_POINTER_NEW | 0 ));
}

%typemap(out) blitz::Array<TYPE, DIM>& {
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  $result = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE>(), 
			stride, $1->data(), 0, 0, 0);
  PyArray_UpdateFlags((PyArrayObject*)$result, NPY_ARRAY_WRITEABLE);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_SetBaseObject((PyArrayObject*)$result, 
			SWIG_NewPointerObj(SWIG_as_voidptr(t), 
				   $descriptor(blitz::Array<TYPE, DIM>*), 					   SWIG_POINTER_NEW | 0 ));
}

//--------------------------------------------------------------
/// Handle multiple array returns
//--------------------------------------------------------------

%typemap(in, numinputs=0) blitz::Array<TYPE, DIM>& OUTPUT (blitz::Array<TYPE, DIM> temp) {
   $1 = &temp;
}

%typemap(in, numinputs=0) blitz::Array<TYPE, DIM>& OUTPUT1 (blitz::Array<TYPE, DIM> temp) {
   $1 = &temp;
}

%typemap(in, numinputs=0) blitz::Array<TYPE, DIM>& OUTPUT2 (blitz::Array<TYPE, DIM> temp) {
   $1 = &temp;
}


%typemap(argout) blitz::Array<TYPE, DIM>& OUTPUT {
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  PyObject *res = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE >(), 
			stride, $1->data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_SetBaseObject((PyArrayObject*)res, 
			SWIG_NewPointerObj(SWIG_as_voidptr(t), 
				   $descriptor(blitz::Array<TYPE, DIM>*), 					   SWIG_POINTER_NEW | 0 ));
  $result = SWIG_AppendOutput($result, res);
}

%typemap(argout) blitz::Array<TYPE, DIM>& OUTPUT1 {
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  PyObject *res = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE >(), 
			stride, $1->data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_SetBaseObject((PyArrayObject*)res, 
			SWIG_NewPointerObj(SWIG_as_voidptr(t), 
				   $descriptor(blitz::Array<TYPE, DIM>*), 					   SWIG_POINTER_NEW | 0 ));
  $result = SWIG_AppendOutput($result, res);
}

%typemap(argout) blitz::Array<TYPE, DIM>& OUTPUT2 {
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1->extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1->stride(i) * sizeof(TYPE);
  }
  PyObject *res = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE >(), 
			stride, $1->data(), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>(*$1);
  PyArray_SetBaseObject((PyArrayObject*)res, 
			SWIG_NewPointerObj(SWIG_as_voidptr(t), 
				   $descriptor(blitz::Array<TYPE, DIM>*), 					   SWIG_POINTER_NEW | 0 ));
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
    numpy.obj = to_numpy<TYPE >($input);
    if(!numpy.obj) {
      SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
      return NULL;
    }
    if(PyArray_NDIM((PyArrayObject*)numpy.obj) !=DIM) {
      SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
      return NULL;
    }
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
    numpy.obj = to_numpy<TYPE >($input);
    if(!numpy.obj) {
      SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
    }
    if(PyArray_NDIM((PyArrayObject*)numpy.obj) !=DIM) {
      SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
    }
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
  numpy.obj = to_numpy<TYPE >($input);
  if(!numpy.obj) {
    SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
  }
  if(PyArray_NDIM((PyArrayObject*)numpy.obj) !=DIM) {
    SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
  }
  $1 = to_blitz_array<TYPE, DIM>(numpy);
}

//--------------------------------------------------------------
// Handle conversion in directors
//--------------------------------------------------------------

%typemap(directorout) blitz::Array<TYPE, DIM> (PythonObject numpy) 
{
  PythonObject t(to_numpy<TYPE >($input));
  if(!t.obj) {
    SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
  }
  if(PyArray_NDIM((PyArrayObject*)t.obj) !=DIM) {
    SWIG_Error(SWIG_TypeError, "in method '$symname', expecting type  Array<TYPE,DIM>");
  }
  $result.reference(to_blitz_array<TYPE, DIM>(t).copy());
}

%typemap(directorin) const blitz::Array<TYPE, DIM>& 
{
  npy_intp dims[DIM], stride[DIM];
  for(int i = 0; i < DIM; ++i) {
    dims[i] = $1.extent(i);
    // Note numpy stride is in terms of bytes, while blitz in in terms
    // of type T.
    stride[i] = $1.stride(i) * sizeof(TYPE);
  }
  PyObject* res = PyArray_New(&PyArray_Type, DIM, dims, type_to_npy<TYPE >(), 
			      stride, const_cast<TYPE*>($1.data()), 0, 0, 0);
  blitz::Array<TYPE, DIM>* t = new blitz::Array<TYPE, DIM>($1);
  PyArray_SetBaseObject((PyArrayObject*)res, 
			SWIG_NewPointerObj(SWIG_as_voidptr(t), 
				   $descriptor(blitz::Array<TYPE, DIM>*), 					   SWIG_POINTER_NEW | 0 ));
  $input = res;
}

//--------------------------------------------------------------
// Check if object can be converted to a blitz::Array.
//--------------------------------------------------------------

%typecheck(PRECEDENCE) blitz::Array<TYPE, DIM>, const blitz::Array<TYPE, DIM>& {
  PythonObject t(to_numpy<TYPE >($input));
  $1 = (t.obj && PyArray_NDIM((PyArrayObject*)t.obj) ==DIM ? 1 : 0);
}

%template(NAME) blitz::Array<TYPE, DIM>;

%enddef

%array_template(BlitzArray_double_1, double, 1, 1150);
%array_template(BlitzArray_double_2, double, 2, 1151);
%array_template(BlitzArray_double_3, double, 3, 1152);
%array_template(BlitzArray_double_4, double, 4, 1153);
%array_template(BlitzArray_double_5, double, 5, 1154);
%array_template(BlitzArray_double_6, double, 6, 1155);
%array_template(BlitzArray_double_7, double, 7, 1156);
%array_template(BlitzArray_double_8, double, 8, 1157);
%array_template(BlitzArray_bool_1, bool, 1, 1158);
%array_template(BlitzArray_bool_2, bool, 2, 1159);
%array_template(BlitzArray_bool_3, bool, 3, 1160);
%array_template(BlitzArray_bool_4, bool, 4, 1161);
%array_template(BlitzArray_bool_5, bool, 5, 1162);
%array_template(BlitzArray_bool_6, bool, 6, 1163);
%array_template(BlitzArray_bool_7, bool, 7, 1164);
%array_template(BlitzArray_bool_8, bool, 8, 1165);
%array_template(BlitzArray_int_1, int, 1, 1166);
%array_template(BlitzArray_int_2, int, 2, 1167);
%array_template(BlitzArray_int_3, int, 3, 1168);
%array_template(BlitzArray_int_4, int, 4, 1169);
%array_template(BlitzArray_int_5, int, 5, 1170);
%array_template(BlitzArray_int_6, int, 6, 1171);
%array_template(BlitzArray_int_7, int, 7, 1172);
%array_template(BlitzArray_int_8, int, 8, 1173);
#endif  // end SWIGPYTHON

// List of things "import *" will include
%pythoncode %{
__all__ = ["BlitzArray_double_1", "BlitzArray_double_2", "BlitzArray_double_3", "BlitzArray_double_4", "BlitzArray_double_5", "BlitzArray_double_6", "BlitzArray_double_7", "BlitzArray_double_8", "BlitzArray_bool_1", "BlitzArray_bool_2", "BlitzArray_bool_3", "BlitzArray_bool_4", "BlitzArray_bool_5", "BlitzArray_bool_6", "BlitzArray_bool_7", "BlitzArray_bool_8", "BlitzArray_int_1", "BlitzArray_int_2", "BlitzArray_int_3", "BlitzArray_int_4", "BlitzArray_int_5", "BlitzArray_int_6", "BlitzArray_int_7", "BlitzArray_int_8"]
%}



