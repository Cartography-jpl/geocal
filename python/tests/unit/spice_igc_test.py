from geocal import pds_to_isis, isis_to_igc, ImageCoordinate, Planetocentric
from conftest import long_test
import pytest


# Needs ISIS and Mars data. The original had a hard coded path to a ctx.cub
# file used while developing, so this needs to be cleaned up before it can run.
@pytest.mark.skip(
    reason="Hard coded development ctx.cub path and requires ISIS, needs cleanup"
)
@long_test
def test_ctx_isis_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/ctx.cub"
    else:
        pds_to_isis(str(mars_test_data / "P16_007388_2049_XI_24N020W.IMG"), "ctx.cub")
        fname = "ctx.cub"
    igc = isis_to_igc(fname, spice_igc=True)
    ic = ImageCoordinate(300, 10)
    gc = igc.ground_coordinate(ic)
    print(Planetocentric(gc))
    icres = igc.image_coordinate(gc)
    print(icres)
    assert abs(icres.line - ic.line) < 0.1
    assert abs(icres.sample - ic.sample) < 0.1
