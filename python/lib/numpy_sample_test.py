from test_support import *
from geocal_swig import NumpySample

# Note that these tests should be run with valgrind, otherwise we don't know
# if the lifetimes are correct or not.
#
# A nice way to run this is with --free-fill=0xff in valgrind, so it forces
# the tests to fail (since the values get changed). Something like:
#
# PYTHONPATH=`pwd`:${PYTHONPATH} valgrind --log-file=valgrind.out --free-fill=0xff python `which py.test` -s lib/numpy_sample_test.py
#
# Note that python has lots and lots of things that valgrind hates. You should
# ignore all of this - it is optimization stuff that the python code writers
# need to worry about. We stick all of this in a separate file, just so we can
# see the output. If you need to track something down, you can go ahead and
# write to the screen and then look for the string "Find me in valgrind" for
# where you can start paying attention to problems.
#
# Can also compile with flags to check lifetime:
#
# CXXFLAGS="-ggdb -DBZ_DEBUG -fbounds-check -DBZ_DEBUG_LOG_ALLOCATIONS -DBZ_DEBUG_LOG_REFERENCES"

def test_lifetime_return_array():
    '''Return a blitz::array, and then make sure numpy still valid after 
    original blitz::array goes away.'''
    print("Find me in valgrind")
    t = NumpySample()
    a = t.data
    print("Deleting NumpySample")
    t = None
    print(a[0,0])
    assert a[0,0] == 1
    print("Deleting last sample of numpy")
    a = None
    print("End of find me in valgrind")
