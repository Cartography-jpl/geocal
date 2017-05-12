from .nitf_tre import *
from .nitf_tre_csepha import *
from test_support import *
import io, six

#To Do: Figure out how to deal w floating point numbers
#and update the time component field unit test
def test_tre_csepha_basic():

    t = TreCSEPHA()

    r = 100
    offset1 = 1000
    offset2 = 2000

    # Set some values
    t.ephem_flag = "COLLECT_TIME"
    #t.dt_ephem = 1.5
    t.date_ephem = 20170101
    #t.t0_ephem = 235900.123456
    t.num_ephem = 100
    for n in range(r):
        t.ephem_x[n] = n * n
        t.ephem_y[n] = n * n + offset1
        t.ephem_z[n] = n * n + offset2

    fh = six.BytesIO()
    t.write_to_file(fh)
    #print(fh.getvalue())
    #assert fh.getvalue() == b'CSEPHA00'

    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreCSEPHA()
    t2.read_from_file(fh2)
    assert t2.ephem_flag == "COLLECT_TIME"
    #assert t2.dt_ephem == 1.5
    assert t2.date_ephem == 20170101
    #assert t2.t0_ephem == 235900.123456
    assert t2.num_ephem == 100
    for n in range(r):
        assert t.ephem_x[n] == n * n
        assert t.ephem_y[n] == n * n + offset1
        assert t.ephem_z[n] == n * n + offset2