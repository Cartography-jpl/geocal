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
    print(GlasGfmCovariance.read_des(d))


@require_pynitf
def test_spdcf_list():
    lis = GlasGfmSpdcfList()
    lis.append(GlasGfmSpdcfCsm(1.0,2.0,3.0,4.0, id_number=1))
    lis.append(GlasGfmSpdcfPiecwiseLinear([1.0, 0.5], [0, 10.0], id_number=2))
    lis.append(GlasGfmSpdcfDampedCosine(0.5, 10.0, 5, id_number=3))
    lis.append(GlasGfmSpdcfComposite(
        [GlasGfmSpdcfCsm(1.0,2.0,3.0,4.0, weight=0.5),
         GlasGfmSpdcfPiecwiseLinear([1.0, 0.5], [0, 10.0], weight=0.25),
         GlasGfmSpdcfDampedCosine(0.5, 10.0, 5, weight=0.25)],
        id_number=4))
    d_write = pynitf.DesCSCSDB()
    lis.write_des(d_write)
    print(d_write)
    lis2 = GlasGfmSpdcfList.read_des(d_write)
    print(lis2)
    
