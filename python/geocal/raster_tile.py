from geocal_swig import *


def tile_iterator(data):
    """
    This gives a tile_iterator. This returns a RasterImageTileIterator
    that can be used to step through a file. See also read_iterator for
    an alternative. This can either be used with a RasterImage or a
    RasterImageMultiBand. For the RasterImageMultiBand we call generate
    a iterator for raster_image(0).

    Example usage:

    for i in tile_iterator(file):
       print i
    """
    if isinstance(data, RasterImageMultiBand):
        t = RasterImageTileIterator(data.raster_image(0))
    else:
        t = RasterImageTileIterator(data)

    while not t.end:
        yield t
        t.next()


def read_iterator(data):
    """
    This is similar to tile_iterator, but it is used as a short cut when
    all you want to do is read the tile given by tile_iterator. Instead
    of returning a RasterImageTileIterator, this returns a numpy array of the
    data given by the tile. This can be use either with a RasterImage or a
    RasterImageMultiBand. For the RasterImageMultiBand the iterator is
    generated for raster_image(0), and then we read all bands at the same
    location.

    Example usage:

    for i in read_iterator(file):
       print i.shape
    """

    if isinstance(data, RasterImageMultiBand):
        t = RasterImageTileIterator(data.raster_image(0))
    else:
        t = RasterImageTileIterator(data)

    while not t.end:
        yield data.read(t.istart, t.jstart, t.number_line, t.number_sample)
        t.next()


__all__ = ["tile_iterator", "read_iterator"]
