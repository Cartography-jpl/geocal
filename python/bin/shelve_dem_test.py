import geocal
from test_support import *

# Simple test to see if we have AFIDS data available. We check for the
# presence of one of the AFIDS environment variables, and if there
# assume we have the data
have_srtm_data = "ELEV_ROOT" in os.environ and os.path.exists(os.environ["ELEV_ROOT"])

@require_serialize
def test_shelve_dem(isolated_dir):
    '''Create simple GDAL based DEM.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_dem", 
                           stereo_unit_test_data + "nevada_elv_aoi.img",
                           "sqlite_shelf.db:dem_initial"])
    if(have_srtm_data):
        if(geocal.VicarFile.vicar_available()):
            subprocess.check_call(["shelve_dem", "--srtm",
                                   "sqlite_shelf.db:dem_srtm"])
    subprocess.check_call(["shelve_dem", "--constant-dem=10",
                           "sqlite_shelf.db:dem_constant"])

