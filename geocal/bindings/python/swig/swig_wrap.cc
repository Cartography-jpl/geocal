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
  void init_swig_boost_array(void);
  void init_swig_quaternion(void);
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
  void init_image_coordinate(void);
  void init_look_vector(void);
  void init_ground_coordinate(void);
  void init_coordinate_converter(void);
  void init_ecr(void);
  void init_eci(void);
  void init_geodetic(void);
  void init_dem(void);
  void init_map_info(void);
  void init_geocal_rpc(void);
  void init_raster_image(void);
  void init_image_ground_connection(void);
  void init_frame_coordinate(void);
  void init_camera(void);
  void init_orbit(void);
  void init_time_table(void);
  void init_ipi(void);
  void init_raster_image_variable(void);
  void init_sub_raster_image(void);
  void init_raster_image_multi_band(void);
  void init_raster_image_multi_band_variable(void);
  void init_mask(void);
  void init_geocal_datum(void);
  void init_simple_dem(void);
  void init_memory_raster_image(void);
  void init_dem_map_info(void);
  void init_feature_detector(void);
  void init_forstner_feature_detector(void);
  void init_image_matcher(void);
  void init_image_to_image_match(void);
  void init_ccorr_matcher(void);
  void init_lsm_matcher(void);
  void init_ccorr_lsm_matcher(void);
  void init_rpc_image(void);
  void init_raster_multifile(void);
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
  init_extension_module(package, "_swig_boost_array", init_swig_boost_array);
  init_extension_module(package, "_swig_quaternion", init_swig_quaternion);
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
  init_extension_module(package, "_image_coordinate", init_image_coordinate);
  init_extension_module(package, "_look_vector", init_look_vector);
  init_extension_module(package, "_ground_coordinate", init_ground_coordinate);
  init_extension_module(package, "_coordinate_converter", init_coordinate_converter);
  init_extension_module(package, "_ecr", init_ecr);
  init_extension_module(package, "_eci", init_eci);
  init_extension_module(package, "_geodetic", init_geodetic);
  init_extension_module(package, "_dem", init_dem);
  init_extension_module(package, "_map_info", init_map_info);
  init_extension_module(package, "_geocal_rpc", init_geocal_rpc);
  init_extension_module(package, "_raster_image", init_raster_image);
  init_extension_module(package, "_image_ground_connection", init_image_ground_connection);
  init_extension_module(package, "_frame_coordinate", init_frame_coordinate);
  init_extension_module(package, "_camera", init_camera);
  init_extension_module(package, "_orbit", init_orbit);
  init_extension_module(package, "_time_table", init_time_table);
  init_extension_module(package, "_ipi", init_ipi);
  init_extension_module(package, "_raster_image_variable", init_raster_image_variable);
  init_extension_module(package, "_sub_raster_image", init_sub_raster_image);
  init_extension_module(package, "_raster_image_multi_band", init_raster_image_multi_band);
  init_extension_module(package, "_raster_image_multi_band_variable", init_raster_image_multi_band_variable);
  init_extension_module(package, "_mask", init_mask);
  init_extension_module(package, "_geocal_datum", init_geocal_datum);
  init_extension_module(package, "_simple_dem", init_simple_dem);
  init_extension_module(package, "_memory_raster_image", init_memory_raster_image);
  init_extension_module(package, "_dem_map_info", init_dem_map_info);
  init_extension_module(package, "_feature_detector", init_feature_detector);
  init_extension_module(package, "_forstner_feature_detector", init_forstner_feature_detector);
  init_extension_module(package, "_image_matcher", init_image_matcher);
  init_extension_module(package, "_image_to_image_match", init_image_to_image_match);
  init_extension_module(package, "_ccorr_matcher", init_ccorr_matcher);
  init_extension_module(package, "_lsm_matcher", init_lsm_matcher);
  init_extension_module(package, "_ccorr_lsm_matcher", init_ccorr_lsm_matcher);
  init_extension_module(package, "_rpc_image", init_rpc_image);
  init_extension_module(package, "_raster_multifile", init_raster_multifile);
}
