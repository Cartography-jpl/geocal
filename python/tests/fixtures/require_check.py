# Check for various things needed for tests

from geocal import (
    have_serialize_supported,
    VicarFile,
    SpiceHelper,
    setup_isis,
    have_msp
)
import geocal_swig
import os
import sys
import pytest
import subprocess

have_pynitf = False
try:
    # Comment turns off lint. We are testing an import, that we don't actually
    # do anything with. But want to make sure the import is successful
    from pynitf import NitfFile  # noqa: F401

    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    pass

# In any case, for python < 3 we can't use pynitf
if sys.version_info < (3,):
    have_pynitf = False

# Data is way too big to check into source, so we put it here. This means
# we have tests that can only be run on pistol. We may fold this into
# the afids data area at some point.
geocal_test_data = "/data/geocal_test_data/"
geocal_test_igc = geocal_test_data + "igccol_rolling_shutter.xml"
geocal_test_igc_sim_error = (
    geocal_test_data + "igccol_rolling_shutter_simulated_error.xml"
)
geocal_test_tpcol = geocal_test_data + "tpcol.xml"

# RSM sample data. This is too big to carry in our source, so we have a
# separate directory.
# This comes from http://www.gwg.nga.mil/ntb/baseline/software/testfile/rsm/samples.htm
rsm_sample_data = "/bigdata/smyth/SampleRsm/"


def cmd_exists(cmd):
    """Check if a cmd exists by using type, which returns a nonzero status if
    the program isn't found"""
    return (
        subprocess.call(
            "type " + cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )
        == 0
    )


def check_vicarb():
    """Check for existence of vicarb program. If not found, return False"""
    if not cmd_exists("vicarb"):
        return False
    # AFIDS uses python 3.5, so don't bother supporting python 2 with this code.
    # We just skip all the tests if we are using python 2.
    if sys.version_info < (3, 5):
        return False
    # We may have vicarb in our path, but not have actually set up AFIDS.
    # Will this can sort of work, but mostly this causes problems since
    # expected vicar procs aren't in the TAE_PATH. So say we  don't have
    # vicarb if we don't have AFIDS setup, even if we find it in our path.
    # We can revisit this if it becomes an issue, perhaps a refined
    # require_vicarb_and_afids or something like that.
    if "AFIDSTOP" not in os.environ:
        return False
    return True


def check_isis():
    """Check for the existence of isis code"""
    try:
        setup_isis()
    except RuntimeError:
        return False
    return (
        subprocess.call(
            "type $ISISROOT/bin/spiceinit",
            shell=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        == 0
    )


# Marker that skips a test if we have a build without boost serialization
# support
require_serialize = pytest.mark.skipif(
    not have_serialize_supported(),
    reason="need a geocal build with boost serialization support to run",
)

# Marker that skips a test if we have a build without spice
# support
require_spice = pytest.mark.skipif(
    not SpiceHelper.have_spice(), reason="need a geocal build with spice support to run"
)

# Marker that skips a test if we have a build without MSP
# support
require_msp = pytest.mark.skipif(
    not have_msp, reason="need a geocal build with MSP support to run"
)

# Marker that skips a test if we don't have mars data
require_mars_spice = pytest.mark.skipif(
    not SpiceHelper.have_spice()
    or "MARS_KERNEL" not in os.environ
    or not os.path.exists(os.environ["MARS_KERNEL"] + "/mro_kernel/mro.ker")
    or not os.path.exists(os.environ["MARS_KERNEL"] + "/mro_kernel/kernel.json")
    or not os.path.exists(os.environ["MARS_KERNEL"] + "/mex_kernel/mex.ker")
    or not os.path.exists(os.environ["MARS_KERNEL"] + "/mex_kernel/kernel.json"),
    reason="need a geocal build with spice support, and the mars spice kernels available to run",
)


# Marker that tests if we have HdfFile available.
require_hdf5 = pytest.mark.skipif(
    not hasattr(geocal_swig, "HdfFile"),
    reason="need a geocal build with HDF 5 support to run",
)

# Marker that tests if we have VICAR RTL available
require_vicar = pytest.mark.skipif(
    not VicarFile.vicar_available(),
    reason="need a geocal build with VICAR support to run",
)

require_isis = pytest.mark.skipif(
    not check_isis(), reason="need to have ISIS available to run"
)

# Marker that tests if we have vicarb command available
require_vicarb = pytest.mark.skipif(
    not check_vicarb(), reason="need to have vicarb available to run"
)

# Marker that tests if we have Carto library available.
require_carto = pytest.mark.skipif(
    not hasattr(geocal_swig, "EciTodBurl"),
    reason="need a geocal build with carto library support to run",
)

# Marker that tests if we have the VICAR
# Marker that tests if we have afids data available
require_afids_data = pytest.mark.skipif(
    "AFIDS_VDEV_DATA" not in os.environ,
    reason="need to have AFIDS data available to run.",
)

require_srtm_data = pytest.mark.skipif(
    "ELEV_ROOT" not in os.environ or not os.path.exists(os.environ["ELEV_ROOT"]),
    reason="need to have SRTM data available to run.",
)

require_panchromatic_orthobase = pytest.mark.skipif(
    not os.path.exists("/raid22/band8_VICAR"),
    reason="need to have orthobase data available to run.",
)

# Marker that test requires the geocal test data in /data/geocal_test_data
require_geocal_test_data = pytest.mark.skipif(
    not os.path.exists(geocal_test_igc),
    reason="need to have geocal test data (/data/geocal_test_data) available to run.",
)

# Marker that test requires the vicar gdal plugin
require_vicar_gdalplugin = pytest.mark.skipif(
    "NO_VICAR_GDALPLUGIN" in os.environ, reason="need the VICAR GDAL plugin to run."
)

require_rsm_sample_data = pytest.mark.skipif(
    not os.path.exists(rsm_sample_data),
    reason="need to have RSM sample data (%s) available to run." % rsm_sample_data,
)

require_pynitf = pytest.mark.skipif(
    not have_pynitf, reason="need to have pynitf available to run."
)

# Some code depends on rsync. We don't want to make this a strict requirement
# for geocal, just have the code fail if rsync isn't found. So skip tests
# that depend on this.
require_rsync = pytest.mark.skipif(
    not cmd_exists("rsync"), reason="need rsync on system to run."
)

# Some tests are python 3 only. Don't want the python 2 tests to fail for
# python code that we know can't be run
require_python3 = pytest.mark.skipif(
    not sys.version_info > (3,), reason="require python 3 to run"
)

# Have tests that require /raid be available. We generally skip these if not
# available, tests are nice to make sure things don't break but not essential.
# Things that really matter have small test data sets put into unit_test_data,
# but we do want the option of running larger tests when available
require_raid = pytest.mark.skipif(
    not os.path.exists("/raid1"), reason="require /raid* test data to run"
)
