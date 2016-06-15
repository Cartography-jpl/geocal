from geocal.spot_reflectance import *
from test_support import *


# Doesn't currently work
@skip
def test_print():
    '''Test printing of metadata'''
    sr = SpotReflectance(unit_test_data + "spot6_multi_meta.xml", 
                     unit_test_data + "spot6_pan_meta.xml")
    sr.printMetadata()

# Doesn't currently work
@skip
def test_refl():
    '''Test reflectance calculation.'''
    sr = SpotReflectance(unit_test_data + "spot6_multi_meta.xml", 
                     unit_test_data + "spot6_pan_meta.xml")
    dn = np.array([[252, 199, 200], 
                   [241, 155, 142], 
                   [224, 142, 124]])
    ref_expect = np.array([[0.1388, 0.1096, 0.1102],
                           [0.1327, 0.08537, 0.07821],
                           [0.1234, 0.07821, 0.06830]]);
    assert_almost_equal(sr.dn2TOAReflectance(dn, 3), ref_expect, 3) 

