from future import standard_library
standard_library.install_aliases()
from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
from geocal.air_mspi_igc import *
import os.path
import pickle

test_data = "/raid3/test_data/AirMSPI_ER2_Hanford_GRP_ELLIPSOID_20130118_174900Z_286F_F01_V001.hdf"

def test_air_mspi_igc():
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        HdfFile
    except NameError:
        raise SkipTest
    # We can  only do this if data is available. If it isn't, just skip
    # the test.
    if(not os.path.exists(test_data)):
        raise SkipTest
    igc = AirMspiIgc(test_data)
    t = pickle.dumps(igc)
    igc2 = pickle.loads(t)
    assert_almost_equal(igc.view_zenith(ImageCoordinate(500,600)),
                        30.021980285644531, 5)
    assert_almost_equal(igc2.view_zenith(ImageCoordinate(500,600)),
                        30.021980285644531, 5)
    assert_almost_equal(igc.view_azimuth(ImageCoordinate(500,600)),
                        326.30319213867188, 5)
    assert_almost_equal(igc.view_azimuth(ImageCoordinate(500,600)),
                        326.30319213867188, 5)
    ic = igc.image_coordinate(igc.ground_coordinate(ImageCoordinate(500, 600)))
    assert_almost_equal(ic.line, 500, 1)
    assert_almost_equal(ic.sample, 600, 1)
