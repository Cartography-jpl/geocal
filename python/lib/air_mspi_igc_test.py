from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
from air_mspi_igc import *
import os.path
import cPickle

test_data = "/raid3/test_data/AirMSPI_ER2_Hanford_GRP_ELLIPSOID_20130118_174900Z_286F_F01_V001.hdf"

def test_air_mspi_igc():
    # We can  only do this if data is available. If it isn't, just skip
    # the test.
    if(not os.path.exists(test_data)):
        raise SkipTest
    igc = AirMspiIgc(test_data)
    t = cPickle.dumps(igc)
    igc2 = cPickle.loads(t)
