# This contains support routines for unit tests.
from numpy.testing import assert_almost_equal
from nose.plugins.skip import Skip, SkipTest
from geocal_swig import *
import os.path
import pytest

unit_test_data = os.path.dirname(__file__) + "/../../unit_test_data/"
stereo_unit_test_data = unit_test_data + "Stereo/"

# Marker that skips a test if we have a build without boost serialization
# support
require_serialize = pytest.mark.skipif(not have_serialize_supported(),
    reason="need a geocal build with boost serialization support to run")

# Short hand for marking as unconditional skipping. Good for tests we
# don't normally run, but might want to comment out for a specific debugging
# reason.
skip = pytest.mark.skip
