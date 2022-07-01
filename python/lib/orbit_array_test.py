from geocal_swig import *
from test_support import *
import numpy as np

def test_orbit_array():
    '''Tests or OrbitArray that are easier to do in python'''
    t = Time.parse_time("2020-01-01T00:00:00Z")
    orb = KeplerOrbit()
    eph_time0 = t.j2000 - 0.001
    eph_time = np.array([eph_time0 + i * 0.1 for i in range(20)])
    att_time0 = t.j2000 + 0.001
    att_time = np.array([att_time0 + i * 0.1 for i in range(20)])
    eph_pos = np.empty((20,3))
    eph_vel = np.empty((20,3))
    for (i,t) in enumerate(eph_time):
        od = orb.orbit_data(Time.time_j2000(t))
        eph_pos[i,:] = od.position_ci.position
        eph_vel[i,:] = od.velocity_ci
    att_quat = np.zeros((20,4))
    att_quat[:,3] = 1.0
    orb2 = OrbitArray_Eci_TimeJ2000(eph_time, eph_pos, eph_vel, att_time,
                                    att_quat)
    # Test handling the edge of the orbit
    print(orb2.min_time)
    print(orb2.max_time)
    print(orb2.orbit_data(orb2.min_time))
    print(orb2.orbit_data(orb2.max_time))
    # Test extending the edges with extrapolation
    orb2.min_time = orb2.min_time - 2
    orb2.max_time = orb2.max_time + 2
    print(orb2.min_time)
    print(orb2.max_time)
    print(orb2.orbit_data(orb2.min_time))
    print(orb2.orbit_data(orb2.max_time))
    # Test accessing the data in the OrbitQuaternionList.
    for q in orb2.quaternion_orbit_data_gen():
        print(q)

