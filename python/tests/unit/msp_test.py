from fixtures.require_check import require_msp
try:
    from msp_swig import Test
except ImportError:
    # Ok if we don't have msp_swig, we just can't do tests on it
    pass

def test_msp():
    t = Test()
    t.msp_print_plugin_list()
    
