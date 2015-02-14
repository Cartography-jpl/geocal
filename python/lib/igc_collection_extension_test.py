from nose.tools import *
from geocal_swig import *
from image_ground_connection import *
from igc_collection_extension import *
from nose.plugins.skip import Skip, SkipTest
import scipy
import scipy.optimize
import numpy.testing as npt
from sqlite_shelf import write_shelve, read_shelve
from multiprocessing import Pool

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

# Some test data we have on pistol, along with a point of 
# "interesting features"
raster_source = "/data/smyth/LADem/12SEP-01_proj.img"
raster_aoi_pt = Geodetic(34.10426496, -117.929771)

# Data is way too big to check into source, so we put it here. This means
# we can only tests that depend on this on pistol. We may fold this into
# the afids data area at some point.
geocal_test_data = "/data/geocal_test_data/"

def test_connection_igc():
    '''Note that this also tests the inheritance in python of a C++ class
    ImageGroundConnection, and passing this back to C++ (the generate_rpc
    command)'''
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc_coll = IgcArray([igc1, igc2, igc3])
    igc = igc_coll.image_ground_connection(2)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert distance(gp, igc.ground_coordinate(ic)) < 0.01
    ic = igc3.image_coordinate(gp)
    assert_almost_equal(igc.image_coordinate(gp).line, ic.line, 2)
    assert_almost_equal(igc.image_coordinate(gp).sample, ic.sample, 2)
    rpc = Rpc.generate_rpc(igc, -100, 100)
    assert_almost_equal(rpc.image_coordinate(gp).line, ic.line, 2)
    assert_almost_equal(rpc.image_coordinate(gp).sample, ic.sample, 2)

def test_time():
    raise SkipTest
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc_coll = IgcArray([igc1, igc2, igc3])
    ic = ImageCoordinate(100, 200)
    gp = igc_coll.ground_coordinate(2, ic)
    for i in range(100000):
        t = igc_coll.image_coordinate(2, gp)

def test_igc_array():
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    t = [False] * 20
    t[0] = True
    igc1.rpc.fit_line_numerator = t
    igc1.rpc.fit_sample_numerator = t
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc2.rpc.fit_line_numerator = t
    igc2.rpc.fit_sample_numerator = t
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc3.rpc.fit_line_numerator = t
    igc3.rpc.fit_sample_numerator = t
    igc_coll = IgcArray([igc1, igc2, igc3])
    assert igc_coll.number_image == 3
    assert_almost_equal(igc_coll.parameter_subset[0], 
                        igc1.parameter_subset[0], 10)
    assert_almost_equal(igc_coll.parameter_subset[3], 
                        igc2.parameter_subset[1], 10)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert abs(igc_coll.image_coordinate(2, gp).line - ic.line) < 0.1
    assert abs(igc_coll.image_coordinate(2, gp).sample - ic.sample) < 0.1
    assert abs(igc_coll.image_coordinate_jac_cf(2, Ecr(gp)) - 
               igc3.image_coordinate_jac_cf(Ecr(gp))).max() < 1e-6
    igc_coll.parameter_subset = [1,2,3,4,5,6]
    assert_almost_equal(igc1.parameter_subset[0], 1, 10)
    assert_almost_equal(igc2.parameter_subset[1], 4, 10)
    if(False):
        '''This is just to compare with the Jacobian from 
        test_igc_array_with_igc_collection below. We manually ran this 
        and compared the results'''
        jac = scipy.sparse.lil_matrix((len(igc_coll.parameter_subset), len(igc_coll.parameter_subset)))
        igc_coll.image_coordinate_jac_parm(2, gp, jac, 0, 0)
        print jac

# We don't support this anymore. I don't think we actually need this, but
# leave the test in place in case we need to come back to this.
def test_igc_array_with_igc_collection():
    '''Test a IgcArray when one of the entries is a IgcCollection.'''
    raise SkipTest
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    t = [False] * 20
    t[0] = True
    igc1.rpc.fit_line_numerator = t
    igc1.rpc.fit_sample_numerator = t
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc2.rpc.fit_line_numerator = t
    igc2.rpc.fit_sample_numerator = t
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc3.rpc.fit_line_numerator = t
    igc3.rpc.fit_sample_numerator = t
    igc_coll_sub = IgcArray([igc2, igc3])
    igc_coll = IgcArray([igc1, igc_coll_sub])
    assert igc_coll.number_image == 3
    assert_almost_equal(igc_coll.parameter_subset[0], 
                        igc1.parameter_subset[0], 10)
    assert_almost_equal(igc_coll.parameter_subset[3], 
                        igc2.parameter_subset[1], 10)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert abs(igc_coll.image_coordinate(2, gp).line - ic.line) < 0.1
    assert abs(igc_coll.image_coordinate(2, gp).sample - ic.sample) < 0.1
    assert abs(igc_coll.image_coordinate_jac_cf(2, Ecr(gp)) - 
               igc3.image_coordinate_jac_cf(Ecr(gp))).max() < 1e-6
    igc_coll.parameter_subset = [1,2,3,4,5,6]
    assert_almost_equal(igc1.parameter_subset[0], 1, 10)
    assert_almost_equal(igc2.parameter_subset[1], 4, 10)
    assert igc_coll.title(0) == "10MAY21-1.img"
    assert igc_coll.title(1) == "10MAY21-2.img"
    assert igc_coll.title(2) == "10MAY21-3.img"
    assert os.path.basename(igc_coll.image(0).file.file_name) == \
        "10MAY21-1.img"
    assert os.path.basename(igc_coll.image(1).file.file_name) == \
        "10MAY21-2.img"
    assert os.path.basename(igc_coll.image(2).file.file_name) == \
        "10MAY21-3.img"
    assert os.path.basename(igc_coll.image_ground_connection(0).
                            image.file.file_name) == \
                            "10MAY21-1.img"
    assert os.path.basename(igc_coll.image_ground_connection(1).
                            image.file.file_name) == \
                            "10MAY21-2.img"
    assert os.path.basename(igc_coll.image_ground_connection(2).
                            image.file.file_name) == \
                            "10MAY21-3.img"
    jac = scipy.sparse.lil_matrix((len(igc_coll.parameter_subset), 
                                   len(igc_coll.parameter_subset)))
    igc_coll.image_coordinate_jac_parm(2, gp, jac, 0, 0)
    # We got the result we expect from running the igc_array test above
    assert_almost_equal(jac[0, 4], 15387.7492279, 2)
    assert_almost_equal(jac[1, 5], 17633.137014, 2)

def kepler_orbit_over_point(pt, cam ,fc):
    '''We create a KeplerOrbit that sees a particular point'''
    orb = KeplerOrbit()
    def f(t):
        return orb.reference_surface_intersect_approximate(orb.epoch + t, cam, fc).latitude - pt.latitude
    def f2(ra):
        orb.right_ascension = ra
        tpass = scipy.optimize.brentq(f, orb.period / 4, orb.period * 3 / 4)
        return orb.reference_surface_intersect_approximate(orb.epoch + tpass, cam, fc).longitude - pt.longitude
    ra = scipy.optimize.brentq(f2, 90, 180)
    orb.right_ascension = ra
    tpass = scipy.optimize.brentq(f, orb.period / 4, orb.period * 3 / 4)
    return orb, tpass + orb.epoch

def create_image_data(index):
    '''Creates the image data going with igc'''
    igccol = read_shelve(geocal_test_data + "igccol_rolling_shutter_temp.xml")
    f = VicarLiteRasterImage(raster_source)
    ras = IgcSimulated(igccol.image_ground_connection(index), f, -1, False)
    fout = VicarRasterImage(geocal_test_data + "igc_rolling_shutter_image%d.img" % index, "HALF", ras.number_line, ras.number_sample)
    geocal_swig.copy(ras, fout, True)

def create_image_data_all():
    '''This generates all the image data needed by create_igc_collection_rolling_shutter'''
    pool = Pool(20)
    v = [i for i in range(20)]
    res = pool.map(create_image_data, v)
    
def create_igc_collection_rolling_shutter():
    '''Create a IgcCollectionRollingShutter that we can use for testing. 
    Note this takes about 40 minutes to generate on pistol using 20 
    processors'''
    dem = SimpleDem()
    cam = QuaternionCamera(quat_rot("zyx", 0.1, 0.2, 0.3),
                           3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
                           1.0, FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    cam.fit_epsilon = False
    cam.fit_beta = False
    cam.fit_delta = False
    cam.fit_line_pitch = False
    cam.fit_sample_pitch = False
    cam.fit_focal_length = False
    cam.fit_principal_point_line(False, 0)
    cam.fit_principal_point_sample(False, 0)
    orb, tnadir = kepler_orbit_over_point(raster_aoi_pt, cam, FrameCoordinate(cam.number_line(0) / 2, cam.number_sample(0) / 2))
    tmin = tnadir - 10 * 10 - 0.5
    igccol = IgcCollectionRollingShutter(orb, cam, dem)
    for i in range(20):
        t = tmin + 10 * i
        tspace = 1.0 / cam.number_line(0);
        tt = RollingShutterConstantTimeTable(t, 
           t + cam.number_line(0) * tspace, tspace);
        title = "Image %d" % (i+1)
        igccol.add_image(None, tt, title)
    ic = ImageCoordinate(cam.number_line(0) / 2, cam.number_sample(0) / 2)
    igccol.determine_orbit_to_match(ic, 10)
    orb = OrbitOffsetCorrection(igccol.orbit)
    for i in range(10):
        tm, fc = igccol.image_ground_connection(i).time_table.time(ic)
        orb.insert_time_point(tm)
    igccol.orbit = orb
    write_shelve(geocal_test_data + "igccol_rolling_shutter_temp.xml", igccol)
    create_image_data_all()
    igccol_final = IgcCollectionRollingShutter(orb, cam, dem)
    for i in range(20):
        tt = igccol.image_ground_connection(i).time_table
        title = igccol.image_ground_connection(i).title
        ras = VicarLiteRasterImage(geocal_test_data + "igc_rolling_shutter_image%d.img" % i)
        igccol_final.add_image(ras, tt, title)
    write_shelve(geocal_test_data + "igccol_rolling_shutter.xml", igccol_final)
    print '''You should manually edit igccol_rolling_shutter.xml to use 
relative paths. You can just edit the text file in emacs.
'''
    
def test_igc_collection_rolling_shutter():
    '''Test IgcCollectionRollingShutter.'''
    tmin = Time.parse_time("1998-06-30T10:51:28.32Z");
    dem = SimpleDem()
    cam = QuaternionCamera(quat_rot("zyx", 0.1, 0.2, 0.3),
                           3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
                           1.0, FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    orb = KeplerOrbit(tmin, tmin + 1000)
    igccol = IgcCollectionRollingShutter(orb, cam, dem)
    for i in range(10):
        t = tmin + 20 * i
        tspace = 1e-3;
        tt = RollingShutterConstantTimeTable(t, 
           t + cam.number_line(0) * tspace, tspace);
        title = "Image %d" % (i+1)
        igccol.add_image(None, tt, title)
    ic = ImageCoordinate(cam.number_line(0) / 2, cam.number_sample(0) / 2)
    igccol.determine_orbit_to_match(ic, 4)
    gp = igccol.ground_coordinate(4, ic)
    for i in range(10):
        assert abs(igccol.image_coordinate(i, gp).line - ic.line) < 0.1
        assert abs(igccol.image_coordinate(i, gp).sample - ic.sample) < 0.1

