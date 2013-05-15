#include <Python.h>
#include <iostream>
#include "swig_type_mapper_base.h"

using namespace GeoCal;
// Map used between type_index and object to map this to python.
std::map<type_index, boost::shared_ptr<SwigTypeMapperBase> > 
  GeoCal::swig_type_map;

extern "C" {
  void init_swig_wrap(void);
  void init_swig_std(void);
  void init_swig_array(void);
  void init_constant(void);
  void init_generic_object(void);
  void init_covariance(void);
  void init_wgs84_constant(void);
  void init_geocal_exception(void);
  void init_functor(void);
  void init_vfunctor_with_derivative(void);
  void init_statistic(void);
  void init_geocal_gsl_fit(void);
  void init_geocal_gsl_root(void);
  void init_geocal_time(void);
  void init_tiled_file(void);
  void init_hdf_file(void);
}

// Used throughout SWIG wrapper, define here because it is convenient.
std::string parse_python_exception() {
  PyObject *type = NULL, *value = NULL, *tb = NULL;
  PyErr_Fetch(&type, &value, &tb);
  PyObject* mod = PyImport_ImportModule("traceback");
  PyObject* err_str_list = PyObject_CallMethodObjArgs(mod,
		 PyString_FromString("format_exception"), type, value, tb, NULL);
  std::string ret = "Python error that I can't parse";
  if(err_str_list) {
    PyObject* err_str = 
      PyObject_CallMethodObjArgs(PyString_FromString(""),
				 PyString_FromString("join"), 
				 err_str_list, NULL);
    if(err_str)
      ret = PyString_AsString(err_str);
    Py_XDECREF(err_str);
  } else if(value) {
    ret = PyString_AsString(value);
  }
  Py_XDECREF(mod);
  Py_XDECREF(err_str_list);
  Py_XDECREF(type);
  Py_XDECREF(value);
  Py_XDECREF(tb);
  return ret;
}

static void init_extension_module(PyObject* package, const char *modulename,
				  void (*initfunction)(void)) {
  PyObject *module = PyImport_AddModule((char *)modulename);
  if(!module) {
    std::cerr << "Initialisation in PyImport_AddModule failed for module "
	      << modulename << "\n";
    return;
  }
  if(PyModule_AddObject(package, (char *)modulename, module)) {
    std::cerr << "Initialisation in PyImport_AddObject failed for module "
	      << modulename << "\n";
    return;
  }
  Py_INCREF(module);
  initfunction();
}

void init_swig_wrap(void) 
{
  // Initialise this module
  PyObject* module = Py_InitModule("_swig_wrap", NULL); // NULL only
  if(!module) {
    std::cerr << "Initialization failed\n";
    return;
  }

  PyObject *package = PyImport_AddModule((char *)"new_full_physics");
  if(!package) {
    std::cerr << "Initialization failed\n";
    return;
  }
  
  init_extension_module(package, "_swig_std", init_swig_std);
  init_extension_module(package, "_swig_array", init_swig_array);
  init_extension_module(package, "_constant", init_constant);
  init_extension_module(package, "_generic_object", init_generic_object);
  init_extension_module(package, "_covariance", init_covariance);
  init_extension_module(package, "_wgs84_constant", init_wgs84_constant);
  init_extension_module(package, "_geocal_exception", init_geocal_exception);
  init_extension_module(package, "_functor", init_functor);
  init_extension_module(package, "_vfunctor_with_derivative", init_vfunctor_with_derivative);
  init_extension_module(package, "_statistic", init_statistic);
  init_extension_module(package, "_geocal_gsl_fit", init_geocal_gsl_fit);
  init_extension_module(package, "_geocal_gsl_root", init_geocal_gsl_root);
  init_extension_module(package, "_geocal_time", init_geocal_time);
  init_extension_module(package, "_tiled_file", init_tiled_file);
  init_extension_module(package, "_hdf_file", init_hdf_file);
}
