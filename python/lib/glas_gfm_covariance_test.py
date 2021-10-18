try:
    import pynitf
except ImportError:
    pass

from test_support import *
from .glas_gfm_covariance import *

@require_pynitf
def test_rip_glas_covariance(nitf_sample_rip):
    '''Test reading the covariance part of the NITF RIP sample'''
    f = pynitf.NitfFile(nitf_sample_rip)
    # Index for the image segment that has GLAS data.
    iseg_index = 1 
    iseg = f.image_segment[iseg_index]
    d = iseg.glas_gfm.find_one_des("CSCSDB")
    print(d)
