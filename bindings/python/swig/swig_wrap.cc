#include <Python.h>
#include <iostream>
#include "swig_type_mapper_base.h"

// Python 2 and 3 do strings differently, so we have a simple macro to 
// keep from having lots of ifdefs spread around. See 
// https://wiki.python.org/moin/PortingExtensionModulesToPy3k for details on
// this.
#if PY_MAJOR_VERSION > 2
#define Text_FromUTF8(str) PyUnicode_FromString(str)
#else
#define Text_FromUTF8(str) PyString_FromString(str)
#endif

// Python 2 and 3 have different name for their swig init functions
#if PY_MAJOR_VERSION > 2
#define SWIG_INIT_FUNC(S) PyInit__ ## S
#define SWIG_INIT_TYPE PyObject *
#define SWIG_INIT_MODULE init_extension_module3
#else
#define SWIG_INIT_FUNC(S) init_ ## S
#define SWIG_INIT_TYPE void
#define SWIG_INIT_MODULE init_extension_module2
#endif
using namespace GeoCal;
// Map used between type_index and object to map this to python.
std::map<type_index, boost::shared_ptr<SwigTypeMapperBase> > 
  GeoCal::swig_type_map;

extern "C" {
#if PY_MAJOR_VERSION > 2
  PyObject * PyInit__swig_wrap(void);
#else
  void init_swig_wrap(void);
#endif
  SWIG_INIT_TYPE SWIG_INIT_FUNC(swig_std)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(swig_array)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(swig_boost_array)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(swig_quaternion)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(constant)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(generic_object)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(auto_derivative)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(array_ad)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(observer)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_quaternion)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(covariance)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(wgs84_constant)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_exception)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(functor)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(vfunctor_with_derivative)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(dfunctor_with_derivative)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(statistic)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_gsl_fit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_gsl_root)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_time)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(tiled_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_serialize_function)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(image_coordinate)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(tie_point)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(with_parameter)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(look_vector)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ground_coordinate)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(coordinate_converter)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ecr)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(eci)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geodetic)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(map_info)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_rpc)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(image_ground_connection)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_collection)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(frame_coordinate)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(camera)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ray_caster)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(time_table)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ipi)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_image_variable)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(sub_raster_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_image_multi_band)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_image_multi_band_variable)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(sub_raster_image_multi_band)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ground_mask)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(image_mask)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_datum)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(simple_dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(memory_raster_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(constant_raster_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(dem_map_info)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(feature_detector)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(forstner_feature_detector)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(image_matcher)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(image_to_image_match)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ccorr_matcher)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(lsm_matcher)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ccorr_lsm_matcher)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geometric_model)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_multifile)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(did_datum)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(dem_map_info_offset)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(map_info_image_ground_connection)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ground_mask_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(image_mask_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_averaged)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_subsample)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(calc_raster)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(dem_to_raster)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(calc_raster_multi_band)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(magnify_bilinear)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(magnify_replicate)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(smooth_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raster_image_tiled_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(dem_tiled_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(memory_dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(map_reprojected_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(pyramid_image_matcher)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_map_projected)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_simulated)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_simulated_ray_caster)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_image_to_image_match)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(surface_image_to_image_match)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(location_to_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ray_intersect)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(dem_match)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(quadratic_geometric_model)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geometric_model_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(memory_multi_band)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(spice_helper)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(raw_raster_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(aircraft_orbit_data)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(orbit_quaternion_list)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(pan_sharpen)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(scale_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(apply_mask)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(rpc_image_ground_connection)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ipi_image_ground_connection)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(eci_tod)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(quaternion_camera)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(galileo_camera)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(refraction)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(orbit_data_image_ground_connection)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(planet_coordinate)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_ray_caster)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_rolling_shutter)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_array)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_multiple_pass)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_collection_rolling_shutter)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(igc_collection_orbit_data)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(rolling_shutter_constant_time_table)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(piecewise_linear)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(orbit_correction)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(orbit_offset_correction)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(orbit_piecewise_correction)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(orbit_time_shift)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(tle_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(spice_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(argus_camera)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(argus_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(quickbird_camera)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(spot_camera)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(spot_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(quickbird_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(quickbird_time_table)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(pos_export_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(worldview2_cloudmask)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(doughnut_average)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(paint_class)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(vicar_lite_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(cart_lab_multifile)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(landsat7_panchromatic)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(material_detect)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ibis_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(vicar_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(vicar_argument)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(vicar_dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(datum_geoid96)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(srtm_dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(vicar_raster_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(mspi_config_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(mspi_paraxial_transform)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(mspi_camera)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(mspi_gimbal)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(air_mspi_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ground_mspi_orbit)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ground_mspi_igc)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(usgs_dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(srtm_90m_dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(air_mspi_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(air_mspi_time_table)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(air_mspi_l1b1)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(air_mspi_igc)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(air_mspi_igc_collection)(void);
#ifdef HAVE_CARTO
  SWIG_INIT_TYPE SWIG_INIT_FUNC(eci_tod_burl)(void);
#endif
#ifdef HAVE_FFTW
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_fftw)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(phase_correlation_matcher)(void);
#endif
#ifdef HAVE_HDF5
  SWIG_INIT_TYPE SWIG_INIT_FUNC(hdf_file)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(hdf_orbit)(void);
#endif
#ifdef HAVE_HDF5
#ifdef HAVE_CARTO
  SWIG_INIT_TYPE SWIG_INIT_FUNC(hdf_orbit_carto)(void);
#endif
#endif
#ifdef HAVE_GDAL
  SWIG_INIT_TYPE SWIG_INIT_FUNC(geocal_gdal)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(gdal_raster_image)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(gdal_datum)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(gdal_dem)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(gdal_multi_band)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(image_point_display)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(ogr_coordinate)(void);
  SWIG_INIT_TYPE SWIG_INIT_FUNC(vicar_ogr)(void);
#endif
#ifdef HAVE_VICAR_RTL
#endif
#ifdef HAVE_GDAL_VICAR_RTL
#endif
}

// Used throughout SWIG wrapper, define here because it is convenient.
std::string parse_python_exception() {
  PyObject *type = NULL, *value = NULL, *tb = NULL;
  PyErr_Fetch(&type, &value, &tb);
  PyObject* mod = PyImport_ImportModule("traceback");
  PyObject* err_str_list = NULL;
  if(tb)
    err_str_list = PyObject_CallMethodObjArgs(mod,
	      Text_FromUTF8("format_exception"), type, value, tb, NULL);
  std::string ret = "Python error that I can't parse";
  if(err_str_list) {
    PyObject* err_str = 
      PyObject_CallMethodObjArgs(Text_FromUTF8(""),
				 Text_FromUTF8("join"), 
				 err_str_list, NULL);
    if(err_str) {
        PyObject * temp_bytes = PyUnicode_AsEncodedString(err_str, "ASCII", 
	"strict");
        ret = PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
        Py_DECREF(temp_bytes);
    }
    Py_XDECREF(err_str);
  } else if(value) {
    PyObject * temp_bytes = PyUnicode_AsEncodedString(value, "ASCII", 
	"strict");
    ret = PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
    Py_DECREF(temp_bytes);
  }
  Py_XDECREF(mod);
  Py_XDECREF(err_str_list);
  Py_XDECREF(type);
  Py_XDECREF(value);
  Py_XDECREF(tb);
  return ret;
}

#if PY_MAJOR_VERSION > 2
// Version for python 3
static void init_extension_module3(PyObject* package, const char *modulename,
				  PyObject * (*initfunction)(void)) {
  PyObject *module = initfunction();
  PyObject *module_dic = PyImport_GetModuleDict();
  PyDict_SetItem(module_dic, Text_FromUTF8(modulename), module);
  if(PyModule_AddObject(package, (char *)modulename, module)) {
    std::cerr << "Initialisation in PyImport_AddObject failed for module "
	      << modulename << "\n";
    return;
  }
  Py_INCREF(module);
}
#else 
// Version for python 2
static void init_extension_module2(PyObject* package, const char *modulename,
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
#endif


// This next blob of code comes from 
// https://wiki.python.org/moin/PortingExtensionModulesToPy3k

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject *
error_out(PyObject *m) {
    struct module_state *st = GETSTATE(m);
    PyErr_SetString(st->error, "something bad happened");
    return NULL;
}

static PyMethodDef swig_wrap_methods[] = {
    {"error_out", (PyCFunction)error_out, METH_NOARGS, NULL},
    {NULL, NULL}
};

#if PY_MAJOR_VERSION >= 3

static int swig_wrap_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int swig_wrap_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "_swig_wrap",
        NULL,
        sizeof(struct module_state),
        swig_wrap_methods,
        NULL,
        swig_wrap_traverse,
        swig_wrap_clear,
        NULL
};

#define INITERROR return NULL

PyObject *
PyInit__swig_wrap(void)

#else
#define INITERROR return

void
init_swig_wrap(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("_swig_wrap", swig_wrap_methods);
#endif

    if (module == NULL) {
        std::cerr << "Initialization failed\n";
        INITERROR;
    }
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("swig_wrap.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

  PyObject *package = PyImport_AddModule((char *)"new_geocal");
  if(!package) {
      std::cerr << "Initialization failed\n";
      INITERROR;
  }
  
  SWIG_INIT_MODULE(package, "_swig_std", SWIG_INIT_FUNC(swig_std));
  SWIG_INIT_MODULE(package, "_swig_array", SWIG_INIT_FUNC(swig_array));
  SWIG_INIT_MODULE(package, "_swig_boost_array", SWIG_INIT_FUNC(swig_boost_array));
  SWIG_INIT_MODULE(package, "_swig_quaternion", SWIG_INIT_FUNC(swig_quaternion));
  SWIG_INIT_MODULE(package, "_constant", SWIG_INIT_FUNC(constant));
  SWIG_INIT_MODULE(package, "_generic_object", SWIG_INIT_FUNC(generic_object));
  SWIG_INIT_MODULE(package, "_auto_derivative", SWIG_INIT_FUNC(auto_derivative));
  SWIG_INIT_MODULE(package, "_array_ad", SWIG_INIT_FUNC(array_ad));
  SWIG_INIT_MODULE(package, "_observer", SWIG_INIT_FUNC(observer));
  SWIG_INIT_MODULE(package, "_geocal_quaternion", SWIG_INIT_FUNC(geocal_quaternion));
  SWIG_INIT_MODULE(package, "_covariance", SWIG_INIT_FUNC(covariance));
  SWIG_INIT_MODULE(package, "_wgs84_constant", SWIG_INIT_FUNC(wgs84_constant));
  SWIG_INIT_MODULE(package, "_geocal_exception", SWIG_INIT_FUNC(geocal_exception));
  SWIG_INIT_MODULE(package, "_functor", SWIG_INIT_FUNC(functor));
  SWIG_INIT_MODULE(package, "_vfunctor_with_derivative", SWIG_INIT_FUNC(vfunctor_with_derivative));
  SWIG_INIT_MODULE(package, "_dfunctor_with_derivative", SWIG_INIT_FUNC(dfunctor_with_derivative));
  SWIG_INIT_MODULE(package, "_statistic", SWIG_INIT_FUNC(statistic));
  SWIG_INIT_MODULE(package, "_geocal_gsl_fit", SWIG_INIT_FUNC(geocal_gsl_fit));
  SWIG_INIT_MODULE(package, "_geocal_gsl_root", SWIG_INIT_FUNC(geocal_gsl_root));
  SWIG_INIT_MODULE(package, "_geocal_time", SWIG_INIT_FUNC(geocal_time));
  SWIG_INIT_MODULE(package, "_tiled_file", SWIG_INIT_FUNC(tiled_file));
  SWIG_INIT_MODULE(package, "_geocal_serialize_function", SWIG_INIT_FUNC(geocal_serialize_function));
  SWIG_INIT_MODULE(package, "_image_coordinate", SWIG_INIT_FUNC(image_coordinate));
  SWIG_INIT_MODULE(package, "_tie_point", SWIG_INIT_FUNC(tie_point));
  SWIG_INIT_MODULE(package, "_with_parameter", SWIG_INIT_FUNC(with_parameter));
  SWIG_INIT_MODULE(package, "_look_vector", SWIG_INIT_FUNC(look_vector));
  SWIG_INIT_MODULE(package, "_ground_coordinate", SWIG_INIT_FUNC(ground_coordinate));
  SWIG_INIT_MODULE(package, "_coordinate_converter", SWIG_INIT_FUNC(coordinate_converter));
  SWIG_INIT_MODULE(package, "_ecr", SWIG_INIT_FUNC(ecr));
  SWIG_INIT_MODULE(package, "_eci", SWIG_INIT_FUNC(eci));
  SWIG_INIT_MODULE(package, "_geodetic", SWIG_INIT_FUNC(geodetic));
  SWIG_INIT_MODULE(package, "_dem", SWIG_INIT_FUNC(dem));
  SWIG_INIT_MODULE(package, "_map_info", SWIG_INIT_FUNC(map_info));
  SWIG_INIT_MODULE(package, "_geocal_rpc", SWIG_INIT_FUNC(geocal_rpc));
  SWIG_INIT_MODULE(package, "_raster_image", SWIG_INIT_FUNC(raster_image));
  SWIG_INIT_MODULE(package, "_image_ground_connection", SWIG_INIT_FUNC(image_ground_connection));
  SWIG_INIT_MODULE(package, "_igc_collection", SWIG_INIT_FUNC(igc_collection));
  SWIG_INIT_MODULE(package, "_frame_coordinate", SWIG_INIT_FUNC(frame_coordinate));
  SWIG_INIT_MODULE(package, "_camera", SWIG_INIT_FUNC(camera));
  SWIG_INIT_MODULE(package, "_ray_caster", SWIG_INIT_FUNC(ray_caster));
  SWIG_INIT_MODULE(package, "_orbit", SWIG_INIT_FUNC(orbit));
  SWIG_INIT_MODULE(package, "_time_table", SWIG_INIT_FUNC(time_table));
  SWIG_INIT_MODULE(package, "_ipi", SWIG_INIT_FUNC(ipi));
  SWIG_INIT_MODULE(package, "_raster_image_variable", SWIG_INIT_FUNC(raster_image_variable));
  SWIG_INIT_MODULE(package, "_sub_raster_image", SWIG_INIT_FUNC(sub_raster_image));
  SWIG_INIT_MODULE(package, "_raster_image_multi_band", SWIG_INIT_FUNC(raster_image_multi_band));
  SWIG_INIT_MODULE(package, "_raster_image_multi_band_variable", SWIG_INIT_FUNC(raster_image_multi_band_variable));
  SWIG_INIT_MODULE(package, "_sub_raster_image_multi_band", SWIG_INIT_FUNC(sub_raster_image_multi_band));
  SWIG_INIT_MODULE(package, "_ground_mask", SWIG_INIT_FUNC(ground_mask));
  SWIG_INIT_MODULE(package, "_image_mask", SWIG_INIT_FUNC(image_mask));
  SWIG_INIT_MODULE(package, "_geocal_datum", SWIG_INIT_FUNC(geocal_datum));
  SWIG_INIT_MODULE(package, "_simple_dem", SWIG_INIT_FUNC(simple_dem));
  SWIG_INIT_MODULE(package, "_memory_raster_image", SWIG_INIT_FUNC(memory_raster_image));
  SWIG_INIT_MODULE(package, "_constant_raster_image", SWIG_INIT_FUNC(constant_raster_image));
  SWIG_INIT_MODULE(package, "_dem_map_info", SWIG_INIT_FUNC(dem_map_info));
  SWIG_INIT_MODULE(package, "_feature_detector", SWIG_INIT_FUNC(feature_detector));
  SWIG_INIT_MODULE(package, "_forstner_feature_detector", SWIG_INIT_FUNC(forstner_feature_detector));
  SWIG_INIT_MODULE(package, "_image_matcher", SWIG_INIT_FUNC(image_matcher));
  SWIG_INIT_MODULE(package, "_image_to_image_match", SWIG_INIT_FUNC(image_to_image_match));
  SWIG_INIT_MODULE(package, "_ccorr_matcher", SWIG_INIT_FUNC(ccorr_matcher));
  SWIG_INIT_MODULE(package, "_lsm_matcher", SWIG_INIT_FUNC(lsm_matcher));
  SWIG_INIT_MODULE(package, "_ccorr_lsm_matcher", SWIG_INIT_FUNC(ccorr_lsm_matcher));
  SWIG_INIT_MODULE(package, "_geometric_model", SWIG_INIT_FUNC(geometric_model));
  SWIG_INIT_MODULE(package, "_raster_multifile", SWIG_INIT_FUNC(raster_multifile));
  SWIG_INIT_MODULE(package, "_did_datum", SWIG_INIT_FUNC(did_datum));
  SWIG_INIT_MODULE(package, "_dem_map_info_offset", SWIG_INIT_FUNC(dem_map_info_offset));
  SWIG_INIT_MODULE(package, "_map_info_image_ground_connection", SWIG_INIT_FUNC(map_info_image_ground_connection));
  SWIG_INIT_MODULE(package, "_ground_mask_image", SWIG_INIT_FUNC(ground_mask_image));
  SWIG_INIT_MODULE(package, "_image_mask_image", SWIG_INIT_FUNC(image_mask_image));
  SWIG_INIT_MODULE(package, "_raster_averaged", SWIG_INIT_FUNC(raster_averaged));
  SWIG_INIT_MODULE(package, "_raster_subsample", SWIG_INIT_FUNC(raster_subsample));
  SWIG_INIT_MODULE(package, "_calc_raster", SWIG_INIT_FUNC(calc_raster));
  SWIG_INIT_MODULE(package, "_dem_to_raster", SWIG_INIT_FUNC(dem_to_raster));
  SWIG_INIT_MODULE(package, "_calc_raster_multi_band", SWIG_INIT_FUNC(calc_raster_multi_band));
  SWIG_INIT_MODULE(package, "_magnify_bilinear", SWIG_INIT_FUNC(magnify_bilinear));
  SWIG_INIT_MODULE(package, "_magnify_replicate", SWIG_INIT_FUNC(magnify_replicate));
  SWIG_INIT_MODULE(package, "_smooth_image", SWIG_INIT_FUNC(smooth_image));
  SWIG_INIT_MODULE(package, "_raster_image_tiled_file", SWIG_INIT_FUNC(raster_image_tiled_file));
  SWIG_INIT_MODULE(package, "_dem_tiled_file", SWIG_INIT_FUNC(dem_tiled_file));
  SWIG_INIT_MODULE(package, "_memory_dem", SWIG_INIT_FUNC(memory_dem));
  SWIG_INIT_MODULE(package, "_map_reprojected_image", SWIG_INIT_FUNC(map_reprojected_image));
  SWIG_INIT_MODULE(package, "_pyramid_image_matcher", SWIG_INIT_FUNC(pyramid_image_matcher));
  SWIG_INIT_MODULE(package, "_igc_map_projected", SWIG_INIT_FUNC(igc_map_projected));
  SWIG_INIT_MODULE(package, "_igc_simulated", SWIG_INIT_FUNC(igc_simulated));
  SWIG_INIT_MODULE(package, "_igc_simulated_ray_caster", SWIG_INIT_FUNC(igc_simulated_ray_caster));
  SWIG_INIT_MODULE(package, "_igc_image_to_image_match", SWIG_INIT_FUNC(igc_image_to_image_match));
  SWIG_INIT_MODULE(package, "_surface_image_to_image_match", SWIG_INIT_FUNC(surface_image_to_image_match));
  SWIG_INIT_MODULE(package, "_location_to_file", SWIG_INIT_FUNC(location_to_file));
  SWIG_INIT_MODULE(package, "_ray_intersect", SWIG_INIT_FUNC(ray_intersect));
  SWIG_INIT_MODULE(package, "_dem_match", SWIG_INIT_FUNC(dem_match));
  SWIG_INIT_MODULE(package, "_quadratic_geometric_model", SWIG_INIT_FUNC(quadratic_geometric_model));
  SWIG_INIT_MODULE(package, "_geometric_model_image", SWIG_INIT_FUNC(geometric_model_image));
  SWIG_INIT_MODULE(package, "_memory_multi_band", SWIG_INIT_FUNC(memory_multi_band));
  SWIG_INIT_MODULE(package, "_spice_helper", SWIG_INIT_FUNC(spice_helper));
  SWIG_INIT_MODULE(package, "_raw_raster_image", SWIG_INIT_FUNC(raw_raster_image));
  SWIG_INIT_MODULE(package, "_aircraft_orbit_data", SWIG_INIT_FUNC(aircraft_orbit_data));
  SWIG_INIT_MODULE(package, "_orbit_quaternion_list", SWIG_INIT_FUNC(orbit_quaternion_list));
  SWIG_INIT_MODULE(package, "_pan_sharpen", SWIG_INIT_FUNC(pan_sharpen));
  SWIG_INIT_MODULE(package, "_scale_image", SWIG_INIT_FUNC(scale_image));
  SWIG_INIT_MODULE(package, "_apply_mask", SWIG_INIT_FUNC(apply_mask));
  SWIG_INIT_MODULE(package, "_rpc_image_ground_connection", SWIG_INIT_FUNC(rpc_image_ground_connection));
  SWIG_INIT_MODULE(package, "_ipi_image_ground_connection", SWIG_INIT_FUNC(ipi_image_ground_connection));
  SWIG_INIT_MODULE(package, "_eci_tod", SWIG_INIT_FUNC(eci_tod));
  SWIG_INIT_MODULE(package, "_quaternion_camera", SWIG_INIT_FUNC(quaternion_camera));
  SWIG_INIT_MODULE(package, "_galileo_camera", SWIG_INIT_FUNC(galileo_camera));
  SWIG_INIT_MODULE(package, "_refraction", SWIG_INIT_FUNC(refraction));
  SWIG_INIT_MODULE(package, "_orbit_data_image_ground_connection", SWIG_INIT_FUNC(orbit_data_image_ground_connection));
  SWIG_INIT_MODULE(package, "_planet_coordinate", SWIG_INIT_FUNC(planet_coordinate));
  SWIG_INIT_MODULE(package, "_igc_ray_caster", SWIG_INIT_FUNC(igc_ray_caster));
  SWIG_INIT_MODULE(package, "_igc_rolling_shutter", SWIG_INIT_FUNC(igc_rolling_shutter));
  SWIG_INIT_MODULE(package, "_igc_array", SWIG_INIT_FUNC(igc_array));
  SWIG_INIT_MODULE(package, "_igc_multiple_pass", SWIG_INIT_FUNC(igc_multiple_pass));
  SWIG_INIT_MODULE(package, "_igc_collection_rolling_shutter", SWIG_INIT_FUNC(igc_collection_rolling_shutter));
  SWIG_INIT_MODULE(package, "_igc_collection_orbit_data", SWIG_INIT_FUNC(igc_collection_orbit_data));
  SWIG_INIT_MODULE(package, "_rolling_shutter_constant_time_table", SWIG_INIT_FUNC(rolling_shutter_constant_time_table));
  SWIG_INIT_MODULE(package, "_piecewise_linear", SWIG_INIT_FUNC(piecewise_linear));
  SWIG_INIT_MODULE(package, "_orbit_correction", SWIG_INIT_FUNC(orbit_correction));
  SWIG_INIT_MODULE(package, "_orbit_offset_correction", SWIG_INIT_FUNC(orbit_offset_correction));
  SWIG_INIT_MODULE(package, "_orbit_piecewise_correction", SWIG_INIT_FUNC(orbit_piecewise_correction));
  SWIG_INIT_MODULE(package, "_orbit_time_shift", SWIG_INIT_FUNC(orbit_time_shift));
  SWIG_INIT_MODULE(package, "_tle_orbit", SWIG_INIT_FUNC(tle_orbit));
  SWIG_INIT_MODULE(package, "_spice_orbit", SWIG_INIT_FUNC(spice_orbit));
  SWIG_INIT_MODULE(package, "_argus_camera", SWIG_INIT_FUNC(argus_camera));
  SWIG_INIT_MODULE(package, "_argus_orbit", SWIG_INIT_FUNC(argus_orbit));
  SWIG_INIT_MODULE(package, "_quickbird_camera", SWIG_INIT_FUNC(quickbird_camera));
  SWIG_INIT_MODULE(package, "_spot_camera", SWIG_INIT_FUNC(spot_camera));
  SWIG_INIT_MODULE(package, "_spot_orbit", SWIG_INIT_FUNC(spot_orbit));
  SWIG_INIT_MODULE(package, "_quickbird_orbit", SWIG_INIT_FUNC(quickbird_orbit));
  SWIG_INIT_MODULE(package, "_quickbird_time_table", SWIG_INIT_FUNC(quickbird_time_table));
  SWIG_INIT_MODULE(package, "_pos_export_orbit", SWIG_INIT_FUNC(pos_export_orbit));
  SWIG_INIT_MODULE(package, "_worldview2_cloudmask", SWIG_INIT_FUNC(worldview2_cloudmask));
  SWIG_INIT_MODULE(package, "_doughnut_average", SWIG_INIT_FUNC(doughnut_average));
  SWIG_INIT_MODULE(package, "_paint_class", SWIG_INIT_FUNC(paint_class));
  SWIG_INIT_MODULE(package, "_vicar_lite_file", SWIG_INIT_FUNC(vicar_lite_file));
  SWIG_INIT_MODULE(package, "_cart_lab_multifile", SWIG_INIT_FUNC(cart_lab_multifile));
  SWIG_INIT_MODULE(package, "_landsat7_panchromatic", SWIG_INIT_FUNC(landsat7_panchromatic));
  SWIG_INIT_MODULE(package, "_material_detect", SWIG_INIT_FUNC(material_detect));
  SWIG_INIT_MODULE(package, "_ibis_file", SWIG_INIT_FUNC(ibis_file));
  SWIG_INIT_MODULE(package, "_vicar_file", SWIG_INIT_FUNC(vicar_file));
  SWIG_INIT_MODULE(package, "_vicar_argument", SWIG_INIT_FUNC(vicar_argument));
  SWIG_INIT_MODULE(package, "_vicar_dem", SWIG_INIT_FUNC(vicar_dem));
  SWIG_INIT_MODULE(package, "_datum_geoid96", SWIG_INIT_FUNC(datum_geoid96));
  SWIG_INIT_MODULE(package, "_srtm_dem", SWIG_INIT_FUNC(srtm_dem));
  SWIG_INIT_MODULE(package, "_vicar_raster_image", SWIG_INIT_FUNC(vicar_raster_image));
  SWIG_INIT_MODULE(package, "_mspi_config_file", SWIG_INIT_FUNC(mspi_config_file));
  SWIG_INIT_MODULE(package, "_mspi_paraxial_transform", SWIG_INIT_FUNC(mspi_paraxial_transform));
  SWIG_INIT_MODULE(package, "_mspi_camera", SWIG_INIT_FUNC(mspi_camera));
  SWIG_INIT_MODULE(package, "_mspi_gimbal", SWIG_INIT_FUNC(mspi_gimbal));
  SWIG_INIT_MODULE(package, "_air_mspi_orbit", SWIG_INIT_FUNC(air_mspi_orbit));
  SWIG_INIT_MODULE(package, "_ground_mspi_orbit", SWIG_INIT_FUNC(ground_mspi_orbit));
  SWIG_INIT_MODULE(package, "_ground_mspi_igc", SWIG_INIT_FUNC(ground_mspi_igc));
  SWIG_INIT_MODULE(package, "_usgs_dem", SWIG_INIT_FUNC(usgs_dem));
  SWIG_INIT_MODULE(package, "_srtm_90m_dem", SWIG_INIT_FUNC(srtm_90m_dem));
  SWIG_INIT_MODULE(package, "_air_mspi_file", SWIG_INIT_FUNC(air_mspi_file));
  SWIG_INIT_MODULE(package, "_air_mspi_time_table", SWIG_INIT_FUNC(air_mspi_time_table));
  SWIG_INIT_MODULE(package, "_air_mspi_l1b1", SWIG_INIT_FUNC(air_mspi_l1b1));
  SWIG_INIT_MODULE(package, "_air_mspi_igc", SWIG_INIT_FUNC(air_mspi_igc));
  SWIG_INIT_MODULE(package, "_air_mspi_igc_collection", SWIG_INIT_FUNC(air_mspi_igc_collection));
#ifdef HAVE_CARTO
  SWIG_INIT_MODULE(package, "_eci_tod_burl", SWIG_INIT_FUNC(eci_tod_burl));
#endif
#ifdef HAVE_FFTW
  SWIG_INIT_MODULE(package, "_geocal_fftw", SWIG_INIT_FUNC(geocal_fftw));
  SWIG_INIT_MODULE(package, "_phase_correlation_matcher", SWIG_INIT_FUNC(phase_correlation_matcher));
#endif
#ifdef HAVE_HDF5
  SWIG_INIT_MODULE(package, "_hdf_file", SWIG_INIT_FUNC(hdf_file));
  SWIG_INIT_MODULE(package, "_hdf_orbit", SWIG_INIT_FUNC(hdf_orbit));
#endif
#ifdef HAVE_HDF5
#ifdef HAVE_CARTO
  SWIG_INIT_MODULE(package, "_hdf_orbit_carto", SWIG_INIT_FUNC(hdf_orbit_carto));
#endif
#endif
#ifdef HAVE_GDAL
  SWIG_INIT_MODULE(package, "_geocal_gdal", SWIG_INIT_FUNC(geocal_gdal));
  SWIG_INIT_MODULE(package, "_gdal_raster_image", SWIG_INIT_FUNC(gdal_raster_image));
  SWIG_INIT_MODULE(package, "_gdal_datum", SWIG_INIT_FUNC(gdal_datum));
  SWIG_INIT_MODULE(package, "_gdal_dem", SWIG_INIT_FUNC(gdal_dem));
  SWIG_INIT_MODULE(package, "_gdal_multi_band", SWIG_INIT_FUNC(gdal_multi_band));
  SWIG_INIT_MODULE(package, "_image_point_display", SWIG_INIT_FUNC(image_point_display));
  SWIG_INIT_MODULE(package, "_ogr_coordinate", SWIG_INIT_FUNC(ogr_coordinate));
  SWIG_INIT_MODULE(package, "_vicar_ogr", SWIG_INIT_FUNC(vicar_ogr));
#endif
#ifdef HAVE_VICAR_RTL
#endif
#ifdef HAVE_GDAL_VICAR_RTL
#endif

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}


