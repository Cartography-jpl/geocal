from geocal.pds_label import *
from test_support import *
import os
import glob

def test_pds_label_text():
    if(not os.path.exists("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")):
        # Skip if we don't have test data available
        raise SkipTest
    txt = pds_label_text("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")
    print(txt)

def test_pds_label():
    if(not os.path.exists("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")):
        # Skip if we don't have test data available
        raise SkipTest
    d = pds_label("/raid26/tllogan/mars/mawrth_vallis/source/unmapped/P16_007388_2049_XI_24N020W.IMG")
    print(d)

def test_spice_kernel_by_time():
    bdir = "/raid26/mars_kernels/mro_kernels/spk"
    if(not os.path.exists("/raid26/mars_kernels/mro_kernels/spk")):
        # Skip if we don't have test data available
        raise SkipTest
    kdata = SpiceKernelByTime(glob.glob(bdir + "/mro_*_ssd_mro110c.lbl"))
    tstart = Time.parse_time("2008-02-23T03:36:10.894Z")
    assert kdata.kernel(tstart) == "/raid26/mars_kernels/mro_kernels/spk/mro_psp6_ssd_mro110c.bsp"
    
