from geocal import *
from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
import subprocess
import os
import sys

def cmd_exists(cmd):
    '''Check if a cmd exists by using type, which returns a nonzero status if
    the program isn't found'''
    return subprocess.call("type " + cmd, shell=True, 
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE) == 0

def test_call_sample_vicar_python():
    '''Class the sample vicar program.'''
    # Temporary work around. We currently have "python" being python 2
    # on afids, and "python3" being 3, since we are working with a mixed
    # environment. This should be temporary, we plan on switching to
    # python 3 (with tests that python 2). But as things currently stand
    # the shebang in call_sample_vicar_python doesn't work correctly since
    # it points to the wrong version of python. We'll sort this out in 
    # a bit, but stub out for now.
    if sys.version_info > (3,):
        raise SkipTest()
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
        if sys.version_info > (3,):
            res = res.decode("utf-8")
        assert res.find("ret=101") != -1
    finally:
        if(curdir):
            os.chdir(curdir)
        


