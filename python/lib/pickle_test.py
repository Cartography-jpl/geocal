# This contains various tests to make sure pickling of low level GeoCal objects
# work correctly.
import cPickle
from geocal_swig import *
from nose.tools import *
from nose.plugins.skip import Skip, SkipTest

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"

# Simple test to see if we have AFIDS data available. We check for the
# presence of one of the AFIDS environment variables, and if there
# assume we have the data
have_afid_data = "AFIDS_VDEV_DATA" in os.environ 

# Test picking of Time
def test_time_pickle():
    tm = Time.time_j2000(100.0)
    t = cPickle.dumps(tm, cPickle.HIGHEST_PROTOCOL)
    tm2 = cPickle.loads(t)
    assert tm == tm2

# Test pickling of ImageCoordinate
def test_image_coordinate_pickle():
    ic = ImageCoordinate(1, 2)
    t = cPickle.dumps(ic, cPickle.HIGHEST_PROTOCOL)
    x = cPickle.loads(t)
    assert_almost_equal(ic.line, x.line)
    assert_almost_equal(ic.sample, x.sample)

# Test pickling of RPC
def test_rpc_pickle():
    # A sample RPC. Nothing special about this, these are just reasonable
    # values
    rpc = Rpc()
    rpc.rpc_type = Rpc.RPC_B
    rpc.line_offset = 2881
    rpc.line_scale = 2921
    rpc.sample_offset = 13763
    rpc.sample_scale = 14238
    rpc.latitude_offset = 35.8606
    rpc.latitude_scale = 0.0209
    rpc.longitude_offset = 44.9534
    rpc.longitude_scale = 0.1239
    rpc.height_offset = 1017
    rpc.height_scale = 634
    rpc.line_numerator = [0.0132748, -0.14751, -1.13465,
                          -0.0138959, 0.0020018,
                          6.35242e-05, 0.000115861, -0.00286551, -0.00778726, 
                          3.88308e-06, -1.26487e-06, 7.881069999999999e-06, 
                          3.65929e-05, 2.32154e-06, -2.25421e-05, -2.08933e-05, 
                          1.8091e-05, 3.6393e-07, -9.39815e-07, -4.31269e-08]
    rpc.line_denominator = [1, 0.00380256, 0.00643151, 0.00031479,
      1.84958e-05, -1.98243e-06, -1.16422e-06,
      -1.92637e-05, 7.224010000000001e-05, -1.61779e-05,
      4.95049e-06, 1.26794e-06, 0.000190771, -1.57429e-07,
      2.46815e-05, 0.00049166, -5.41022e-07, 3.209e-07,
       1.81401e-05, 1.43592e-07]
    rpc.sample_numerator = [-0.0104025, 0.96885, -0.000487887, -0.0325142,
      -0.000710444, 0.000217572, -6.549690000000001e-05,
      0.0107384, -5.19453e-06, -1.76761e-05, -1.21058e-06,
      0.000106017, 5.41226e-06, -3.8968e-06, 1.03307e-05,
      5.84016e-05, 3.80777e-08, 9.01765e-06, 1.65988e-06,
       -1.19374e-07]
    rpc.sample_denominator = [1, -0.000319831, 0.000681092, -0.000549762,
     -2.67977e-06, -6.19388e-06, 2.67975e-08, 4.76371e-06,
     -1.47985e-05, -4.23457e-06, 1.44009e-08, -1.07213e-06,
     1.1059e-07, 4.10217e-08, -1.69482e-07, 1.08104e-06,
      1e-9, -2.33038e-07, 1.86413e-08, -1.35637e-08]
    t = cPickle.dumps(rpc, cPickle.HIGHEST_PROTOCOL)
    x = cPickle.loads(t)
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
        assert_almost_equal(rpc.fit_line_numerator[i], x.fit_line_numerator[i])
        assert_almost_equal(rpc.fit_sample_numerator[i],
                            x.fit_sample_numerator[i])

def test_quaternion_camera_pickle():
    '''Test pickling of QuaternionCamera'''
    cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                           3375, 3648,
                           1.0 / 2500000,
                           1.0 / 2500000,
                           1.0,
                           FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    t = cPickle.dumps(cam, cPickle.HIGHEST_PROTOCOL)
    cam2 = cPickle.loads(t)
    assert_almost_equal(cam.focal_length, 1.0, 4)
    assert_almost_equal(cam.line_pitch,   1.0 / 2500000, 4)
    assert_almost_equal(cam.sample_pitch,   1.0 / 2500000, 4)
    assert_almost_equal(cam.principal_point(0).line, 1688, 4)
    assert_almost_equal(cam.principal_point(0).sample, 1824.5, 4)
    assert cam.number_line(0) == 3375
    assert cam.number_sample(0) == 3648
    assert cam.number_band == 1

def test_quaternion_pickle():
    '''Make sure we can pickle quaternions'''
    t = cPickle.dumps(Quaternion_double(1, 2, 3, 4), cPickle.HIGHEST_PROTOCOL)
    q = cPickle.loads(t)
    assert_almost_equal(q.R_component_1, 1, 4)
    assert_almost_equal(q.R_component_2, 2, 4)
    assert_almost_equal(q.R_component_3, 3, 4)
    assert_almost_equal(q.R_component_4, 4, 4)

def create_rpc_sample():
    '''Small routine to create a sample RPC that can be used elsewhere. We
    only run this when we need to update the file'''
    rpc = Rpc()
    rpc.rpc_type = Rpc.RPC_B
    rpc.error_bias = 0
    rpc.error_random = 0
    rpc.line_offset = 2881
    rpc.line_scale = 2921
    rpc.sample_offset = 13763
    rpc.sample_scale = 14238
    rpc.latitude_offset = 35.8606
    rpc.latitude_scale = 0.0209
    rpc.longitude_offset = 44.9534
    rpc.longitude_scale = 0.1239
    rpc.height_offset = 1017
    rpc.height_scale = 634
    rpc.line_numerator = [0.0132748, -0.14751, -1.13465,
                          -0.0138959, 0.0020018,
                          6.35242e-05, 0.000115861, -0.00286551, -0.00778726, 
                          3.88308e-06, -1.26487e-06, 7.881069999999999e-06, 
                          3.65929e-05, 2.32154e-06, -2.25421e-05, -2.08933e-05, 
                          1.8091e-05, 3.6393e-07, -9.39815e-07, -4.31269e-08]
    rpc.line_denominator = [1, 0.00380256, 0.00643151, 0.00031479,
      1.84958e-05, -1.98243e-06, -1.16422e-06,
      -1.92637e-05, 7.224010000000001e-05, -1.61779e-05,
      4.95049e-06, 1.26794e-06, 0.000190771, -1.57429e-07,
      2.46815e-05, 0.00049166, -5.41022e-07, 3.209e-07,
       1.81401e-05, 1.43592e-07]
    rpc.sample_numerator = [-0.0104025, 0.96885, -0.000487887, -0.0325142,
      -0.000710444, 0.000217572, -6.549690000000001e-05,
      0.0107384, -5.19453e-06, -1.76761e-05, -1.21058e-06,
      0.000106017, 5.41226e-06, -3.8968e-06, 1.03307e-05,
      5.84016e-05, 3.80777e-08, 9.01765e-06, 1.65988e-06,
       -1.19374e-07]
    rpc.sample_denominator = [1, -0.000319831, 0.000681092, -0.000549762,
     -2.67977e-06, -6.19388e-06, 2.67975e-08, 4.76371e-06,
     -1.47985e-05, -4.23457e-06, 1.44009e-08, -1.07213e-06,
     1.1059e-07, 4.10217e-08, -1.69482e-07, 1.08104e-06,
      1e-9, -2.33038e-07, 1.86413e-08, -1.35637e-08]
    out = open(test_data + "rpc_example.pkl", "wb")
    t = cPickle.dump(rpc, out, cPickle.HIGHEST_PROTOCOL)

#create_rpc_sample()

def test_mapinfo_pickle():
    r = GdalRasterImage(test_data + "dem_foot.tif")
    m = r.map_info
    cPickle.dumps(m.transform)
    cPickle.dumps(m.number_x_pixel)
    cPickle.dumps(m.number_y_pixel)
    cPickle.dumps(m.coordinate_converter)
    t = cPickle.dumps(m, cPickle.HIGHEST_PROTOCOL)
    m2 = cPickle.loads(t)
    assert m2.number_x_pixel == m.number_x_pixel
    assert m2.number_y_pixel == m.number_y_pixel
    assert distance(m2.ground_coordinate(0, 0), 
                    m.ground_coordinate(0,0)) < 0.01
    assert distance(m2.ground_coordinate(10, 10), 
                    m.ground_coordinate(10,10)) < 0.01

# Test pickle of VicarLiteFile
def test_vicar_lite_file():
    f = VicarLiteFile(test_data + "vicar.img")
    t = cPickle.dumps(f)
    x = cPickle.loads(t)
    assert f.file_name == x.file_name

# Test pickle of VicarLiteRasterImage
def test_vicar_lite_raster_image():
    f = VicarLiteRasterImage(test_data + "vicar.img")
    t = cPickle.dumps(f)
    x = cPickle.loads(t)
    assert f.file.file_name == x.file.file_name

# Test pickle of VicarLiteDem
def test_vicar_lite_dem():
    f = VicarLiteDem(test_data + "vicar.img")
    t = cPickle.dumps(f)
    x = cPickle.loads(t)
    assert f.file.file_name == x.file.file_name

# Test pickle of VicarRasterImage
def test_vicar_raster_image():
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        VicarRasterImage
    except NameError:
        raise SkipTest
    f = VicarRasterImage(test_data + "vicar.img")
    t = cPickle.dumps(f)
    x = cPickle.loads(t)
    assert f.vicar_file.file_name == x.vicar_file.file_name

def test_srtm():
    if(not have_afid_data):
        raise SkipTest
    try:
        SrtmDem
    except NameError:
        raise SkipTest
    dem = SrtmDem()
    t = cPickle.dumps(dem)
    dem2 = cPickle.loads(t)
    assert dem.database_name == dem2.database_name
    assert dem.directory_base == dem2.directory_base

def test_gdal_raster_image():
    r = GdalRasterImage(test_data + "rpc.ntf")
    t = cPickle.dumps(r)
    r2 = cPickle.loads(t)
    assert r.number_line == r2.number_line
    assert r.number_sample == r2.number_sample
    assert r.number_tile_line == r2.number_tile_line
    assert r.number_tile_sample == r2.number_tile_sample

def test_simple_dem():
    d = SimpleDem(10.0)
    t = cPickle.dumps(d)
    d2 = cPickle.loads(t)
    assert_almost_equal(d.h, d2.h)

def test_forstner_feature_detector():
    f = ForstnerFeatureDetector()
    t = cPickle.dumps(f)
    f2 = cPickle.loads(t)
    assert f.basic_ip_large_number == f2.basic_ip_large_number
    assert f.maximum_weight_threshold == f2.maximum_weight_threshold
    assert f.forstner_max_window_large_size == f2.forstner_max_window_large_size
    assert f.forstner_max_window_small_size == f2.forstner_max_window_small_size
    assert f.forstner_window_size == f2.forstner_window_size
    assert_almost_equal(f.roundness_threshold, f2.roundness_threshold)
    assert_almost_equal(f.weight_threshold, f2.weight_threshold)
    assert_almost_equal(f.basic_robert_threshold, f2.basic_robert_threshold)
    
def test_gdal_dem():
    # Datum is nonsense, but we want to make sure it gets restored correctly
    d = GdalDem(test_data + "dem_foot.tif", SimpleDatum(10.0))
    t = cPickle.dumps(d)
    d2 = cPickle.loads(t)
    assert d.file_name == d2.file_name
    assert d.band_id == d2.band_id
    assert_almost_equal(d.datum.u, d2.datum.u)
    assert d.outside_dem_is_error == d2.outside_dem_is_error
    assert d.number_tile == d2.number_tile
    
def test_ipi():
    tmin = Time.parse_time("2003-01-01T11:00:00Z")
    tmax = tmin + 10000 * 40.8e-3
    orb = KeplerOrbit()
    cam = SimpleCamera()
    tt = ConstantSpacingTimeTable(tmin, tmax)
    ipi = Ipi(orb, cam, 0, tmin, tmax, tt)
    t = cPickle.dumps(ipi.orbit)
    # t = cPickle.dumps(ipi.camera)
    t = cPickle.dumps(ipi.band)
    t = cPickle.dumps(ipi.min_time)
    t = cPickle.dumps(ipi.max_time)
    # t = cPickle.dumps(ipi.time_table)
    t = cPickle.dumps(ipi.local_time_window_size)
    t = cPickle.dumps(ipi.root_min_separation)
    t = cPickle.dumps(ipi.time_tolerance)
    t = cPickle.dumps(ipi)
    ipi2 = cPickle.loads(t)

def test_simple_camera():
    c = SimpleCamera()
    t = cPickle.dumps(c)
    c2 = cPickle.loads(t)
    assert_almost_equal(c.beta, c2.beta)
    assert_almost_equal(c.delta, c2.delta)
    assert_almost_equal(c.epsilon, c2.epsilon)
    assert_almost_equal(c.focal_length, c2.focal_length)
    assert_almost_equal(c.line_pitch, c2.line_pitch)
    assert_almost_equal(c.sample_pitch, c2.sample_pitch)
    assert_almost_equal(c.number_sample(0), c2.number_sample(0))

def test_kepler_orbit():
    orb = KeplerOrbit()
    t = cPickle.dumps(orb)
    orb2 = cPickle.loads(t)
    assert orb.min_time == orb2.min_time
    assert orb.max_time == orb2.max_time
    assert orb.epoch == orb2.epoch
    assert_almost_equal(orb.semimajor_axis, orb2.semimajor_axis)
    assert_almost_equal(orb.eccentricity, orb2.eccentricity)
    assert_almost_equal(orb.inclination, orb2.inclination)
    assert_almost_equal(orb.right_ascension, orb2.right_ascension)
    assert_almost_equal(orb.argument_of_perigee, orb2.argument_of_perigee)
    assert_almost_equal(orb.mean_anomoly, orb2.mean_anomoly)
