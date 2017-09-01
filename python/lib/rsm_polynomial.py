# This is a python implementation of a RSM rational polynomial. We will almost
# certainly want to reimplement this is C++, but do this in python first to
# have much quicker to write baseline
import numpy as np
import scipy
import math
from geocal_swig import Rpc, RsmPolynomial

class RsmRationalPolynomial(object):
    '''This is an RSM. The X,Y,Z can be either longitude,latitude,height,
    or it can be Rectangular system. For a rectangular system, we can go 
    with [x y z] = Rot * (Cartesian_fixed - origin).  By convention, we 
    pass in the Cartesian_fixed X, Y, Z and carry the transformation in
    this class.'''
    def __init__(self, NP_X, NP_Y, NP_Z, fit_rpc_param_only = False,
                 DP_X=None, DP_Y=None,DP_Z=None, N_max_order = -1,
                 D_max_order = -1):
        '''Right now, we use the same order polynomial for both line and
        sample. This isn't an actual requirement, we'll just keep
        things simple for right now.

        If the denominator size isn't given, we assume it is the same size
        as the numerator. You can specify a different size if you like.

        fit_rpc_param_only uses the old GeoCal RPC fitting code and then
        sets this RSM to have the same parameters. This is really just meant
        for testing purposes, we will likely remove this at some point in the 
        future.

        You can limit the over all order of cross terms by supplying a
        max_order for the numerator, denominator, or both. This holds all 
        cross terms with a total order > max to 0.
        '''
        self.fit_rpc_param_only = fit_rpc_param_only
        self.z_offset = 0
        self.z_scale = 0
        self.y_offset = 0
        self.y_scale = 0
        self.x_offset = 0
        self.x_scale = 0
        self.line_offset = 0
        self.line_scale = 0
        self.sample_offset = 0
        self.sample_scale = 0
        if(fit_rpc_param_only) :
            NP_X = 3
            NP_Y = 3
            NP_Z = 3
        if(DP_X is None):
            DP_X = NP_X
        if(DP_Y is None):
            DP_Y = NP_Y
        if(DP_Z is None):
            DP_Z = NP_Z
        self.line_num = RsmPolynomial(NP_X, NP_Y, NP_Z, False, N_max_order)
        self.line_den = RsmPolynomial(DP_X, DP_Y, DP_Z, True, D_max_order)
        self.sample_num = RsmPolynomial(NP_X, NP_Y, NP_Z, False, N_max_order)
        self.sample_den = RsmPolynomial(DP_X, DP_Y, DP_Z, True, D_max_order)

    def rpc_set(self, rpc):
        '''Set this to match an existing RPC, useful for testing against
        our existing and tested RPC code.'''
        self.z_offset = rpc.height_offset
        self.z_scale = rpc.height_scale
        self.y_offset = rpc.latitude_offset
        self.y_scale = rpc.latitude_scale
        self.x_offset = rpc.longitude_offset
        self.x_scale = rpc.longitude_scale
        self.line_offset = rpc.line_offset
        self.line_scale = rpc.line_scale
        self.sample_offset = rpc.sample_offset
        self.sample_scale = rpc.sample_scale
        self.line_num.set_rpc_coeff(rpc.line_numerator)
        self.line_den.set_rpc_coeff(rpc.line_denominator)
        self.sample_num.set_rpc_coeff(rpc.sample_numerator)
        self.sample_den.set_rpc_coeff(rpc.sample_denominator)
        
    def __call__(self, xin,yin,zin):
        '''For geodetic, x is longitude, y is latitude, z is height.'''
        x = (xin - self.x_offset) / self.x_scale
        y = (yin - self.y_offset) / self.y_scale
        z = (zin - self.z_offset) / self.z_scale
        f1 = self.line_num(x,y,z)
        f2 = self.line_den(x,y,z)
        f3 = self.sample_num(x,y,z)
        f4 = self.sample_den(x,y,z)
        return [f1 / f2 * self. line_scale + self.line_offset,
                f3 / f4 * self.sample_scale + self.sample_offset]

    def fit_offset_and_scale(self, min_line, max_line, min_sample, max_sample,
                             min_z, max_z, x, y):
        '''Set line_offset, line_scale etc to cover the range for each
        of the set of data provided.'''
        self.z_offset = (max_z + min_z) / 2.0
        self.z_scale = (max_z - min_z) / 2.0
        self.line_offset = (max_line + min_line) / 2.0
        self.line_scale = (max_line - min_line) / 2.0
        self.sample_offset = (max_sample + min_sample) / 2.0
        self.sample_scale = (max_sample - min_sample) / 2.0
        min_y = y.min()
        max_y = y.max()
        min_x = x.min()
        max_x = x.max()
        self.y_offset = (max_y + min_y) / 2
        self.y_scale = (max_y - min_y) / 2
        self.x_offset = (max_x + min_x) / 2
        self.x_scale = (max_x - min_x) / 2

    def fit(self, line, sample, xin, yin, zin,
            do_scale = True, wh = None):
        '''Fit rational polynomial to the given data. We usually also want
        to determine the scale for line_offset etc. from this data. Set 
        do_scale to false if you want to instead call fit_offset_and_scale
        yourself and hold things fixed here.

        You can pass a "wh" value to use to restrict the data used, this is
        useful for when we are doing multisection fits and only some of the
        data should be used by this RsmRationalPolynomial.

        The data is a slightly complicated, we want to evaluate things
        at fixed height planes. This doesn't matter for fitting
        polynomials, but does for gridded data (and we want a common
        interface for both). So we give height as 1 d arrays, and then
        report longitude, latitude as n_line x n_sample x n_height
        arrays.

        line and sample can either be 1d or 3d (so n_line and n_sample, or
        n_line x n_sample x n_height). For 1d we assume the same line and 
        sample at each height, otherwise we allow the line and samples to be
        different for each height.
        '''
        if(do_scale):
            if(wh is not None):
                self.fit_offset_and_scale(line[wh].min(), line[wh].max(),
                                          sample[wh].min(),
                                          sample[wh].max(),
                                          zin.min(), zin.max(),
                                          xin[wh], yin[wh])
            else:
                self.fit_offset_and_scale(line.min(), line.max(),
                                          sample.min(),
                                          sample.max(),
                                          zin.min(), zin.max(),
                                          xin, yin)
        if(self.fit_rpc_param_only):
            self._fit_rpc_param(line, sample, xin, yin, zin, wh)
                
        ln_lhs_v = (line - self.line_offset) / self.line_scale
        if(len(ln_lhs_v.shape) == 1):
            ln_lhs = np.empty(xin.shape)
            ln_lhs[:,:,:] = ln_lhs_v[:, np.newaxis,np.newaxis]
        else:
            ln_lhs = ln_lhs_v
        if(wh is not None):
            ln_lhs = ln_lhs[wh]
        ln_lhs = ln_lhs.reshape(ln_lhs.size)
        smp_lhs_v = (sample - self.sample_offset) / self.sample_scale
        if(len(smp_lhs_v.shape) == 1):
            smp_lhs = np.empty(xin.shape)
            smp_lhs[:,:,:] = smp_lhs_v[np.newaxis,:,np.newaxis]
        else:
            smp_lhs = smp_lhs_v
        if(wh is not None):
            smp_lhs = smp_lhs[wh]
        smp_lhs = smp_lhs.reshape(smp_lhs.size)
        x = (xin - self.x_offset) / self.x_scale
        y = (yin - self.y_offset) / self.y_scale
        zv = (zin - self.z_offset) / self.z_scale
        z = np.empty(x.shape)
        z[:,:,:] = zv[np.newaxis, np.newaxis, :]
        if(wh is not None):
            x = x[wh]
            y = y[wh]
            z = z[wh]
        else:
            x = x.reshape((x.size))
            y = y.reshape((y.size))
            z = z.reshape((z.size))
        ljac1 = self.line_num.jacobian(x, y, z)
        ljac2 = self.line_den.jacobian(x, y, z)
        ljac2 = np.transpose(-ln_lhs * np.transpose(ljac2))
        ljac = np.concatenate((ljac1, ljac2), axis=1)
        sjac1 = self.sample_num.jacobian(x, y, z)
        sjac2 = self.sample_den.jacobian(x, y, z)
        sjac2 = np.transpose(-smp_lhs * np.transpose(sjac2))
        sjac = np.concatenate((sjac1, sjac2), axis=1)
        lnpar = np.linalg.lstsq(ljac, ln_lhs)[0]
        smpar = np.linalg.lstsq(sjac, smp_lhs)[0]
        self.line_num.fitted_coefficent = lnpar[0:ljac1.shape[1]]
        self.line_den.fitted_coefficent = lnpar[ljac1.shape[1]:]
        self.sample_num.fitted_coefficent = smpar[0:sjac1.shape[1]]
        self.sample_den.fitted_coefficent = smpar[sjac1.shape[1]:]

    def _fit_rpc_param(self, line, sample, xin, yin, zin, wh):
        rpc = Rpc()
        rpc.height_offset = self.z_offset
        rpc.height_scale = self.z_scale
        rpc.line_offset = self.line_offset
        rpc.line_scale = self.line_scale
        rpc.sample_offset = self.sample_offset
        rpc.sample_scale = self.sample_scale
        rpc.latitude_offset = self.y_offset
        rpc.latitude_scale = self.y_scale
        rpc.longitude_offset = self.x_offset
        rpc.longitude_scale = self.x_scale
        z2 = np.empty(xin.shape)
        z2[:,:,:] = zin[np.newaxis,np.newaxis,:]
        if(wh is not None):
            rpc.fit_all(line[wh], sample[wh], yin[wh], xin[wh],
                        z2[wh])
        else:
            rpc.fit_all(line.flatten(), sample.flatten(),
                        yin.flatten(), xin.flatten(),
                        z2.flatten())
        self.line_num.set_rpc_coeff(rpc.line_numerator)
        self.line_den.set_rpc_coeff(rpc.line_denominator)
        self.sample_num.set_rpc_coeff(rpc.sample_numerator)
        self.sample_den.set_rpc_coeff(rpc.sample_denominator)
        

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
    def __call__(self, xin, yin, zin):
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

    def __call__(self, x, y, z):
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
    def __call__(self, x, y, z):
        lp_line, lp_samp = self.lp(x, y, z)
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
        lp_line, lp_samp = self.lp(x, y, z)
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

    def __call__(self, x, y, z):
        line, sample = self.rational_poly(x, y, z)
        lcorr, scorr  = self.corr_grid(x, y, z)
        return [line + lcorr, sample + scorr] 

    def fit(self, line, sample, x, y, z):
        self.rational_poly.fit(line,sample,x, y, z)
        z2 = np.empty(x.shape)
        z2[:,:,:] = z[np.newaxis,np.newaxis,:]
        lcalc, scalc = self.rational_poly(x,y,z2)
        lcorr = line - lcalc
        scorr = sample - scalc
        self.corr_grid.fit(lcorr, scorr, x,y,z)

