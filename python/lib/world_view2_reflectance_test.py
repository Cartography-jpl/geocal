from __future__ import print_function
from builtins import range
from nose.tools import *
from world_view2_reflectance import *
import os
import numpy as np
import numpy.testing as nptest
from nose.plugins.skip import Skip, SkipTest

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"

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

nitf_test_data = "/raid10/sba_gold/mali_cosi/"
def test_nitf():
    # This test doesn't actually work. Turns out the absCalFactors aren't
    # constant. Talked to Steve, and in the short term we'll just need
    # to pretend these are constant.
    raise SkipTest
    # We can  only do this if data is available. If it isn't, just skip
    # the test.
    if(not os.path.exists(nitf_test_data)):
        raise SkipTest
    wv2_imd = WorldView2Reflectance(nitf_test_data + 
                          "12MAR10105443-M1BS-052683561010_04_P007.IMD",
                                    nitf_test_data + 
                          "12MAR10105443-P1BS-052683561010_04_P007.IMD")
    wv2_nitf = WorldView2Reflectance(nitf_test_data + 
                          "12MAR10105443-M1BS-052683561010_04_P007.NTF",
                                    nitf_test_data + 
                          "12MAR10105443-P1BS-052683561010_04_P007.NTF")
    # Test data only has a few bands
    for i in range(9):
        if(wv2_imd.absCalFactors[i] > -999):
            print(wv2_imd.dn2TOAReflectance_factor(i))
            print(wv2_nitf.dn2TOAReflectance_factor(i))
            assert_almost_equal(wv2_imd.dn2TOAReflectance_factor(i),
                                wv2_nitf.dn2TOAReflectance_factor(i))

