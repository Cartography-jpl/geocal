from fixtures.require_check import require_msp
from geocal import IgcMsp

@require_msp
def test_igc_msp():
    igc = IgcMsp()
    igc.print_plugin_list()
    print(igc.plugin_list)
    print(igc.model_dict)
    
