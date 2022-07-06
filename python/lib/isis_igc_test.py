from test_support import *
from .isis_support import pds_to_isis
from .isis_igc import *
from .isis_to_igc import isis_to_igc
from geocal_swig import ImageCoordinate, Planetocentric, MolaDemFile, GdalDem

@long_test
@require_isis
def test_ctx_isis_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/ctx.cub"
    else:
        pds_to_isis(mars_test_data + "P16_007388_2049_XI_24N020W.IMG",
                    "ctx.cub")
        fname = "ctx.cub"
    igc = IsisIgc(fname)
    ic = ImageCoordinate(1000,10)
    gc1 = igc.ground_coordinate(ic)
    print(Planetocentric(gc1))
    igc2 = isis_to_igc(fname)
    dem = MolaDemFile("/raid26/marsdem/meg128/megr44n270hb.lbl")
    dem2 = GdalDem("/bigdata/isisdata/base/dems/molaMarsPlanetaryRadius0005.cub")
    gc2 = igc2.ground_coordinate_dem(ic,dem)
    print(Planetocentric(gc2))
    print(Planetocentric(igc2.ground_coordinate_dem(ic,dem2)))
    igc2.dem = dem
    print(igc2.image_coordinate(gc2))
    print(igc2.image_coordinate(gc1))
    print(igc.image_coordinate(gc2))
    print(igc.image_coordinate(gc1))
    print(igc.pixel_time(ic))
    print(igc2.pixel_time(ic))
    
    
        

    
