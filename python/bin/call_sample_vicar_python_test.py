from geocal import *
from test_support import *
import subprocess
import os
import sys

local_dir = os.path.abspath(os.path.dirname(__file__))

@require_vicarb    
def test_call_sample_vicar_python(vicarb_env):
    '''Class the sample vicar program.'''
    # Want to run from local directory. Note that vicarb resets TAE_PATH
    # when we are done with this test, so we don't need to worry about
    # cleaning this up here.
    os.environ['TAE_PATH'] = local_dir + ":" + os.environ['TAE_PATH']

    # This should return the string "ret=101", along with the normal VICAR
    # verbage
    res = subprocess.check_output(["vicarb", 
                                   "call_sample_vicar_python"])
    if sys.version_info > (3,):
        res = res.decode("utf-8")
    assert res.find("ret=101") != -1
        


