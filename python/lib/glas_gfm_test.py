try:
    import pynitf
except ImportError:
    pass

from test_support import *
from geocal.geocal_nitf_des import *
from geocal.glas_gfm import *
from geocal_swig import (IgcMsp, SimpleDem, ImageCoordinate, distance, Geodetic)

@require_msp    
@require_pynitf
def test_rip_glas(nitf_sample_rip):
    '''Test reading GLAS data from the NITF RIP sample'''
    # Index for the image segment that has GLAS data.
    print(nitf_sample_rip)
    iseg_index = 1 
    igc1 = IgcMsp(nitf_sample_rip, SimpleDem(), iseg_index, "GLAS", "GLAS")
    f = pynitf.NitfFile(nitf_sample_rip)
    glas = f.image_segment[iseg_index].glas_gfm
    igc2 = glas.igc()
    ic = ImageCoordinate(0,256/2)
    print(Geodetic(igc1.ground_coordinate(ic)))
    print(Geodetic(igc2.ground_coordinate(ic)))
    print(igc1.resolution_meter())
    print(igc2.resolution_meter())
    print(distance(igc1.ground_coordinate(ic), igc2.ground_coordinate(ic)))
    ic = ImageCoordinate(1000, 256/2)
    print(distance(igc1.ground_coordinate(ic), igc2.ground_coordinate(ic)))
    print(igc2.image_coordinate(igc1.ground_coordinate(ic)))
