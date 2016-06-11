from nose.plugins.skip import Skip, SkipTest
from geocal.spot_reflectance import *
import os
import numpy as np
import numpy.testing as nptest

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"

sr = SpotReflectance(test_data + 
                         "spot6_multi_meta.xml", 
                         test_data + 
                         "spot6_pan_meta.xml"
                         )

def test_print():
    '''Test printing of metadata'''
    # Doesn't currently work
    raise SkipTest
    sr.printMetadata()

def test_refl():
    '''Test reflectance calculation.'''
    # Doesn't currently work
    raise SkipTest
    dn = np.array([[252, 199, 200], 
                   [241, 155, 142], 
                   [224, 142, 124]])
    ref_expect = np.array([[0.1388, 0.1096, 0.1102],
                           [0.1327, 0.08537, 0.07821],
                           [0.1234, 0.07821, 0.06830]]);
    nptest.assert_almost_equal(sr.dn2TOAReflectance(dn, 3), 
                               ref_expect, 3) 

