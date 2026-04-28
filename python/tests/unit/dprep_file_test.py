from geocal import DprepEphemerisFile, DprepAttitudeFile


def test_dprep_ephemeris(unit_test_data):
    """Test printing of metadata"""
    feph = DprepEphemerisFile(
        unit_test_data / "AM1EPHN0.A2017244.0000.001.2017244032008",
    )
    fatt = DprepAttitudeFile(
        unit_test_data / "AM1ATTNF.A2017244.0000.001.2017244112814",
    )
    assert feph.pos.shape == (7031,3)
    assert str(feph.start_time) == "2017-09-01T00:00:00.851976Z"
    assert str(feph.end_time) == "2017-09-01T01:59:59.571976Z"
    assert list(feph.onum) == [94173, 94174]
    assert fatt.euler_angle.shape == (14059,3)
    assert str(fatt.start_time) == "2017-09-01T00:00:00.878761Z"
    assert str(fatt.end_time) == "2017-09-01T01:59:59.598240Z"

