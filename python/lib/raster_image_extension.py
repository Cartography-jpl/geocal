from geocal import *
import numpy as np
import safe_matplotlib_import
import matplotlib.pyplot as plt
from shape_file import *

# Add some useful functions to RasterImage
def _read_with_pad(self, Lstart, Sstart, Number_line, Number_sample):
    '''This is variation of read that allows the data read to extend
    past the actual RasterImage. In that case, we return 0's for the 
    outside area.'''
    res = np.zeros((Number_line, Number_sample), dtype = np.int)
    if(Lstart >= self.number_line or
       Sstart >= self.number_sample or
       Lstart + Number_line <= 0 or
       Sstart + Number_sample <= 0):
        return res
    lind = 0
    rline = Lstart
    sind = 0
    rsamp = Sstart
    nlind = Number_line
    nsind = Number_sample
    if(Lstart < 0):
        lind = -Lstart
        nlind -= lind
        rline = 0
    if(Sstart < 0):
        sind = -Sstart
        nsind -= sind
        rsamp = 0
    if(nlind > self.number_line - rline): 
        nlind = self.number_line - rline
    if(nsind > self.number_sample - rsamp): 
        nsind = self.number_sample - rsamp
    res[lind:(lind + nlind), sind:(sind + nsind)] = \
        self.read(rline, rsamp, nlind, nsind)
    return res

geocal.RasterImage.read_with_pad = _read_with_pad

def _display(self, ic, sz, cross_hair = True):
    '''This executes plt.imshow for the image centered as the given
    image coordinate, with the given number of lines and samples. It
    is ok if this goes past the end of the image data, we just fill
    with 0's in that case.
    
    You can optionally put a crosshairs at ic. The default is to do this'''

    nline = sz
    nsamp = sz
    ln = int(round(ic.line)) - nline / 2
    smp = int(round(ic.sample)) - nsamp / 2
    d = self.read_with_pad(ln, smp, nline, nsamp)
    max = np.max(d)
    if(max > 0):
        min = np.min(d[np.nonzero(d)])
    else:
        min = 0
    if(cross_hair):
        plt.plot(ic.sample, ic.line, 'w+', markersize = 20)
    plt.imshow(self.read_with_pad(ln, smp, nline, nsamp), cmap = plt.cm.gray,
               vmin = min, vmax = max, 
               extent = [smp, smp + nsamp, ln + nline, ln])

geocal.RasterImage.display = _display

def _footprint_geometry(self, cconver = geocal.GeodeticConverter()):
    '''Return a ogr Geometry object describing the footprint of the 
    RasterImage. This includes the 4 corners of the image.

    You can supply the coordinates to use, the default it Geodetic
    longitude and latitude.

    This can then be used to write information to a ShapeFile.'''
    corners = []
    for pt in [geocal.ImageCoordinate(0,0), 
               geocal.ImageCoordinate(self.number_line, 0), 
               geocal.ImageCoordinate(self.number_line, 
                                      self.number_sample), 
               geocal.ImageCoordinate(0, self.number_sample)]:
        x, y, z = cconver.convert_to_coordinate(self.ground_coordinate(pt))
        corners.append((x, y))
    return ShapeLayer.polygon_2d(corners)

geocal.RasterImage.footprint_geometry = _footprint_geometry




