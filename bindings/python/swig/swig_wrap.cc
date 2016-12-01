#define PYTHON_MODULE_NAME _swig_wrap
#include "python_lib_init.h"
#include "geocal_config.h"

extern "C" {
  INIT_TYPE INIT_FUNC(_swig_std)(void);
  INIT_TYPE INIT_FUNC(_swig_array)(void);
  INIT_TYPE INIT_FUNC(_swig_boost_array)(void);
  INIT_TYPE INIT_FUNC(_swig_quaternion)(void);
  INIT_TYPE INIT_FUNC(_constant)(void);
  INIT_TYPE INIT_FUNC(_generic_object)(void);
  INIT_TYPE INIT_FUNC(_auto_derivative)(void);
  INIT_TYPE INIT_FUNC(_array_ad)(void);
  INIT_TYPE INIT_FUNC(_observer)(void);
  INIT_TYPE INIT_FUNC(_geocal_quaternion)(void);
  INIT_TYPE INIT_FUNC(_covariance)(void);
  INIT_TYPE INIT_FUNC(_wgs84_constant)(void);
  INIT_TYPE INIT_FUNC(_geocal_exception)(void);
  INIT_TYPE INIT_FUNC(_functor)(void);
  INIT_TYPE INIT_FUNC(_vfunctor_with_derivative)(void);
  INIT_TYPE INIT_FUNC(_dfunctor_with_derivative)(void);
  INIT_TYPE INIT_FUNC(_statistic)(void);
  INIT_TYPE INIT_FUNC(_geocal_gsl_fit)(void);
  INIT_TYPE INIT_FUNC(_geocal_gsl_root)(void);
  INIT_TYPE INIT_FUNC(_geocal_time)(void);
  INIT_TYPE INIT_FUNC(_tiled_file)(void);
  INIT_TYPE INIT_FUNC(_geocal_serialize_function)(void);
  INIT_TYPE INIT_FUNC(_image_coordinate)(void);
  INIT_TYPE INIT_FUNC(_tie_point)(void);
  INIT_TYPE INIT_FUNC(_with_parameter)(void);
  INIT_TYPE INIT_FUNC(_look_vector)(void);
  INIT_TYPE INIT_FUNC(_ground_coordinate)(void);
  INIT_TYPE INIT_FUNC(_coordinate_converter)(void);
  INIT_TYPE INIT_FUNC(_ecr)(void);
  INIT_TYPE INIT_FUNC(_eci)(void);
  INIT_TYPE INIT_FUNC(_geodetic)(void);
  INIT_TYPE INIT_FUNC(_dem)(void);
  INIT_TYPE INIT_FUNC(_map_info)(void);
  INIT_TYPE INIT_FUNC(_geocal_rpc)(void);
  INIT_TYPE INIT_FUNC(_raster_image)(void);
  INIT_TYPE INIT_FUNC(_image_ground_connection)(void);
  INIT_TYPE INIT_FUNC(_igc_collection)(void);
  INIT_TYPE INIT_FUNC(_frame_coordinate)(void);
  INIT_TYPE INIT_FUNC(_camera)(void);
  INIT_TYPE INIT_FUNC(_ray_caster)(void);
  INIT_TYPE INIT_FUNC(_orbit)(void);
  INIT_TYPE INIT_FUNC(_time_table)(void);
  INIT_TYPE INIT_FUNC(_ipi)(void);
  INIT_TYPE INIT_FUNC(_raster_image_variable)(void);
  INIT_TYPE INIT_FUNC(_sub_raster_image)(void);
  INIT_TYPE INIT_FUNC(_raster_image_multi_band)(void);
  INIT_TYPE INIT_FUNC(_raster_image_multi_band_variable)(void);
  INIT_TYPE INIT_FUNC(_sub_raster_image_multi_band)(void);
  INIT_TYPE INIT_FUNC(_ground_mask)(void);
  INIT_TYPE INIT_FUNC(_image_mask)(void);
  INIT_TYPE INIT_FUNC(_geocal_datum)(void);
  INIT_TYPE INIT_FUNC(_simple_dem)(void);
  INIT_TYPE INIT_FUNC(_memory_raster_image)(void);
  INIT_TYPE INIT_FUNC(_constant_raster_image)(void);
  INIT_TYPE INIT_FUNC(_dem_map_info)(void);
  INIT_TYPE INIT_FUNC(_feature_detector)(void);
  INIT_TYPE INIT_FUNC(_forstner_feature_detector)(void);
  INIT_TYPE INIT_FUNC(_image_matcher)(void);
  INIT_TYPE INIT_FUNC(_image_to_image_match)(void);
  INIT_TYPE INIT_FUNC(_ccorr_matcher)(void);
  INIT_TYPE INIT_FUNC(_lsm_matcher)(void);
  INIT_TYPE INIT_FUNC(_ccorr_lsm_matcher)(void);
  INIT_TYPE INIT_FUNC(_geometric_model)(void);
  INIT_TYPE INIT_FUNC(_raster_multifile)(void);
  INIT_TYPE INIT_FUNC(_did_datum)(void);
  INIT_TYPE INIT_FUNC(_dem_map_info_offset)(void);
  INIT_TYPE INIT_FUNC(_map_info_image_ground_connection)(void);
  INIT_TYPE INIT_FUNC(_ground_mask_image)(void);
  INIT_TYPE INIT_FUNC(_image_mask_image)(void);
  INIT_TYPE INIT_FUNC(_raster_averaged)(void);
  INIT_TYPE INIT_FUNC(_raster_subsample)(void);
  INIT_TYPE INIT_FUNC(_calc_raster)(void);
  INIT_TYPE INIT_FUNC(_dem_to_raster)(void);
  INIT_TYPE INIT_FUNC(_calc_raster_multi_band)(void);
  INIT_TYPE INIT_FUNC(_magnify_bilinear)(void);
  INIT_TYPE INIT_FUNC(_magnify_replicate)(void);
  INIT_TYPE INIT_FUNC(_smooth_image)(void);
  INIT_TYPE INIT_FUNC(_raster_image_tiled_file)(void);
  INIT_TYPE INIT_FUNC(_dem_tiled_file)(void);
  INIT_TYPE INIT_FUNC(_memory_dem)(void);
  INIT_TYPE INIT_FUNC(_map_reprojected_image)(void);
  INIT_TYPE INIT_FUNC(_pyramid_image_matcher)(void);
  INIT_TYPE INIT_FUNC(_igc_map_projected)(void);
  INIT_TYPE INIT_FUNC(_igc_simulated)(void);
  INIT_TYPE INIT_FUNC(_igc_simulated_ray_caster)(void);
  INIT_TYPE INIT_FUNC(_igc_image_to_image_match)(void);
  INIT_TYPE INIT_FUNC(_surface_image_to_image_match)(void);
  INIT_TYPE INIT_FUNC(_location_to_file)(void);
  INIT_TYPE INIT_FUNC(_ray_intersect)(void);
  INIT_TYPE INIT_FUNC(_dem_match)(void);
  INIT_TYPE INIT_FUNC(_quadratic_geometric_model)(void);
  INIT_TYPE INIT_FUNC(_geometric_model_image)(void);
  INIT_TYPE INIT_FUNC(_memory_multi_band)(void);
  INIT_TYPE INIT_FUNC(_spice_helper)(void);
  INIT_TYPE INIT_FUNC(_raw_raster_image)(void);
  INIT_TYPE INIT_FUNC(_aircraft_orbit_data)(void);
  INIT_TYPE INIT_FUNC(_orbit_quaternion_list)(void);
  INIT_TYPE INIT_FUNC(_pan_sharpen)(void);
  INIT_TYPE INIT_FUNC(_scale_image)(void);
  INIT_TYPE INIT_FUNC(_apply_mask)(void);
  INIT_TYPE INIT_FUNC(_rpc_image_ground_connection)(void);
  INIT_TYPE INIT_FUNC(_ipi_image_ground_connection)(void);
  INIT_TYPE INIT_FUNC(_eci_tod)(void);
  INIT_TYPE INIT_FUNC(_quaternion_camera)(void);
  INIT_TYPE INIT_FUNC(_galileo_camera)(void);
  INIT_TYPE INIT_FUNC(_refraction)(void);
  INIT_TYPE INIT_FUNC(_orbit_data_image_ground_connection)(void);
  INIT_TYPE INIT_FUNC(_planet_coordinate)(void);
  INIT_TYPE INIT_FUNC(_igc_ray_caster)(void);
  INIT_TYPE INIT_FUNC(_igc_rolling_shutter)(void);
  INIT_TYPE INIT_FUNC(_igc_array)(void);
  INIT_TYPE INIT_FUNC(_igc_multiple_pass)(void);
  INIT_TYPE INIT_FUNC(_igc_collection_rolling_shutter)(void);
  INIT_TYPE INIT_FUNC(_igc_collection_orbit_data)(void);
  INIT_TYPE INIT_FUNC(_rolling_shutter_constant_time_table)(void);
  INIT_TYPE INIT_FUNC(_piecewise_linear)(void);
  INIT_TYPE INIT_FUNC(_orbit_correction)(void);
  INIT_TYPE INIT_FUNC(_orbit_offset_correction)(void);
  INIT_TYPE INIT_FUNC(_orbit_piecewise_correction)(void);
  INIT_TYPE INIT_FUNC(_orbit_time_shift)(void);
  INIT_TYPE INIT_FUNC(_tle_orbit)(void);
  INIT_TYPE INIT_FUNC(_spice_orbit)(void);
  INIT_TYPE INIT_FUNC(_argus_camera)(void);
  INIT_TYPE INIT_FUNC(_argus_orbit)(void);
  INIT_TYPE INIT_FUNC(_quickbird_camera)(void);
  INIT_TYPE INIT_FUNC(_spot_camera)(void);
  INIT_TYPE INIT_FUNC(_spot_orbit)(void);
  INIT_TYPE INIT_FUNC(_quickbird_orbit)(void);
  INIT_TYPE INIT_FUNC(_quickbird_time_table)(void);
  INIT_TYPE INIT_FUNC(_pos_export_orbit)(void);
  INIT_TYPE INIT_FUNC(_worldview2_cloudmask)(void);
  INIT_TYPE INIT_FUNC(_doughnut_average)(void);
  INIT_TYPE INIT_FUNC(_paint_class)(void);
  INIT_TYPE INIT_FUNC(_vicar_lite_file)(void);
  INIT_TYPE INIT_FUNC(_cart_lab_multifile)(void);
  INIT_TYPE INIT_FUNC(_landsat7_panchromatic)(void);
  INIT_TYPE INIT_FUNC(_material_detect)(void);
  INIT_TYPE INIT_FUNC(_ibis_file)(void);
  INIT_TYPE INIT_FUNC(_vicar_file)(void);
  INIT_TYPE INIT_FUNC(_vicar_argument)(void);
  INIT_TYPE INIT_FUNC(_vicar_dem)(void);
  INIT_TYPE INIT_FUNC(_datum_geoid96)(void);
  INIT_TYPE INIT_FUNC(_srtm_dem)(void);
  INIT_TYPE INIT_FUNC(_vicar_raster_image)(void);
  INIT_TYPE INIT_FUNC(_mspi_config_file)(void);
  INIT_TYPE INIT_FUNC(_mspi_paraxial_transform)(void);
  INIT_TYPE INIT_FUNC(_mspi_camera)(void);
  INIT_TYPE INIT_FUNC(_mspi_gimbal)(void);
  INIT_TYPE INIT_FUNC(_air_mspi_orbit)(void);
  INIT_TYPE INIT_FUNC(_ground_mspi_orbit)(void);
  INIT_TYPE INIT_FUNC(_ground_mspi_igc)(void);
  INIT_TYPE INIT_FUNC(_usgs_dem)(void);
  INIT_TYPE INIT_FUNC(_srtm_90m_dem)(void);
  INIT_TYPE INIT_FUNC(_air_mspi_file)(void);
  INIT_TYPE INIT_FUNC(_air_mspi_time_table)(void);
  INIT_TYPE INIT_FUNC(_air_mspi_l1b1)(void);
  INIT_TYPE INIT_FUNC(_air_mspi_igc)(void);
  INIT_TYPE INIT_FUNC(_air_mspi_igc_collection)(void);
  INIT_TYPE INIT_FUNC(_jia_sample)(void);
#ifdef HAVE_CARTO
  INIT_TYPE INIT_FUNC(_eci_tod_burl)(void);
#endif
#ifdef HAVE_FFTW
  INIT_TYPE INIT_FUNC(_geocal_fftw)(void);
  INIT_TYPE INIT_FUNC(_phase_correlation_matcher)(void);
#endif
#ifdef HAVE_HDF5
  INIT_TYPE INIT_FUNC(_hdf_file)(void);
  INIT_TYPE INIT_FUNC(_hdf_orbit)(void);
#endif
#ifdef HAVE_HDF5
#ifdef HAVE_CARTO
  INIT_TYPE INIT_FUNC(_hdf_orbit_carto)(void);
#endif
#endif
#ifdef HAVE_GDAL
  INIT_TYPE INIT_FUNC(_geocal_gdal)(void);
  INIT_TYPE INIT_FUNC(_gdal_raster_image)(void);
  INIT_TYPE INIT_FUNC(_gdal_datum)(void);
  INIT_TYPE INIT_FUNC(_gdal_dem)(void);
  INIT_TYPE INIT_FUNC(_gdal_multi_band)(void);
  INIT_TYPE INIT_FUNC(_image_point_display)(void);
  INIT_TYPE INIT_FUNC(_ogr_coordinate)(void);
  INIT_TYPE INIT_FUNC(_vicar_ogr)(void);
#endif
#ifdef HAVE_VICAR_RTL
#endif
#ifdef HAVE_GDAL_VICAR_RTL
#endif
}

static void module_init(PyObject* module)
{
  INIT_MODULE(module, "_swig_std", INIT_FUNC(_swig_std));
  INIT_MODULE(module, "_swig_array", INIT_FUNC(_swig_array));
  INIT_MODULE(module, "_swig_boost_array", INIT_FUNC(_swig_boost_array));
  INIT_MODULE(module, "_swig_quaternion", INIT_FUNC(_swig_quaternion));
  INIT_MODULE(module, "_constant", INIT_FUNC(_constant));
  INIT_MODULE(module, "_generic_object", INIT_FUNC(_generic_object));
  INIT_MODULE(module, "_auto_derivative", INIT_FUNC(_auto_derivative));
  INIT_MODULE(module, "_array_ad", INIT_FUNC(_array_ad));
  INIT_MODULE(module, "_observer", INIT_FUNC(_observer));
  INIT_MODULE(module, "_geocal_quaternion", INIT_FUNC(_geocal_quaternion));
  INIT_MODULE(module, "_covariance", INIT_FUNC(_covariance));
  INIT_MODULE(module, "_wgs84_constant", INIT_FUNC(_wgs84_constant));
  INIT_MODULE(module, "_geocal_exception", INIT_FUNC(_geocal_exception));
  INIT_MODULE(module, "_functor", INIT_FUNC(_functor));
  INIT_MODULE(module, "_vfunctor_with_derivative", INIT_FUNC(_vfunctor_with_derivative));
  INIT_MODULE(module, "_dfunctor_with_derivative", INIT_FUNC(_dfunctor_with_derivative));
  INIT_MODULE(module, "_statistic", INIT_FUNC(_statistic));
  INIT_MODULE(module, "_geocal_gsl_fit", INIT_FUNC(_geocal_gsl_fit));
  INIT_MODULE(module, "_geocal_gsl_root", INIT_FUNC(_geocal_gsl_root));
  INIT_MODULE(module, "_geocal_time", INIT_FUNC(_geocal_time));
  INIT_MODULE(module, "_tiled_file", INIT_FUNC(_tiled_file));
  INIT_MODULE(module, "_geocal_serialize_function", INIT_FUNC(_geocal_serialize_function));
  INIT_MODULE(module, "_image_coordinate", INIT_FUNC(_image_coordinate));
  INIT_MODULE(module, "_tie_point", INIT_FUNC(_tie_point));
  INIT_MODULE(module, "_with_parameter", INIT_FUNC(_with_parameter));
  INIT_MODULE(module, "_look_vector", INIT_FUNC(_look_vector));
  INIT_MODULE(module, "_ground_coordinate", INIT_FUNC(_ground_coordinate));
  INIT_MODULE(module, "_coordinate_converter", INIT_FUNC(_coordinate_converter));
  INIT_MODULE(module, "_ecr", INIT_FUNC(_ecr));
  INIT_MODULE(module, "_eci", INIT_FUNC(_eci));
  INIT_MODULE(module, "_geodetic", INIT_FUNC(_geodetic));
  INIT_MODULE(module, "_dem", INIT_FUNC(_dem));
  INIT_MODULE(module, "_map_info", INIT_FUNC(_map_info));
  INIT_MODULE(module, "_geocal_rpc", INIT_FUNC(_geocal_rpc));
  INIT_MODULE(module, "_raster_image", INIT_FUNC(_raster_image));
  INIT_MODULE(module, "_image_ground_connection", INIT_FUNC(_image_ground_connection));
  INIT_MODULE(module, "_igc_collection", INIT_FUNC(_igc_collection));
  INIT_MODULE(module, "_frame_coordinate", INIT_FUNC(_frame_coordinate));
  INIT_MODULE(module, "_camera", INIT_FUNC(_camera));
  INIT_MODULE(module, "_ray_caster", INIT_FUNC(_ray_caster));
  INIT_MODULE(module, "_orbit", INIT_FUNC(_orbit));
  INIT_MODULE(module, "_time_table", INIT_FUNC(_time_table));
  INIT_MODULE(module, "_ipi", INIT_FUNC(_ipi));
  INIT_MODULE(module, "_raster_image_variable", INIT_FUNC(_raster_image_variable));
  INIT_MODULE(module, "_sub_raster_image", INIT_FUNC(_sub_raster_image));
  INIT_MODULE(module, "_raster_image_multi_band", INIT_FUNC(_raster_image_multi_band));
  INIT_MODULE(module, "_raster_image_multi_band_variable", INIT_FUNC(_raster_image_multi_band_variable));
  INIT_MODULE(module, "_sub_raster_image_multi_band", INIT_FUNC(_sub_raster_image_multi_band));
  INIT_MODULE(module, "_ground_mask", INIT_FUNC(_ground_mask));
  INIT_MODULE(module, "_image_mask", INIT_FUNC(_image_mask));
  INIT_MODULE(module, "_geocal_datum", INIT_FUNC(_geocal_datum));
  INIT_MODULE(module, "_simple_dem", INIT_FUNC(_simple_dem));
  INIT_MODULE(module, "_memory_raster_image", INIT_FUNC(_memory_raster_image));
  INIT_MODULE(module, "_constant_raster_image", INIT_FUNC(_constant_raster_image));
  INIT_MODULE(module, "_dem_map_info", INIT_FUNC(_dem_map_info));
  INIT_MODULE(module, "_feature_detector", INIT_FUNC(_feature_detector));
  INIT_MODULE(module, "_forstner_feature_detector", INIT_FUNC(_forstner_feature_detector));
  INIT_MODULE(module, "_image_matcher", INIT_FUNC(_image_matcher));
  INIT_MODULE(module, "_image_to_image_match", INIT_FUNC(_image_to_image_match));
  INIT_MODULE(module, "_ccorr_matcher", INIT_FUNC(_ccorr_matcher));
  INIT_MODULE(module, "_lsm_matcher", INIT_FUNC(_lsm_matcher));
  INIT_MODULE(module, "_ccorr_lsm_matcher", INIT_FUNC(_ccorr_lsm_matcher));
  INIT_MODULE(module, "_geometric_model", INIT_FUNC(_geometric_model));
  INIT_MODULE(module, "_raster_multifile", INIT_FUNC(_raster_multifile));
  INIT_MODULE(module, "_did_datum", INIT_FUNC(_did_datum));
  INIT_MODULE(module, "_dem_map_info_offset", INIT_FUNC(_dem_map_info_offset));
  INIT_MODULE(module, "_map_info_image_ground_connection", INIT_FUNC(_map_info_image_ground_connection));
  INIT_MODULE(module, "_ground_mask_image", INIT_FUNC(_ground_mask_image));
  INIT_MODULE(module, "_image_mask_image", INIT_FUNC(_image_mask_image));
  INIT_MODULE(module, "_raster_averaged", INIT_FUNC(_raster_averaged));
  INIT_MODULE(module, "_raster_subsample", INIT_FUNC(_raster_subsample));
  INIT_MODULE(module, "_calc_raster", INIT_FUNC(_calc_raster));
  INIT_MODULE(module, "_dem_to_raster", INIT_FUNC(_dem_to_raster));
  INIT_MODULE(module, "_calc_raster_multi_band", INIT_FUNC(_calc_raster_multi_band));
  INIT_MODULE(module, "_magnify_bilinear", INIT_FUNC(_magnify_bilinear));
  INIT_MODULE(module, "_magnify_replicate", INIT_FUNC(_magnify_replicate));
  INIT_MODULE(module, "_smooth_image", INIT_FUNC(_smooth_image));
  INIT_MODULE(module, "_raster_image_tiled_file", INIT_FUNC(_raster_image_tiled_file));
  INIT_MODULE(module, "_dem_tiled_file", INIT_FUNC(_dem_tiled_file));
  INIT_MODULE(module, "_memory_dem", INIT_FUNC(_memory_dem));
  INIT_MODULE(module, "_map_reprojected_image", INIT_FUNC(_map_reprojected_image));
  INIT_MODULE(module, "_pyramid_image_matcher", INIT_FUNC(_pyramid_image_matcher));
  INIT_MODULE(module, "_igc_map_projected", INIT_FUNC(_igc_map_projected));
  INIT_MODULE(module, "_igc_simulated", INIT_FUNC(_igc_simulated));
  INIT_MODULE(module, "_igc_simulated_ray_caster", INIT_FUNC(_igc_simulated_ray_caster));
  INIT_MODULE(module, "_igc_image_to_image_match", INIT_FUNC(_igc_image_to_image_match));
  INIT_MODULE(module, "_surface_image_to_image_match", INIT_FUNC(_surface_image_to_image_match));
  INIT_MODULE(module, "_location_to_file", INIT_FUNC(_location_to_file));
  INIT_MODULE(module, "_ray_intersect", INIT_FUNC(_ray_intersect));
  INIT_MODULE(module, "_dem_match", INIT_FUNC(_dem_match));
  INIT_MODULE(module, "_quadratic_geometric_model", INIT_FUNC(_quadratic_geometric_model));
  INIT_MODULE(module, "_geometric_model_image", INIT_FUNC(_geometric_model_image));
  INIT_MODULE(module, "_memory_multi_band", INIT_FUNC(_memory_multi_band));
  INIT_MODULE(module, "_spice_helper", INIT_FUNC(_spice_helper));
  INIT_MODULE(module, "_raw_raster_image", INIT_FUNC(_raw_raster_image));
  INIT_MODULE(module, "_aircraft_orbit_data", INIT_FUNC(_aircraft_orbit_data));
  INIT_MODULE(module, "_orbit_quaternion_list", INIT_FUNC(_orbit_quaternion_list));
  INIT_MODULE(module, "_pan_sharpen", INIT_FUNC(_pan_sharpen));
  INIT_MODULE(module, "_scale_image", INIT_FUNC(_scale_image));
  INIT_MODULE(module, "_apply_mask", INIT_FUNC(_apply_mask));
  INIT_MODULE(module, "_rpc_image_ground_connection", INIT_FUNC(_rpc_image_ground_connection));
  INIT_MODULE(module, "_ipi_image_ground_connection", INIT_FUNC(_ipi_image_ground_connection));
  INIT_MODULE(module, "_eci_tod", INIT_FUNC(_eci_tod));
  INIT_MODULE(module, "_quaternion_camera", INIT_FUNC(_quaternion_camera));
  INIT_MODULE(module, "_galileo_camera", INIT_FUNC(_galileo_camera));
  INIT_MODULE(module, "_refraction", INIT_FUNC(_refraction));
  INIT_MODULE(module, "_orbit_data_image_ground_connection", INIT_FUNC(_orbit_data_image_ground_connection));
  INIT_MODULE(module, "_planet_coordinate", INIT_FUNC(_planet_coordinate));
  INIT_MODULE(module, "_igc_ray_caster", INIT_FUNC(_igc_ray_caster));
  INIT_MODULE(module, "_igc_rolling_shutter", INIT_FUNC(_igc_rolling_shutter));
  INIT_MODULE(module, "_igc_array", INIT_FUNC(_igc_array));
  INIT_MODULE(module, "_igc_multiple_pass", INIT_FUNC(_igc_multiple_pass));
  INIT_MODULE(module, "_igc_collection_rolling_shutter", INIT_FUNC(_igc_collection_rolling_shutter));
  INIT_MODULE(module, "_igc_collection_orbit_data", INIT_FUNC(_igc_collection_orbit_data));
  INIT_MODULE(module, "_rolling_shutter_constant_time_table", INIT_FUNC(_rolling_shutter_constant_time_table));
  INIT_MODULE(module, "_piecewise_linear", INIT_FUNC(_piecewise_linear));
  INIT_MODULE(module, "_orbit_correction", INIT_FUNC(_orbit_correction));
  INIT_MODULE(module, "_orbit_offset_correction", INIT_FUNC(_orbit_offset_correction));
  INIT_MODULE(module, "_orbit_piecewise_correction", INIT_FUNC(_orbit_piecewise_correction));
  INIT_MODULE(module, "_orbit_time_shift", INIT_FUNC(_orbit_time_shift));
  INIT_MODULE(module, "_tle_orbit", INIT_FUNC(_tle_orbit));
  INIT_MODULE(module, "_spice_orbit", INIT_FUNC(_spice_orbit));
  INIT_MODULE(module, "_argus_camera", INIT_FUNC(_argus_camera));
  INIT_MODULE(module, "_argus_orbit", INIT_FUNC(_argus_orbit));
  INIT_MODULE(module, "_quickbird_camera", INIT_FUNC(_quickbird_camera));
  INIT_MODULE(module, "_spot_camera", INIT_FUNC(_spot_camera));
  INIT_MODULE(module, "_spot_orbit", INIT_FUNC(_spot_orbit));
  INIT_MODULE(module, "_quickbird_orbit", INIT_FUNC(_quickbird_orbit));
  INIT_MODULE(module, "_quickbird_time_table", INIT_FUNC(_quickbird_time_table));
  INIT_MODULE(module, "_pos_export_orbit", INIT_FUNC(_pos_export_orbit));
  INIT_MODULE(module, "_worldview2_cloudmask", INIT_FUNC(_worldview2_cloudmask));
  INIT_MODULE(module, "_doughnut_average", INIT_FUNC(_doughnut_average));
  INIT_MODULE(module, "_paint_class", INIT_FUNC(_paint_class));
  INIT_MODULE(module, "_vicar_lite_file", INIT_FUNC(_vicar_lite_file));
  INIT_MODULE(module, "_cart_lab_multifile", INIT_FUNC(_cart_lab_multifile));
  INIT_MODULE(module, "_landsat7_panchromatic", INIT_FUNC(_landsat7_panchromatic));
  INIT_MODULE(module, "_material_detect", INIT_FUNC(_material_detect));
  INIT_MODULE(module, "_ibis_file", INIT_FUNC(_ibis_file));
  INIT_MODULE(module, "_vicar_file", INIT_FUNC(_vicar_file));
  INIT_MODULE(module, "_vicar_argument", INIT_FUNC(_vicar_argument));
  INIT_MODULE(module, "_vicar_dem", INIT_FUNC(_vicar_dem));
  INIT_MODULE(module, "_datum_geoid96", INIT_FUNC(_datum_geoid96));
  INIT_MODULE(module, "_srtm_dem", INIT_FUNC(_srtm_dem));
  INIT_MODULE(module, "_vicar_raster_image", INIT_FUNC(_vicar_raster_image));
  INIT_MODULE(module, "_mspi_config_file", INIT_FUNC(_mspi_config_file));
  INIT_MODULE(module, "_mspi_paraxial_transform", INIT_FUNC(_mspi_paraxial_transform));
  INIT_MODULE(module, "_mspi_camera", INIT_FUNC(_mspi_camera));
  INIT_MODULE(module, "_mspi_gimbal", INIT_FUNC(_mspi_gimbal));
  INIT_MODULE(module, "_air_mspi_orbit", INIT_FUNC(_air_mspi_orbit));
  INIT_MODULE(module, "_ground_mspi_orbit", INIT_FUNC(_ground_mspi_orbit));
  INIT_MODULE(module, "_ground_mspi_igc", INIT_FUNC(_ground_mspi_igc));
  INIT_MODULE(module, "_usgs_dem", INIT_FUNC(_usgs_dem));
  INIT_MODULE(module, "_srtm_90m_dem", INIT_FUNC(_srtm_90m_dem));
  INIT_MODULE(module, "_air_mspi_file", INIT_FUNC(_air_mspi_file));
  INIT_MODULE(module, "_air_mspi_time_table", INIT_FUNC(_air_mspi_time_table));
  INIT_MODULE(module, "_air_mspi_l1b1", INIT_FUNC(_air_mspi_l1b1));
  INIT_MODULE(module, "_air_mspi_igc", INIT_FUNC(_air_mspi_igc));
  INIT_MODULE(module, "_air_mspi_igc_collection", INIT_FUNC(_air_mspi_igc_collection));
  INIT_MODULE(module, "_jia_sample", INIT_FUNC(_jia_sample));
#ifdef HAVE_CARTO
  INIT_MODULE(module, "_eci_tod_burl", INIT_FUNC(_eci_tod_burl));
#endif
#ifdef HAVE_FFTW
  INIT_MODULE(module, "_geocal_fftw", INIT_FUNC(_geocal_fftw));
  INIT_MODULE(module, "_phase_correlation_matcher", INIT_FUNC(_phase_correlation_matcher));
#endif
#ifdef HAVE_HDF5
  INIT_MODULE(module, "_hdf_file", INIT_FUNC(_hdf_file));
  INIT_MODULE(module, "_hdf_orbit", INIT_FUNC(_hdf_orbit));
#endif
#ifdef HAVE_HDF5
#ifdef HAVE_CARTO
  INIT_MODULE(module, "_hdf_orbit_carto", INIT_FUNC(_hdf_orbit_carto));
#endif
#endif
#ifdef HAVE_GDAL
  INIT_MODULE(module, "_geocal_gdal", INIT_FUNC(_geocal_gdal));
  INIT_MODULE(module, "_gdal_raster_image", INIT_FUNC(_gdal_raster_image));
  INIT_MODULE(module, "_gdal_datum", INIT_FUNC(_gdal_datum));
  INIT_MODULE(module, "_gdal_dem", INIT_FUNC(_gdal_dem));
  INIT_MODULE(module, "_gdal_multi_band", INIT_FUNC(_gdal_multi_band));
  INIT_MODULE(module, "_image_point_display", INIT_FUNC(_image_point_display));
  INIT_MODULE(module, "_ogr_coordinate", INIT_FUNC(_ogr_coordinate));
  INIT_MODULE(module, "_vicar_ogr", INIT_FUNC(_vicar_ogr));
#endif
#ifdef HAVE_VICAR_RTL
#endif
#ifdef HAVE_GDAL_VICAR_RTL
#endif
}


