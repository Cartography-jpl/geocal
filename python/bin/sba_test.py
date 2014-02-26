from afids import *
import subprocess
import os
test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

def test_sba():
    '''Create a simple GDAL based Igc, collect tie points on it, and run an 
    SBA.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_dem", 
                           test_data + "nevada_elv_aoi.img",
                           "sqlite_shelf.db:dem_initial"])
    subprocess.check_call(["shelve_igccol", 
                           '--rpc-line-fit=0', '--rpc-sample-fit=0',
                           "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:dem_initial",
                           test_data + "10MAY21-1.img", "Image 1",
                           test_data + "10MAY21-2.img", "Image 2",
                           test_data + "10MAY21-3.img", "Image 3"])
    subprocess.check_call(["tp_collect", "--number-x", "10",
                           "--number-y", "10", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:tpcol"])
    subprocess.check_call(["sba", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:tpcol", "sqlite_shelf.db:igc_sba",
                           "sqlite_shelf.db:tpcol_sba"])
    igc = read_shelve("sqlite_shelf.db:igc_sba")
    tpcol = read_shelve("sqlite_shelf.db:tpcol_sba")
    dem = igc.dem(0)
    sba = SimultaneousBundleAdjustment(igc, tpcol, dem);
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    assert chisq < 2.0



