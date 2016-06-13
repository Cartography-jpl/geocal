from geocal import *
import subprocess

@require_serialize
@require_vicar
def test_shelve_image(isolated_dir):
    '''Create simple GDAL based image.'''
    try:
        os.remove("sqlite_shelf.db")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["shelve_image", 
                           stereo_unit_test_data + "nevada_doq_aoi.img",
                           "sqlite_shelf.db:ref_image"])
    subprocess.check_call(["write_image", "--no-data-value=0",
                           "--vicar-type=DOUB",
                           "sqlite_shelf.db:ref_image", "test.img"])
