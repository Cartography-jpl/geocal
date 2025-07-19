from .cart_lab_multifile_extension import *
from test_support import *

@require_panchromatic_orthobase
def test_create_image_chip(isolated_dir):
    # GCP from GRI
    ichip = GdalRasterImage(f"{unit_test_data}/11SLT_0455_00.TIF")
    orthobase = Landsat7Global("/raid22", Landsat7Global.BAND8)
    # Just check that we can generate this. We'll need to manually verify
    # that it is the right projection and data - but the unit test to
    # verifies we get no errors
    orthobase.create_image_chip("ichip_landsat.img", ichip)
