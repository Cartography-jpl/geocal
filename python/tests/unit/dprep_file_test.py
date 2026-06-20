from geocal import (
    DprepEphemerisFile,
    DprepAttitudeFile,
    DprepOrbit,
    rad_to_deg,
    KeplerOrbit,
    Time,
    distance,
    SpiceKeplerOrbit,
    Ecr,
    SpiceHelper
)
import numpy as np

def test_dprep_ephemeris(unit_test_data):
    feph = DprepEphemerisFile(
        unit_test_data / "AM1EPHN0.A2017244.0000.001.2017244032008",
    )
    assert feph.pos.shape == (7031, 3)
    assert str(feph.start_time) == "2017-09-01T00:00:00.851976Z"
    assert str(feph.end_time) == "2017-09-01T01:59:59.571976Z"
    assert list(feph.onum) == [94173, 94174]


def test_dprep_attitude(unit_test_data):
    fatt = DprepAttitudeFile(
        unit_test_data / "AM1ATTNF.A2017244.0000.001.2017244112814",
    )
    assert fatt.euler_angle.shape == (14059, 3)
    assert str(fatt.start_time) == "2017-09-01T00:00:00.878761Z"
    assert str(fatt.end_time) == "2017-09-01T01:59:59.598240Z"


def test_dprep_orbit(unit_test_data):
    orb = DprepOrbit(
        [
            unit_test_data / "AM1EPHN0.A2017244.0000.001.2017244032008",
        ],
        [
            unit_test_data / "AM1ATTNF.A2017244.0000.001.2017244112814",
        ],
    )
    kepoch = orb.dprep_ephermeris_file[0].kepler_epoch
    a, e, inc, an, ap, ma = orb.dprep_ephermeris_file[0].kepler_element
    korb = KeplerOrbit(
        Time.min_valid_time,
        Time.max_valid_time,
        kepoch,
        a,
        e,
        inc * rad_to_deg,
        an * rad_to_deg,
        ap * rad_to_deg,
        ma * rad_to_deg,
    )
    for i in range(0, 1000, 100):
        print(
            distance(
                orb.position_cf(orb.min_time + i), korb.position_cf(orb.min_time + i)
            )
        )
    SpiceHelper.add_kernel(str(unit_test_data / "gm_de440.tpc"))
    mu = SpiceKeplerOrbit.mu("EARTH")
    element = SpiceKeplerOrbit.spice_oscelt(korb.orbit_data(kepoch))
    # Spice uses the perifocal distance rather than the semimajor axis, and also
    # has this in km rather than m.
    element2 = np.array([a / 1000 * (1-e), e, inc, an, ap, ma, kepoch.j2000, mu])
    sorb = SpiceKeplerOrbit(element2)
    for i in range(0, 1000, 100):
        print(
            distance(
                orb.position_cf(orb.min_time + i), sorb.position_cf(orb.min_time + i)
            )
        )
    for i in range(0, 1000, 100):
        print(
            distance(
                korb.position_cf(orb.min_time + i), sorb.position_cf(orb.min_time + i)
            )
        )
