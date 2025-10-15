from fixtures.require_check import require_msp
from geocal import IgcMsp

def test_igc_msp():
    igc = IgcMsp()
    igc.msp_print_plugin_list()
    
