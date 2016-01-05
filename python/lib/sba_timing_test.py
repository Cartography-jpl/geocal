from __future__ import print_function
from nose.tools import *
from geocal_swig import *
from tie_point import *
from image_ground_connection import *
from igc_collection_extension import *
from simultaneous_bundle_adjustment import *
from lm_optimize import *
from nose.plugins.skip import Skip, SkipTest
import logging
import sys

console = logging.StreamHandler(stream=sys.stdout)
console.setLevel(logging.INFO)
logging.getLogger("geocal-python").addHandler(console)

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

def test_time():
    raise SkipTest
    orb_uncorr = HdfOrbit_EciTod_TimeAcs(test_data + "../sample_orbit.h5")
    orb = OrbitOffsetCorrection(orb_uncorr)
    cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                           3375, 3648,
                           1.0 / 2500000,
                           1.0 / 2500000,
                           1.0,
                           FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    cam.fit_epsilon = False
    cam.fit_beta = False
    cam.fit_delta = False
    cam.fit_line_pitch = False
    cam.fit_sample_pitch = False
    cam.fit_focal_length = False
    cam.fit_principal_point_line(False, 0)
    cam.fit_principal_point_sample(False, 0)
    demin = SimpleDem()
    t1 = Time.time_acs(215077459.472);
    parameter_true = [0,0,0]
    #nimg = 88
    nimg = 6
    igc_coll = IgcCollectionOrbitData(orb, cam, demin)
    for i in range(nimg):
        tm = t1 + i * 0.2
        igc_coll.add_image(None, tm, "Image %d" % i)
        igc_coll.orbit.insert_time_point(tm)
        parameter_true.extend([100,4 + 0.1 * i, 3 - 0.1 * i])

    igc_coll.fit_position_x = False
    igc_coll.fit_position_y = False
    igc_coll.fit_position_z = False
    igc_coll.fit_yaw = False
    nl = igc_coll.camera.number_line(0)
    ns = igc_coll.camera.number_sample(0)
    border = 100
    parameter_start = igc_coll.parameter
    igc_coll.orbit.parameter = parameter_true
    tplist = []
    ntogether = 6
    for basecam in range(ntogether / 2, nimg, ntogether / 2):
        for ln in range(border, nl - border, 
                        int(math.ceil((nl - border * 2) / 10.0))):
            for smp in range(border, ns - border, 
                             int(math.ceil((ns - border * 2) / 10.0))):
                ic = ImageCoordinate(ln, smp)
                gp = igc_coll.ground_coordinate(basecam, ic)
                tp = TiePoint(nimg)
                for i in range(basecam - ntogether / 2,
                               min(basecam + ntogether  / 2, nimg)):
                    tp.image_location[i] = igc_coll.image_coordinate(i, gp), \
                                           0.05, 0.05
                tp.ground_location = gp
                tplist.append(tp)

    igc_coll.parameter = parameter_start
    sba = SimultaneousBundleAdjustment(igc_coll, TiePointCollection(tplist),
                                       demin)
#    res = sba.sba_jacobian(sba.parameter)
#    print res.tolil()
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    print(len(igc_coll.parameter_subset))
    print(igc_coll.parameter_subset)
    print(igc_coll.parameter_mask)

