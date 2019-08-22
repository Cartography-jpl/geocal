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
    iseg_index = 1 
    igc1 = IgcMsp(nitf_sample_rip, SimpleDem(), iseg_index, "GLAS", "GLAS")
    f = pynitf.NitfFile(nitf_sample_rip)
    glas = f.image_segment[iseg_index].glas_gfm
    igc2 = glas.igc()
    igc3 = IgcMsp(nitf_sample_rip, SimpleDem(), iseg_index, "RSM", "RSM")
    print("Resolution %f m" % igc1.resolution_meter())
    print("Resolution %f m" % igc2.resolution_meter())
    print("Resolution %f m" % igc3.resolution_meter())
    max_diff1 = -1e8
    max_diff2 = -1e8
    for i in range(0, igc2.number_line, 20):
        for j in range (0, igc2.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc1.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            d2 = distance(igc3.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            max_diff1 = max(d1, max_diff1)
            max_diff2 = max(d2, max_diff2)
    print(max_diff1)
    print(max_diff2)
    # We'll want to beat this down, but for now we need to be within a pixel
    assert max_diff1 < 20.0
