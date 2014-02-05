from nose.tools import *
from geocal import *
from tie_point import *
from image_ground_connection import *
from igc_offset_correction import *
from simultaneous_bundle_adjustment import *
from lm_optimize import *
from nose.plugins.skip import Skip, SkipTest

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

def test_time():
    raise SkipTest
    orb_uncorr = HdfOrbit_EciTod_TimeAcs(test_data + "../sample_orbit.h5")
    cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                           3375, 3648,
                           1.0 / 2500000,
                           1.0 / 2500000,
                           1.0,
                           FrameCoordinate(1688.0, 1824.5),
                           1.0, 1.0, QuaternionCamera.LINE_IS_Y)
    demin = SimpleDem()
# Image not actually used for anything, but the SBA expects it
    t1 = Time.time_acs(215077459.472);
    imglist = []
    tmlist = []
    parameter_true = [0,0,0]
    nimg = 88
    #nimg = 6
    for i in range(nimg):
        tm = t1 + i * 0.2
        img = VicarLiteRasterImage(test_data + "10MAY21-1.img")
        img.time = tm
        img.title = "Image %d" % i
        imglist.append(img)
        tmlist.append(tm)
        parameter_true.extend([100,4 + 0.1 * i, 3 - 0.1 * i])

    igc_coll = IgcOffsetCorrection(imglist, cam, demin, orb_uncorr,
                                   time_point = tmlist)
    igc_coll.fit_position = False
    igc_coll.fit_yaw = False
    nl = igc_coll.cam.number_line(0)
    ns = igc_coll.cam.number_sample(0)
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
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian, 
                       diagnostic = True)
    print len(igc_coll.parameter_subset)
    print igc_coll.parameter_subset
    print igc_coll.parameter_subset_mask

