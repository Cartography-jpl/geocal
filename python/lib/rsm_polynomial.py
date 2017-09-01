# This is a python implementation of a RSM rational polynomial. We will almost
# certainly want to reimplement this is C++, but do this in python first to
# have much quicker to write baseline
import numpy as np
import scipy
import math
from geocal_swig import Rpc, RsmPolynomial, RsmRationalPolynomial

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

    
class RsmLowOrderPolynomial(object):
    '''This is the low order polynomial used to determine approximate 
    row/column (line/sample, the RSM documentation calls this Row/Column)'''
    def __init__(self):
        self.pline = np.zeros(10)
        self.psamp = np.zeros(10)

    def image_coordinate(self, x, y, z):
        p = self.pline
        line =  p[0] + x * (p[1] + p[4] * x + p[5] * y + p[6] * z) + \
            y * (p[2] + p[7] * y + p[8] * z) + z * (p[3] + p[9] * z)
        p = self.psamp
        sample =  p[0] + x * (p[1] + p[4] * x + p[5] * y + p[6] * z) + \
            y * (p[2] + p[7] * y + p[8] * z) + z * (p[3] + p[9] * z)
        return [line, sample]

    def fit(self, line, sample, x, y, z):
        '''The data is a slightly complicated, we want to evaluate things
        at fixed height planes. This doesn't matter for fitting
        polynomials, but does for gridded data (and we want a common
        interface for both). So we give height as 1 d arrays, and then
        report latitude and longitude as n_line x n_sample x n_height
        arrays.

        line and sample can either be 1d or 3d (so n_line and n_sample, or
        n_line x n_sample x n_height). For 1d we assume the same line and 
        sample at each height, otherwise we allow the line and samples to be
        different for each height.
        '''
        if(len(line.shape) == 1):
            line_v = np.empty(x.shape)
            line_v[:,:,:] = line[:,np.newaxis, np.newaxis]
        else:
            line_v = line
        line_v = line_v.reshape(line_v.size)
        if(len(sample.shape) == 1):
            sample_v = np.empty(x.shape)
            sample_v[:,:,:] = sample[np.newaxis,:,np.newaxis]
        else:
            sample_v = sample
        sample_v = sample_v.reshape(sample_v.size)
        
        g = np.zeros(x.shape + (10,))
        g[...,0] = 1
        g[...,1] = x
        g[...,2] = y
        g[...,3] = z
        g[...,4] = x * x
        g[...,5] = x * y
        g[...,6] = x * z[np.newaxis, np.newaxis, :]
        g[...,7] = y * y
        g[...,8] = y * z[np.newaxis, np.newaxis, :]
        g[...,9] = (z * z)[np.newaxis, np.newaxis, :]
        g = g.reshape((x.size, 10))
        self.pline = np.linalg.lstsq(g,line_v)[0]
        self.psamp = np.linalg.lstsq(g,sample_v)[0]

class RsmMultiSection(object):
    '''Handle multiple sections.'''
    def __init__(self, nline, nsamp, nrow_section, ncol_section,
                 object_creator):
        self.nline = nline
        self.nsamp = nsamp
        self.section = np.empty((nrow_section, ncol_section), dtype=object)
        self.lp = RsmLowOrderPolynomial()
        for i in range(self.section.shape[0]):
            for j in range(self.section.shape[1]):
                self.section[i,j] = object_creator()
        self.nline_sec = float(self.nline) / self.section.shape[0]
        self.nsamp_sec = float(self.nsamp) / self.section.shape[1]
    def image_coordinate(self, x, y, z):
        lp_line, lp_samp = self.lp.image_coordinate(x, y, z)
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
                    line[wh], sample[wh] = self.section[i,j](x[wh], y[wh],
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
    def fit(self, line, sample, x, y, z):
        ''' The data is a slightly complicated, we want to evaluate things
        at fixed height planes. This doesn't matter for fitting
        polynomials, but does for gridded data (and we want a common
        interface for both). So we give height as 1 d arrays, and then
        report latitude and longitude as n_line x n_sample x n_height
        arrays.

        line and sample can either be 1d or 3d (so n_line and n_sample, or
        n_line x n_sample x n_height). For 1d we assume the same line and 
        sample at each height, otherwise we allow the line and samples to be
        different for each height.
        '''
        self.lp.fit(line, sample, x, y, z)
        lp_line, lp_samp = self.lp.image_coordinate(x, y, z)
        lp_line[lp_line < 0] = 0
        lp_line[lp_line >= self.nline_sec * self.section.shape[0]] = \
            self.nline_sec * self.section.shape[0] - 0.1
        lp_samp[lp_samp < 0] = 0
        lp_samp[lp_samp >= self.nsamp_sec * self.section.shape[1]] = \
            self.nsamp_sec * self.section.shape[1] - 0.1
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
                self.section[i,j].fit(line,sample,x, y, z2, wh=wh)
                
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
        self.rational_poly.fit(line.flatten(),sample.flatten(), x.flatten(),
                               y.flatten(), z2.flatten())
        lcalc, scalc = self.rational_poly.image_coordinate(x,y,z2)
        lcorr = line - lcalc
        scorr = sample - scalc
        self.corr_grid.fit(lcorr, scorr, x,y,z)

