from __future__ import division
from past.utils import old_div
from geocal_swig import *
from numpy.testing import assert_almost_equal

def test_odgc():
    orb = KeplerOrbit()
    tm = Time.parse_time("2003-01-01T11:11:00Z")
    od = orb.orbit_data(tm)
    cam = SimpleCamera()
    dem = SimpleDem(100)
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = OrbitDataImageGroundConnection(od, cam, dem, img)
    assert_almost_equal(igc.resolution, 30, 2)
    assert igc.band == 0
    assert_almost_equal(igc.max_height, 9000, 2)
    g = igc.ground_coordinate(ImageCoordinate(1, old_div(1504, 2)))
    assert_almost_equal(g.latitude, -4.840663932844596, 2)
    assert_almost_equal(g.longitude, 165.5531678459437, 2)
