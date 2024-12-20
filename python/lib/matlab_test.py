from test_support import *
import numpy.testing as npt

# Optional support for running matlab from python
try:
    from pymatbridge import Matlab
    have_matlab = True
except ImportError:
    have_matlab = False

@matlab
def test_matlab():
    '''Basic test of matlab interface. Note that in addition to python,
    there is ipython magic by %load_ext pymatbridge'''
    if(not have_matlab):
        raise SkipTest
    mlab = Matlab()
    mlab.start()
    results = mlab.run_code('a=1;')
    assert mlab.get_variable('a') == 1
    b = np.array([[1, 2],[3,4.0]])
    mlab.set_variable('b', b)
    bb = mlab.get_variable('b')
    npt.assert_almost_equal(bb, b)
    # Have a jk.m file in same location as test file.
    res = mlab.run_func("%s/jk.m" % os.path.dirname(__file__), 
                        {'arg1':3, 'arg2':5})
    assert res['result'] == 8

    
    
