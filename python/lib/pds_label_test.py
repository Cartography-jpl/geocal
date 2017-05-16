from geocal.pds_label import *
from test_support import *
import os

def test_pds_label_text():
    if(not os.path.exists("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")):
        # Skip we don't have test data available
        raise SkipTest
    txt = pds_label_text("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")
    print(txt)

def test_pds_label():
    if(not os.path.exists("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")):
        # Skip we don't have test data available
        raise SkipTest
    d = pds_label("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")
    print(d)
    
