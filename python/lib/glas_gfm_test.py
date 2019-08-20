try:
    import pynitf
except ImportError:
    pass

from test_support import *
from geocal.geocal_nitf_des import *
from geocal.glas_gfm import *
from geocal_swig import (IgcMsp, SimpleDem, ImageCoordinate, distance)

@require_msp    
@require_pynitf
def test_rip_glas(nitf_sample_rip):
    '''Test reading GLAS data from the NITF RIP sample'''
    # Index for the image segment that has GLAS data.
    iseg_index = 1 
    igc1 = IgcMsp(nitf_sample_rip, SimpleDem(), iseg_index, "GLAS", "GLAS")
    f = pynitf.NitfFile(nitf_sample_rip)
    glas = f.image_segment[iseg_index].glas_gfm
    print(glas)
    print(glas.orbit)
    
    
