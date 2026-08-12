import subprocess
from fixtures.require_check import require_serialize


@require_serialize
def test_shelve_image(isolated_dir, stereo_unit_test_data):
    """Create simple GDAL based image."""
    subprocess.check_call(
        [
            "shelve_image",
            str(stereo_unit_test_data / "nevada_doq_aoi.img"),
            "sqlite_shelf.db:ref_image",
        ]
    )
