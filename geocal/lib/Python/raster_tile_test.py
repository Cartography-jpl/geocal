import geocal as gc
from nose.tools import *
import os

test_data = os.path.dirname(__file__) + "/../unit_test_data/"

def setUp():
        # Nothing special about this data, it is just a reasonable size for
        # stepping through
        global f
        f = gc.GdalRasterImage(test_data + "egm96.img")

@with_setup(setUp())
def test_tile_iterator():
    tot_size = 0
    for i in gc.tile_iterator(f):
        tot_size += i.number_line() * i.number_sample()
    assert tot_size == f.number_line() * f.number_sample()

@with_setup(setUp())
def test_read_iterator():
    tot_size = 0
    for i in gc.read_iterator(f):
        tot_size += i.size
    assert tot_size == f.number_line() * f.number_sample()

