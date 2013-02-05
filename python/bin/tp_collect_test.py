from afids import *
import subprocess
import os
test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

def test_tp_collection():
    '''Create a simple GDAL based Igc, and then collect tie points on it'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["create_gdal_igc", "sqlite_shelf.db:igc",
                           test_data + "nevada_elv_aoi.img",
                           test_data + "10MAY21-1.img", "Image 1",
                           test_data + "10MAY21-2.img", "Image 2",
                           test_data + "10MAY21-3.img", "Image 3"])
    subprocess.check_call(["tp_collect", "--number-x", "10",
                           "--number-y", "10", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:tpcol"])
    tpcol = read_shelve("sqlite_shelf.db:tpcol")
    assert len(tpcol) > 90
