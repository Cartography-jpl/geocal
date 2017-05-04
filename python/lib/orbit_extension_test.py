from orbit_extension import *
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

