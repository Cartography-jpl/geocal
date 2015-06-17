from nose.tools import *
from geocal_swig import *
from image_ground_connection import *
from tie_point import *
from ray_intersect import *
from feature_detector_extension import *
from sqlite_shelf import *
from nose.plugins.skip import Skip, SkipTest

# Data is way too big to check into source, so we put it here. This means
# we can only tests that depend on this on pistol. We may fold this into
# the afids data area at some point.
geocal_test_igc = "/data/geocal_test_data/igccol_rolling_shutter.xml"
geocal_test_igc_sim_error = "/data/geocal_test_data/igccol_rolling_shutter_simulated_error.xml"
geocal_test_tpcol = "/data/geocal_test_data/data.db:tpcol"

def test_basic():
    '''Basic test of tiepoint'''
    t = TiePoint(5)
    assert t.number_camera, 5

def test_tie_point():
    '''Further testing of tiepoint, requires access to the geocal_test_data
    '''
    if(not os.path.exists(geocal_test_igc)):
        raise SkipTest
    igccol = read_shelve(geocal_test_igc)
    tpcol = read_shelve(geocal_test_tpcol)
    tp = tpcol[0]
    print tp.ic
    tp.image_location[5] = None
    print tp.ic
    print tp.ic_sigma
    print tp.ic_pred(igccol)
    print tp.ic_diff(igccol)

# If you run this, make sure to include this import. Otherwise the namespace
# for the tiepoint isn't correct in the TiePointCollection shelf
from geocal import *
def generate_tie_point_collection():
    '''This creates a "simulated" version of the igccol_rolling_shutter.xml.
    We generate tiepoints based on the "truth".
    '''
    igccol = read_shelve(geocal_test_igc)
    igccol_original = read_shelve(geocal_test_igc)
    igccol.orbit.fit_yaw = False
    igccol.parameter_subset =  [0,0,0,1.0,0.5, 1, 0.5, 1, -0.5, -1, -0.5, 1.2, -0.4, -0.9, 0.3, 0.8, 0.4, 0.7, 0.3, 0.8, 1.4, 1.0, 0.2]
    # We pretend that igccol_original is the "truth", and igccol has
    # uncorrected errors
    write_shelve(geocal_test_igc_sim_error, igccol)
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
    write_shelve(geocal_test_tpcol, tpcol)
