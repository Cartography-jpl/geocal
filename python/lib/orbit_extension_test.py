from .orbit_extension import *
from test_support import *

def test_orbit_from_tle():
    # TLE comes from http://www.n2yo.com/satellite/?s=25544, and this is
    # for ISS
    tle = '''\
1 25544U 98067A   15299.90708094  .00011146  00000-0  17215-3 0  9995
2 25544  51.6444 146.6664 0006740  82.5539   7.0440 15.54629204968532
'''
    orb = KeplerOrbit.orbit_from_tle(tle)
    t = orb.position_cf(Time.parse_time('2015-10-27T00:05:10Z'))
    assert distance(orb.position_cf(Time.parse_time('2015-10-27T00:05:10Z')),
                    Geodetic(-51.77286358544813, 20.073929772655067,
                             420921.42725819454)) < 1.0

def test_quat_from_principal_point():
    # Arbitrary orbit, but simple for us to generate
    orb = KeplerOrbit()
    tm = Time.parse_time("2014-02-03T10:00:00Z")
    # Camera that has a roughly 0.5 meter resolution nadir looking, i.e.,
    # about the resolution of WV-2
    cam = QuaternionCamera(Quaternion_double(1,0,0,0), 1, 2048, 20e-9,
                           20-9, 1.6e7, FrameCoordinate(0,1024))
    # Original point
    gp = orb.orbit_data(tm).reference_surface_intersect_approximate(cam,FrameCoordinate(0,1024))
    # Get a point a little earlier and point to the same location
    od = orb.orbit_data(tm-100)
    # Rotate to fit original point
    twist = 10
    od.quat_from_principal_gp(gp, twist=twist)
    # Should be close to the point we are fitting
    assert distance(gp, od.reference_surface_intersect_approximate(cam, FrameCoordinate(0, 1024))) < 0.01
    gp2, twist2 = od.principal_gp(SimpleDem())
    assert distance(gp, gp2) < 0.01
    assert twist == pytest.approx(twist2)
