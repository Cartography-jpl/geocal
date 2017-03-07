from builtins import str
from builtins import range
from tre_struct import *
import subprocess
from test_support import *
import sys

@require_vicar_gdalplugin    
def test_nitf_use00a_create(isolated_dir):
    '''Create a nitf from existing VICAR data, adding a TRE for use00a'''
    try:
        os.remove("use00a.vrt")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    try:
        os.remove("use00a.ntf")
    except OSError as exc:
        pass                    # Ok if doesn't exist
    subprocess.check_call(["gdal_translate", "-of", "VRT", "-q",
                           unit_test_data + "vicar.img", "use00a.vrt"])
    t = GdalRasterImage("use00a.vrt")
    tre = TreUSE00A()
    tre.angle_to_north = 270
    tre.mean_gsd = 105.2
    tre.dynamic_range = 2047
    tre.obl_ang = 34.12
    tre.roll_ang = -21.15
    tre.n_ref= 0
    tre.rev_num = 3317
    tre.n_seg= 1
    tre.max_lp_seg = 6287
    tre.sun_el = 68.5
    tre.sun_az = 131.3
    assert not t.has_use00a
    t.use00a = tre
    assert t.has_use00a
    t.close()
    subprocess.check_call(["gdal_to_nitf", "-q",
                           unit_test_data + "vicar.img", "use00a.ntf"])
    t = GdalRasterImage("use00a.ntf")
    tre = t.use00a
    assert tre.angle_to_north == 270
    assert tre.mean_gsd == 29.8
    assert tre.dynamic_range ==2047
    assert tre.obl_ang == 28.55
    assert tre.roll_ang == -28.03
    assert tre.n_ref == 0
    assert tre.rev_num == 0
    assert tre.n_seg == 0
    assert tre.max_lp_seg == 0
    assert tre.sun_el == 62.5
    assert tre.sun_az == 129.6
    subprocess.check_call(["nitf_to_vicar", "-q",
                           "use00a.ntf", "use00a.img"])
    if(VicarFile.vicar_available()):
        t = VicarRasterImage("use00a.img")
        assert t["GEOTIFF", "NITF_USE00A_ANGLE_TO_NORTH"][0] == "270"
    
# Check that GdalRasterImage got extended
def test_gdal_raster():
    t = GdalRasterImage(unit_test_data + "test_use00a.ntf")
    tre = t.use00a
    assert tre.angle_to_north == 270
    assert tre.mean_gsd == 105.2
    assert tre.dynamic_range ==2047
    assert tre.obl_ang == 34.12
    assert tre.roll_ang == -21.15
    assert tre.n_ref == 0
    assert tre.rev_num == 3317
    assert tre.n_seg == 1
    assert tre.max_lp_seg == 6287
    assert tre.sun_el == 68.5
    assert tre.sun_az == 131.3

def test_tre_rpc():
    '''Test reading and writing a RPC'''
    rpc = VicarLiteRasterImage(stereo_unit_test_data + "10MAY21-1.img").rpc
    rpc_a = rpc.rpc_type_a()
    tb = TreRPC00B()
    ta = TreRPC00A()
    # Test creating a type B rpc, and make sure we can read it back.
    tb.rpc = rpc
    rpc2 = tb.rpc
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

    # Test creating a type B rpc, and make sure we can read it back.
    ta.rpc = rpc_a
    rpc2 = ta.rpc
    assert_almost_equal(rpc_a.error_bias, rpc2.error_bias, 5)
    assert_almost_equal(rpc_a.error_random, rpc2.error_random, 5)
    assert_almost_equal(rpc_a.height_offset, rpc2.height_offset, 3)
    assert_almost_equal(rpc_a.height_scale, rpc2.height_scale, 3)
    assert_almost_equal(rpc_a.latitude_offset, rpc2.latitude_offset, 3)
    assert_almost_equal(rpc_a.latitude_scale, rpc2.latitude_scale, 3)
    assert_almost_equal(rpc_a.longitude_offset, rpc2.longitude_offset, 3)
    assert_almost_equal(rpc_a.longitude_scale, rpc2.longitude_scale, 3)
    assert_almost_equal(rpc_a.line_offset, rpc2.line_offset, 3)
    assert_almost_equal(rpc_a.line_scale, rpc2.line_scale, 3)
    assert_almost_equal(rpc_a.sample_offset, rpc2.sample_offset, 3)
    assert_almost_equal(rpc_a.sample_scale, rpc2.sample_scale, 3)
    for i in range(20):
        assert_almost_equal(rpc_a.line_numerator[i], rpc2.line_numerator[i], 6)
        assert_almost_equal(rpc_a.line_denominator[i], 
                            rpc2.line_denominator[i], 6)
        assert_almost_equal(rpc_a.sample_numerator[i], 
                            rpc2.sample_numerator[i], 6)
        assert_almost_equal(rpc_a.sample_denominator[i], 
                            rpc2.sample_denominator[i], 6)

def noruntest_nitf_rpc():
    tb = TreRPC00B()
    ta = TreRPC00A()
    assert tb.is_rpc_a == False
    assert ta.is_rpc_a == True
    # Temporary, we need to create a RPC NTF file for unit_test
    t = GdalRasterImage("/home/smyth/Local/NevadaTripletDem/10MAY21184820-P1BS-052366903050_01_P001.NTF")
    rpc = t.rpc00b.rpc
    # RPC directly from Gdal.
    rpc2 = t.rpc
    assert rpc.rpc_type == Rpc.RPC_B
    for i in range(20):
        assert rpc.fit_line_numerator[i] == False
        assert rpc.fit_sample_numerator[i] == False
    # Gdal doesn't currently read the error terms for NITF.
    #assert_almost_equal(rpc.error_bias, rpc2.error_bias, 5)
    #assert_almost_equal(rpc.error_random, rpc2.error_random, 5)
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

    # Set a TRE based on an rpc, and make sure we can get it back
    tb.rpc = rpc
    rpc2 = tb.rpc
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

    


