from .nitf_des_csatta import *
from test_support import *
import io, six

def test_des_csatta_basic():

    d = DesCSATTA()

    d.dsclas = 'U'
    d.att_type = 'ORIGINAL'
    d.dt_att = '900.5000000000'
    d.date_att = 20170501
    d.t0_att = '235959.100001'
    d.num_att = 5
    for n in range(d.num_att):
        d.att_q1[n] = 10.1
        d.att_q2[n] = 10.1
        d.att_q3[n] = 10.1
        d.att_q4[n] = 10.1

    fh = six.BytesIO()
    d.write_to_file(fh)
    assert fh.getvalue() == b'                           00U                                                                                                                                                                      0000ORIGINAL    900.500000000020170501235959.1000010000510.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    10.1    '
    fh2 = six.BytesIO(fh.getvalue())
    d2 = DesCSATTA()
    d2.read_from_file(fh2)

    assert d2.dsclas  ==  'U'
    assert d2.att_type == 'ORIGINAL'
    assert d2.dt_att == '900.5000000000'
    assert d2.date_att == 20170501
    assert d2.t0_att == '235959.100001'
    assert d2.num_att == 5
    for n in range(d.num_att):
        assert d2.att_q1[n] == 10.1
        assert d2.att_q2[n] == 10.1
        assert d2.att_q3[n] == 10.1
        assert d2.att_q4[n] == 10.1

    print (d2.summary())