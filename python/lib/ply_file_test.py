from nose.tools import *
from ply_file import *

def test_ply_file():
    with PlyFile("test.ply") as f:
        f.vertex = [[1,2,3],[4,5,6]]

def test_ply_file_value():
    with PlyFile("test.ply", binary_format = False) as f:
        f.vertex = [[1,2,3, 0.2],[4,5,6, 0.3]]
