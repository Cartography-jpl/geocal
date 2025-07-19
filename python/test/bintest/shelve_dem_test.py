import geocal
import os
import subprocess
from fixtures.require_check import require_serialize

# Simple test to see if we have AFIDS data available. We check for the
# presence of one of the AFIDS environment variables, and if there
# assume we have the data
have_srtm_data = "ELEV_ROOT" in os.environ and os.path.exists(os.environ["ELEV_ROOT"])


@require_serialize
def test_shelve_dem(isolated_dir, stereo_unit_test_data):
    """Create simple GDAL based DEM."""
    subprocess.check_call(
        [
            "shelve_dem",
            str(stereo_unit_test_data / "nevada_elv_aoi.img"),
            "sqlite_shelf.db:dem_initial",
        ]
    )
    if have_srtm_data:
        if geocal.VicarFile.vicar_available():
            subprocess.check_call(["shelve_dem", "--srtm", "sqlite_shelf.db:dem_srtm"])
    subprocess.check_call(
        ["shelve_dem", "--constant-dem=10", "sqlite_shelf.db:dem_constant"]
    )
