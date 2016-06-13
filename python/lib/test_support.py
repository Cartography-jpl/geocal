# This contains support routines for unit tests.
import numpy as np
from numpy.testing import assert_almost_equal
from unittest import SkipTest
from geocal_swig import *
import geocal_swig
import os.path
import os
import pytest

unit_test_data = os.path.dirname(__file__) + "/../../unit_test_data/"
stereo_unit_test_data = unit_test_data + "Stereo/"

# Marker that skips a test if we have a build without boost serialization
# support
require_serialize = pytest.mark.skipif(not have_serialize_supported(),
    reason="need a geocal build with boost serialization support to run")

# Marker that tests if we have HdfFile available.
require_hdf5 = pytest.mark.skipif(not hasattr(geocal_swig, "HdfFile"),
    reason="need a geocal build with HDF 5 support to run")

# Marker that tests if we have vicar available
require_vicar = pytest.mark.skipif(not VicarFile.vicar_available(),
    reason="need a geocal build with VICAR support to run")

# Marker that tests if we have afids data available
require_afids_data = pytest.mark.skipif(not "AFIDS_VDEV_DATA" in os.environ,
    reason="need to have AFIDS data available to run.")

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
