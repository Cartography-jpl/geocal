from builtins import range
import multiprocessing
from multiprocessing import Pool
from test_support import *
import functools

def f(x):
    return x * x

def test_basic():
    pool = Pool()
    print(multiprocessing.cpu_count())
    print(pool.map(f, list(range(100)), 100 // multiprocessing.cpu_count()))

def g(x, y):
    return x * y

@skip
def test_partial():
    # This doesn't actually work with python 2.6, although it seems
    # to work with 2.7. For now, we just avoid using partial with 
    # multiprocessing. We can always rewrite this in terms of a helper
    # class, so this isn't too big of a deal
    pool = Pool()
    g2 = functools.partial(g, 10)
    print(pool.map(g2, list(range(100)), 100 // multiprocessing.cpu_count()))
