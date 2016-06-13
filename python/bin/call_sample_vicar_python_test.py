from geocal import *
from vicar_test_support import *
import subprocess
import os
import sys

@require_vicarb    
def test_call_sample_vicar_python(vicarb_env):
    '''Class the sample vicar program.'''
    # This should return the string "ret=101", along with the normal VICAR
    # verbage
    res = subprocess.check_output(["vicarb", 
                                   "call_sample_vicar_python"])
    if sys.version_info > (3,):
        res = res.decode("utf-8")
    assert res.find("ret=101") != -1
        


