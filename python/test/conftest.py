import pytest
from pathlib import Path
import os
import sys

# Add source to path. For some reason pytest can miss this even if we
# have this installed editable with a --prefix pip install (probably
# some weird PYTHONPATH interaction not worth tracking down when we can
# just work around it).
sys.path.append(str(Path(os.path.dirname(__file__)).parent))

# Short hand for marking as unconditional skipping. Good for tests we
# don't normally run, but might want to comment out for a specific debugging
# reason.
skip = pytest.mark.skip

# Marker for long tests. Only run with --run-long
long_test = pytest.mark.long_test

# Don't normally run this. It is common for the matlab license server to
# be down for brief periods, and this can cause unit tests to fail. We
# only need to run this when we are explicitly looking at matlab.
# Also mark tests that take a long time to run, we don't normally run
# these.
def pytest_addoption(parser):
    parser.addoption("--run-matlab", action="store_true",
                     help="run matlab tests")
    parser.addoption("--run-long", action="store_true",
                     help="run long tests")
    
def pytest_collection_modifyitems(config, items):
    if not config.getoption("--run-matlab"):
        skip_matlab = pytest.mark.skip(reason="need --run-matlab option to run")
        for item in items:
            if "matlab" in item.keywords:
                item.add_marker(skip_matlab)
    if not config.getoption("--run-long"):
        skip_long_test = pytest.mark.skip(reason="need --run-long option to run")
        for item in items:
            if "long_test" in item.keywords:
                item.add_marker(skip_long_test)

# ------------------------------------------
# Includes fixtures, made available to all tests.
# ------------------------------------------

pytest_plugins = [
#    "fixtures.dir_fixture",
#    "fixtures.igc_fixture",
    "fixtures.misc_fixture",
]
                
