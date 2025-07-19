from .ibis_file_extension import *
from test_support import *

@require_vicar
def test_ibis(isolated_dir):
    '''Basic test of reading and writing an ibis file.'''
    with IbisFile("ibis.img", 10, 
                  ["BYTE", "HALF", "FULL", "REAL", "DOUB", "A10"]) as f:
        for i in range(10):
            f[i, 0] = i
            f[i, 1] = 2
            f[i, 2] = 3
            f[i, 3] = 4.0
            f[i, 4] = 5.0
            f[i, 5] = "aaa"
    f = IbisFile("ibis.img", IbisFile.UPDATE)
    assert f.column_data_type(0) == IbisFile.VICAR_BYTE
    assert f.column_data_type(1) == IbisFile.VICAR_HALF
    assert f.column_data_type(2) == IbisFile.VICAR_FULL
    assert f.column_data_type(3) == IbisFile.VICAR_FLOAT
    assert f.column_data_type(4) == IbisFile.VICAR_DOUBLE
    assert f.column_data_type(5) == IbisFile.VICAR_ASCII
    assert f.column(5).size_byte == 11 # Extra 1 is for the null
    assert f.shape[0] == 10
    assert f.shape[1] == 6
    assert list(f[3:5,0]) == [3,4]
    assert list(f[0,3:]) == [4.0, 5.0, "aaa"]
    assert f[3:5,0:3] == [[3,2,3],[4,2,3]]
    f[3:5,0] = [5,6]
    assert list(f[3:5,0]) == [5,6]
    f[2,3:] = [8.0, 9.0, "bbb"]
    assert list(f[2,3:]) == [8.0, 9.0, "bbb"]
    f[3:5,0:3] = [[25,11,2],[26,12,3]]
    assert f[3:5,0:3] == [[25,11,2],[26,12,3]]

@require_vicar
def test_ibis_create(isolated_dir):
    '''Test IbisFile.create'''
    d = np.array([[1.0,2,3],[4,5,6]])
    IbisFile.create("ibis.img", d)
    f = IbisFile("ibis.img")
    assert_almost_equal(np.array(f[:,:]), d)
