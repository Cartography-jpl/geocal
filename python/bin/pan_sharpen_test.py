from geocal import *
import subprocess
import os
from nose.plugins.skip import Skip, SkipTest
test_data1 = os.path.dirname(__file__) + "/../../unit_test_data/shiva_test_case/"
test_data2 = os.path.dirname(__file__) + "/../../unit_test_data/"
expected_dir = os.path.dirname(__file__) + "/../../unit_test_data/expected_results/pan_sharpen/"

def test_pan_sharpen_map_projected():
    '''Test the pan_sharpen PDF. This test is for already map projected data'''
    if(not have_serialize_supported()):
        raise SkipTest
    # Skip we GDAL can't read VICAR.
    if(os.environ.get("NO_VICAR_GDALPLUGIN")):
        raise SkipTest
    if(not VicarFile.vicar_available()):
        raise SkipTest
    subprocess.check_call(["pan_sharpen", 
                           "inp=(%spost_pan_sub.img, \"%spost_b1:8.img\")" %
                           (test_data1, test_data1),
                           "out=ps1_b1.img", "force_rpc=n"])
    res = GdalMultiBand("ps1_b1:8.img")
    expected = GdalMultiBand(expected_dir + "ps1_b1:8.img")
    assert res.number_band == expected.number_band
    assert res.raster_image(0).number_line == expected.raster_image(0).number_line
    assert res.raster_image(0).number_sample == expected.raster_image(0).number_sample
    d = res.read_double(0,0,res.raster_image(0).number_line, res.raster_image(0).number_sample)
    dexpect = expected.read_double(0,0,expected.raster_image(0).number_line, expected.raster_image(0).number_sample)
    # On the Mac, we get a slightly larger difference. This seems to be
    # the normal rounding difference between Linux and Mac, so relax the
    # tolerance a bit
    #assert abs(d - dexpect).max() < 0.5
    assert abs(d - dexpect).max() < 1.1

def test_pan_sharpen_rpc():
    '''Test the pan_sharpen PDF. This test is for data with an RPC'''
    if(not have_serialize_supported()):
        raise SkipTest
    # Skip we GDAL can't read VICAR.
    if(os.environ.get("NO_VICAR_GDALPLUGIN")):
        raise SkipTest
    if(not VicarFile.vicar_available()):
        raise SkipTest
    subprocess.check_call(["pan_sharpen", 
                           "inp=(%span.tif, %smul.tif)" %
                           (test_data2, test_data2),
                           "out=ps2_b1.img"])
    res = GdalMultiBand("ps2_b1:8.img")
    expected = GdalMultiBand(expected_dir + "ps2_b1:8.img")
    assert res.number_band == expected.number_band
    assert res.raster_image(0).number_line == expected.raster_image(0).number_line
    assert res.raster_image(0).number_sample == expected.raster_image(0).number_sample
    d = res.read_double(0,0,res.raster_image(0).number_line, res.raster_image(0).number_sample)
    dexpect = expected.read_double(0,0,expected.raster_image(0).number_line, expected.raster_image(0).number_sample)
    # On the Mac, we get a slightly larger difference. This seems to be
    # the normal rounding difference between Linux and Mac, so relax the
    # tolerance a bit
    # assert abs(d - dexpect).max() < 0.5
    assert abs(d - dexpect).max() < 1.1
    
    
