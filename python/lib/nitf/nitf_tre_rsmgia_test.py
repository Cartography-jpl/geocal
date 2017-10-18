from .nitf_tre import *
from .nitf_tre_rsmgia import *
from test_support import *
import io, six

def test_tre_rsmgia():
    t = TreRSMGIA()
    t.iid = 'abc'
    t.edition = 'abc'
    t.gr0 = 0.1234567890
    t.grx = 0.1234567890
    t.gry = 0.1234567890
    t.grz = 0.1234567890
    t.grxx = 0.1234567890
    t.grxy = 0.1234567890
    t.grxz = 0.1234567890
    t.gryy = 0.1234567890
    t.grzz = 0.1234567890
    t.gc0 = 0.1234567890
    t.gcx = 0.1234567890
    t.gcy = 0.1234567890
    t.gcz = 0.1234567890
    t.gcxx = 0.1234567890
    t.gcxy = 0.1234567890
    t.gcxz = 0.1234567890
    t.gcyy = 0.1234567890
    t.gcyz = 0.1234567890
    t.gczz = 0.1234567890
    t.grnis = 4
    t.gcnis = 5
    t.gtnis = 6
    t.grssiz = 0.1234567890
    t.gcssiz = 0.1234567890
    fh = six.BytesIO()
    t.write_to_file(fh)
    print(fh.getvalue())

    assert fh.getvalue() == b'RSMGIA00591abc                                                                             abc                                      1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 0.00000000000000E+00 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01 1.23456789000000E-01004005006 1.23456789000000E-01 1.23456789000000E-01'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreRSMGIA()
    t2.read_from_file(fh2)
    assert t.iid == 'abc'
    assert t.edition == 'abc'
    assert t.gr0 == 0.1234567890
    assert t.grx == 0.1234567890
    assert t.gry == 0.1234567890
    assert t.grz == 0.1234567890
    assert t.grxx == 0.1234567890
    assert t.grxy == 0.1234567890
    assert t.grxz == 0.1234567890
    assert t.gryy == 0.1234567890
    assert t.grzz == 0.1234567890
    assert t.gc0 == 0.1234567890
    assert t.gcx == 0.1234567890
    assert t.gcy == 0.1234567890
    assert t.gcz == 0.1234567890
    assert t.gcxx == 0.1234567890
    assert t.gcxy == 0.1234567890
    assert t.gcxz == 0.1234567890
    assert t.gcyy == 0.1234567890
    assert t.gcyz == 0.1234567890
    assert t.gczz == 0.1234567890
    assert t.grnis == 4
    assert t.gcnis == 5
    assert t.gtnis == 6
    assert t.grssiz == 0.1234567890
    assert t.gcssiz == 0.1234567890

# Tests for other parts
