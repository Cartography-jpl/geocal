import geocal
from test_support import *

@require_serialize
def test_tp_collection(isolated_dir):
    '''Create a simple GDAL based Igc, and then collect tie points on it'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_dem", 
                           stereo_unit_test_data + "nevada_elv_aoi.img",
                           "sqlite_shelf.db:dem_initial"])
    subprocess.check_call(["shelve_igccol", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:dem_initial",
                           stereo_unit_test_data + "10MAY21-1.img", "Image 1",
                           stereo_unit_test_data + "10MAY21-2.img", "Image 2",
                           stereo_unit_test_data + "10MAY21-3.img", "Image 3"])
    subprocess.check_call(["tp_collect", "--number-x", "10",
                           "--number-y", "10", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:tpcol"])
    tpcol = geocal.read_shelve("sqlite_shelf.db:tpcol")
    assert len(tpcol) > 90
