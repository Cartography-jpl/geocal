from nose.tools import *
from geocal import *
from vicar_interface import *

def test_vicar_to_numpy():
    '''Test vicar_to_numpy.'''
    out = vicar_to_numpy("gen out NL=15 NS=15 IVAL=90")
    assert out[1, 2] == 93


