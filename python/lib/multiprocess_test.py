import multiprocessing
from multiprocessing import Pool
import functools

def f(x):
    return x * x

def test_basic():
    pool = Pool()
    print multiprocessing.cpu_count()
    print pool.map(f, range(100), 100 / multiprocessing.cpu_count())

def g(x, y):
    return x * y

def test_partial():
    pool = Pool()
    g2 = functools.partial(g, 10)
    print pool.map(g2, range(100), 100 / multiprocessing.cpu_count())
