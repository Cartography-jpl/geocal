# This is a python implementation of a RSM rational polynomial. We will almost
# certainly want to reimplement this is C++, but do this in python first to
# have much quicker to write baseline
import numpy as np
import scipy
import math
from geocal_swig import Rpc, RsmPolynomial, RsmRationalPolynomial, RsmLowOrderPolynomial, RsmGrid, RsmMultiSection

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

    # Needs work
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

