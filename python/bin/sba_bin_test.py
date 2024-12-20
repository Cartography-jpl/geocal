from __future__ import print_function
import geocal
from test_support import *
import subprocess

@require_serialize
def test_sba(isolated_dir):
    '''Create a simple GDAL based Igc, collect tie points on it, and run an 
    SBA.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_dem", 
                           stereo_unit_test_data + "nevada_elv_aoi.img",
                           "sqlite_shelf.db:dem_initial"])
    subprocess.check_call(["shelve_igccol", 
                           '--rpc-line-fit=0', '--rpc-sample-fit=0',
                           "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:dem_initial",
                           stereo_unit_test_data + "10MAY21-1.img", "Image 1",
                           stereo_unit_test_data + "10MAY21-2.img", "Image 2",
                           stereo_unit_test_data + "10MAY21-3.img", "Image 3"])
    subprocess.check_call(["tp_collect", "--number-x", "10",
                           "--number-y", "10", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:tpcol"])
    subprocess.check_call(["sba", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:tpcol", "sqlite_shelf.db:igc_sba",
                           "sqlite_shelf.db:tpcol_sba"])
    igc = geocal.read_shelve("sqlite_shelf.db:igc_sba")
    tpcol = geocal.read_shelve("sqlite_shelf.db:tpcol_sba")
    dem = igc.dem(0)
    sba = geocal.SimultaneousBundleAdjustment(igc, tpcol, dem);
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print(chisq)
    assert chisq < 2.5



