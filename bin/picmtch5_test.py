from geocal import *
import subprocess
import re
from vicar_test_support import *

# Setup and teardown called automatically by nosetest for whole module.
# We assume that picmtch5 is in current directory, which is true if we
# call nosetest from the build directory
do_cleanup = True
original_env = None
prefix = "picmtch5_xx"
def setup():
    if(not VicarFile.vicar_available()):
        raise SkipTest
    check_vicarb()
    add_tae_path(os.path.abspath(os.getcwd()), original_env)
    # Check for picmtch5. Depending on how we are built we may or may not
    # have this available. If not available, then just skip these tests.
    if not check_for_proc("picmtch5"):
        raise SkipTest
    vicarb_run("gen picmtch5_xxim0 nl=300 ns=310")
    vicarb_run("f2 inp=picmtch5_xxim0 out=picmtch5_xxim1 func=\"(line*327+samp*193+line*samp)%256\"")
    vicarb_run('''+
gtgen inp=picmtch5_xxim1 'tiecnvrt +
   geotiff=("ModelTiePointTag=(0,0,0,.3,.3,0.0)", +
          "ModelTiePointTag=(300,0,0,.5,.3,0.0)", +
          "ModelTiePointTag=(0,300,0,.3,.5,0.0)", +
          "ProjectionGeoKey=20(CT_MillerCylindrical)", +
          "GTRasterTypeGeoKey=2(RasterPixelIsPoint)", +
          "GeogEllipsoidGeoKey=7030(Ellipse_WGS84)") +
''')
    vicarb_run("gtcopy picmtch5_xxim1 picmtch5_xxim2 size=(5,6,295,304)")

def teardown():
    set_original_env(original_env)
    # Clean up the various temporary files
    if do_cleanup:
        subprocess.run("rm %s*" % prefix, shell=True,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)

def test_perfect_predictor():
    '''Test with a perfect predictor.'''
    IbisFile.create("picmtch5_xxa", np.array([[150.0,150,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
  ITIE=(5,6,5,306,305,6) +
  OTIE=(1,1,1,301,301,1) SEARCH=32 fftsize=32 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 146.0)
    assert_approx_equal(ic.sample, 145.0)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 124.4722)
    assert_approx_equal(z2, 124.4722)
    assert corr_val > 0.4

def test_off_predictor_scale_ok():
    '''Have predictor off by 6+ pixel, but scale ok'''
    IbisFile.create("picmtch5_xxa", np.array([[150.0,150,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
  ITIE=(1,1,1,301,301,1) +
  OTIE=(1,1,1,301,301,1) SEARCH=32 fftsize=32 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 146.006, 6)
    assert_approx_equal(ic.sample, 144.936, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 124.4722)
    assert_approx_equal(z2, 124.2989)
    assert corr_val > 0.25

def test_scale_off():
    '''Have predictor off by scale, but close at the point.'''
    IbisFile.create("picmtch5_xxa", np.array([[150.0,150,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
  ITIE=(1,2,1,310,309,2) +
  OTIE=(1,1,1,301,301,1) SEARCH=96 fftsize=32 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 146.101, 6)
    assert_approx_equal(ic.sample, 145.175, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 124.4722)
    assert_approx_equal(z2, 124.2038)
    assert corr_val > 0.08

def test_perfect_predictor_geographic():
    '''Test with a perfect predictor. This is test_perfect_predictor but
    using geographic coordinates.'''
    IbisFile.create("picmtch5_xxa", np.array([[0.45,0.44,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) 'geocord1 'geocord2 +
  ITIE=(0.31,0.31,0.31,0.49,0.49,0.31) +
  OTIE=(0.31,0.31,0.31,0.49,0.49,0.31) SEARCH=32  fftsize=32 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 207, 6)
    assert_approx_equal(ic.sample, 221, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 125.5278)
    assert_approx_equal(z2, 125.5278)
    assert corr_val > 0.4

def test_magnify():
    '''Magnify case'''
    IbisFile.create("picmtch5_xxa", np.array([[150.0,150,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
  ITIE=(5,6,5,306,305,6) +
  OTIE=(1,1,1,301,301,1) SEARCH=32 magnify=(7.0,7.0) fftsize=32 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 146, 6)
    assert_approx_equal(ic.sample, 145, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 136.0278)
    assert_approx_equal(z2, 136.0278)
    assert corr_val > 0.4

def test_64x64():
    '''64x64 case'''
    IbisFile.create("picmtch5_xxa", np.array([[150.0,150,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
  ITIE=(5,6,5,306,305,6) +
  OTIE=(1,1,1,301,302,1) SEARCH=64 fftsize=64 minsrch=64 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 145.9786, 6)
    assert_approx_equal(ic.sample, 144.9949, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 124.4722)
    assert_approx_equal(z2, 124.6039)
    assert corr_val > 0.3

def test_no_itie_otie():
    '''no itie-otie case'''
    IbisFile.create("picmtch5_xxa", np.array([[150.0,150,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
SEARCH=96 fftsize=64 minsrch=96 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 145.9910, 6)
    assert_approx_equal(ic.sample, 144.9979, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 124.4722)
    assert_approx_equal(z2, 124.5224)
    assert corr_val > 0.25

def test_no_itie_otie_geocord():
    '''no itie-otie case with geocord keywords'''
    IbisFile.create("picmtch5_xxa", np.array([[0.45,0.44,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
 SEARCH=64 fftsize=64 minsrch=64 +
 'geocord1 'geocord2 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 207, 6)
    assert_approx_equal(ic.sample, 221, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 125.5278)
    assert_approx_equal(z2, 125.5278)
    assert corr_val > 0.4

def test_no_itie_otie_rotated_geocord():
    '''no itie-otie case with rotated 2d image and geocord'''
    vicarb_run("gtrot picmtch5_xxim2 picmtch5_xxim3 rot=2")
    IbisFile.create("picmtch5_xxa", np.array([[0.45,0.44,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim3,picmtch5_xxa) +
 SEARCH=64 fftsize=64 minsrch=64 +
 'geocord1 'geocord2 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 84, 6)
    assert_approx_equal(ic.sample, 89, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 125.5278)
    assert_approx_equal(z2, 125.5278)
    assert corr_val > 0.4

def test_redo():
    '''no itie-otie case with rotated 2d image, pixel data; also redo case'''
    vicarb_run("gtrot picmtch5_xxim2 picmtch5_xxim3 rot=2")
    IbisFile.create("picmtch5_xxa", np.array([[140.0,130,0,0,0,0,0,0,0,0,0]]))
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim3,picmtch5_xxa) +
SEARCH=64 fftsize=64 minsrch=64 redo=1
''')
    f = IbisFile("picmtch5_xxa")
    ic = VicarImageCoordinate(f[0,5], f[0,6])
    corr_val = f[0,8]
    assert_approx_equal(ic.line, 180, 6)
    assert_approx_equal(ic.sample, 160, 6)
    z1 = f[0,4]
    z2 = f[0,7]
    assert_approx_equal(z1, 128.3611)
    assert_approx_equal(z2, 128.3611)
    assert corr_val > 0.4

def test_edge_case():
    '''the four edge cases for getgrid'''
    d = np.zeros((5,11))
    d[:,0:2] = [[240,238],
                [30,30],
                [270,270],
                [20,20],
                [100,100]]
    IbisFile.create("picmtch5_xxa", d)
    vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
  ITIE=(5,6,5,306,305,6) +
  OTIE=(1,1,1,301,302,1) SEARCH=64 fftsize=64 minsrch=64 redo=0
''')
    f = IbisFile("picmtch5_xxa")
    ic0 = VicarImageCoordinate(f[0,5], f[0,6])
    ic1 = VicarImageCoordinate(f[1,5], f[1,6])
    ic2 = VicarImageCoordinate(f[2,5], f[2,6])
    ic3 = VicarImageCoordinate(f[3,5], f[3,6])
    ic4 = VicarImageCoordinate(f[4,5], f[4,6])
    z0_1 = f[0,4]
    z0_2 = f[0,7]
    z1_1 = f[1,4]
    z1_2 = f[1,7]
    z2_1 = f[2,4]
    z2_2 = f[2,7]
    z3_1 = f[3,4]
    z3_2 = f[3,7]
    z4_1 = f[4,4]
    z4_2 = f[4,7]
    corr_val0 = f[0,8]
    corr_val1 = f[1,8]
    corr_val2 = f[2,8]
    corr_val3 = f[3,8]
    corr_val4 = f[4,8]
    assert_approx_equal(ic0.line, 235.858, 6)
    assert_approx_equal(ic0.sample, 233.008, 6)
    assert_approx_equal(z0_1, 123.9167)
    assert_approx_equal(z0_2, 124.7971)
    assert_approx_equal(ic4.line, 96.2261, 6)
    assert_approx_equal(ic4.sample, 95.002, 6)
    assert_approx_equal(z4_1, 130.9167)
    assert_approx_equal(z4_2, 129.0418)
    assert corr_val0 > 0.4
    assert corr_val1 == -9999.0
    assert corr_val2 == -9999.0
    assert corr_val3 == -9999.0
    assert corr_val4 > 0.4

def test_autothresh():
    '''autothresh case'''
    vicarb_run('''+
gengrid2 out=picmtch5_xxa ncol=11 nah=5 nav=7 +
     l1=145 s1=145 l2=145 s2=155 l3=155 s3=145 l4=155 s4=155
''')
    res = vicarb_run('''+
picmtch5 (picmtch5_xxim1,picmtch5_xxim2,picmtch5_xxa) +
  ITIE=(5,6,5,306,305,6) +
  OTIE=(1,1,1,301,302,1) SEARCH=64 fftsize=32 redo=0 autofit=12
''')
    # Not a lot of checks done in the original picmtch4 tests, really just
    # look at the output and visually inspect it. This doesn't map well to 
    # nosetests, so we don't normally print the output and just check the
    # number of good matches. But we can look at the output if we need to
    # debug something.
    if(False):
        print(res.stdout.decode('utf-8'))
    f = IbisFile("picmtch5_xxa")
    # This counts the number of good matches
    assert np.count_nonzero(np.array(f[:,8]) > 0.1) == 37
