from nose.tools import *
from geocal import *
from vicar_interface import *
from nose.plugins.skip import Skip, SkipTest

def test_vicar_to_numpy():
    '''Test vicar_to_numpy.'''
    # Temporary, we'll come back and make this optional depending on if
    # we have afids available or not
    raise SkipTest
    out = vicar_to_numpy("gen out NL=15 NS=15 IVAL=90")
    assert out[1, 2] == 93


