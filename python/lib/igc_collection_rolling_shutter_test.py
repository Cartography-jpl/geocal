from nose.tools import *
import cPickle
from geocal_swig import *
from igc_collection_rolling_shutter import *
from nose.plugins.skip import Skip, SkipTest
import numpy.testing as npt
import scipy.optimize
import numpy as np

def determine_quat_rot(v1, v2):
    '''This finds the quaternion that rotates v1 to v2. Note that
    this isn't actually unique, but this is the 'shortest arc' solution.'''
    a = np.cross(v1, v2)
    w = np.linalg.norm(v1) * np.linalg.norm(v1) + np.dot(v1, v2)
    q = Quaternion_double(w, a[0], a[1], a[2])
    q /= Quaternion_double_abs(q)
    return q

def determine_orbit_parm(gp, igccol, i):
    '''This adjusts the orbit parameters so that the given ground point
    returns an image coordinate of (0,0)'''
    # Vector that we have now
    igc = igccol.image_ground_connection(i)
    orb = igc.orbit
    od = orb.orbit_data(igc.time_table.min_time)
    ic = ImageCoordinate(0,0)
    t = od.sc_look_vector(igc.cf_look_vector_lv(ic))
    v1 = np.array([t.direction[0], t.direction[1], t.direction[2]])

    # Vector that we want
    t = od.sc_look_vector(CartesianFixedLookVector(igc.cf_look_vector_pos(ic), gp))
    v2 = np.array([t.direction[0], t.direction[1], t.direction[2]])
    
    # Quaternion that does that
    q = determine_quat_rot(v2, v1)
    y,p,r = quat_to_ypr(q)
    parm = orb.parameter.copy()
    parm[(3 + i * 3):(3 + i * 3 + 3)] = \
        [y / arcsecond_to_rad, p / arcsecond_to_rad, r / arcsecond_to_rad]
    orb.parameter = parm

def test_determine_quat_rot():
    v1 = np.array([1.0,0.0,0.0])
    v2 = np.array([0.5,0.6,0.7])
    v2 /= np.linalg.norm(v2)
    v1_q = Quaternion_double(0.0, v1[0], v1[1], v1[2])
    q = determine_quat_rot(v1, v2)
    v2_q = q * v1_q * q.conj()
    v2_c = np.array([v2_q.R_component_2, v2_q.R_component_3, v2_q.R_component_4])
    np.testing.assert_almost_equal(v2, v2_c)


def test_igc_collection_rolling_shutter():
    tmin = Time.parse_time("1998-06-30T10:51:28.32Z");
    dem = SimpleDem()
    cam = QuaternionCamera(quat_rot("zyx", 0.1, 0.2, 0.3),
                           3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
                           1.0, FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    orb_uncorr = KeplerOrbit(tmin, tmin + 1000)
    orb = OrbitOffsetCorrection(orb_uncorr)
    ttlist = []
    imglist = []
    for i in range(10):
        t = tmin + 20 * i
        orb.insert_time_point(t)
        tspace = 1e-3;
        ttlist.append(RollingShutterConstantTimeTable(t, 
           t + cam.number_line(0) * tspace, tspace));
        imglist.append(None)
    igccol = IgcCollectionRollingShutter(imglist=imglist, ttlist=ttlist,
                                        orbit=orb, cam=cam, dem=dem)
    t = cPickle.dumps(igccol)
    igccol2 = cPickle.loads(t)
    # Set things up so that all the igc point to the same location
    # on the ground.
    ic = ImageCoordinate(0, 0)
    gp = igccol.ground_coordinate(4, ic)
    for i in range(10):
        determine_orbit_parm(gp, igccol, i)
    for i in range(10):
        print igccol.image_coordinate(i, gp)

    gp = igccol.ground_coordinate(4, ImageCoordinate(100, 200))
    for i in range(10):
        print igccol.image_coordinate(i, gp)

    
