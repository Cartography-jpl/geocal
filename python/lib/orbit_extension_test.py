from nose.tools import *
from geocal_swig import *
from orbit_extension import *
from nose.plugins.skip import Skip, SkipTest

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
                    Geodetic(-51.772357805, 19.9739717878, 420921.889798)) < 1.0

