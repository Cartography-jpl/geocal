from geocal.ply_file import *
from test_support import *

def test_ply_file(isolated_dir):
    with PlyFile("test.ply") as f:
        f.vertex = [[1,2,3],[4,5,6]]

def test_ply_file_value(isolated_dir):
    with PlyFile("test.ply", binary_format = False) as f:
        f.vertex = [[1,2,3, 0.2],[4,5,6, 0.3]]
