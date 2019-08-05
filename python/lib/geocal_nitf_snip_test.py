try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (IgcMsp, SimpleDem, ImageCoordinate, distance)
import os

@require_msp    
@require_pynitf
def test_snip_example(isolated_dir):
    fname = "/home/smyth/Local/SNIP NITF Example/07APR2005_Hyperion_331405N0442002E_SWIR172_001_L1R.ntf"
    # Ok if data isn't available. We can only run test if we have the
    # test data.
    if(not os.path.exists(fname)):
        raise SkipTest
    igc1 = IgcMsp(fname, SimpleDem(), 1, "GLAS", "GLAS")
    igc2 = IgcMsp(fname, SimpleDem(), 1, "RSM", "RSM")
    # Not sure why, but default IgcMsp is not what we would expect. Neither
    # GLAS nor RSM is selected
    igc3 = IgcMsp(fname, SimpleDem(), 1)
    print(igc1)
    print(igc2)
    print(igc3)
    pt1 = igc1.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt2 = igc2.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt3 = igc3.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    print(distance(pt1, pt2))
    print(distance(pt1, pt3))
    
