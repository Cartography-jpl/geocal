from nose.tools import *
from geoeye1_reflectance import *
import os
import numpy as np
import numpy.testing as nptest

test_data = os.path.dirname(__file__) + "/../unit_test_data/"

ge1 = Geoeye1Reflectance(test_data + 
                         "5V090727M0003172424A222000100202M_001535172.pvl")

def test_print():
    '''Test printing of metadata'''
    ge1.printMetadata()

def test_refl():
    '''Test reflectance calculation.'''
    dn = np.array([[416, 398, 378], 
                   [435, 403, 369], 
                   [438, 409, 375]])
    ref_expect = np.array([[ 0.10461536,  0.10008873,  0.09505915],
                           [ 0.10939347,  0.10134613,  0.09279584],
                           [ 0.1101479,   0.10285501,  0.09430471]])
    nptest.assert_almost_equal(ge1.dn2TOAReflectance(dn, 4), 
                               ref_expect, 8) 

