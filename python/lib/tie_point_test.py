from nose.tools import *
from geocal_swig import *
from image_ground_connection import *
from tie_point import *
from ray_intersect import *
from feature_detector_extension import *
from sqlite_shelf import *
from nose.plugins.skip import Skip, SkipTest

# Basic test of tiepoint
def test_tie_point():
    t = TiePoint(5)
    assert t.number_camera, 5

# Data is way too big to check into source, so we put it here. This means
# we can only tests that depend on this on pistol. We may fold this into
# the afids data area at some point.
geocal_test_data = "/data/geocal_test_data/igccol_rolling_shutter.xml"

# If you run this, make sure to include this import. Otherwise the namespace
# for the tiepoint isn't correct in the TiePointCollection shelf
from geocal import *
def generate_tie_point_collection():
    '''This creates a "simulated" version of the igccol_rolling_shutter.xml.
    We generate tiepoints based on the "truth".
    '''
    igccol = read_shelve(geocal_test_data)
    igccol_original = read_shelve("/data/geocal_test_data/igccol_rolling_shutter.xml")
    igccol.orbit.fit_yaw = False
    igccol.parameter_subset =  [0,0,0,1.0,0.5, 1, 0.5, 1, -0.5, -1, -0.5, 1.2, -0.4, -0.9, 0.3, 0.8, 0.4, 0.7, 0.3, 0.8, 1.4, 1.0, 0.2]
    # We pretend that igccol_original is the "truth", and igccol has
    # uncorrected errors
    write_shelve("/data/geocal_test_data/igccol_rolling_shutter_simulated_error.xml", igccol)
    # Collect tiepoints. We don't actually bother with doing image matching 
    # here, we have other tests for that. But go ahead and locate interest
    # points, so when we test visualizing the data we have something to clearly
    # see.

    nimg = np.array([igccol_original.image_ground_connection(i).view_angle()[0] for i in range(igccol.number_image)]).argmin()
    fd = ForstnerFeatureDetector()
    iplist = fd.interest_point_grid(igccol_original.image(nimg), 20, 20)
    tpcol = TiePointCollection()
    ri = RayIntersect2(igccol, max_ground_covariance = 200 * 200)
    for i, ip in enumerate(iplist):
        tp = TiePoint(igccol_original.number_image)
        tp.id = i
        gc = igccol_original.ground_coordinate(nimg, ip)
        for i in range(igccol_original.number_image):
            tp.image_location[i] = (igccol_original.image_coordinate(i, gc),
                                    0.1, 0.1)
        tp = ri.ray_intersect(tp)
        if(tp):
            tpcol.append(tp)
    write_shelve("/data/geocal_test_data/data.db:tpcol", tpcol)
