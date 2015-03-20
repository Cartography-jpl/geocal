from nose.tools import *
from geocal_swig import *
from igc_collection_extension import *
from tie_point import *
from image_ground_connection import *
from igc_collection_extension import *
from simultaneous_bundle_adjustment import *
from lm_optimize import *
from nose.plugins.skip import Skip, SkipTest
from sqlite_shelf import write_shelve, read_shelve
import numpy as np
import logging
import sys

console = logging.StreamHandler(stream=sys.stdout)
console.setLevel(logging.INFO)
logging.getLogger("geocal-python").addHandler(console)

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

# Note the test data igccol_rolling_shutter.xml is created in 
# igc_collection_extension_test.py, in case you need to regenerate it
# (e.g., change in the format)

def create_tie_point(igccol, gp):
    tp = TiePoint(igccol.number_image)
    tp.ground_location = gp
    for i in range(igccol.number_image):
        try:
            tp.image_location[i] = igccol.image_coordinate(i, gp), 0.05, 0.05
        except RuntimeError:
            pass # Ok if we can't get image coordinate
    return tp

def test_sba_end_to_end():
    '''This is a more realistic sba end to end test. We use a IgcRollingShutter
    because it is one of the more complicated ImageGroundConnection we have.
    We generate the input data, and then run the sba to make sure we can
    retrieve the "truth".'''
    raise SkipTest
    igccol = read_shelve(test_data + "igccol_rolling_shutter.xml")
    poriginal = igccol.parameter_subset
    ptrue = np.random.randn(poriginal.shape[0]) * 5
    igccol.parameter_subset = ptrue
    tparr = TiePointCollection()
    igc = igccol.image_ground_connection(4)
    for ln in range(10, igc.number_line - 10, 500):
        for smp in range(10, igc.number_sample - 10, 500):
            gp = igc.ground_coordinate(ImageCoordinate(ln, smp))
            tp = create_tie_point(igccol, gp)
            tparr.append(tp)
    igc.parameter_subset = poriginal
    sba = SimultaneousBundleAdjustment(igccol, tparr, igc.dem)
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    print len(igccol.parameter_subset)
    print igccol.parameter_subset

