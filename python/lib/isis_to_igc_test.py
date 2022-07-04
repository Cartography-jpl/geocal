from test_support import *
from .isis_support import pds_to_isis
from .isis_to_igc import *

@long_test
@require_isis
def test_ctx_to_igc(mars_test_data, isolated_dir):
    if False:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/ctx.cub"
    else:
        pds_to_isis(mars_test_data + "P16_007388_2049_XI_24N020W.IMG",
                    "ctx.cub")
        fname = "ctx.cub"
    igc = isis_to_igc(fname, glas_gfm=True)
    print(igc)
    
@long_test
@require_isis
def test_hirise_to_igc(mars_test_data, isolated_dir):
    f1 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_0.img"
    f2 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_1.img"
    pds_to_isis(f1, "ctx.cub", pds_fname2=f2)
    # Test handling the kernels and downloading from the web.
    klist = read_kernel_from_isis("ctx.cub")
    print(klist)
    klist.load_kernel()

    
