from builtins import str
from builtins import range
# This contains various tests to make sure pickling of low level GeoCal objects
# work correctly.
import pickle
from test_support import *

# Simple test to see if we have AFIDS data available. We check for the
# presence of one of the AFIDS environment variables, and if there
# assume we have the data
have_afid_data = "AFIDS_VDEV_DATA" in os.environ 

# Test picking of Time
@require_serialize    
def test_time_pickle():
    tm = Time.time_j2000(100.0)
    t = pickle.dumps(tm, pickle.HIGHEST_PROTOCOL)
    tm2 = pickle.loads(t)
    assert tm == tm2

# Test pickling of ImageCoordinate
@require_serialize    
def test_image_coordinate_pickle():
    ic = ImageCoordinate(1, 2)
    t = pickle.dumps(ic, pickle.HIGHEST_PROTOCOL)
    x = pickle.loads(t)
    assert_almost_equal(ic.line, x.line)
    assert_almost_equal(ic.sample, x.sample)

@require_serialize    
def test_rpc_pickle(rpc):
    '''Test pickling of RPC'''
    t = pickle.dumps(rpc, pickle.HIGHEST_PROTOCOL)
    x = pickle.loads(t)
    assert str(rpc) == str(x)
    assert_almost_equal(rpc.height_offset, x.height_offset)
    assert_almost_equal(rpc.height_scale, x.height_scale)
    assert_almost_equal(rpc.latitude_offset, x.latitude_offset)
    assert_almost_equal(rpc.latitude_scale, x.latitude_scale)
    assert_almost_equal(rpc.longitude_offset, x.longitude_offset)
    assert_almost_equal(rpc.longitude_scale, x.longitude_scale)
    assert_almost_equal(rpc.line_offset, x.line_offset)
    assert_almost_equal(rpc.line_scale, x.line_scale)
    assert_almost_equal(rpc.sample_offset, x.sample_offset)
    assert_almost_equal(rpc.sample_scale, x.sample_scale)
    for i in range(20):
        assert_almost_equal(rpc.line_denominator[i], x.line_denominator[i])
        assert_almost_equal(rpc.line_numerator[i], x.line_numerator[i])
        assert_almost_equal(rpc.sample_denominator[i], x.sample_denominator[i])
        assert_almost_equal(rpc.sample_numerator[i], x.sample_numerator[i])
        assert rpc.fit_line_numerator[i] == x.fit_line_numerator[i]
        assert rpc.fit_sample_numerator[i] == x.fit_sample_numerator[i]

@require_serialize    
def test_quaternion_camera_pickle():
    '''Test pickling of QuaternionCamera'''
    cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                           3375, 3648,
                           1.0 / 2500000,
                           1.0 / 2500000,
                           1.0,
                           FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    t = pickle.dumps(cam, pickle.HIGHEST_PROTOCOL)
    cam2 = pickle.loads(t)
    assert_almost_equal(cam.focal_length, 1.0, 4)
    assert_almost_equal(cam.line_pitch, 1.0 / 2500000, 4)
    assert_almost_equal(cam.sample_pitch, 1.0 / 2500000, 4)
    assert_almost_equal(cam.principal_point(0).line, 1688, 4)
    assert_almost_equal(cam.principal_point(0).sample, 1824.5, 4)
    assert cam.number_line(0) == 3375
    assert cam.number_sample(0) == 3648
    assert cam.number_band == 1

@require_serialize    
def test_quaternion_pickle():
    '''Make sure we can pickle quaternions'''
    t = pickle.dumps(Quaternion_double(1, 2, 3, 4), pickle.HIGHEST_PROTOCOL)
    q = pickle.loads(t)
    assert_almost_equal(q.R_component_1, 1, 4)
    assert_almost_equal(q.R_component_2, 2, 4)
    assert_almost_equal(q.R_component_3, 3, 4)
    assert_almost_equal(q.R_component_4, 4, 4)

def create_rpc_sample(rpc):
    '''Small routine to create a sample RPC that can be used elsewhere. We
    only run this when we need to update the file'''
    out = open(unit_test_data + "rpc_example.pkl", "wb")
    # Use protocol 0, since it is the most portable. This is just a small test
    # example, so size/efficiency isn't any kind of an issue.
    t = pickle.dump(rpc, out, 0)

#create_rpc_sample()

@require_serialize    
def test_mapinfo_pickle():
    r = GdalRasterImage(unit_test_data + "dem_foot.tif")
    m = r.map_info
    pickle.dumps(m.transform)
    pickle.dumps(m.number_x_pixel)
    pickle.dumps(m.number_y_pixel)
    pickle.dumps(m.coordinate_converter)
    t = pickle.dumps(m, pickle.HIGHEST_PROTOCOL)
    m2 = pickle.loads(t)
    assert m2.number_x_pixel == m.number_x_pixel
    assert m2.number_y_pixel == m.number_y_pixel
    assert distance(m2.ground_coordinate(0, 0), 
                    m.ground_coordinate(0,0)) < 0.01
    assert distance(m2.ground_coordinate(10, 10), 
                    m.ground_coordinate(10,10)) < 0.01

# Test pickle of VicarLiteFile
@require_serialize    
def test_vicar_lite_file():
    f = VicarLiteFile(unit_test_data + "vicar.img")
    t = pickle.dumps(f)
    x = pickle.loads(t)
    assert f.file_name == x.file_name

# Test pickle of VicarLiteRasterImage
@require_serialize    
def test_vicar_lite_raster_image():
    f = VicarLiteRasterImage(unit_test_data + "vicar.img")
    t = pickle.dumps(f)
    x = pickle.loads(t)
    assert f.file.file_name == x.file.file_name

# Test pickle of VicarLiteDem
@require_serialize    
def test_vicar_lite_dem():
    f = VicarLiteDem(unit_test_data + "vicar.img")
    t = pickle.dumps(f)
    x = pickle.loads(t)
    assert f.file.file_name == x.file.file_name

# Test pickle of VicarRasterImage
@require_serialize
@require_vicar
def test_vicar_raster_image():
    f = VicarRasterImage(unit_test_data + "vicar.img")
    t = pickle.dumps(f)
    x = pickle.loads(t)
    assert f.vicar_file.file_name == x.vicar_file.file_name

@require_serialize    
@require_afids_data
@require_srtm_data
@require_vicar
def test_srtm():
    dem = SrtmDem()
    t = pickle.dumps(dem)
    dem2 = pickle.loads(t)
    assert dem.directory_base == dem2.directory_base

@require_serialize    
def test_gdal_raster_image():
    r = GdalRasterImage(unit_test_data + "rpc.ntf")
    t = pickle.dumps(r)
    r2 = pickle.loads(t)
    assert r.number_line == r2.number_line
    assert r.number_sample == r2.number_sample
    assert r.number_tile_line == r2.number_tile_line
    assert r.number_tile_sample == r2.number_tile_sample

@require_serialize    
def test_simple_dem():
    d = SimpleDem(10.0)
    t = pickle.dumps(d)
    d2 = pickle.loads(t)
    assert_almost_equal(d.h, d2.h)

@require_serialize    
def test_forstner_feature_detector():
    f = ForstnerFeatureDetector()
    t = pickle.dumps(f)
    f2 = pickle.loads(t)
    assert f.basic_ip_large_number == f2.basic_ip_large_number
    assert f.maximum_weight_threshold == f2.maximum_weight_threshold
    assert f.forstner_max_window_large_size == f2.forstner_max_window_large_size
    assert f.forstner_max_window_small_size == f2.forstner_max_window_small_size
    assert f.forstner_window_size == f2.forstner_window_size
    assert_almost_equal(f.roundness_threshold, f2.roundness_threshold)
    assert_almost_equal(f.weight_threshold, f2.weight_threshold)
    assert_almost_equal(f.basic_robert_threshold, f2.basic_robert_threshold)
    
@require_serialize    
def test_gdal_dem():
    # Datum is nonsense, but we want to make sure it gets restored correctly
    d = GdalDem(unit_test_data + "dem_foot.tif", SimpleDatum(10.0))
    t = pickle.dumps(d)
    d2 = pickle.loads(t)
    assert d.file_name == d2.file_name
    assert d.band_id == d2.band_id
    assert_almost_equal(d.datum.u, d2.datum.u)
    assert d.outside_dem_is_error == d2.outside_dem_is_error
    assert d.number_tile == d2.number_tile
    
@require_serialize    
def test_ipi():
    tmin = Time.parse_time("2003-01-01T11:00:00Z")
    tmax = tmin + 10000 * 40.8e-3
    orb = KeplerOrbit()
    cam = SimpleCamera()
    tt = ConstantSpacingTimeTable(tmin, tmax)
    ipi = Ipi(orb, cam, 0, tmin, tmax, tt)
    t = pickle.dumps(ipi.orbit)
    # t = cPickle.dumps(ipi.camera)
    t = pickle.dumps(ipi.band)
    t = pickle.dumps(ipi.min_time)
    t = pickle.dumps(ipi.max_time)
    # t = cPickle.dumps(ipi.time_table)
    t = pickle.dumps(ipi.local_time_window_size)
    t = pickle.dumps(ipi.root_min_separation)
    t = pickle.dumps(ipi.time_tolerance)
    t = pickle.dumps(ipi)
    ipi2 = pickle.loads(t)

@require_serialize    
def test_simple_camera():
    c = SimpleCamera()
    t = pickle.dumps(c)
    c2 = pickle.loads(t)
    assert_almost_equal(c.beta, c2.beta)
    assert_almost_equal(c.delta, c2.delta)
    assert_almost_equal(c.epsilon, c2.epsilon)
    assert_almost_equal(c.focal_length, c2.focal_length)
    assert_almost_equal(c.line_pitch, c2.line_pitch)
    assert_almost_equal(c.sample_pitch, c2.sample_pitch)
    assert_almost_equal(c.number_sample(0), c2.number_sample(0))

@require_serialize    
def test_kepler_orbit():
    orb = KeplerOrbit()
    t = pickle.dumps(orb)
    orb2 = pickle.loads(t)
    assert orb.min_time == orb2.min_time
    assert orb.max_time == orb2.max_time
    assert orb.epoch == orb2.epoch
    assert_almost_equal(orb.semimajor_axis, orb2.semimajor_axis)
    assert_almost_equal(orb.eccentricity, orb2.eccentricity)
    assert_almost_equal(orb.inclination, orb2.inclination)
    assert_almost_equal(orb.right_ascension, orb2.right_ascension)
    assert_almost_equal(orb.argument_of_perigee, orb2.argument_of_perigee)
    assert_almost_equal(orb.mean_anomoly, orb2.mean_anomoly)
