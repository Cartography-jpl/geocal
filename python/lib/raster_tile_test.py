from raster_tile import *
from test_support import *

# Nothing special about this data, it is just a reasonable size for
# stepping through
f = GdalRasterImage(unit_test_data + "egm96.img")

def test_tile_iterator():
    tot_size = 0
    for i in tile_iterator(f):
        tot_size += i.number_line * i.number_sample
    assert tot_size == f.number_line * f.number_sample

def test_read_iterator():
    tot_size = 0
    for i in read_iterator(f):
        tot_size += i.size
    assert tot_size == f.number_line * f.number_sample

