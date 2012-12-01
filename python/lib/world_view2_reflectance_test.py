from nose.tools import *
from world_view2_reflectance import *
import os
import numpy as np
import numpy.testing as nptest

test_data = os.path.dirname(__file__) + "/../unit_test_data/"

wv2 = WorldView2Reflectance(test_data + 
                         "10MAY07185751-M1BS-052360785020_01_P001.IMD",
                         test_data + 
                         "10MAY07185751-P1BS-052360785020_01_P001.IMD"
                         )

def test_print():
    '''Test printing of metadata'''
    wv2.printMetadata()

def test_refl():
    '''Test reflectance calculation.'''
    dn = np.array([[416, 398, 378], 
                   [435, 403, 369], 
                   [438, 409, 375]])
    ref_expect = np.array([[ 0.18104724,  0.17321347,  0.16450928],
                           [ 0.18931623,  0.17538952,  0.16059239],
                           [ 0.19062186,  0.17800078,  0.16320365]])
    nptest.assert_almost_equal(wv2.dn2TOAReflectance(dn, 8), 
                               ref_expect, 8) 

