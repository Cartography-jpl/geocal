from .vicar_func import add_tae_path, set_original_env
import pytest


@pytest.fixture(scope="function")
def vicarb_env(isolated_dir):
    """This sets up for a test of our own pdf files. We make sure that we
    have the proper TAEPATH, and we also set up to run in an isolated
    directory so we capture all the files vicar tends to vomit."""
    original_env = None
    try:
        original_env = add_tae_path(isolated_dir)
        yield
    finally:
        if original_env is not None:
            set_original_env(original_env)
