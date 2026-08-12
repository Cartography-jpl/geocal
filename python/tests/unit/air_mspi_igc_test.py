from geocal import AirMspiIgc, ImageCoordinate
from fixtures.require_check import require_raid, require_hdf5
import pickle
import numpy.testing as npt
import pytest
import os

test_data = "/raid3/test_data/AirMSPI_ER2_Hanford_GRP_ELLIPSOID_20130118_174900Z_286F_F01_V001.hdf"


@require_raid
@require_hdf5
def test_air_mspi_igc():
    # We can  only do this if data is available. If it isn't, just skip
    # the test.
    if not os.path.exists(test_data):
        pytest.skip("Don't have test data for air_mspi_igc")
    igc = AirMspiIgc(test_data)
    t = pickle.dumps(igc)
    igc2 = pickle.loads(t)
    npt.assert_almost_equal(
        igc.view_zenith(ImageCoordinate(500, 600)), 30.021980285644531, 5
    )
    npt.assert_almost_equal(
        igc2.view_zenith(ImageCoordinate(500, 600)), 30.021980285644531, 5
    )
    npt.assert_almost_equal(
        igc.view_azimuth(ImageCoordinate(500, 600)), 326.30319213867188, 5
    )
    npt.assert_almost_equal(
        igc.view_azimuth(ImageCoordinate(500, 600)), 326.30319213867188, 5
    )
    ic = igc.image_coordinate(igc.ground_coordinate(ImageCoordinate(500, 600)))
    npt.assert_almost_equal(ic.line, 500, 1)
    npt.assert_almost_equal(ic.sample, 600, 1)
