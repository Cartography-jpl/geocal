# This is a python implementation of a RSM rational polynomial. We will almost
# certainly want to reimplement this is C++, but do this in python first to
# have much quicker to write baseline
import numpy as np

class RsmPolynomial(object):
    '''This is a polynomial, already working in the scaled XYZ coordinate
    system.'''
    def __init__(self, NP_X, NP_Y, NP_Z):
        # By convention, the coefficients are ordered in row major/fortran
        # order.
        self.coefficient = np.zeros((NP_X+1, NP_Y+1, NP_Z+1), order='C')

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
    
