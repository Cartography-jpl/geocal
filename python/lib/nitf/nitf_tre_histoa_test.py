from .nitf_tre import *
from .nitf_tre_histoa import *
from test_support import *
import io, six
    
def test_basic():

    numEvents = 2

    t = TreHISTOA()

    #Set some values
    t.systype = "SYSTEM_TYPE"
    t.pc = "NO_COMPRESSI"
    t.pe = "NONE"
    t.remap_flag = "0"
    t.lutid = 0
    t.nevents = numEvents

    for i in range(numEvents):
        t.pdate[i] = "20170615121212"
        t.psite[i] = "ABCDEFGHIJ"
        t.pas[i] = "AAAAAAAAAA"
        t.nipcom[i] = 2
        for j in range(t.nipcom[i]):
            t.ipcom[i, j] = "HELLO1"
            t.ipcom[i, j] = "HELLO2"

    fh = six.BytesIO()
    t.write_to_file(fh)

    print (t.summary())

    '''assert fh.getvalue() == b'HISTOA00309YMy sensor                blah                            My platform              blah                            blah      1WGS84GHAESID  2017042700000000000000201704270000000000000100201704270000000000NNN00000000000000000000000000000000000001000000000002000000100000000000000000000000000000NNNN000000000000'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreSENSRB()
    t2.read_from_file(fh2)
    assert t2.sensor_array_data == 'N'
    assert t2.sensor_calibration_data == 'N'
    assert t2.image_formation_data == 'N'''
