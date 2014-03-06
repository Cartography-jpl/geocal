from geocal import *
from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
import subprocess
import os
import string

def cmd_exists(cmd):
    '''Check if a cmd exists by using type, which returns a nonzero status if
    the program isn't found'''
    return subprocess.call("type " + cmd, shell=True, 
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE) == 0

def test_call_sample_vicar_python():
    '''Class the sample vicar program.'''
    # We may or may not have vicarb. If not, we just skip this test.
    if(not cmd_exists("vicarb")):
        raise SkipTest()
    try:
        curdir = os.getcwd()
        os.chdir(os.path.dirname(__file__))
        # This should return the string "ret=101", along with the normal VICAR
        # verbage
        res = subprocess.check_output(["vicarb", 
                                       "call_sample_vicar_python.pdf"])
        assert string.find(res, "ret=101") != -1
    finally:
        if(curdir):
            os.chdir(curdir)
        


