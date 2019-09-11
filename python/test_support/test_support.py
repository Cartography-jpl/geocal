# This contains support routines for unit tests.
import numpy as np
from numpy.testing import assert_almost_equal, assert_approx_equal
from unittest import SkipTest
from geocal_swig import *
import geocal_swig
import os.path
import os
import sys
import subprocess
import pytest
try:
    from pynitf import NitfFile
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False

# In any case, for python < 3 we can't use pynitf
if sys.version_info < (3,):
    have_pynitf = False
    
# Location of test data that is part of source
unit_test_data = os.path.abspath(os.path.dirname(__file__) + "/../../unit_test_data/") + "/"
stereo_unit_test_data = unit_test_data + "Stereo/"
mspi_test_data = unit_test_data + "mspi/"
shiva_test_data = unit_test_data + "shiva_test_case/"

# Data is way too big to check into source, so we put it here. This means
# we have tests that can only be run on pistol. We may fold this into
# the afids data area at some point.
geocal_test_data = "/data/geocal_test_data/"
geocal_test_igc = geocal_test_data + "igccol_rolling_shutter.xml"
geocal_test_igc_sim_error = geocal_test_data + "igccol_rolling_shutter_simulated_error.xml"
geocal_test_tpcol = geocal_test_data + "tpcol.xml"

# RSM sample data. This is too big to carry in our source, so we have a
# separate directory.
# This comes from http://www.gwg.nga.mil/ntb/baseline/software/testfile/rsm/samples.htm
rsm_sample_data = "/data/smyth/SampleRsm/"

# Have tests that require NITF sample files be available. We skip these if not
# available, tests are nice to make sure things don't break but not essential.
# Things that really matter have small test data sets put into unit_test_data,
# but we do want the option of running larger tests when available

@pytest.yield_fixture(scope="function")
def nitf_sample_files(isolated_dir):
    if(os.path.exists("/raid1/smyth/NitfSamples/")):
        return "/raid1/smyth/NitfSamples/"
    elif(os.path.exists("/opt/nitf_files/NitfSamples/")):
        return "/opt/nitf_files/NitfSamples/"
    pytest.skip("Require NitfSamples test data to run")

@pytest.yield_fixture(scope="function")
def nitf_sample_rip(nitf_sample_files):
    fname = nitf_sample_files + "rip/07APR2005_Hyperion_331405N0442002E_SWIR172_001_L1R.ntf"
    if(os.path.exists(fname)):
        return fname
    pytest.skip("Required file %s not found, so skipping test" % fname)
    
def cmd_exists(cmd):
    '''Check if a cmd exists by using type, which returns a nonzero status if
    the program isn't found'''
    return subprocess.call("type " + cmd, shell=True, 
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE) == 0

def check_vicarb():
    '''Check for existence of vicarb program. If not found, return False'''
    if(not cmd_exists("vicarb")):
        return False
    # AFIDS uses python 3.5, so don't bother supporting python 2 with this code.
    # We just skip all the tests if we are using python 2.
    if sys.version_info < (3,5):
        return False
    # We may have vicarb in our path, but not have actually set up AFIDS.
    # Will this can sort of work, but mostly this causes problems since
    # expected vicar procs aren't in the TAE_PATH. So say we  don't have
    # vicarb if we don't have AFIDS setup, even if we find it in our path.
    # We can revisit this if it becomes an issue, perhaps a refined
    # require_vicarb_and_afids or something like that.
    if("AFIDSTOP" not in os.environ):
        return False
    return True


# Marker that skips a test if we have a build without boost serialization
# support
require_serialize = pytest.mark.skipif(not have_serialize_supported(),
    reason="need a geocal build with boost serialization support to run")

# Marker that skips a test if we have a build without spice
# support
require_spice = pytest.mark.skipif(not SpiceHelper.have_spice(),
    reason="need a geocal build with spice support to run")

# Marker that skips a test if we have a build without MSP
# support
require_msp = pytest.mark.skipif(not have_msp_supported(),
    reason="need a geocal build with MSP support to run")

# Marker that skips a test if we don't have mars data
require_mars_spice = pytest.mark.skipif(not SpiceHelper.have_spice() or
       not "MARS_KERNEL" in os.environ or
       not os.path.exists(os.environ["MARS_KERNEL"] + "/mro_kernel/mro.ker") or
       not os.path.exists(os.environ["MARS_KERNEL"] + "/mex_kernel/mex.ker"),
       reason="need a geocal build with spice support, and the mars spice kernels available to run")

                                       
# Marker that tests if we have HdfFile available.
require_hdf5 = pytest.mark.skipif(not hasattr(geocal_swig, "HdfFile"),
    reason="need a geocal build with HDF 5 support to run")

# Marker that tests if we have VICAR RTL available
require_vicar = pytest.mark.skipif(not VicarFile.vicar_available(),
    reason="need a geocal build with VICAR support to run")

# Marker that tests if we have vicarb command available
require_vicarb = pytest.mark.skipif(not check_vicarb(),
    reason="need to have vicarb available to run")

# Marker that tests if we have Carto library available.
require_carto = pytest.mark.skipif(not hasattr(geocal_swig, "EciTodBurl"),
    reason="need a geocal build with carto library support to run")

# Marker that tests if we have the VICAR 
# Marker that tests if we have afids data available
require_afids_data = pytest.mark.skipif(not "AFIDS_VDEV_DATA" in os.environ,
    reason="need to have AFIDS data available to run.")

require_srtm_data = pytest.mark.skipif(not "ELEV_ROOT" in os.environ or not os.path.exists(os.environ["ELEV_ROOT"]),
    reason="need to have SRTM data available to run.")

# Marker that test requires the geocal test data in /data/geocal_test_data
require_geocal_test_data = pytest.mark.skipif(not os.path.exists(geocal_test_igc),
    reason="need to have geocal test data (/data/geocal_test_data) available to run.")

# Marker that test requires the vicar gdal plugin
require_vicar_gdalplugin = pytest.mark.skipif("NO_VICAR_GDALPLUGIN" in os.environ,
    reason = "need the VICAR GDAL plugin to run.")

require_rsm_sample_data = pytest.mark.skipif(not os.path.exists(rsm_sample_data),
      reason="need to have RSM sample data (%s) available to run." % rsm_sample_data)

require_pynitf = pytest.mark.skipif(not have_pynitf,
      reason="need to have pynitf available to run.")

# Short hand for marking as unconditional skipping. Good for tests we
# don't normally run, but might want to comment out for a specific debugging
# reason.
skip = pytest.mark.skip

# Some code depends on rsync. We don't want to make this a strict requirement
# for geocal, just have the code fail if rsync isn't found. So skip tests
# that depend on this.
require_rsync = pytest.mark.skipif(not cmd_exists("rsync"),
       reason = "need rsync on system to run.")

# Some tests are python 3 only. Don't want the python 2 tests to fail for
# python code that we know can't be run
require_python3 = pytest.mark.skipif(not sys.version_info > (3,),
       reason = "require python 3 to run")                                     

# Have tests that require /raid be available. We generally skip these if not
# available, tests are nice to make sure things don't break but not essential.
# Things that really matter have small test data sets put into unit_test_data,
# but we do want the option of running larger tests when available
#require_raid = pytest.mark.skipif(not os.path.exists("/raid1"),
#                                  reason = "require /raid* test data to run")
# Temporary, /raid is down.
require_raid = pytest.mark.skipif(True,
                                  reason = "require /raid* test data to run")

@pytest.yield_fixture(scope="function")
def isolated_dir(tmpdir):
    '''This is a fixture that creates a temporary directory, and uses this
    while running a unit tests. Useful for tests that write out a test file
    and then try to read it.

    This fixture changes into the temporary directory, and at the end of
    the test it changes back to the current directory.

    Note that this uses the fixture tmpdir, which keeps around the last few
    temporary directories (cleaning up after a fixed number are generated).
    So if a test fails, you can look at the output at the location of tmpdir, 
    e.g. /tmp/pytest-of-smyth
    '''
    curdir = os.getcwd()
    try:
        tmpdir.chdir()
        yield curdir
    finally:
        os.chdir(curdir)

matlab = pytest.mark.matlab
long_test = pytest.mark.long_test
        
@pytest.fixture(scope="function")
def rpc():
    '''Basic RPC that can be used in testing. Nothing special about this, it
    is just a sample RPC I tend to use in the C++ testing so we duplicate it
    here.'''
    r = Rpc()
    r.error_bias = 0;
    r.error_random = 0;
    r.rpc_type = Rpc.RPC_B;
    r.line_offset = 2881;
    r.line_scale = 2921;
    r.sample_offset = 13763;
    r.sample_scale = 14238;
    r.latitude_offset = 35.8606;
    r.latitude_scale = 0.0209;
    r.longitude_offset = 44.9534;
    r.longitude_scale = 0.1239;
    r.height_offset = 1017;
    r.height_scale = 634;
    r.line_numerator = [0.0132748, -0.14751, -1.13465, -0.0138959, 0.0020018,
      6.35242e-05, 0.000115861, -0.00286551, -0.00778726, 
      3.88308e-06, -1.26487e-06, 7.881069999999999e-06, 
      3.65929e-05, 2.32154e-06, -2.25421e-05, -2.08933e-05, 
       1.8091e-05, 3.6393e-07, -9.39815e-07, -4.31269e-08]
    r.line_denominator = [ 1, 0.00380256, 0.00643151, 0.00031479,
      1.84958e-05, -1.98243e-06, -1.16422e-06,
      -1.92637e-05, 7.224010000000001e-05, -1.61779e-05,
      4.95049e-06, 1.26794e-06, 0.000190771, -1.57429e-07,
      2.46815e-05, 0.00049166, -5.41022e-07, 3.209e-07,
       1.81401e-05, 1.43592e-07 ]
    r.sample_numerator = [-0.0104025, 0.96885, -0.000487887, -0.0325142,
      -0.000710444, 0.000217572, -6.549690000000001e-05,
      0.0107384, -5.19453e-06, -1.76761e-05, -1.21058e-06,
      0.000106017, 5.41226e-06, -3.8968e-06, 1.03307e-05,
      5.84016e-05, 3.80777e-08, 9.01765e-06, 1.65988e-06,
       -1.19374e-07]
    r.sample_denominator = [1, -0.000319831, 0.000681092, -0.000549762,
     -2.67977e-06, -6.19388e-06, 2.67975e-08, 4.76371e-06,
     -1.47985e-05, -4.23457e-06, 1.44009e-08, -1.07213e-06,
     1.1059e-07, 4.10217e-08, -1.69482e-07, 1.08104e-06,
      1e-9, -2.33038e-07, 1.86413e-08, -1.35637e-08]
    return r

@pytest.fixture(scope="function")
def igc_rpc(rpc):
    image = MemoryRasterImage(int(rpc.line_offset * 2),
                              int(rpc.sample_offset * 2), 0)
    return RpcImageGroundConnection(rpc, SimpleDem(), image)

@pytest.fixture(scope="function")
def rsm_rational_polynomial(igc_rpc):
    '''Create a RsmRationalPolynomial that matches our rpc test fixture'''
    r = RsmRationalPolynomial(3,3,3,3,3,3,3,3)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale 
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(igc_rpc, GeodeticRadianConverter(), hmin, hmax, 0, igc_rpc.number_line,
          0, igc_rpc.number_sample)
    return r

@pytest.fixture(scope="function")
def rsm_rational_polynomial_lc(igc_rpc):
    '''Create a RsmRationalPolynomial that matches our rpc test fixture'''
    r = RsmRationalPolynomial(3,3,3,3,3,3,3,3)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale 
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(igc_rpc, LocalRcConverter(LocalRcParameter(igc_rpc)), hmin, hmax, 0,
          igc_rpc.number_line, 0, igc_rpc.number_sample)
    return r

@pytest.fixture(scope="function")
def rsm_grid(igc_rpc):
    '''Create a RsmGrid that matches our rpc test fixture'''
    r = RsmGrid(40,40,2)
    r.total_number_row_digit = 8
    r.total_number_col_digit = 8
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale 
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(igc_rpc, GeodeticRadianConverter(), hmin, hmax, 0, igc_rpc.number_line,
          0, igc_rpc.number_sample)
    return r

@pytest.fixture(scope="function")
def rsm_ms_polynomial(igc_rpc):
    rp = RsmRationalPolynomial(3,3,3,3,3,3,3,3)
    res = RsmMultiSection(igc_rpc.number_line, igc_rpc.number_sample, 3, 2, rp)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale 
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    res.fit(igc_rpc, GeodeticRadianConverter(), hmin, hmax, 0, igc_rpc.number_line, 0,
	    igc_rpc.number_sample)
    return res

# Note the polynomial here tends to have a few poles, since there are zeros
# in the denominator. Our code works fine with this, but the MSP code fails.
# This is in some sense not a "real" failure, so we use a different polynomial
# for this. The down side is that this is slow (taking a minute or so),
# so we cache the results. If the read here fails, just regenerate the data.
@pytest.fixture(scope="function")
def rsm_ms_polynomial_np(igc_rpc):
    fname = unit_test_data + "rsm_ms_polynomial_np.xml"
    if(False):
        rp = RsmRationalPolynomial(5,5,3,1,1,1)
        res = RsmMultiSection(igc_rpc.number_line, igc_rpc.number_sample,
                              3, 2, rp)
        hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale 
        hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
        res.fit(igc_rpc, GeodeticRadianConverter(), hmin, hmax, 0,
                igc_rpc.number_line, 0, igc_rpc.number_sample)
        geocal_swig.serialize_write(fname, res)
    return geocal_swig.serialize_read_generic(fname)

@pytest.fixture(scope="function")
def rsm_ms_grid(igc_rpc):
    rg = RsmGrid(10,10,2)
    rg.total_number_row_digit = 8
    rg.total_number_col_digit = 8
    res = RsmMultiSection(igc_rpc.number_line, igc_rpc.number_sample, 3, 2, rg)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale 
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    res.fit(igc_rpc, GeodeticRadianConverter(), hmin, hmax, 0, igc_rpc.number_line, 0,
	    igc_rpc.number_sample)
    return res

@pytest.fixture(scope="function")
def rsm_rp_cgrid(igc_rpc):
    '''Create a RsmRationalPolynomial with a correction grid that 
    matches our rpc test fixture'''
    rp = RsmRationalPolynomial(2,2,2,1,1,1)
    g = RsmGrid(10,10,3)
    r = RsmRpPlusGrid(rp, g)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale 
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(igc_rpc, GeodeticRadianConverter(), hmin, hmax, 0, igc_rpc.number_line,
          0, igc_rpc.number_sample)
    res = Rsm(r, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(500, 1500)
    return res

@pytest.fixture(scope="function")
def rsm(rsm_rational_polynomial):
    res = Rsm(rsm_rational_polynomial, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(500, 1500)
    return res

@pytest.fixture(scope="function")
def rsm_lc(rsm_rational_polynomial_lc, igc_rpc):
    res = Rsm(rsm_rational_polynomial_lc,
              LocalRcConverter(LocalRcParameter(igc_rpc)))
    res.fill_in_ground_domain_vertex(500, 1500)
    return res

@pytest.fixture(scope="function")
def rsm_g(rsm_grid):
    res = Rsm(rsm_grid, GeodeticRadianConverter())
    # This fails. We'll have to figure out what the problem is and fix it,
    # but in the mean time we put dummy data in so we can bypass this problem
    # A warning will get printed out
    res.fill_in_ground_domain_vertex(500, 1500)
    res.rsm_id.min_line = rsm_grid.min_line
    res.rsm_id.max_line = rsm_grid.max_line
    res.rsm_id.min_sample = rsm_grid.min_sample
    res.rsm_id.max_sample = rsm_grid.max_sample
    return res

@pytest.fixture(scope="function")
def rsm_ms_rp(rsm_ms_polynomial):
    res = Rsm(rsm_ms_polynomial, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(500, 1500)
    return res

@pytest.fixture(scope="function")
def rsm_ms_rp_np(rsm_ms_polynomial_np):
    res = Rsm(rsm_ms_polynomial_np, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(500, 1500)
    return res

@pytest.fixture(scope="function")
def rsm_ms_g(rsm_ms_grid):
    res = Rsm(rsm_ms_grid, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(500, 1500)
    return res

@pytest.fixture(scope="module")
def mars_kernel():
    # Since this is module level fixture, we need to check before running
    # spice setup. The tests should be skipped by the require_mars_spice,
    # but that is only checked when we get to the functions
    if(SpiceHelper.have_spice() and
       "MARS_KERNEL" in os.environ and
       os.path.exists(os.environ["MARS_KERNEL"] + "/mro_kernel/mro.ker") and
       os.path.exists(os.environ["MARS_KERNEL"] + "/mex_kernel/mex.ker")):
       SpiceHelper.add_kernel(os.environ["MARS_KERNEL"] + "/mro_kernel/mro.ker")
       SpiceHelper.add_kernel(os.environ["MARS_KERNEL"] + "/mex_kernel/mex.ker")

@pytest.fixture(scope="function")
def mars_test_data():
    if(not SpiceHelper.have_spice() or
       not "MARS_KERNEL" in os.environ or
       not os.path.exists(os.environ["MARS_KERNEL"] + "/mro_kernel/mro.ker") or
       not os.path.exists(os.environ["MARS_KERNEL"] + "/mro_kernel/kernel.json") or
       not os.path.exists(os.environ["MARS_KERNEL"] + "/mex_kernel/mex.ker") or
       not os.path.exists(os.environ["MARS_KERNEL"] + "/mes_kernel/kernel.json")
    ):
        raise SkipTest
    res = "/raid27/smyth/MiplMarsTest/"
    if(not os.path.exists(res)):
        raise SkipTest
    return res
                                       
       
