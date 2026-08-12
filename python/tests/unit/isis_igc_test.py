from geocal import pds_to_isis, IsisIgc
from fixtures.require_check import require_isis
import pytest


@pytest.mark.long_test
@require_isis
def test_ctx_isis_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/ctx.cub"
    else:
        pds_to_isis(mars_test_data + "P16_007388_2049_XI_24N020W.IMG", "ctx.cub")
        fname = "ctx.cub"
    igc = IsisIgc(fname)
    print(igc)


@pytest.mark.long_test
@require_isis
def test_hirise_isis_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/hirise.cub"
    else:
        f1 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_0.img"
        f2 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_1.img"
        pds_to_isis(f1, "hirise.cub", pds_fname2=f2)
        fname = "hirise.cub"
    igc = IsisIgc(fname)
    print(igc)
