from geocal import GdalRasterImage, tile_iterator, read_iterator


# Nothing special about this data, it is just a reasonable size for
# stepping through
def test_tile_iterator(unit_test_data):
    f = GdalRasterImage(str(unit_test_data / "egm96.img"))
    tot_size = 0
    for i in tile_iterator(f):
        tot_size += i.number_line * i.number_sample
    assert tot_size == f.number_line * f.number_sample


def test_read_iterator(unit_test_data):
    f = GdalRasterImage(str(unit_test_data / "egm96.img"))
    tot_size = 0
    for i in read_iterator(f):
        tot_size += i.size
    assert tot_size == f.number_line * f.number_sample
