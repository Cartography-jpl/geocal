from afids import *
import subprocess
import os
test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

def test_shelve_image():
    '''Create simple GDAL based image.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_image", 
                           test_data + "nevada_doq_aoi.img",
                           "sqlite_shelf.db:ref_image"])

