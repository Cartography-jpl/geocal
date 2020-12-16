from .quick_bird_reflectance import *
from test_support import *


def test_print():
    '''Test printing of metadata'''
    qb = QuickBirdReflectance(unit_test_data + 
                              "07JUN08184839-M1BS-005616921010_01_P001.IMD",
                              unit_test_data + 
                              "07JUN08184839-P1BS-005616921010_01_P001.IMD"
                              )                        
    qb.printMetadata()

def test_refl():
    '''Test reflectance calculation.'''
    qb = QuickBirdReflectance(unit_test_data + 
                              "07JUN08184839-M1BS-005616921010_01_P001.IMD",
                              unit_test_data + 
                              "07JUN08184839-P1BS-005616921010_01_P001.IMD"
                              )                        
    dn = np.array([[416, 398, 378], 
                   [435, 403, 369], 
                   [438, 409, 375]])
    ref_expect = np.array([[ 0.21578284,  0.20644608,  0.1960719 ],
                           [ 0.2256383,   0.20903962,  0.19140353],
                           [ 0.22719443,  0.21215187,  0.19451578]])
    assert_almost_equal(qb.dn2TOAReflectance(dn, 4), ref_expect, 8) 

