from test_support import *
from .isis_support import *

@long_test
@require_isis
def test_import_ctx(mars_test_data, isolated_dir):
    pds_to_isis(mars_test_data + "P16_007388_2049_XI_24N020W.IMG",
                "ctx.cub")
    
