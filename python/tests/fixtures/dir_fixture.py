# This defines fixtures that gives the paths to various directories with
# test data
from pathlib import Path
import pytest
import warnings
from geocal import SpiceHelper


@pytest.fixture(scope="function")
def unit_test_data():
    return Path(__file__).parent.parent.parent.parent.absolute() / "unit_test_data"


@pytest.fixture(scope="function")
def stereo_unit_test_data(unit_test_data):
    return unit_test_data / "Stereo"


@pytest.fixture(scope="function")
def mspi_unit_test_data(unit_test_data):
    return unit_test_data / "mspi"


@pytest.fixture(scope="function")
def shiva_test_data(unit_test_data):
    return unit_test_data / "shiva_test_case"


# RSM sample data. This is too big to carry in our source, so we have a
# separate directory.
# This comes from http://www.gwg.nga.mil/ntb/baseline/software/testfile/rsm/samples.htm
@pytest.fixture(scope="function")
def rsm_sample_data():
    f = Path("/bigdata/smyth/SampleRsm/")
    if f.exists():
        return f
    pytest.skip("Require SampleRsm test data to run")


# Have tests that require NITF sample files be available. We skip these if not
# available, tests are nice to make sure things don't break but not essential.
# Things that really matter have small test data sets put into unit_test_data,
# but we do want the option of running larger tests when available


@pytest.fixture(scope="function")
def nitf_sample_files():
    for f in (
        Path("/bigdata/smyth/NitfSamples"),
        Path("/opt/nitf_files/NitfSamples"),
        Path("/data2/smythdata/NitfSamples/"),
        Path("/data2/smythdata/NitfSamples/"),
        Path("/Users/smyth/NitfSamples/"),
    ):
        if f.exists():
            return f
    pytest.skip("Require NitfSamples test data to run")


@pytest.fixture(scope="function")
def nitf_sample_rip(nitf_sample_files):
    # Older version, but newer version of file doesn't work with MSP software
    fname = (
        nitf_sample_files
        / "rip"
        / "07APR2005_Hyperion_331405N0442002E_SWIR172_001_L1R.ntf"
    )
    # Suppress warnings messages about MATESA. This has changed, but since
    # we can't use the newer version of the RIP we just suppress the warning
    with warnings.catch_warnings():
        warnings.filterwarnings(
            action="ignore", message=".*Error while reading TRE b'MATESA'.*"
        )
        if fname.exists():
            yield fname
        else:
            pytest.skip(f"Required file {fname} not found, so skipping test")


@pytest.fixture(scope="function")
def nitf_sample_wv2(nitf_sample_files):
    fname = nitf_sample_files / "wv2/12JAN23015358-P1BS-052654848010_01_P003.NTF"
    if fname.exists():
        return fname
    pytest.skip(f"Required file {fname} not found, so skipping test")

@pytest.fixture(scope="function")
def mars_test_data():
    if(not SpiceHelper.have_spice()):
        pytest.skip("require spice")
    res = Path("/bigdata/smyth/MiplMarsTest")
    if not res.exists:
        pytest.skip("Required mars test data not found")
    return res
    
