import re
from test_support import *

def check_for_proc(proc):
    '''Check for the existence of a vicar/AFIDS proc.'''
    # AFIDS uses python 3.5, so don't bother supporting python 2 with this code.
    # We just skip all the tests if we are using python 2.
    if sys.version_info < (3,5):
        return False
    cmd2 = re.sub(r'\+?\n',"", proc)
    cmd2 = re.sub(r'"',"\\\"", cmd2)
    res = subprocess.run("vicarb \"%s\"" % cmd2, shell=True,
                         stdout = subprocess.PIPE)
    if re.search(br"\[TAE-NOPROC\]", res.stdout):
        return False
    return True

def vicarb_run(cmd, print_results = False):
    '''Wrapper for running vicarb. Returns results from subprocess.run if you
    need to access stdout or other variables.

    Note you can use the standard tae "+\n" to have multiple lines, useful 
    if you are cutting and pasting from an existing VICAR test script'''
    cmd2 = re.sub(r'\+?\n',"", cmd)
    cmd2 = re.sub(r'"',"\\\"", cmd2)
    if(not print_results):
        res = subprocess.run("vicarb \"%s\"" % cmd2, shell=True,check=True,
                             stdout = subprocess.PIPE, stderr=subprocess.STDOUT)
        return res
    # This is a little more complicated than just subprocess.run
    # because we want to print the output as it is generated, not
    # all bunched up at the end - so a long running process will have
    # output while we wait
    process = subprocess.Popen("vicarb \"%s\"" % cmd2, shell=True,
                               stdout = subprocess.PIPE,
                               stderr=subprocess.STDOUT)
    res = b''
    while(True):
        output = process.stdout.readline()
        if output == b'' and process.poll() is not None:
            break
        if output:
            res = res + output
            print(output.strip().decode('utf-8'))
    if(process.poll() != 0):
        raise subprocess.CalledProcessError(process.poll(),
                                            "vicarb \"%s\"" % cmd2,
                                            output=res)
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


@pytest.yield_fixture(scope="function")
def vicarb_env(isolated_dir):
    '''This sets up for a test of our own pdf files. We make sure that we
    have the proper TAEPATH, and we also set up to run in an isolated 
    directory so we capture all the files vicar tends to vomit.'''
    original_env = None
    add_tae_path(isolated_dir, original_env)
    yield
    set_original_env(original_env)
    
