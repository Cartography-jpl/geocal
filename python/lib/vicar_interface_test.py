from nose.tools import *
from geocal_swig import *
from geocal.vicar_interface import *
from nose.plugins.skip import Skip, SkipTest
import os

# Simple test to see if we have VICAR available. We check for the presence of
# one of the AFIDS environment variables, and if there assume we have VICAR
have_vicar = "AFIDS_ROOT" in os.environ 

def test_vicar_to_numpy():
    '''Test vicar_to_numpy.'''
    if(not have_vicar):
        raise SkipTest
    out = vicar_to_numpy("gen out NL=15 NS=15 IVAL=90")
    assert out[1, 2] == 93


