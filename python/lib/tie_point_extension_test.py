from __future__ import print_function
from builtins import range
from geocal.tie_point_extension import *
from geocal.image_ground_connection import *
from geocal.ray_intersect import *
from geocal.feature_detector_extension import *
from geocal.sqlite_shelf import *
from test_support import *
import shutil

def test_basic():
    '''Basic test of tiepoint'''
    t = TiePoint(5)
    assert t.number_image, 5

def test_old_mspi_format(isolated_dir):
    '''Test for reading the old MSPI format.'''
    fname = mspi_test_data + "old_tie_point/tie_point_211051000.dat"
    t = TiePoint(5)
    tp = TiePoint.read_old_mspi_format(fname)
    assert tp.id == 211051000
    assert not tp.is_gcp
    assert distance(tp.ground_location, Ecr(-2.47279636204959126e+06, -4.61999558631069399e+06, 3.62513840793766454e+06)) < 1
    assert tp.number_image_location == 3
    tpcol = TiePointCollection.read_old_mspi_format(mspi_test_data + 
                                                    "old_tie_point")
    assert len(tpcol) == 165
    assert tpcol.number_gcp == 18
    try:
        shutil.rmtree("old_mpsi_tp")
    except OSError:
        pass
    tpcol.write_old_mspi_format("old_mspi_tp")


@require_serialize
@require_geocal_test_data
def test_create_multiple_pass():
    '''Test create_multiple_pass'''
    tpcol1 = read_shelve(geocal_test_tpcol)
    igccol1 = read_shelve(geocal_test_igc)
    tpcol2 = read_shelve(geocal_test_tpcol)
    igccol2 = read_shelve(geocal_test_igc)
    igccol = IgcMultiplePass()
    igccol.add_igc(igccol1)
    igccol.add_igc(igccol2)
    tpcol = TiePointCollection.create_multiple_pass(tpcol1, tpcol2)
    assert igccol.number_image == tpcol[0].number_image
    

@require_serialize
@require_geocal_test_data
def test_tie_point():
    '''Further testing of tiepoint, requires access to the geocal_test_data
    '''
    igccol = read_shelve(geocal_test_igc)
    tpcol = read_shelve(geocal_test_tpcol)
    tp = tpcol[0]
    print(tp.ic)
    tp.image_coordinate(5, None)
    print(tp.ic)
    print(tp.ic_sigma)
    print(tp.ic_pred(igccol))
    print(tp.ic_diff(igccol))
    print(tpcol.data_frame(igccol, 0))
    print(tpcol.panel(igccol))
    print(tpcol.table_format())

def generate_tie_point_collection():
    '''This creates a "simulated" version of the igccol_rolling_shutter.xml.
    We generate tiepoints based on the "truth".
    '''
    igccol = read_shelve(geocal_test_igc)
    igccol_original = read_shelve(geocal_test_igc)
    igccol.orbit.fit_yaw = False
    igccol.parameter_subset =  [1.0,0.5, 1, 0.5, 1, -0.5, -1, -0.5, 1.2, -0.4, -0.9, 0.3, 0.8, 0.4, 0.7, 0.3, 0.8, 1.4, 1.0, 0.2]
    # We pretend that igccol_original is the "truth", and igccol has
    # uncorrected errors
    write_shelve(geocal_test_igc_sim_error, igccol)
    # Collect tiepoints. We don't actually bother with doing image matching 
    # here, we have other tests for that. But go ahead and locate interest
    # points, so when we test visualizing the data we have something to clearly
    # see.

    nimg = np.array([igccol_original.image_ground_connection(i).view_angle()[0] for i in range(igccol.number_image)]).argmin()
    nimg = int(nimg)
    fd = ForstnerFeatureDetector()
    iplist = fd.interest_point_grid(igccol_original.image(nimg), 20, 20)
    tpcol = TiePointCollection()
    ri = RayIntersect2(igccol, max_ground_covariance = 200 * 200)
    for i, ip in enumerate(iplist):
        tp = TiePoint(igccol_original.number_image)
        tp.id = i
        gc = igccol_original.ground_coordinate(nimg, ip)
        for i in range(igccol_original.number_image):
            tp.image_coordinate(i, igccol_original.image_coordinate(i, gc))
        tp = ri.ray_intersect(tp)
        if(tp):
            tpcol.append(tp)
    write_shelve(geocal_test_tpcol, tpcol)
