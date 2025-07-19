import geocal_swig
import numpy as np
from .safe_matplotlib_import import *
import matplotlib.pyplot as plt
from .shape_file import *

# Add some useful functions to RasterImage


def _display(self, ic, sz, cross_hair=True):
    """This executes plt.imshow for the image centered as the given
    image coordinate, with the given number of lines and samples. It
    is ok if this goes past the end of the image data, we just fill
    with 0's in that case.

    You can optionally put a crosshairs at ic. The default is to do this"""

    nline = sz
    nsamp = sz
    ln = int(round(ic.line)) - nline // 2
    smp = int(round(ic.sample)) - nsamp // 2
    d = self.read_with_pad(ln, smp, nline, nsamp)
    max = np.max(d)
    if max > 0:
        min = np.min(d[d > 0])
    else:
        min = 0
    if cross_hair:
        plt.plot(ic.sample, ic.line, "r+", markersize=20, markeredgewidth=1.0)
    plt.imshow(
        d,
        cmap=plt.cm.gray,
        vmin=min,
        vmax=max,
        extent=[smp, smp + nsamp, ln + nline, ln],
    )


geocal_swig.RasterImage.display = _display


def _read_all(self):
    """Do a read for the whole image. We do this often enough that it is
    nice to have a function for this."""
    return self.read(0, 0, self.number_line, self.number_sample)


geocal_swig.RasterImage.read_all = _read_all


def _read_all2(self):
    """Do a read for the whole image. We do this often enough that it is
    nice to have a function for this."""
    return self.read(
        0, 0, self.raster_image(0).number_line, self.raster_image(0).number_sample
    )


geocal_swig.RasterImageMultiBand.read_all = _read_all2


def _read_all_double(self):
    """Do a read for the whole image. We do this often enough that it is
    nice to have a function for this."""
    return self.read_double(0, 0, self.number_line, self.number_sample)


geocal_swig.RasterImage.read_all_double = _read_all_double


def _read_all_double2(self):
    """Do a read for the whole image. We do this often enough that it is
    nice to have a function for this."""
    return self.read_double(
        0, 0, self.raster_image(0).number_line, self.raster_image(0).number_sample
    )


geocal_swig.RasterImageMultiBand.read_all_double = _read_all_double2


def _read_all_byte_scale(self):
    """OpenCV has some functions that only work with byte data. This function
    reads all the data, and then scaled this to byte. Right now we just scale
    by the maximum value - we may want to do something more sophisticated in
    the future if we find that we need to."""
    d = self.read_all_double()
    return (d / float(d.max()) * 255).round().astype(np.uint8)


geocal_swig.RasterImage.read_all_byte_scale = _read_all_byte_scale
geocal_swig.RasterImageMultiBand.read_all_byte_scale = _read_all_byte_scale


def _footprint_geometry(self, cconver=geocal_swig.GeodeticConverter()):
    """Return a ogr Geometry object describing the footprint of the
    RasterImage. This includes the 4 corners of the image.

    You can supply the coordinates to use, the default it Geodetic
    longitude and latitude.

    This can then be used to write information to a ShapeFile."""
    corners = []
    for pt in [
        geocal_swig.ImageCoordinate(0, 0),
        geocal_swig.ImageCoordinate(self.number_line, 0),
        geocal_swig.ImageCoordinate(self.number_line, self.number_sample),
        geocal_swig.ImageCoordinate(0, self.number_sample),
    ]:
        x, y, z = cconver.convert_to_coordinate(self.ground_coordinate(pt))
        corners.append((x, y))
    return ShapeLayer.polygon_2d(corners)


if have_shape_file:
    geocal_swig.RasterImage.footprint_geometry = _footprint_geometry

__all__ = []
