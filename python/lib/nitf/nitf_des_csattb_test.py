from .nitf_des_csattb import *
from test_support import *
import io, six

def test_des_csattb_basic():

    d = DesCSATTB()

    d.dsclas = 'U'
    d.id = '4385ab47-f3ba-40b7-9520-13d6b7a7f311'
    d.numais = '010'
    for i in range(int(d.numais)):
        d.aisdlvl[i] = 5 + i
    d.reservedsubh_len = 0
    d.qual_flag_att = 1
    d.interp_type_att = 1
    d.att_type = 1
    d.eci_ecf_att = 0
    d.dt_att = 900.5
    d.date_att = 20170501
    d.t0_att = 235959.1000010101
    d.num_att = 5
    for n in range(d.num_att):
        d.q1[n] = -0.11111
        d.q2[n] = -0.11111
        d.q3[n] = 0.11111
        d.q4[n] = 0.11111
    d.reserved_len = 0

    fh = six.BytesIO()
    d.write_to_file(fh)
    print(fh.getvalue())
    assert fh.getvalue() == b'                           00U                                                                                                                                                                      00004385ab47-f3ba-40b7-9520-13d6b7a7f31101000500600700800901001101201301400000001110900.50000000020170501235959.10000101000005-0.111110000000000-0.111110000000000+0.111110000000000+0.111110000000000-0.111110000000000-0.111110000000000+0.111110000000000+0.111110000000000-0.111110000000000-0.111110000000000+0.111110000000000+0.111110000000000-0.111110000000000-0.111110000000000+0.111110000000000+0.111110000000000-0.111110000000000-0.111110000000000+0.111110000000000+0.11111000000000000000'
    fh2 = six.BytesIO(fh.getvalue())
    d2 = DesCSATTB()
    d2.read_from_file(fh2)

    assert d2.dsclas  ==  'U'
    assert d2.id == '4385ab47-f3ba-40b7-9520-13d6b7a7f311'
    d2.numais == 10
    for i in range(int(d2.numais)):
        d2.aisdlvl[i] == 5 + i
    d2.reservedsubh_len == 0
    d2.qual_flag_att == 1
    d2.interp_type_att == 1
    d2.att_type == 1
    d2.eci_ecf_att == 0
    d2.dt_att == 900.5
    d2.date_att == 20170501
    d2.t0_att == 235959.1000010101
    d2.num_att == 5
    for n in range(d2.num_att):
        d2.q1[n] == -0.11111
        d2.q2[n] == -0.11111
        d2.q3[n] == 0.11111
        d2.q4[n] == 0.11111
    d2.reserved_len == 0

    print (d2.summary())