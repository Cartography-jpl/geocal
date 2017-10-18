from .nitf_tre import *
from .nitf_tre_rsmgga import *
from test_support import *
import io, six

@pytest.mark.skip(reason="rcoord and ccoord fields in RSMGGA are not implemented yet")
def test_tre_rsmgga():
    t = TreRSMGGA()
    t.iid = 'abc'
    t.edition = 'abc'
    t.refrow = -99999999
    t.npln = 2
    t.tnumrd = 3
    t.tnumcd = 3
    t.fnumcd = 8
    for a in range(t.npln-1):
        t.ixo[a] = 4352
    for a in range(t.npln):
        t.nxpts[a] = 2
        t.nypts[a] = 2
        for b in range(t.nxpts[a] * t.nypts[a]):
            t.rcoord[a,b] = 432

    fh = six.BytesIO()
    t.write_to_file(fh)
    print(fh.getvalue())

    assert fh.getvalue() == b'RSMGIA00591abc                                                                             abc                                      1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 0.00000000000000E+00 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01004005006 1.23456789000000E-01 1.23456789000000E-01'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreRSMGIA()
    t2.read_from_file(fh2)
    assert t.iid == 'abc'