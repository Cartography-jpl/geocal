from afids import *
import subprocess
import os
test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

def test_shelve_dem():
    '''Create simple GDAL based DEM.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_dem", 
                           test_data + "nevada_elv_aoi.img",
                           "sqlite_shelf.db:dem_initial"])

