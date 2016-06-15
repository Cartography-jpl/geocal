from geocal import *
from test_support import *

@require_serialize
def test_shelve_image(isolated_dir):
    '''Create simple GDAL based image.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_image", 
                           stereo_unit_test_data + "nevada_doq_aoi.img",
                           "sqlite_shelf.db:ref_image"])

