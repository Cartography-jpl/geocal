import h5py
import numpy as np
import os
from geocal_swig import *
from nose.plugins.skip import Skip, SkipTest
import cPickle

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"

def create_sample_file():
    '''This create a HDF 5 file that contains orbit information. We can use
    this is a base for converting from some other format to HDF 5 (e.g., 
    reading Level 0).'''
    # Create file, overwriting any existing file
    f = h5py.File(test_data + "sample_orbit.h5", "w")
    # Create group, so we could have other things in the HDF 5 if we wanted
    # without worrying about possible name classes
    g = f.create_group("Orbit")
    # Separate ephemeris and attitude
    eph = g.create_group("Ephemeris")
    att = g.create_group("Attitude")
    # Easiest if we know ahead of time what size we want to write. We
    # can have resizable arrays if needed.
    tm = eph.create_dataset("Time", (100,), "=f8")
    pos = eph.create_dataset("Position", (100,3), "=f8")
    vel = eph.create_dataset("Velocity", (100,3), "=f8")
    # Include descriptions
    tm.attrs["Description"] = "This is ACS time that position is measured"
    pos.attrs["Description"] = "This is ECI True of Date Position, in meters"
    vel.attrs["Description"] = "This is the velocity that goes with the position, in meter/second"
    # Repeat with attitude
    tm2 = att.create_dataset("Time", (1000,), "=f8")
    quat = att.create_dataset("Quaternion", (1000,4), "=f8")
    tm2.attrs["Description"] = "This is ACS time that attitude quaternion is measured"
    quat.attrs["Description"] = "This is the attitude quaternion. This gives the orientation of the spacecraft with the ECI TOD position"

    # This isn't the best test data, but stick the spacecraft at one
    # unmoving position, and one orientation. This matches the data
    # used in the quaternion_camera_test.cc, which is turn matches
    # the unit test of sc2rpc. We can come up with better data at some
    # point if needed.
    # Time of interest is 215077459.472
    for i, t in enumerate(range(215077450 - 50 * 60, 215077450 + 50 * 60, 60)):
        tm[i] = t
        pos[i,:] = np.array([3435100.496, 945571.538, -6053387.573])
        vel[i,:] = np.array([0.0,0.0,0.0])
    for i, t in enumerate(range(215077450 - 500 * 1, 215077450 + 500 * 1, 1)):
        tm2[i] = t
        quat[i, :] = [0.946366, 0.0, -0.323096813, 0.0]

# create_sample_file()

def test_sc2rpc():
    '''This attempts to match the results of the sc2rpc unit tests.
    **NOTE** We get results that are about 7 meters different than what
    sc2rpc gets. This was tracked down to what I believe is a more
    accurate calculation the spice toolkit for J2000 to ECEF.  We allow
    a 10 meter error in comparing to sc2rpc.'''
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        HdfOrbit_EciTod_TimeAcs
    except NameError:
        raise SkipTest

    orb = HdfOrbit_EciTod_TimeAcs(test_data + "sample_orbit.h5")
    cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                           3375, 3648,
                           1.0 / 2500000,
                           1.0 / 2500000,
                           1.0,
                           FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    t = Time.time_acs(215077459.472);
    pt = orb.reference_surface_intersect_approximate(t, cam, FrameCoordinate(3375, 3648))
    pt_geod = Geodetic(-60.3162137, 47.2465154)
    assert distance(pt_geod, pt) < 10.0
    
    
def test_pickle():
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        HdfOrbit_EciTod_TimeAcs
    except NameError:
        raise SkipTest
    # Only run if we have serialization support in geocal
    if(!have_serialize_supported()):
        raise SkipTest
    orb = HdfOrbit_EciTod_TimeAcs(test_data + "sample_orbit.h5")
    t = cPickle.dumps(orb, cPickle.HIGHEST_PROTOCOL)
    orb2 = cPickle.loads(t)
    cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                           3375, 3648,
                           1.0 / 2500000,
                           1.0 / 2500000,
                           1.0,
                           FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    t = Time.time_acs(215077459.472);
    pt = orb.reference_surface_intersect_approximate(t, cam, FrameCoordinate(3375, 3648))
    pt2 = orb2.reference_surface_intersect_approximate(t, cam, FrameCoordinate(3375, 3648))
    assert distance(pt, pt2) < 0.01
    od1 = orb.orbit_data(t)
    pick = cPickle.dumps(od1, cPickle.HIGHEST_PROTOCOL)
    od2 = cPickle.loads(pick)
    pt = od1.reference_surface_intersect_approximate(cam, FrameCoordinate(3375, 3648))
    pt2 = od2.reference_surface_intersect_approximate(cam, FrameCoordinate(3375, 3648))
    assert distance(pt, pt2) < 0.01
    
    
