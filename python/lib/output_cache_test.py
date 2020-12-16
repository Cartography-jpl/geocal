from .output_cache import *
from .local_target import *
from .misc import makedirs_p
from test_support import *
import shutil

@require_python3
@require_rsync
def test_output_cache(isolated_dir):
    '''Test the use of the OutputCache.'''
    fname1 = os.path.abspath("./test_output/temp.txt")
    fname2 = os.path.abspath("./test_output/temp2.txt")
    ft1 = OutLocalTarget(fname1, "./local_directory")
    ft2 = OutLocalTarget(fname2, "./local_directory")
    with open(ft1.local_filename(), "w") as fh:
        print("hi there1", file=fh)
    with open(ft2.local_filename(), "w") as fh:
        print("hi there2", file=fh)
    stdout = b'fake stdout\nSecond fake line\n'
    key_arr = ['fake_exec', 'in1', 'temp.txt', 'temp2.txt']
    ocache = OutputCache("./output_cache")
    assert not ocache.contains(key_arr)
    ocache.store(key_arr,[ft1, ft2], stdout)
    shutil.rmtree("./local_directory", True)
    shutil.rmtree("./test_output", True)
    stdout = None
    assert ocache.contains(key_arr)
    stdout = ocache.load(key_arr, [ft1, ft2])
    assert os.path.exists(ft1.local_filename())
    assert os.path.exists(ft2.local_filename())
    assert stdout == b'fake stdout\nSecond fake line\n'

@require_python3
@require_rsync
def test_output_cache_dir(isolated_dir):
    '''Test the use of OutputCache with a directory'''
    ft1 = OutTempDirLocalTarget("./test_output/data", "./local_directory")
    makedirs_p(ft1.local_filename())
    with open(ft1.local_filename() + "/temp.txt", "w") as fh:
        print("hi there1", file=fh)
    stdout = b'fake stdout\nSecond fake line\n'
    key_arr = ['fake_exec_dir', 'data']
    ocache = OutputCache("./output_cache")
    assert not ocache.contains(key_arr)
    ocache.store(key_arr,[ft1,], stdout)
    shutil.rmtree("./local_directory", True)
    stdout = None
    assert ocache.contains(key_arr)
    stdout = ocache.load(key_arr, [ft1,])
    assert os.path.exists(ft1.local_filename() + "/temp.txt")
    assert stdout == b'fake stdout\nSecond fake line\n'
    
    

    
    
