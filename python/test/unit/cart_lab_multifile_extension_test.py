from geocal import GdalRasterImage, Landsat7Global
from fixtures.require_check import require_panchromatic_orthobase

@require_panchromatic_orthobase
def test_create_image_chip(isolated_dir, unit_test_data):
    # GCP from GRI
    ichip = GdalRasterImage(str(unit_test_data / "11SLT_0455_00.TIF"))
    orthobase = Landsat7Global("/raid22", Landsat7Global.BAND8)
    # Just check that we can generate this. We'll need to manually verify
    # that it is the right projection and data - but the unit test to
    # verifies we get no errors
    orthobase.create_image_chip("ichip_landsat.img", ichip)
