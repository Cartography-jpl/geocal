import numpy as np
import numpy.testing as npt
import pytest
from pathlib import Path
import geocal

# Optional support for running matlab from python
try:
    from pymatbridge import Matlab

    have_matlab = True
except ImportError:
    have_matlab = False


@pytest.mark.matlab
def test_matlab():
    """Basic test of matlab interface. Note that in addition to python,
    there is ipython magic by %load_ext pymatbridge"""
    if not have_matlab:
        pytest.skip("pymatbridge not available")
    mlab = Matlab()
    mlab.start()
    mlab.run_code("a=1;")
    assert mlab.get_variable("a") == 1
    b = np.array([[1, 2], [3, 4.0]])
    mlab.set_variable("b", b)
    bb = mlab.get_variable("b")
    npt.assert_almost_equal(bb, b)
    # Have a jk.m file in same location as test file.
    res = mlab.run_func(
        str(Path(geocal.__file__).parent / "jk.m"), {"arg1": 3, "arg2": 5}
    )
    assert res["result"] == 8
