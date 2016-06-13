from geocal.misc import *
from test_support import *

def test_makedirs_p(isolated_dir):
    '''Test makedirs_p by creating a directory and checking that it actually
    gets created.'''
    try:
       assert not os.path.exists("makedirs_test_dir_a/foo/bar")
       assert not os.path.exists("makedirs_test_dir_a/foo")
       assert not os.path.exists("makedirs_test_dir_a")
       makedirs_p("makedirs_test_dir_a/foo/bar")
       assert os.path.exists("makedirs_test_dir_a/foo/bar")
       assert os.path.exists("makedirs_test_dir_a/foo")
       assert os.path.exists("makedirs_test_dir_a")
       # Call a second time and make sure no errors occur even though
       # directory exists.
       makedirs_p("makedirs_test_dir_a/foo/bar")
       makedirs_p("makedirs_test_dir_a/foo")
       makedirs_p("makedirs_test_dir_a")
       assert os.path.exists("makedirs_test_dir_a/foo/bar")
       assert os.path.exists("makedirs_test_dir_a/foo")
       assert os.path.exists("makedirs_test_dir_a")
    finally:
        os.removedirs("makedirs_test_dir_a/foo/bar")

@require_afids_data
@require_vicar
# Doesn't currently work, we'll need to fix this
@skip
def test_cib01_data():
    if(not os.path.exists(os.environ["CIB1_ROOT"] + "/cib01_db.int")):
        raise SkipTest
    cib01 = cib01_data()
    assert cib01.number_line == 8795732
    assert cib01.number_sample == 27109425

@require_serialize    
def test_cib01_mapinfo():
    m = cib01_mapinfo()
    assert m.number_y_pixel == 8795732
    assert m.number_x_pixel == 27109425
