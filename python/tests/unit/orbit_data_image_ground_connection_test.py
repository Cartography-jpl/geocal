import pytest

from geocal import (
    ImageCoordinate,
    KeplerOrbit,
    MemoryRasterImage,
    OrbitDataImageGroundConnection,
    SimpleCamera,
    SimpleDem,
    Time,
)


def test_odgc():
    orb = KeplerOrbit()
    tm = Time.parse_time("2003-01-01T11:11:00Z")
    od = orb.orbit_data(tm)
    cam = SimpleCamera()
    dem = SimpleDem(100)
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = OrbitDataImageGroundConnection(od, cam, dem, img)
    assert igc.resolution == pytest.approx(30, abs=1e-2)
    assert igc.band == 0
    assert igc.max_height == pytest.approx(9000, abs=1e-2)
    g = igc.ground_coordinate(ImageCoordinate(1, 1504 / 2))
    assert g.latitude == pytest.approx(-4.840663932844596, abs=1e-2)
    assert g.longitude == pytest.approx(165.5531678459437, abs=1e-2)
