import subprocess
from fixtures.require_check import require_serialize, require_vicar


@require_serialize
@require_vicar
def test_shelve_image(isolated_dir, stereo_unit_test_data):
    subprocess.check_call(
        [
            "shelve_image",
            str(stereo_unit_test_data / "nevada_doq_aoi.img"),
            "sqlite_shelf.db:ref_image",
        ]
    )
    subprocess.check_call(
        [
            "write_image",
            "--no-data-value=0",
            "--vicar-type=DOUB",
            "sqlite_shelf.db:ref_image",
            "test.img",
        ]
    )
