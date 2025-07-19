from geocal_swig import *
import math
import numpy as np


@classmethod
def _kepler_orbit_from_tle(self, tle):
    """This creates a Kepler orbit from a two-line element set (TLE,
    see https://en.wikipedia.org/wiki/Two-line_element_set). Note that
    the Kepler orbit completely ignores the sorts of perturbation that
    a simplified perturbation model (e.g., SDP4) models, so this isn't
    particularly accurate. But can be nice for unit tests since this
    is independent of SPICE toolkit.

    Compare this to the TleOrbit, which does use the full model but
    depends on the SPICE toolkit"""
    tle_arr = tle.split()
    t = tle_arr[3]
    # Note we assume year is for 20xx. We'd need to add handling for
    # older TLEs for 19xx.
    epoch = (
        Time.parse_time("20%s-01-01T00:00:00Z" % t[0:2])
        + (float(t[2:]) - 1) * 24 * 60 * 60
    )
    # Look for mu at http://www.orbiterwiki.org/wiki/Orbital_elements
    mu = 398600441800000
    ecc = float("0." + tle_arr[13])
    inc = float(tle_arr[11])
    ra = float(tle_arr[12])
    ap = float(tle_arr[14])
    ma = float(tle_arr[15])
    n = float(tle_arr[16][0:11])
    orb_num = int(tle_arr[16][11:-1])
    a = pow(mu / pow(n * 2 * math.pi / (24 * 3600), 2), 1.0 / 3)
    return KeplerOrbit(
        Time.min_valid_time, Time.max_valid_time, epoch, a, ecc, inc, ra, ap, ma
    )


KeplerOrbit.orbit_from_tle = _kepler_orbit_from_tle


def _quat_from_principal_gp(self, gp, twist=0):
    """Find a quaternion that maps the principal point to the given
    ground location. This isn't unique, so we also take a "twist" and
    an angle in degrees to rotate about the camera z axis."""
    v1 = [0, 0, 1]
    v2 = gp.convert_to_cf().position - self.position_cf.position
    # Inverse of aberration correction
    v2 += np.linalg.norm(v2) / speed_of_light * self.velocity_cf
    self.sc_to_cf = determine_quat_rot(v1, v2) * quat_rot_z(twist * deg_to_rad)
    quat_normalize(self.sc_to_cf)


QuaternionOrbitData.quat_from_principal_gp = _quat_from_principal_gp


def _quat_from_principal_gic(self, ic, image, dem, twist=0):
    """Find a quaternion that maps the principal point to the given
    ground location. This isn't unique, so we also take a "twist"
    an angle in degrees to rotate about the camera z axis.

    This variation takes the gp determined by the image coordinate in the
    given image"""
    gp = image.ground_coordinate(ic, dem)
    self.quat_from_principal_gp(gp, twist=twist)


QuaternionOrbitData.quat_from_principal_gic = _quat_from_principal_gic


def _principal_gp(self, dem, resolution=1, max_height=9000):
    """Inverse of quat_from_principal_gp, returns gp and twist"""
    cflv = self.cf_look_vector(ScLookVector(0, 0, 1))
    gp = dem.intersect(self.position_cf, cflv, resolution, max_height)
    v1 = [0, 0, 1]
    v2 = gp.convert_to_cf().position - self.position_cf.position
    # Inverse of aberration correction
    v2 += np.linalg.norm(v2) / speed_of_light * self.velocity_cf
    q = determine_quat_rot(v1, v2)
    q2 = q.conj() * self.sc_to_cf
    twist = math.atan2(q2.R_component_4, q2.R_component_1) * rad_to_deg * 2
    return gp, twist


QuaternionOrbitData.principal_gp = _principal_gp


def _principal_gic(self, image, dem, resolution=1, max_height=9000):
    """Inverse of quat_from_principal_gic, returns ic and twist"""
    gp, twist = self.principal_gp(dem, resolution=resolution, max_height=max_height)
    return image.coordinate(gp), twist


QuaternionOrbitData.principal_gic = _principal_gic

__all__ = []
