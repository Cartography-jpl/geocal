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
  void init_ground_mask(void);
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
  void init_did_datum(void);
  void init_map_info_image_ground_connection(void);
  void init_ground_mask_image(void);
  void init_raster_averaged(void);
  void init_raster_subsample(void);
  void init_calc_raster(void);
  void init_calc_raster_multi_band(void);
  void init_magnify_bilinear(void);
  void init_magnify_replicate(void);
  void init_smooth_image(void);
  void init_raster_image_tiled_file(void);
  void init_dem_tiled_file(void);
  void init_memory_dem(void);
  void init_map_reprojected_image(void);
  void init_calc_map_projected(void);
  void init_orbit_map_projected(void);
  void init_igc_map_projected(void);
  void init_igc_image_to_image_match(void);
  void init_surface_image_to_image_match(void);
  void init_ipi_map_projected(void);
  void init_location_to_file(void);
  void init_ray_intersect(void);
  void init_dem_match(void);
  void init_geocal_gdal(void);
  void init_gdal_raster_image(void);
  void init_gdal_datum(void);
  void init_gdal_dem(void);
  void init_gdal_multi_band(void);
  void init_image_point_display(void);
  void init_ogr_coordinate(void);
  void init_memory_multi_band(void);
  void init_spice_helper(void);
  void init_raw_raster_image(void);
  void init_aircraft_orbit_data(void);
  void init_orbit_quaternion_list(void);
  void init_pan_sharpen(void);
  void init_scale_image(void);
  void init_apply_mask(void);
  void init_rpc_image_ground_connection(void);
  void init_ipi_image_ground_connection(void);
  void init_eci_tod(void);
  void init_eci_tod_burl(void);
  void init_quaternion_camera(void);
  void init_refraction(void);
  void init_orbit_data_image_ground_connection(void);
  void init_hdf_orbit(void);
  void init_argus_camera(void);
  void init_argus_orbit(void);
  void init_ibis_file(void);
  void init_quickbird_camera(void);
  void init_spot_camera(void);
  void init_spot_orbit(void);
  void init_quickbird_orbit(void);
  void init_quickbird_time_table(void);
  void init_vicar_file(void);
  void init_vicar_argument(void);
  void init_vicar_dem(void);
  void init_vicar_lite_file(void);
  void init_vicar_multi_file(void);
  void init_datum_geoid96(void);
  void init_srtm_dem(void);
  void init_vicar_raster_image(void);
  void init_pos_export_orbit(void);
  void init_worldview2_cloudmask(void);
  void init_doughnut_average(void);
  void init_material_detect(void);
  void init_paint_class(void);
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
  init_extension_module(package, "_ground_mask", init_ground_mask);
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
  init_extension_module(package, "_did_datum", init_did_datum);
  init_extension_module(package, "_map_info_image_ground_connection", init_map_info_image_ground_connection);
  init_extension_module(package, "_ground_mask_image", init_ground_mask_image);
  init_extension_module(package, "_raster_averaged", init_raster_averaged);
  init_extension_module(package, "_raster_subsample", init_raster_subsample);
  init_extension_module(package, "_calc_raster", init_calc_raster);
  init_extension_module(package, "_calc_raster_multi_band", init_calc_raster_multi_band);
  init_extension_module(package, "_magnify_bilinear", init_magnify_bilinear);
  init_extension_module(package, "_magnify_replicate", init_magnify_replicate);
  init_extension_module(package, "_smooth_image", init_smooth_image);
  init_extension_module(package, "_raster_image_tiled_file", init_raster_image_tiled_file);
  init_extension_module(package, "_dem_tiled_file", init_dem_tiled_file);
  init_extension_module(package, "_memory_dem", init_memory_dem);
  init_extension_module(package, "_map_reprojected_image", init_map_reprojected_image);
  init_extension_module(package, "_calc_map_projected", init_calc_map_projected);
  init_extension_module(package, "_orbit_map_projected", init_orbit_map_projected);
  init_extension_module(package, "_igc_map_projected", init_igc_map_projected);
  init_extension_module(package, "_igc_image_to_image_match", init_igc_image_to_image_match);
  init_extension_module(package, "_surface_image_to_image_match", init_surface_image_to_image_match);
  init_extension_module(package, "_ipi_map_projected", init_ipi_map_projected);
  init_extension_module(package, "_location_to_file", init_location_to_file);
  init_extension_module(package, "_ray_intersect", init_ray_intersect);
  init_extension_module(package, "_dem_match", init_dem_match);
  init_extension_module(package, "_geocal_gdal", init_geocal_gdal);
  init_extension_module(package, "_gdal_raster_image", init_gdal_raster_image);
  init_extension_module(package, "_gdal_datum", init_gdal_datum);
  init_extension_module(package, "_gdal_dem", init_gdal_dem);
  init_extension_module(package, "_gdal_multi_band", init_gdal_multi_band);
  init_extension_module(package, "_image_point_display", init_image_point_display);
  init_extension_module(package, "_ogr_coordinate", init_ogr_coordinate);
  init_extension_module(package, "_memory_multi_band", init_memory_multi_band);
  init_extension_module(package, "_spice_helper", init_spice_helper);
  init_extension_module(package, "_raw_raster_image", init_raw_raster_image);
  init_extension_module(package, "_aircraft_orbit_data", init_aircraft_orbit_data);
  init_extension_module(package, "_orbit_quaternion_list", init_orbit_quaternion_list);
  init_extension_module(package, "_pan_sharpen", init_pan_sharpen);
  init_extension_module(package, "_scale_image", init_scale_image);
  init_extension_module(package, "_apply_mask", init_apply_mask);
  init_extension_module(package, "_rpc_image_ground_connection", init_rpc_image_ground_connection);
  init_extension_module(package, "_ipi_image_ground_connection", init_ipi_image_ground_connection);
  init_extension_module(package, "_eci_tod", init_eci_tod);
  init_extension_module(package, "_eci_tod_burl", init_eci_tod_burl);
  init_extension_module(package, "_quaternion_camera", init_quaternion_camera);
  init_extension_module(package, "_refraction", init_refraction);
  init_extension_module(package, "_orbit_data_image_ground_connection", init_orbit_data_image_ground_connection);
  init_extension_module(package, "_hdf_orbit", init_hdf_orbit);
  init_extension_module(package, "_argus_camera", init_argus_camera);
  init_extension_module(package, "_argus_orbit", init_argus_orbit);
  init_extension_module(package, "_ibis_file", init_ibis_file);
  init_extension_module(package, "_quickbird_camera", init_quickbird_camera);
  init_extension_module(package, "_spot_camera", init_spot_camera);
  init_extension_module(package, "_spot_orbit", init_spot_orbit);
  init_extension_module(package, "_quickbird_orbit", init_quickbird_orbit);
  init_extension_module(package, "_quickbird_time_table", init_quickbird_time_table);
  init_extension_module(package, "_vicar_file", init_vicar_file);
  init_extension_module(package, "_vicar_argument", init_vicar_argument);
  init_extension_module(package, "_vicar_dem", init_vicar_dem);
  init_extension_module(package, "_vicar_lite_file", init_vicar_lite_file);
  init_extension_module(package, "_vicar_multi_file", init_vicar_multi_file);
  init_extension_module(package, "_datum_geoid96", init_datum_geoid96);
  init_extension_module(package, "_srtm_dem", init_srtm_dem);
  init_extension_module(package, "_vicar_raster_image", init_vicar_raster_image);
  init_extension_module(package, "_pos_export_orbit", init_pos_export_orbit);
  init_extension_module(package, "_worldview2_cloudmask", init_worldview2_cloudmask);
  init_extension_module(package, "_doughnut_average", init_doughnut_average);
  init_extension_module(package, "_material_detect", init_material_detect);
  init_extension_module(package, "_paint_class", init_paint_class);
}
