# This is a python implementation of a RSM rational polynomial. We will almost
# certainly want to reimplement this is C++, but do this in python first to
# have much quicker to write baseline
import numpy as np
import scipy
import math

class RsmPolynomial(object):
    '''This is a polynomial, already working in the scaled XYZ coordinate
    system.'''
    def __init__(self, NP_X, NP_Y, NP_Z):
        self.coefficient = np.zeros((NP_X+1, NP_Y+1, NP_Z+1))

    def rpc_coeff_set(self, coeff):
        '''Set the subset of self.coefficient that corresponds to the RPC
        coefficients. This assumes RPC_B format.'''
        self.coefficient[:,:,:]=0
        self.coefficient[0,0,0],self.coefficient[0,1,0],self.coefficient[1,0,0],self.coefficient[0,0,1],\
        self.coefficient[1,1,0],self.coefficient[0,1,1],self.coefficient[1,0,1],\
        self.coefficient[0,2,0],self.coefficient[2,0,0],self.coefficient[0,0,2],\
        self.coefficient[1,1,1],self.coefficient[0,3,0],\
        self.coefficient[2,1,0],\
        self.coefficient[0,1,2], self.coefficient[1,2,0],\
        self.coefficient[3,0,0],\
        self.coefficient[1,0,2],self.coefficient[0,2,1],\
        self.coefficient[2,0,1],\
        self.coefficient[0,0,3] = coeff

    def __call__(self, x, y, z):
        xp = [ x**i for i in range(self.coefficient.shape[0]) ]
        yp = [ y**i for i in range(self.coefficient.shape[1]) ]
        zp = [ z**i for i in range(self.coefficient.shape[2]) ]
        # I'm sure we can do this faster, but for now have a clean
        # way of calculating this
        res = 0
        for i in range(self.coefficient.shape[0]):
            for j in range(self.coefficient.shape[1]):
                for k in range(self.coefficient.shape[2]):
                    res += self.coefficient[i,j,k] * xp[i] * yp[j] * zp[k]
        return res;

    def poly_jac(self, x, y, z):
        '''Return the derivative of the polynomial with each coefficient. 
        This is just the xp etc. without the coefficient. We return this
        as the same shape of self.coefficient, but you may want to flatten 
        this.'''
        x_is_array = False
        if(hasattr(x, 'shape')):
            x_is_array = True
        if(x_is_array):
            res = np.zeros(self.coefficient.shape + x.shape)
        else:
            res = np.zeros(self.coefficient.shape)
        xp = [ x**i for i in range(self.coefficient.shape[0]) ]
        yp = [ y**i for i in range(self.coefficient.shape[1]) ]
        zp = [ z**i for i in range(self.coefficient.shape[2]) ]
        # I'm sure we can do this faster, but for now have a clean
        # way of calculating this
        for i in range(self.coefficient.shape[0]):
            for j in range(self.coefficient.shape[1]):
                for k in range(self.coefficient.shape[2]):
                    if(x_is_array):
                        res[i,j,k,...] = xp[i] * yp[j] * zp[k]
                    else:
                        res[i,j,k] = xp[i] * yp[j] * zp[k]
        return res

class RsmRationalPolynomial(object):
    def __init__(self, NP_X, NP_Y, NP_Z):
        '''Right now, we use the same order polynomial for both line and
        sample, and both numerator and denominator. This isn't an actual
        requirement, we'll just keep things simple for right now.'''
        self.height_offset = 0
        self.height_scale = 0
        self.latitude_offset = 0
        self.latitude_scale = 0
        self.longitude_offset = 0
        self.longitude_scale = 0
        self.line_offset = 0
        self.line_scale = 0
        self.sample_offset = 0
        self.sample_scale = 0
        self.line_num = RsmPolynomial(NP_X, NP_Y, NP_Z)
        self.line_den = RsmPolynomial(NP_X, NP_Y, NP_Z)
        self.sample_num = RsmPolynomial(NP_X, NP_Y, NP_Z)
        self.sample_den = RsmPolynomial(NP_X, NP_Y, NP_Z)

    def rpc_set(self, rpc):
        '''Set this to match an existing RPC, useful for testing against
        our existing and tested RPC code.'''
        self.height_offset = rpc.height_offset
        self.height_scale = rpc.height_scale
        self.latitude_offset = rpc.latitude_offset
        self.latitude_scale = rpc.latitude_scale
        self.longitude_offset = rpc.longitude_offset
        self.longitude_scale = rpc.longitude_scale
        self.line_offset = rpc.line_offset
        self.line_scale = rpc.line_scale
        self.sample_offset = rpc.sample_offset
        self.sample_scale = rpc.sample_scale
        self.line_num.rpc_coeff_set(rpc.line_numerator)
        self.line_den.rpc_coeff_set(rpc.line_denominator)
        self.sample_num.rpc_coeff_set(rpc.sample_numerator)
        self.sample_den.rpc_coeff_set(rpc.sample_denominator)
        
    def __call__(self, lat, lon, h):
        x = (lat - self.latitude_offset) / self.latitude_scale
        y = (lon - self.longitude_offset) / self.longitude_scale
        z = (h - self.height_offset) / self.height_scale
        f1 = self.line_num(x,y,z)
        f2 = self.line_den(x,y,z)
        f3 = self.sample_num(x,y,z)
        f4 = self.sample_den(x,y,z)
        return [f1 / f2 * self. line_scale + self.line_offset,
                f3 / f4 * self.sample_scale + self.sample_offset]

    def fit_offset_and_scale(self, min_line, max_line, min_sample, max_sample,
                             min_height, max_height,
                             latitude, longitude):
        '''Set line_offset, line_scale etc to cover the range for each
        of the set of data provided.'''
        self.height_offset = (max_height + min_height) / 2.0
        self.height_scale = (max_height - min_height) / 2.0
        self.line_offset = (max_line + min_line) / 2.0
        self.line_scale = (max_line - min_line) / 2.0
        self.sample_offset = (max_sample + min_sample) / 2.0
        self.sample_scale = (max_sample - min_sample) / 2.0
        min_lat = latitude.min()
        max_lat = latitude.max()
        min_lon = longitude.min()
        max_lon = longitude.max()
        self.latitude_offset = (max_lat + min_lat) / 2
        self.latitude_scale = (max_lat - min_lat) / 2
        self.longitude_offset = (max_lon + min_lon) / 2
        self.longitude_scale = (max_lon - min_lon) / 2

    def fit(self, line, sample, latitude, longitude, height,
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
        report latitude and longitude as n_line x n_sample x n_height
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
                                          height.min(), height.max(),
                                          latitude[wh], longitude[wh])
            else:
                self.fit_offset_and_scale(line.min(), line.max(),
                                          sample.min(),
                                          sample.max(),
                                          height.min(), height.max(),
                                          latitude, longitude)
                
        ln_lhs_v = (line - self.line_offset) / self.line_scale
        if(len(ln_lhs_v.shape) == 1):
            ln_lhs = np.empty(latitude.shape)
            ln_lhs[:,:,:] = ln_lhs_v[:, np.newaxis,np.newaxis]
        else:
            ln_lhs = ln_lhs_v
        if(wh is not None):
            ln_lhs = ln_lhs[wh]
        ln_lhs = ln_lhs.reshape(ln_lhs.size)
        smp_lhs_v = (sample - self.sample_offset) / self.sample_scale
        if(len(smp_lhs_v.shape) == 1):
            smp_lhs = np.empty(latitude.shape)
            smp_lhs[:,:,:] = smp_lhs_v[np.newaxis,:,np.newaxis]
        else:
            smp_lhs = smp_lhs_v
        if(wh is not None):
            smp_lhs = smp_lhs[wh]
        smp_lhs = smp_lhs.reshape(smp_lhs.size)
        x = (latitude - self.latitude_offset) / self.latitude_scale
        y = (longitude - self.longitude_offset) / self.longitude_scale
        zv = (height - self.height_offset) / self.height_scale
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
        ljac1 = self.line_num.poly_jac(x, y, z)
        ljac1 = ljac1.reshape(ljac1.shape[0] * ljac1.shape[1] * ljac1.shape[2],
                              ljac1.shape[3])
        ljac1 = np.transpose(ljac1)
        ljac2 = self.line_den.poly_jac(x, y, z)
        ljac2 = ljac2.reshape(ljac2.shape[0] * ljac2.shape[1] * ljac2.shape[2],
                              ljac2.shape[3])
        ljac2 = -ln_lhs * ljac2
        ljac2 = np.transpose(ljac2[1:,:])
        ljac = np.concatenate((ljac1, ljac2), axis=1)
        sjac1 = self.sample_num.poly_jac(x, y, z)
        sjac1 = sjac1.reshape(sjac1.shape[0] * sjac1.shape[1] * sjac1.shape[2],
                              sjac1.shape[3])
        sjac1 = np.transpose(sjac1)
        sjac2 = self.sample_den.poly_jac(x, y, z)
        sjac2 = sjac2.reshape(sjac2.shape[0] * sjac2.shape[1] * sjac2.shape[2],
                              sjac2.shape[3])
        sjac2 = -smp_lhs * sjac2
        sjac2 = np.transpose(sjac2[1:,:])
        sjac = np.concatenate((sjac1, sjac2), axis=1)
        lnpar = np.linalg.lstsq(ljac, ln_lhs)[0]
        smpar = np.linalg.lstsq(sjac, smp_lhs)[0]
        nx = self.line_num.coefficient.shape[0]
        ny = self.line_num.coefficient.shape[1]
        nz = self.line_num.coefficient.shape[2]
        self.line_num.coefficient[:,:,:] = \
            lnpar[0:nx*ny*nz].reshape((nx,ny,nz))
        nx2 = self.line_den.coefficient.shape[0]
        ny2 = self.line_den.coefficient.shape[1]
        nz2 = self.line_den.coefficient.shape[2]
        self.line_den.coefficient[:,:,:] = \
            np.concatenate(([1.0,], lnpar[nx*ny*nz:])).reshape(nx2,ny2,nz2)
        nx = self.sample_num.coefficient.shape[0]
        ny = self.sample_num.coefficient.shape[1]
        nz = self.sample_num.coefficient.shape[2]
        self.sample_num.coefficient[:,:,:] = \
            smpar[0:nx*ny*nz].reshape((nx,ny,nz))
        nx2 = self.sample_den.coefficient.shape[0]
        ny2 = self.sample_den.coefficient.shape[1]
        nz2 = self.sample_den.coefficient.shape[2]
        self.sample_den.coefficient[:,:,:] = \
            np.concatenate(([1.0,], smpar[nx*ny*nz:])).reshape(nx2,ny2,nz2)

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
    def __call__(self, lat, lon, h):
        x = np.stack((lat, lon, h), axis=-1)
        return self.line_grid(x), self.sample_grid(x)
    def fit(self, line, sample, latitude, longitude, height, wh = None):
        if(height.size != self.np_z):
            raise RuntimeError("Fit was not passed the correct number of heights. Expected %d but got %s" % (np_z, height.shape))
        latv = np.linspace(latitude.min(), latitude.max(), self.np_x)
        lonv = np.linspace(longitude.min(), longitude.max(), self.np_y)
        ldata = np.empty((self.np_x, self.np_y, self.np_z))
        sdata = np.empty(ldata.shape)
        for i in range(self.np_z):
            lat = latitude[:,:,i]
            lon = longitude[:,:,i]
            ln = line[:,:,i]
            smp = sample[:,:,i]
            if(wh is not None):
                lat = lat[wh[:,:,i]]
                lon = lon[wh[:,:,i]]
                ln = ln[wh[:,:,i]]
                smp = smp[wh[:,:,i]]
            else:
                lat = lat.reshape(lat.size)
                lon = lon.reshape(lon.size)
                ln= ln.reshape(ln.size)
                smp = smp.reshape(smp.size)
            gd = scipy.interpolate.griddata((lat,lon), np.stack((ln, smp), axis=1), (np.outer(latv,np.ones_like(lonv)), np.outer(np.ones_like(latv), lonv)))
            ldata[...,i] = gd[...,0]
            sdata[...,i] = gd[...,1]
        self.line_grid = scipy.interpolate.RegularGridInterpolator((latv,lonv,height), ldata, bounds_error=False,fill_value=None)
        self.sample_grid = scipy.interpolate.RegularGridInterpolator((latv,lonv,height), sdata, bounds_error=False,fill_value=None)

    
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

    def __call__(self, lat, lon, h):
        line, sample = self.rational_poly(lat,lon,h)
        lcorr, scorr  = self.corr_grid(lat,lon,h)
        return [line + lcorr, sample + scorr] 

    def fit(self, line, sample, latitude, longitude, height):
        self.rational_poly.fit(line,sample,latitude,longitude,height)
        lcalc, scalc = self.rational_poly(latitude,longitude,height)
        lcorr = line - lcalc
        scorr = sample - scalc
        self.corr_grid.fit(lcorr, scorr, latitude, longitude, height)

