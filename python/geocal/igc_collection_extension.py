from builtins import range
from abc import *
from geocal_swig import *
import numpy as np


def _determine_orbit_parm(self, gp, ic, i):
    """This adjust the orbit parameters so that the given ground point
    return an image coordinate of ic for igc i.

    Note that this is only done approximately. See the discussion in the
    comments of _determine_orbit_to_match."""
    # Vector that we have now
    igc = self.image_ground_connection(i)
    tmpt, fc = igc.time_table.time(ic)
    tlist = [igc.time_table.min_time, tmpt, igc.time_table.max_time]
    for j, tm in enumerate(tlist):
        od = igc.orbit.orbit_data(tm)
        t = od.sc_look_vector(
            od.cf_look_vector(
                igc.camera.sc_look_vector(FrameCoordinate(ic.line, ic.sample), igc.band)
            )
        )
        v1 = np.array([t.direction[0], t.direction[1], t.direction[2]])

        # Vector that we want
        t = od.sc_look_vector(CartesianFixedLookVector(od.position_cf, gp))
        v2 = np.array([t.direction[0], t.direction[1], t.direction[2]])

        # Quaternion that does that
        self.orbit.update_quaterion(i * len(tlist) + j, determine_quat_rot(v2, v1))


def _determine_orbit_to_match(self, ic, ind):
    """This adds an orbit model on top of the existing orbit. We adjust the
    orbit model so that all the image ground connections see the same point
    on the ground, which is whatever image_ground_connection(ind) see from
    the passed in image coordinate ic.

    This is entirely used for generating test data, I don't think there is
    any reason you would want to do this with real data.

    Note that seeing the point on the ground at the image coordinate ic is
    only done approximately. We could probably work at the logic to have this
    match exactly, but there doesn't seem to be much point. What we really
    want is just a bunch of images that are looking at about the same location
    on the ground, the approximate calculation done here should be fine.
    """
    self.orbit = OrbitOffsetCorrection(self.orbit, False, False, False, False, False)
    gp = self.ground_coordinate(ind, ic)
    for i in range(self.number_image):
        igc = self.image_ground_connection(i)
        tm, fc = igc.time_table.time(ic)
        self.orbit.insert_attitude_time_point(igc.time_table.min_time)
        self.orbit.insert_attitude_time_point(tm)
        self.orbit.insert_attitude_time_point(igc.time_table.max_time)
    for i in range(self.number_image):
        self._determine_orbit_parm(gp, ic, i)


IgcCollectionRollingShutter._determine_orbit_parm = _determine_orbit_parm
IgcCollectionRollingShutter.determine_orbit_to_match = _determine_orbit_to_match

__all__ = []
