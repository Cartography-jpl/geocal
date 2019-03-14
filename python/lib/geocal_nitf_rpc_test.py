try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal.geocal_nitf_rpc import *
import geocal_swig
from geocal_swig import ImageCoordinate

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.image_subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0,i,j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

@require_pynitf
def test_rpc(isolated_dir, rpc):
    '''Create a file, and write out a RPC. Then make sure we can read it.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rpc = rpc
    f.write("nitf_rpc.ntf")
    f2 = pynitf.NitfFile("nitf_rpc.ntf")
    rpc2 = f2.image_segment[0].rpc
    assert_almost_equal(rpc.error_bias, rpc2.error_bias, 5)
    assert_almost_equal(rpc.error_random, rpc2.error_random, 5)
    assert_almost_equal(rpc.height_offset, rpc2.height_offset, 3)
    assert_almost_equal(rpc.height_scale, rpc2.height_scale, 3)
    assert_almost_equal(rpc.latitude_offset, rpc2.latitude_offset, 3)
    assert_almost_equal(rpc.latitude_scale, rpc2.latitude_scale, 3)
    assert_almost_equal(rpc.longitude_offset, rpc2.longitude_offset, 3)
    assert_almost_equal(rpc.longitude_scale, rpc2.longitude_scale, 3)
    assert_almost_equal(rpc.line_offset, rpc2.line_offset, 3)
    assert_almost_equal(rpc.line_scale, rpc2.line_scale, 3)
    assert_almost_equal(rpc.sample_offset, rpc2.sample_offset, 3)
    assert_almost_equal(rpc.sample_scale, rpc2.sample_scale, 3)
    for i in range(20):
        assert_almost_equal(rpc.line_numerator[i], rpc2.line_numerator[i], 6)
        assert_almost_equal(rpc.line_denominator[i], 
                            rpc2.line_denominator[i], 6)
        assert_almost_equal(rpc.sample_numerator[i], 
                            rpc2.sample_numerator[i], 6)
        assert_almost_equal(rpc.sample_denominator[i], 
                            rpc2.sample_denominator[i], 6)
    

@require_msp
@require_pynitf
def test_rpc_with_msp(isolated_dir, igc_rpc, msp_init):
    '''Compare the RSM we write to a NITF file with what the MSP library 
    calculates. This verifies both the validity of our NITF and our RSM 
    code'''    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rpc = igc_rpc.rpc
    f.write("nitf_rpc.ntf")
    for i in range(10):
        for j in range(10):
            ic = ImageCoordinate(i,j)
            p1 = msp_terrain_point("nitf_rpc.ntf", ic)
            p2 = igc_rpc.ground_coordinate(ic)
            assert(geocal_swig.distance(p1, p2) < 0.01)

        
    
