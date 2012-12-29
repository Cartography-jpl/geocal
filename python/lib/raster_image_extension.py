from geocal import *
import numpy as np

# Add some useful functions to RasterImage
def read_with_pad(self, Lstart, Sstart, Number_line, Number_sample):
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

setattr(RasterImage, "read_with_pad", read_with_pad)

def display(self, ic, sz, cross_hair = True):
    '''This executes plt.imshow for the image centered as the given
    image coordinate, with the given number of lines and samples. It
    is ok if this goes past the end of the image data, we just fill
    with 0's in that case.
    
    You can optionally put a crosshairs at ic. The default is to do this'''

    # Import is here rather than top of the function. We may be running this
    # in an environment without an interactive display, so this import will fail.
    # As long as the user doesn't call this function, this isn't a problem.
    import matplotlib.pyplot as plt

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

setattr(RasterImage, "display", display)

