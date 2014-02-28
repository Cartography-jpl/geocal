from geocal import *
from ibis_file_extension import *
from nose.plugins.skip import Skip, SkipTest

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
        f[0, 0] = 1
        f[0, 1] = 2
        f[0, 2] = 3
        f[0, 3] = 4.0
        f[0, 4] = 5.0
        f[0, 5] = "aaa"
    f = IbisFile("ibis.img")
    assert f.column_data_type(0) == IbisFile.VICAR_BYTE
    assert f.column_data_type(1) == IbisFile.VICAR_HALF
    assert f.column_data_type(2) == IbisFile.VICAR_FULL
    assert f.column_data_type(3) == IbisFile.VICAR_FLOAT
    assert f.column_data_type(4) == IbisFile.VICAR_DOUBLE
    assert f.column_data_type(5) == IbisFile.VICAR_ASCII
    assert f.column(5).size_byte == 11 # Extra 1 is for the null
    assert f.shape[0] == 10
    assert f.shape[1] == 6

