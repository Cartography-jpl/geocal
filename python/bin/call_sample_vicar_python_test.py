from geocal import *
from vicar_test_support import *
import subprocess
import os
import sys

# Setup and teardown called automatically by nosetest for whole module
original_env = None
def setup():
    check_vicarb()
    add_tae_path(os.path.dirname(__file__), original_env)

def teardown():
    set_original_env(original_env)

def test_call_sample_vicar_python():
    '''Class the sample vicar program.'''
    # This should return the string "ret=101", along with the normal VICAR
    # verbage
    res = subprocess.check_output(["vicarb", 
                                   "call_sample_vicar_python"])
    if sys.version_info > (3,):
        res = res.decode("utf-8")
    assert res.find("ret=101") != -1
        


