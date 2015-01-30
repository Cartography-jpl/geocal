from nose.tools import *
import cPickle
from geocal_swig import *
from igc_collection_rolling_shutter import *
from nose.plugins.skip import Skip, SkipTest
import numpy.testing as npt

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

