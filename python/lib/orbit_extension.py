from __future__ import division
from past.utils import old_div
from geocal_swig import *
import math

@classmethod
def _kepler_orbit_from_tle(self, tle):
    '''This creates a Kepler orbit from a two-line element set (TLE,
    see https://en.wikipedia.org/wiki/Two-line_element_set). Note that
    the Kepler orbit completely ignores the sorts of perturbation that
    a simplified perturbation model (e.g., SDP4) models, so this isn't
    particularly accurate. But can be nice for unit tests since this
    is independent of SPICE toolkit.

    Compare this to the TleOrbit, which does use the full model but 
    depends on the SPICE toolkit'''
    tle_arr = tle.split()
    t = tle_arr[3]
    # Note we assume year is for 20xx. We'd need to add handling for
    # older TLEs for 19xx.
    epoch = Time.parse_time('20%s-01-01T00:00:00Z' % t[0:2] ) + \
        (float(t[2:])-1) * 24 * 60 * 60
    # Look for mu at http://www.orbiterwiki.org/wiki/Orbital_elements
    mu = 398600441800000
    ecc = float("0." + tle_arr[13])
    inc = float(tle_arr[11])
    ra = float(tle_arr[12])
    ap = float(tle_arr[14])
    ma = float(tle_arr[15])
    n = float(tle_arr[16][0:11])
    orb_num = int(tle_arr[16][11:-1])
    a = pow(old_div(mu, pow(n * 2 * math.pi / (24 * 3600), 2)), old_div(1.0, 3))
    return KeplerOrbit(Time.min_valid_time, Time.max_valid_time, epoch, a, 
                       ecc, inc, ra, ap, ma)

KeplerOrbit.orbit_from_tle =_kepler_orbit_from_tle
