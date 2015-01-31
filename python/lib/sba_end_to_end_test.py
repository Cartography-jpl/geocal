from nose.tools import *
from geocal_swig import *
from igc_collection_extension import *

def test_sba_end_to_end():
    '''This is a more realistic sba end to end test. We use a IgcRollingShutter
    because it is one of the more complicated ImageGroundConnection we have.
    We generate the input data, and then run the sba to make sure we can
    retrieve the "truth".'''
    generate_igc()

def generate_igc():
    tmin = Time.parse_time("1998-06-30T10:51:28.32Z");
    dem = SimpleDem()
    cam = QuaternionCamera(quat_rot("zyx", 0.1, 0.2, 0.3),
                           3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
                           1.0, FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    orb_uncorr = KeplerOrbit(tmin, tmin + 1000)
    orb = OrbitOffsetCorrection (orb_uncorr)
    igcol = IgcArray()
    for i in range(10):
        t = tmin + 20 * i
        orb.insert_time_point(t)
        tspace = 1e-3;
        tt = RollingShutterConstantTimeTable(t, 
            t + cam.number_line(0) * tspace, tspace);
        igcol.igc.append(IgcRollingShutter(orb, tt, cam, dem, None))
        
    ic = ImageCoordinate(100, 200)
    gp = igcol.ground_coordinate(4, ic)
    print gp
    igc = igc
    def f(x):
        igcol.
    g = lambda x: distance(igcol.ground_coo
        
