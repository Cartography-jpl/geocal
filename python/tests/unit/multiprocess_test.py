import multiprocessing
from multiprocessing import Pool
import functools
import pytest


def f(x):
    return x * x


def test_basic():
    with Pool() as pool:
        res = pool.map(f, list(range(100)), max(100 // multiprocessing.cpu_count(), 1))
    assert res == [i * i for i in range(100)]


def g(x, y):
    return x * y


@pytest.mark.skip(
    reason="functools.partial doesn't work reliably with multiprocessing; use a helper class instead"
)
def test_partial():
    # This doesn't actually work with python 2.6, although it seems
    # to work with 2.7. For now, we just avoid using partial with
    # multiprocessing. We can always rewrite this in terms of a helper
    # class, so this isn't too big of a deal
    with Pool() as pool:
        g2 = functools.partial(g, 10)
        pool.map(g2, list(range(100)), max(100 // multiprocessing.cpu_count(), 1))
