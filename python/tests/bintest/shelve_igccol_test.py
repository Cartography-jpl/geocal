import subprocess
from fixtures.require_check import require_serialize


@require_serialize
def test_shelve_igccol(isolated_dir, stereo_unit_test_data):
    """Create a simple GDAL based Igc, and then make sure we can list it"""
    subprocess.check_call(
        [
            "shelve_dem",
            str(stereo_unit_test_data / "nevada_elv_aoi.img"),
            "sqlite_shelf.db:dem_initial",
        ]
    )
    subprocess.check_call(
        [
            "shelve_igccol",
            "sqlite_shelf.db:igc",
            "sqlite_shelf.db:dem_initial",
            str(stereo_unit_test_data / "10MAY21-1.img"),
            "Image 1",
            str(stereo_unit_test_data / "10MAY21-2.img"),
            "Image 2",
            str(stereo_unit_test_data / "10MAY21-3.img"),
            "Image 3",
        ]
    )
    lst = subprocess.check_output(["shelve_dir", "sqlite_shelf.db"]).decode("utf-8")
    assert lst == "dem_initial\nigc\n"
