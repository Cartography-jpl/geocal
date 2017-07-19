# This is a python implementation of a RSM rational polynomial. We will almost
# certainly want to reimplement this is C++, but do this in python first to
# have much quicker to write baseline
import numpy as np

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

    def fit_offset_and_scale(self, nline, nsample, min_height, max_height,
                             latitude, longitude):
        '''Set line_offset, line_scale etc to cover the range for each
        of the set of data provided.'''
        self.height_offset = (max_height + min_height) / 2.0
        self.height_scale = (max_height - min_height) / 2.0
        self.line_offset = nline / 2.0
        self.line_scale = nline / 2.0
        self.sample_offset = nsample / 2.0
        self.sample_scale = nsample / 2.0
        min_lat = latitude.min()
        max_lat = latitude.max()
        min_lon = longitude.min()
        max_lon = longitude.max()
        self.latitude_offset = (max_lat + min_lat) / 2
        self.latitude_scale = (max_lat - min_lat) / 2
        self.longitude_offset = (max_lon + min_lon) / 2
        self.longitude_scale = (max_lon - min_lon) / 2

    def fit(self, line, sample, latitude, longitude, height):
        ln_lhs = (line - self.line_offset) / self.line_scale
        smp_lhs = (sample - self.sample_offset) / self.sample_offset
        x = (latitude - self.latitude_offset) / self.latitude_scale
        y = (longitude - self.longitude_offset) / self.longitude_scale
        z = (height - self.height_offset) / self.height_scale
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

class LowOrderPolynomial(object):
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
        g = np.zeros((x.size, 10))
        g[:,0] = 1
        g[:,1] = x
        g[:,2] = y
        g[:,3] = z
        g[:,4] = x * x
        g[:,5] = x * y
        g[:,6] = x * z
        g[:,7] = y * y
        g[:,8] = y * z
        g[:,9] = z * z
        self.pline = np.linalg.lstsq(g,line)[0]
        self.psamp = np.linalg.lstsq(g,sample)[0]
        
