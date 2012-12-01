from nose.tools import *
from ikonos_reflectance import *
import os
import numpy as np
import numpy.testing as nptest

test_data = os.path.dirname(__file__) + "/../unit_test_data/"

ik = IkonosReflectance(test_data + "418524_meta.txt")

def test_print():
    '''Test printing of metadata'''
    ik.printMetadata()

def test_refl():
    '''Test reflectance calculation.'''
    dn = np.array([[416, 398, 378], 
                   [435, 403, 369], 
                   [438, 409, 375]])
    ref_expect = np.array([[ 0.15923692,  0.15234687,  0.14469124],
                           [ 0.16650977,  0.15426077,  0.14124621],
                           [ 0.16765811,  0.15655746,  0.1435429 ]])
    nptest.assert_almost_equal(ik.dn2TOAReflectance(dn, 4), 
                               ref_expect, 8) 

