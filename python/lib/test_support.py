# This contains support routines for unit tests.
import numpy as np
from numpy.testing import assert_almost_equal
from unittest import SkipTest
from geocal_swig import *
import geocal_swig
import os.path
import os
import sys
import subprocess
import pytest

# Location of test data that is part of source
unit_test_data = os.path.dirname(__file__) + "/../../unit_test_data/"
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

def cmd_exists(cmd):
    '''Check if a cmd exists by using type, which returns a nonzero status if
    the program isn't found'''
    return subprocess.call("type " + cmd, shell=True, 
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE) == 0

def check_vicarb():
    '''Check for existence of vicarb program. If not found, return False'''
    if(not cmd_exists("vicarb")):
        return False
    # AFIDS uses python 3.5, so don't both supporting python 2 with this code.
    # We just skip all the tests if we are using python 2.
    if sys.version_info < (3,5):
        return False
    return True


# Marker that skips a test if we have a build without boost serialization
# support
require_serialize = pytest.mark.skipif(not have_serialize_supported(),
    reason="need a geocal build with boost serialization support to run")

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

# Marker that test requires the geocal test data in /data/geocal_test_data
require_geocal_test_data = pytest.mark.skipif(not os.path.exists(geocal_test_igc),
    reason="need to have geocal test data (/data/geocal_test_data) available to run.")

# Marker that test requires the vicar gdal plugin
require_vicar_gdalplugin = pytest.mark.skipif(os.environ.get("NO_VICAR_GDALPLUGIN"),
    reason = "need the VICAR GDAL plugin to run.")

# Short hand for marking as unconditional skipping. Good for tests we
# don't normally run, but might want to comment out for a specific debugging
# reason.
skip = pytest.mark.skip

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
        yield
    finally:
        os.chdir(curdir)
