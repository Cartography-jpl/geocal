from .target import *
from test_support import *

@require_rsync
@require_python3
def test_input_file_target(isolated_dir):
    '''Test a input file target'''
    ft = InFileTarget("test.txt")
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("test.txt")
    assert ft.exists()
    ft.finish()
    assert os.path.exists("test.txt")
    assert ft.exists()
    ft.remove()
    assert os.path.exists("test.txt")

@require_rsync
@require_python3
def test_input_file_target_error(isolated_dir):
    '''Test a input file target when an error occurs'''
    ft = InFileTarget("test.txt")
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert ft.exists()
    assert os.path.exists("test.txt")
    ft.finish_error()
    assert os.path.exists("test.txt")
    assert ft.exists()

@require_rsync
@require_python3
def test_output_file_target(isolated_dir):
    '''Test a output file target'''
    ft = OutFileTarget("test.txt")
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("test.txt.generating")
    assert not os.path.exists("test.txt")
    assert not ft.exists()
    ft.finish()
    assert not os.path.exists("test.txt.generating")
    assert os.path.exists("test.txt")
    assert ft.exists()
    ft.remove()
    assert not os.path.exists("test.txt")
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("test.txt.generating")
    ft.remove()
    assert not os.path.exists("test.txt.generating")
    
@require_rsync
@require_python3
def test_output_file_target_error(isolated_dir):
    '''Test a output file target when an error occurs'''
    ft = OutFileTarget("test.txt")
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("test.txt.generating")
    assert not os.path.exists("test.txt")
    assert not ft.exists()
    ft.finish_error()
    assert not os.path.exists("test.txt.generating")
    assert os.path.exists("test.txt.error")
    assert not ft.exists()
    
