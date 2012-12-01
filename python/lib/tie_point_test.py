from nose.tools import *
from tie_point import *

# Basic test of tiepoint
def test_tie_point():
    t = TiePoint(5)
    assert t.number_camera, 5

