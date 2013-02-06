import subprocess
import os
test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

def test_create_gdal_igc():
    '''Create a simple GDAL based Igc, and then make sure we can list it'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_dem", 
                           test_data + "nevada_elv_aoi.img",
                           "sqlite_shelf.db:dem_initial"])
    subprocess.check_call(["create_gdal_igc", "sqlite_shelf.db:igc",
                           "sqlite_shelf.db:dem_initial",
                           test_data + "10MAY21-1.img", "Image 1",
                           test_data + "10MAY21-2.img", "Image 2",
                           test_data + "10MAY21-3.img", "Image 3"])
    lst = subprocess.check_output(["shelve_dir", "sqlite_shelf.db"])
    assert lst == "dem_initial\nigc\n"
