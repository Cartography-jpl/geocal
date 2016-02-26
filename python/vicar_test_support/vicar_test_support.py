from nose.plugins.skip import Skip, SkipTest
import subprocess
import os
import sys

def cmd_exists(cmd):
    '''Check if a cmd exists by using type, which returns a nonzero status if
    the program isn't found'''
    return subprocess.call("type " + cmd, shell=True, 
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE) == 0

def check_vicarb():
    '''Check for existence of vicarb program. If not found, raise SkipTest.'''
    if(not cmd_exists("vicarb")):
        raise SkipTest()
    # AFIDS uses python 3.5, so don't both supporting python 2 with this code.
    # We just skip all the tests if we are using python 2.
    if sys.version_info < (3,5):
        raise SkipTest()

def vicarb_run(cmd):
    '''Wrapper for running vicarb. Returns results from subprocess.run if you
    need to access stdout or other variables.'''
    res = subprocess.run("vicarb %s" % cmd, shell=True,check=True,
                         stdout = subprocess.PIPE)
    return res
    
def vicarb_tae_path():
    '''Get the TAE_PATH used by vicarb'''
    try:
        curdir = os.getcwd()
        os.chdir(os.path.dirname(__file__))
        res = subprocess.run(["vicarb", "print_tae_path.pdf"], check=True,
                             stdout=subprocess.PIPE)
        return res.stdout.decode("utf-8").splitlines()[-1]
    finally:
        if(curdir):
            os.chdir(curdir)

def add_tae_path(d, original_env):
    '''Add the given directory to the TAE_PATH environment variable, which
    can then be used in testing with vicarb. In particular you can call
    with os.path.dirname(__file__) to test code in the same directory as
    your test. The advantage of doing this instead of just changing to that
    local directory is that all the output files etc. can be kept out of the
    source tree.

    Note that you should set the environment variable back to it original
    value when done. This can be conveniently done with a setup and teardown 
    function, see call_sample_vicar_python_test for an example.
    '''
    original_env = os.environ.get('TAE_PATH', None)
    tae_path = os.path.abspath(d) + ":" + vicarb_tae_path()
    os.environ['TAE_PATH'] = tae_path

def set_original_env(original_env):
    '''Pair to add_tae_path which sets the environment back to what it 
    originally was.'''
    if(original_env):
        os.environ['TAE_PATH'] = original_env
    else:
        if 'TAE_PATH' in os.environ:
            del os.environ['TAE_PATH']
