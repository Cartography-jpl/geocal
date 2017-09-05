# This is a python implementation of a RSM rational polynomial. We will almost
# certainly want to reimplement this is C++, but do this in python first to
# have much quicker to write baseline
import numpy as np
import scipy
import math
from geocal_swig import Rpc, RsmPolynomial, RsmRationalPolynomial, RsmLowOrderPolynomial

class RsmGrid(object):
    '''Use a interpolation grid to map from ground to image. Right now we
    only do a linear interpolation. The RSM documentation suggests doing
    Lagrange interpolation of various orders, we can implement that in the
    future (or just do this when we implement this in C++).'''
    def __init__(self, NP_X, NP_Y, NP_Z):
        self.np_x = NP_X
        self.np_y = NP_Y
        self.np_z = NP_Z
        self.line_grid = None
        self.sample_grid = None
    def image_coordinate(self, xin, yin, zin):
        x = np.stack((xin, yin, zin), axis=-1)
        return self.line_grid(x), self.sample_grid(x)
    def fit(self, line, sample, xin, yin, zin, wh = None):
        if(zin.size != self.np_z):
            raise RuntimeError("Fit was not passed the correct number of heights. Expected %d but got %s" % (self.np_z, zin.shape))
        xv = np.linspace(xin.min(), xin.max(), self.np_x)
        yv = np.linspace(yin.min(), yin.max(), self.np_y)
        ldata = np.empty((self.np_x, self.np_y, self.np_z))
        sdata = np.empty(ldata.shape)
        for i in range(self.np_z):
            x = xin[:,:,i]
            y = yin[:,:,i]
            ln = line[:,:,i]
            smp = sample[:,:,i]
            if(wh is not None):
                x = x[wh[:,:,i]]
                y = y[wh[:,:,i]]
                ln = ln[wh[:,:,i]]
                smp = smp[wh[:,:,i]]
            else:
                x = x.reshape(x.size)
                y = y.reshape(y.size)
                ln= ln.reshape(ln.size)
                smp = smp.reshape(smp.size)
            # Use nearest value outside of the convex hull, linear interpolation
            # otherwise
            gd_nearest = scipy.interpolate.griddata((x,y), np.stack((ln, smp), axis=1), (np.outer(xv,np.ones_like(yv)), np.outer(np.ones_like(xv), yv)), method='nearest')
            gd = scipy.interpolate.griddata((x,y), np.stack((ln, smp), axis=1), (np.outer(xv,np.ones_like(yv)), np.outer(np.ones_like(xv), yv)))
            gd[np.isnan(gd)] = gd_nearest[np.isnan(gd)]
            ldata[...,i] = gd[...,0]
            sdata[...,i] = gd[...,1]
        self.line_grid = scipy.interpolate.RegularGridInterpolator((xv,yv,zin), ldata, bounds_error=False,fill_value=None)
        self.sample_grid = scipy.interpolate.RegularGridInterpolator((xv,yv,zin), sdata, bounds_error=False,fill_value=None)

    
class RsmMultiSection(object):
    '''Handle multiple sections.'''
    def __init__(self, nline, nsamp, nrow_section, ncol_section,
                 object_creator, border=10):
        '''The RsmRationalPolynomial tends to extrapolate badly. Because the
        low order polynomial is only approximately correct, we add a little
        bit of a border to each underlying RsmRationalPolynomial so we can
        avoid extrapolating.'''
        self.nline = nline
        self.nsamp = nsamp
        self.border = border
        self.section = np.empty((nrow_section, ncol_section), dtype=object)
        self.lp = RsmLowOrderPolynomial()
        for i in range(self.section.shape[0]):
            for j in range(self.section.shape[1]):
                self.section[i,j] = object_creator()
        self.nline_sec = float(self.nline) / self.section.shape[0]
        self.nsamp_sec = float(self.nsamp) / self.section.shape[1]
    def image_coordinate(self, x, y, z):
        lp_line, lp_samp = self.lp.image_coordinate(x, y, z)
        lp_line = lp_line.copy()
        lp_samp = lp_samp.copy()
        if isinstance(lp_line, np.ndarray):
            lp_line[lp_line < 0] = 0
            lp_line[lp_line >= self.nline_sec * self.section.shape[0]] = \
                 self.nline_sec * self.section.shape[0] - 0.1
            lp_samp[lp_samp < 0] = 0
            lp_samp[lp_samp >= self.nsamp_sec * self.section.shape[1]] = \
                 self.nsamp_sec * self.section.shape[1] - 0.1
            line = np.empty(lp_line.shape)
            sample = np.empty(lp_line.shape)
            if(len(z.shape) != len(x.shape)):
                z2 = np.empty(x.shape)
                z2[:,:,:] = z[np.newaxis,np.newaxis,:]
            else:
                z2 = z
            for i in range(self.section.shape[0]):
                wh_ln = np.logical_and(lp_line >= self.nline_sec * i,
                                       lp_line < self.nline_sec * (i+1))
                for j in range(self.section.shape[1]):
                    wh = np.logical_and(wh_ln,
                      np.logical_and(lp_samp >= self.nsamp_sec * j,
                      lp_samp < self.nsamp_sec * (j+1)))
                    line[wh], sample[wh] = \
                        self.section[i,j].image_coordinate(x[wh], y[wh],
                                                           z2[wh])
        else:
            i = math.floor(lp_line / self.nline_sec)
            j = math.floor(lp_samp / self.nsamp_sec)
            if(i < 0):
                i = 0
            if(i >= self.section.shape[0]):
                i = self.section.shape[0] - 1
            if(j < 0):
                j = 0
            if(j >= self.section.shape[1]):
                j = self.section.shape[1] - 1
            line, sample = self.section[i,j](x,y, z)
        return line, sample
    def fit(self, igc, cconv, min_height, max_height,
	   min_line, max_line, min_sample, max_sample,
	   nline = 20, nsample = 20, nheight = 20,
	   skip_masked_point = False,
	   ignore_error = False):
        self.lp.fit(igc, cconv, min_height, max_height, min_line,
                    max_line, min_sample, max_sample, nline, nsample,
                    nheight, skip_masked_point, ignore_error)
        for i in range(self.section.shape[0]):
            for j in range(self.section.shape[1]):
                self.section[i,j].fit(igc, cconv, min_height,
                                      max_height,
                                    max(int(self.nline_sec * i)-self.border,
                                        min_line),
                                    min(int(self.nline_sec * (i+1))+
                                        self.border, max_line),
                                    max(int(self.nsamp_sec * j)-self.border,
                                        min_sample),
                                    min(int(self.nsamp_sec * (j+1))+
                                        self.border,max_sample),
                                    nline, nsample, nheight,
                                    skip_masked_point, ignore_error)
                
class RsmRationalPolynomialPlusGrid(object):
    '''We can have both a RsmRationalPolynomial and a RsmGrid that is used
    to correct the line and sample. This handles that.'''
    def __init__(self, rational_poly, corr_grid):
        '''This takes either a RsmRationalPolynomial or a RsmMultiSection that
        contains RsmRationalPolynomial, and the same with RsmGrid for the
        correction grid.'''
        self.rational_poly = rational_poly
        self.corr_grid = corr_grid

    def image_coordinate(self, x, y, z):
        line, sample = self.rational_poly.image_coordinate(x, y, z)
        lcorr, scorr  = self.corr_grid.image_coordinate(x, y, z)
        return [line + lcorr, sample + scorr] 

    def fit(self, line, sample, x, y, z):
        z2 = np.empty(x.shape)
        z2[:,:,:] = z[np.newaxis,np.newaxis,:]
        self.rational_poly.fit_offset_and_scale(line.min(), line.max(),
                                                sample.min(), sample.max(),
                                                x.min(), x.max(),
                                                y.min(), y.max(),
                                                z.min(), z.max())
        self.rational_poly.fit_data(line.flatten(),sample.flatten(),
                                    x.flatten(),
                                    y.flatten(), z2.flatten())
        lcalc, scalc = self.rational_poly.image_coordinate(x,y,z2)
        lcorr = line - lcalc
        scorr = sample - scalc
        self.corr_grid.fit(lcorr, scorr, x,y,z)

