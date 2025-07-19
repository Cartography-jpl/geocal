# This defines fixtures that gives the paths to various directories with
# test data
from pathlib import Path
import pytest


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
