from .nitf_tre import *
from .nitf_tre_rsmpca import *
from test_support import *
import io, six

def test_tre_rsmpca():
    t = TreRSMPCA()
    t.iid = 'abc'
    t.edition = 'abc'
    t.rsn = 1
    t.csn = 1
    t.rfep = 0.1234567890
    t.cfep = 0.1234567890
    t.rnrmo = 0.1234567890
    t.cnrmo = 0.1234567890
    t.xnrmo = 0.1234567890
    t.ynrmo = 0.1234567890
    t.znrmo = 0.1234567890
    t.rnpwrx = 1
    t.rnpwry = 1
    t.rnpwrz = 1
    t.rntrms = 5
    for a in range(t.rntrms):
        t.rnpcf[a] = 0.123+a
    t.rdpwrx = 1
    t.rdpwry = 1
    t.rdpwrz = 1
    t.rdtrms = 5
    for a in range(t.rdtrms):
        t.rdpcf[a] = 0.123 + a
    t.cnpwrx = 1
    t.cnpwry = 1
    t.cnpwrz = 1
    t.cntrms = 5
    for a in range(t.cntrms):
        t.cnpcf[a] = 0.123 + a
    t.cdpwrx = 1
    t.cdpwry = 1
    t.cdpwrz = 1
    t.cdtrms = 5
    for a in range(t.cdtrms):
        t.cdpcf[a] = 0.123 + a

    fh = six.BytesIO()
    t.write_to_file(fh)
    print(fh.getvalue())

    assert fh.getvalue() == b'RSMPCA00822abc                                                                             abc                                     001001+1.23456789000000E-01+1.23456789000000E-01+1.23456789000000E-01+1.23456789000000E-01+1.23456789000000E-01+1.23456789000000E-01+1.23456789000000E-01+0.00000000000000E+00+0.00000000000000E+00+0.00000000000000E+00+0.00000000000000E+00+0.00000000000000E+00111005+1.23000000000000E-01+1.12300000000000E+00+2.12300000000000E+00+3.12300000000000E+00+4.12300000000000E+00111005+1.23000000000000E-01+1.12300000000000E+00+2.12300000000000E+00+3.12300000000000E+00+4.12300000000000E+00111005+1.23000000000000E-01+1.12300000000000E+00+2.12300000000000E+00+3.12300000000000E+00+4.12300000000000E+00111005+1.23000000000000E-01+1.12300000000000E+00+2.12300000000000E+00+3.12300000000000E+00+4.12300000000000E+00'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreRSMPCA()
    t2.read_from_file(fh2)
    assert t.iid == 'abc'
    assert t.edition == 'abc'
    assert t.rsn == 1
    assert t.csn == 1
    assert t.rfep == 0.1234567890
    assert t.cfep == 0.1234567890
    assert t.rnrmo == 0.1234567890
    assert t.cnrmo == 0.1234567890
    assert t.xnrmo == 0.1234567890
    assert t.ynrmo == 0.1234567890
    assert t.znrmo == 0.1234567890
    assert t.rnpwrx == 1
    assert t.rnpwry == 1
    assert t.rnpwrz == 1
    assert t.rntrms == 5
    for a in range(t.rntrms):
        assert t.rnpcf[a] == 0.123 + a
    assert t.rdpwrx == 1
    assert t.rdpwry == 1
    assert t.rdpwrz == 1
    assert t.rdtrms == 5
    for a in range(t.rdtrms):
        assert t.rdpcf[a] == 0.123 + a
    assert t.cnpwrx == 1
    assert t.cnpwry == 1
    assert t.cnpwrz == 1
    assert t.cntrms == 5
    for a in range(t.cntrms):
        assert t.cnpcf[a] == 0.123 + a
    assert t.cdpwrx == 1
    assert t.cdpwry == 1
    assert t.cdpwrz == 1
    assert t.cdtrms == 5
    for a in range(t.cdtrms):
        assert t.cdpcf[a] == 0.123 + a

# Tests for other parts
