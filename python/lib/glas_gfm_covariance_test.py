try:
    import pynitf
except ImportError:
    pass

from test_support import *
from .glas_gfm_covariance import *

if False:    
    pynitf.nitf_field.DEBUG = True
    pynitf.nitf_des.DEBUG = True

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
def test_create_cov():
    cov = GlasGfmCovariance()
    cov.cov_version_date = "20211019"
    cov.core_set.append(GlasGfmCoreSet(GlasGfmCoreSet.ORBITAL_FRAME,
                                       GlasGfmCoreSet.SENSOR_FRAME))
    glist = cov.core_set[0].sensor_error_parameter_group
    cov_m = np.array([[1,0,0],[0,2,0],[0,0,3]])
    post_cov_m = np.array([[5,0,0],[0,6,0],[0,0,7]])
    glist.append(GlasGfmGroup(corr_ref_t =
                              Time.parse_time("2021-10-19T12:00:00Z"),
                              adjustable_parameter=[1,2,3],
                              errcov_c1 = cov_m,
                              sr_spdcf_id = 1,
                              post_sub_alloc = True,
                              post_start_t =
                              Time.parse_time("2021-10-19T12:01:00Z"),
                              post_delta = 10.0,
                              num_post = 3,
                              post_interp = GlasGfmGroup.POST_INTERP_LINEAR,
                              errcov_c2 = post_cov_m,
                              post_sr_spdcf_id = 2,
                              ))
    # SPDCFs
    cov.spdcf.append(GlasGfmSpdcfCsm(1.0,2.0,3.0,4.0, id_number=1))
    cov.spdcf.append(GlasGfmSpdcfPiecwiseLinear([1.0, 0.5], [0, 10.0], id_number=2))
    cov.spdcf.append(GlasGfmSpdcfDampedCosine(0.5, 10.0, 5, id_number=3))
    cov.spdcf.append(GlasGfmSpdcfComposite(
        [GlasGfmSpdcfCsm(1.0,2.0,3.0,4.0, weight=0.5),
         GlasGfmSpdcfPiecwiseLinear([1.0, 0.5], [0, 10.0], weight=0.25),
         GlasGfmSpdcfDampedCosine(0.5, 10.0, 5, weight=0.25)],
        id_number=4))
    print(cov)
    d = cov.create_des()
    cov2 = GlasGfmCovariance.read_des(d)
    print(cov2)
    print(d)

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
    
