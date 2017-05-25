import pytest

# Don't normally run this. It is common for the matlab license server to
# be down for brief periods, and this can cause unit tests to fail. We
# only need to run this when we are explicitly looking at matlab
def pytest_addoption(parser):
    parser.addoption("--run-matlab", action="store_true",
                     help="run matlab tests")
