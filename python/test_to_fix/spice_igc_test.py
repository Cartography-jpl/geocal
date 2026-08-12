from test_support import *
from .isis_support import pds_to_isis
from .spice_igc import *
from .isis_to_igc import *
from geocal_swig import Planetocentric

@long_test
def test_ctx_isis_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/ctx.cub"
    else:
        pds_to_isis(mars_test_data + "P16_007388_2049_XI_24N020W.IMG",
                    "ctx.cub")
        fname = "ctx.cub"
    igc = isis_to_igc(fname, spice_igc=True)
    ic = ImageCoordinate(300,10)
    gc = igc.ground_coordinate(ic)
    print(Planetocentric(gc))
    icres = igc.image_coordinate(gc)
    print(icres)
    assert abs(icres.line - ic.line) < 0.1
    assert abs(icres.sample - ic.sample) < 0.1
    
    
    
    
        

    
