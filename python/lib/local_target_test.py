from __future__ import print_function
from geocal.local_target import *
from test_support import *

@require_rsync
@pytest.yield_fixture(scope="function")
def input_local_data(isolated_dir):
    '''Generate some input test data so we can check that it works.'''
    fname = os.path.abspath("./test_input/temp.txt")
    makedirs_p(os.path.dirname(fname))
    with open(fname, "w") as fh:
        print("hi there", file=fh)
    yield fname
    
@require_rsync
@require_python3
def test_input_local_target(input_local_data):
    '''Test a local input file target'''
    ldir = os.path.abspath("./local_directory")
    assert os.path.exists(input_local_data)
    assert not os.path.exists(ldir + input_local_data)
    ft = InLocalTarget(input_local_data, ldir)
    assert ft.exists()
    assert ft.filename() == input_local_data
    assert ft.local_filename() == ldir + input_local_data
    assert os.path.exists(input_local_data)
    assert not os.path.exists(ldir + input_local_data)
    ft.prepare()
    assert os.path.exists(ldir + input_local_data)
    ft.finish()
    assert ft.exists()
    assert os.path.exists(input_local_data)
    assert os.path.exists(ldir + input_local_data)
    ft.remove()
    assert os.path.exists(input_local_data)
    assert not os.path.exists(ldir + input_local_data)

@require_rsync
@require_python3
def test_input_local_target_error(input_local_data):
    '''Test a local input file target when an error occurs'''
    ldir = os.path.abspath("./local_directory")
    assert os.path.exists(input_local_data)
    assert not os.path.exists(ldir + input_local_data)
    ft = InLocalTarget(input_local_data, ldir)
    assert ft.exists()
    assert ft.filename() == input_local_data
    assert ft.local_filename() == ldir + input_local_data
    assert os.path.exists(input_local_data)
    assert not os.path.exists(ldir + input_local_data)
    ft.prepare()
    assert os.path.exists(ldir + input_local_data)
    ft.finish_error()
    assert ft.exists()
    assert os.path.exists(input_local_data)
    assert os.path.exists(ldir + input_local_data)

@require_rsync
@require_python3
def test_output_local_target(isolated_dir):
    '''Test local output file target'''
    fname = os.path.abspath("./test_output/temp.txt")
    assert not os.path.exists("./test_output")
    ft = OutLocalTarget(fname, "./local_directory")
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("./local_directory" + fname + ".generating")
    assert not os.path.exists(fname)
    assert not ft.exists()
    ft.finish()
    assert os.path.exists("./local_directory" + fname)
    assert os.path.exists(fname)
    assert ft.exists()
    ft.remove()
    assert not os.path.exists("./local_directory" + fname)
    assert not os.path.exists(fname)
    
@require_rsync
@require_python3
def test_output_local_target_error(isolated_dir):
    '''Test a local output file target when an error occurs'''
    fname = os.path.abspath("./test_output/temp.txt")
    assert not os.path.exists("./test_output")
    ft = OutLocalTarget(fname, "./local_directory")
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("./local_directory" + fname + ".generating")
    assert not os.path.exists(fname)
    assert not ft.exists()
    ft.finish_error()
    assert os.path.exists("./local_directory" + fname + ".error")
    assert os.path.exists(fname + ".error")
    assert not ft.exists()

@require_rsync
@require_python3
def test_output_temp_local_target(isolated_dir):
    '''Test local output file target'''
    fname = os.path.abspath("./test_output/temp.txt")
    assert not os.path.exists("./test_output")
    ft = OutTempLocalTarget(fname, "./local_directory")
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("./local_directory" + fname + ".generating")
    assert not os.path.exists(fname)
    assert not ft.exists()
    ft.finish()
    assert os.path.exists("./local_directory" + fname)
    # Check that real file never is created
    assert not os.path.exists(fname)
    assert ft.exists()
    ft.remove_temporary()
    assert not os.path.exists("./local_directory" + fname)
    assert not ft.exists()
    with open(ft.local_filename(), "w") as fh:
        print("hi there", file=fh)
    assert os.path.exists("./local_directory" + fname + ".generating")
    ft.remove()
    assert not os.path.exists("./local_directory" + fname + ".generating")
    
@require_rsync
@require_python3
def test_output_temp_dir_local_target(isolated_dir):
    '''Test local output file target'''
    fname = os.path.abspath("./test_output/dir")
    bname = "./local_directory/" + os.path.dirname(os.path.dirname(fname))
    assert not os.path.exists(bname + "/test_output/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_error/dir/test.txt")
    ft = OutTempDirLocalTarget("test_output/dir", "./local_directory")
    assert not ft.exists()
    makedirs_p(ft.local_filename())
    with open(ft.local_filename() + "/test.txt", "w") as fh:
        print("hi there", file=fh)
    assert not os.path.exists(bname + "/test_output/dir/test.txt")
    assert os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_error/dir/test.txt")
    assert not ft.exists()
    ft.finish()
    assert os.path.exists(bname + "/test_output/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_error/dir/test.txt")
    assert ft.exists()
    ft.remove_temporary()
    assert not os.path.exists(bname + "/test_output/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_error/dir/test.txt")
    assert not ft.exists()

@require_rsync
@require_python3
def test_output_temp_dir_local_target_error(isolated_dir):
    '''Test local output file target'''
    fname = os.path.abspath("./test_output/dir")
    bname = "./local_directory/" + os.path.dirname(os.path.dirname(fname))
    assert not os.path.exists(bname + "/test_output/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_error/dir/test.txt")
    ft = OutTempDirLocalTarget("test_output/dir", "./local_directory")
    assert not ft.exists()
    makedirs_p(ft.local_filename())
    with open(ft.local_filename() + "/test.txt", "w") as fh:
        print("hi there", file=fh)
    assert not os.path.exists(bname + "/test_output/dir/test.txt")
    assert os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_error/dir/test.txt")
    assert not ft.exists()
    ft.finish_error()
    assert not os.path.exists(bname + "/test_output/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert os.path.exists(bname + "/test_output_error/dir/test.txt")
    assert not ft.exists()
    ft.remove_temporary()
    assert not os.path.exists(bname + "/test_output/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_generating/dir/test.txt")
    assert not os.path.exists(bname + "/test_output_error/dir/test.txt")
    assert not ft.exists()

# This was copied over from MSPI. We can't actually do this test here since
# we don't have all the support stuff. I'll leave this here, but we really
# need some other test here.
#from .l1b2_igc_task import L1b2IgcTask
#from geocal import read_shelve
#def test_xml_local_target(task_with_l1b1_test,
#                          test_version_info, test_target_info, cache_result):
    # We have a number of sequences and view here. Just pick the
    # first one to run.
#    views = test_target_info.views(test_target_info.sequence_numbers()[0])
#    tsk = L1b2IgcTask(views, test_version_info, cache_result=cache_result)
#    tsk.run_pipeline(skip_cleanup_on_error=True)
#    igc_fname = tsk.igc.filename()
#    ft = XmlLocalTarget(igc_fname, "./local_directory")
#    igc = read_shelve(ft.filename())
#    ft.prepare()
#    igc = read_shelve(ft.local_filename())
    
    
