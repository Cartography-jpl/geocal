from geocal_swig import *
from geocal.ibis_file_extension import *
from nose.plugins.skip import Skip, SkipTest
import numpy.testing as npt

def test_ibis():
    '''Basic test of reading and writing an ibis file.'''
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        IbisFile
    except NameError:
        raise SkipTest
    
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

def test_ibis_create():
    '''Test IbisFile.create'''
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        IbisFile
    except NameError:
        raise SkipTest

    d = np.array([[1.0,2,3],[4,5,6]])
    IbisFile.create("ibis.img", d)
    f = IbisFile("ibis.img")
    npt.assert_almost_equal(np.array(f[:,:]), d)
