from geocal import *
import subprocess
import os
test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

# Simple test to see if we have AFIDS data available. We check for the
# presence of one of the AFIDS environment variables, and if there
# assume we have the data
have_afid_data = "AFIDS_VDEV_DATA" in os.environ 

def test_shelve_dem():
    '''Create simple GDAL based DEM.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_dem", 
                           test_data + "nevada_elv_aoi.img",
                           "sqlite_shelf.db:dem_initial"])
    if(have_afid_data):
        try:
            SrtmDem
            subprocess.check_call(["shelve_dem", "--srtm",
                                   "sqlite_shelf.db:dem_srtm"])
        except NameError:
            pass                # Skip test if we don't have SrtmDem
    subprocess.check_call(["shelve_dem", "--constant-dem=10",
                           "sqlite_shelf.db:dem_constant"])

