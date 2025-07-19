from geocal_swig import *
from .vicar_interface import *
from .ibis_file_extension import *
import numpy as np
import math

# This wraps various VICAR routines to allow them to be called through
# python


class __scinterp(VicarInterface):
    """Helper class for calling scinterp"""

    def __init__(
        self,
        time,
        pos_previous,
        t_pos_previous,
        att_previous,
        t_att_previous,
        pos_subsequent,
        t_pos_subsequent,
        att_subsequent,
        t_att_subsequent,
    ):
        VicarInterface.__init__(self)
        self.cmd = "scinterp"
        self.pos_sample = pos_previous
        self.time = time
        self.peph = [
            0.0,
            pos_previous.position[0],
            pos_previous.position[1],
            pos_previous.position[2],
            t_pos_previous.acs,
        ]
        self.patt = [
            att_previous.R_component_1,
            att_previous.R_component_2,
            att_previous.R_component_3,
            att_previous.R_component_4,
            t_att_previous.acs,
        ]
        self.seph = [
            0.0,
            pos_subsequent.position[0],
            pos_subsequent.position[1],
            pos_subsequent.position[2],
            t_pos_subsequent.acs,
        ]
        self.satt = [
            att_subsequent.R_component_1,
            att_subsequent.R_component_2,
            att_subsequent.R_component_3,
            att_subsequent.R_component_4,
            t_att_subsequent.acs,
        ]
        self.imtime = time.acs
        self.before_body = """
local w1 real
local w2 real
local w3 real
local o0 real
local o1 real
local o2 real
local o3 real
"""
        self.build_command(
            ["peph", "patt", "seph", "satt", "imtime"],
            [
                "w_t_sv1=w1",
                "w_t_sv2=w2",
                "w_t_sv3=w3",
                "w_q_sv0=o0",
                "w_q_sv1=o1",
                "w_q_sv2=o2",
                "w_q_sv3=o3",
            ],
        )
        self.cmd += """
write "output starts here"
write "&w1"
write "&w2"
write "&w3"
write "&o0"
write "&o1"
write "&o2"
write "&o3"
"""
        self.debug = False
        self.timing = False
        self.force_cleanup = True
        self.vicar_run()

    def post_run(self):
        """Grab the results from the run file before we finish with the
        scratch directory."""
        res = []
        do_append = False
        for line in self.run_out.splitlines():
            if do_append:
                res.append(float(line))
            if line == b"output starts here":
                do_append = True
        p = self.pos_sample.create(res[0:3])
        att = Quaternion_double(*res[3:7])
        self.res = QuaternionOrbitData(self.time, p, [0, 0, 0], att)


def scinterp(
    time,
    pos_previous,
    t_pos_previous,
    att_previous,
    t_att_previous,
    pos_subsequent,
    t_pos_subsequent,
    att_subsequent,
    t_att_subsequent,
):
    """This calls the VICAR program scinterp. Note that we already have
    this functionality in GeoCal (see for example OrbitQuaternionList or
    HdfOrbit). But for comparison with older VICAR programs it is useful
    to be able to call this routine.

    This takes the time as a geocal_swig.Time, along with the
    position and attitude from before and after this time
    (as a geocal_swig.CartesianInertial and geocal_swig.Quaternion_double).
    This returns a geocal_swig.QuaternionOrbitData.

    The ephemeris is linearly interpolated, and the attitude has a
    spherical linear interpolation done between the two rotation quaternions.
    The ephemeris used in the Cartesian inertial (e.g., EciTod), and the
    attitude is the Cartesian intertial version."""
    t = __scinterp(
        time,
        pos_previous,
        t_pos_previous,
        att_previous,
        t_att_previous,
        pos_subsequent,
        t_pos_subsequent,
        att_subsequent,
        t_att_subsequent,
    )
    return t.res


class __sc2rpc(VicarInterface):
    """Helper class for calling sc2rpc"""

    def __init__(
        self,
        orbit_data,
        delta_ut1,
        leapsecond_file,
        image_coordinate,
        height,
        sc_to_cam_q,
        fu,
        fv,
    ):
        VicarInterface.__init__(self)
        # Note, the number of rows here needs to exactly match what urange, vrange
        # and hrange produce. If you are off, you'll get garbage in the output. In
        # our case, we get 2 points from each range, for a total size of
        # 2 * 2 * 2 = 8
        self.cmd = """
gen xxxa nl=3 ns=3
ibis-gen xxxb nc=5 nr=8 deffmt=DOUB
"""
        self.cmd += "sc2rpc (xxxa, xxxb)"
        self.urange = [image_coordinate.sample, 1, image_coordinate.sample + 2]
        self.vrange = [image_coordinate.line, 1, image_coordinate.line + 2]
        self.hrange = [height, 1, height + 1]
        self.leapfile = os.path.basename(leapsecond_file)
        self.acs_time = orbit_data.time.acs
        self.delta_ut1 = delta_ut1
        self.tod_t_sv = [
            orbit_data.position_ci.position[0],
            orbit_data.position_ci.position[1],
            orbit_data.position_ci.position[2],
        ]
        # Note that sc2rpc appears to *require* that the quaternions be
        # normalized. I believe this is assumed in the quaternion to
        # Rodrigues vector and Rodrigues vector to matrix calculations.
        # Without forcing this, you get minor hard to account for
        # differences between GeoCal and sc2rpc.
        self.tod_q_sv = np.array(
            [
                orbit_data.sc_to_ci.R_component_1,
                orbit_data.sc_to_ci.R_component_2,
                orbit_data.sc_to_ci.R_component_3,
                orbit_data.sc_to_ci.R_component_4,
            ]
        )
        self.tod_q_sv /= math.sqrt(np.dot(self.tod_q_sv, self.tod_q_sv))
        self.sv_t_c = [0, 0, 0]
        self.sv_q_c = np.array(
            [
                sc_to_cam_q.R_component_1,
                sc_to_cam_q.R_component_2,
                sc_to_cam_q.R_component_3,
                sc_to_cam_q.R_component_4,
            ]
        )
        self.sv_q_c /= math.sqrt(np.dot(self.sv_q_c, self.sv_q_c))
        self.q = 0
        self.u0 = 1824.5
        self.v0 = 1688.0
        self.kappa = [0.0, 0.0, 0.0, 0.0, 0.0]
        self.fu = fu
        self.fv = fv
        self.input = [leapsecond_file]
        self.build_command(
            [
                "urange",
                "vrange",
                "hrange",
                "leapfile",
                "acs_time",
                "delta_ut1",
                "tod_t_sv",
                "tod_q_sv",
                "sv_t_c",
                "sv_q_c",
                "fu",
                "fv",
                "q",
                "u0",
                "v0",
                "kappa",
            ]
        )
        self.debug = False
        self.timing = False
        self.force_cleanup = False
        self.vicar_run()

    def post_run(self):
        """Grab the results from the ibis file before we finish with the
        scratch directory."""
        f = IbisFile("xxxb")
        self.res = Geodetic(f[0, 1], f[0, 0], f[0, 2])


def sc2rpc(
    orbit_data,
    delta_ut1,
    leapsecond_file,
    image_coordinate,
    height,
    sc_to_cam_q,
    fu,
    fv,
):
    """Run sc2rpc. Note that despite the name, this actually does *not*
    create a RPC. Rather it finds the ground location for a particular
    image location.

    This takes in the orbit data as a geocal_swig.QuaternionOrbitData. We take
    Delta_ut1 and leapseconds file to use (these are passed in rather than
    looked up). The height is in meters.

    Note that the height of the returned point may not be exactly the passed
    in height. This is actually ok and expected, Mike Burl's code doesn\'t
    bother iterating to the surface because we just need reasonable points
    for doing RPC generation."""
    t = __sc2rpc(
        orbit_data,
        delta_ut1,
        leapsecond_file,
        image_coordinate,
        height,
        sc_to_cam_q,
        fu,
        fv,
    )
    return t.res


__all__ = ["scinterp", "sc2rpc"]
