from test_support import *
from geocal.mars_igc import *

@require_spice
@require_mars_spice
def test_igc_mro_context(mars_test_data):
    igc = igc_mro_context(mars_test_data + "P16_007388_2049_XI_24N020W.IMG")
    print(igc)

@require_spice
@require_mars_spice
def test_igc_mex_hrsc(mars_test_data):
    igc = igc_mex_hrsc(mars_test_data + "h1326_0000_nd2.img")
    print(igc)
    
    
    
