from .nitf_tre import *
from .nitf_tre_rsmpia import *
from test_support import *
import io, six

def test_tre_rsmpia():
    t = TreRSMPIA()
    t.iid = 'abc'
    t.edition = 'abc'
    t.r0 = 0.1234567890
    t.rx = 0.1234567890
    t.ry = 0.1234567890
    t.rz = 0.1234567890
    t.rxx = 0.1234567890
    t.rxy = 0.1234567890
    t.rxz = 0.1234567890
    t.ryy = 0.1234567890
    t.rzz = 0.1234567890
    t.c0 = 0.1234567890
    t.cx = 0.1234567890
    t.cy = 0.1234567890
    t.cz = 0.1234567890
    t.cxx = 0.1234567890
    t.cxy = 0.1234567890
    t.cxz = 0.1234567890
    t.cyy = 0.1234567890
    t.cyz = 0.1234567890
    t.czz = 0.1234567890
    t.rnis = 4
    t.cnis = 5
    t.tnis = 6
    t.rssiz = 0.1234567890
    t.cssiz = 0.1234567890
    fh = six.BytesIO()
    t.write_to_file(fh)
    print(fh.getvalue())

    assert fh.getvalue() == b'RSMPIA00591abc                                                                             abc                                      1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 0.00000000000000E+00 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01004005006 1.23456789000000E-01 1.23456789000000E-01'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreRSMPIA()
    t2.read_from_file(fh2)
    assert t.iid == 'abc'
    assert t.edition == 'abc'
    assert t.r0 == 0.1234567890
    assert t.rx == 0.1234567890
    assert t.ry == 0.1234567890
    assert t.rz == 0.1234567890
    assert t.rxx == 0.1234567890
    assert t.rxy == 0.1234567890
    assert t.rxz == 0.1234567890
    assert t.ryy == 0.1234567890
    assert t.rzz == 0.1234567890
    assert t.c0 == 0.1234567890
    assert t.cx == 0.1234567890
    assert t.cy == 0.1234567890
    assert t.cz == 0.1234567890
    assert t.cxx == 0.1234567890
    assert t.cxy == 0.1234567890
    assert t.cxz == 0.1234567890
    assert t.cyy == 0.1234567890
    assert t.cyz == 0.1234567890
    assert t.czz == 0.1234567890
    assert t.rnis == 4
    assert t.cnis == 5
    assert t.tnis == 6
    assert t.rssiz == 0.1234567890
    assert t.cssiz == 0.1234567890

# Tests for other parts
