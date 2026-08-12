# Fixtures that don't really fit in one of the other files.
import pytest
import os
from geocal import Rpc


@pytest.fixture(scope="function")
def isolated_dir(tmpdir):
    """This is a fixture that creates a temporary directory, and uses this
    while running a unit tests. Useful for tests that write out a test file
    and then try to read it.

    This fixture changes into the temporary directory, and at the end of
    the test it changes back to the current directory.

    Note that this uses the fixture tmpdir, which keeps around the last few
    temporary directories (cleaning up after a fixed number are generated).
    So if a test fails, you can look at the output at the location of tmpdir,
    e.g. /tmp/pytest-of-smyth
    """
    curdir = os.getcwd()
    try:
        tmpdir.chdir()
        yield curdir
    finally:
        os.chdir(curdir)


@pytest.fixture(scope="function")
def rpc():
    """Basic RPC that can be used in testing. Nothing special about this, it
    is just a sample RPC I tend to use in the C++ testing so we duplicate it
    here."""
    r = Rpc()
    r.error_bias = 0
    r.error_random = 0
    r.rpc_type = Rpc.RPC_B
    r.line_offset = 2881
    r.line_scale = 2921
    r.sample_offset = 13763
    r.sample_scale = 14238
    r.latitude_offset = 35.8606
    r.latitude_scale = 0.0209
    r.longitude_offset = 44.9534
    r.longitude_scale = 0.1239
    r.height_offset = 1017
    r.height_scale = 634
    r.line_numerator = [
        0.0132748,
        -0.14751,
        -1.13465,
        -0.0138959,
        0.0020018,
        6.35242e-05,
        0.000115861,
        -0.00286551,
        -0.00778726,
        3.88308e-06,
        -1.26487e-06,
        7.881069999999999e-06,
        3.65929e-05,
        2.32154e-06,
        -2.25421e-05,
        -2.08933e-05,
        1.8091e-05,
        3.6393e-07,
        -9.39815e-07,
        -4.31269e-08,
    ]
    r.line_denominator = [
        1,
        0.00380256,
        0.00643151,
        0.00031479,
        1.84958e-05,
        -1.98243e-06,
        -1.16422e-06,
        -1.92637e-05,
        7.224010000000001e-05,
        -1.61779e-05,
        4.95049e-06,
        1.26794e-06,
        0.000190771,
        -1.57429e-07,
        2.46815e-05,
        0.00049166,
        -5.41022e-07,
        3.209e-07,
        1.81401e-05,
        1.43592e-07,
    ]
    r.sample_numerator = [
        -0.0104025,
        0.96885,
        -0.000487887,
        -0.0325142,
        -0.000710444,
        0.000217572,
        -6.549690000000001e-05,
        0.0107384,
        -5.19453e-06,
        -1.76761e-05,
        -1.21058e-06,
        0.000106017,
        5.41226e-06,
        -3.8968e-06,
        1.03307e-05,
        5.84016e-05,
        3.80777e-08,
        9.01765e-06,
        1.65988e-06,
        -1.19374e-07,
    ]
    r.sample_denominator = [
        1,
        -0.000319831,
        0.000681092,
        -0.000549762,
        -2.67977e-06,
        -6.19388e-06,
        2.67975e-08,
        4.76371e-06,
        -1.47985e-05,
        -4.23457e-06,
        1.44009e-08,
        -1.07213e-06,
        1.1059e-07,
        4.10217e-08,
        -1.69482e-07,
        1.08104e-06,
        1e-9,
        -2.33038e-07,
        1.86413e-08,
        -1.35637e-08,
    ]
    return r
