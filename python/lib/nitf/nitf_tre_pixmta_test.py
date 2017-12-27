from .nitf_tre import *
from .nitf_tre_pixmta import *
from test_support import *
import io, six

def test_tre_pixmta():
    '''Basic test pf pixmta'''
    t = TrePIXMTA()
    t.numais = 10
    for i in range(int(t.numais)):
        t.aisdlvl[i] = 5+i
    t.origin_x = 0.001234
    t.origin_y = -6.001234
    t.scale_x = 1
    t.scale_y = 1
    t.sample_mode = 'F'
    t.nummetrics = 3
    t.perband = 'A'
    t.description[0] = 'NESR'
    t.description[1] = 'CENTER WAVELENGTH'
    t.description[2] = 'CENTER WAVENUMBER'
    t.unit[0] = 'W/(m m um sr)'
    t.unit[1] = 'um'
    t.unit[2] = '1/cm'
    t.fittype[0] = 'D'
    t.fittype[1] = 'D'
    t.fittype[2] = 'D'
    t.reserved_len = 0
    fh = six.BytesIO()
    t.write_to_file(fh)
    print(fh.getvalue())
    assert fh.getvalue() == b'PIXMTA0034410 005006007008009010011012013014+1.2340000E-03-6.0012340E+00+1.0000000E+00+1.0000000E+00F00003ANESR                                    W/(m m um sr)                           DCENTER WAVELENGTH                       um                                      DCENTER WAVENUMBER                       1/cm                                    D00000'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TrePIXMTA()
    t2.read_from_file(fh2)
    assert t2.numais == '10'
    assert list(t2.aisdlvl) == [5,6,7,8,9,10,11,12,13,14]
    assert t2.origin_x == 0.001234
    assert t2.origin_y == -6.001234
    assert t2.scale_x == 1
    assert t2.scale_y == 1
    assert t2.sample_mode == 'F'
    assert t2.nummetrics == 3
    assert t2.perband == 'A'
    assert t2.description[0] == 'NESR'
    assert t2.description[1] == 'CENTER WAVELENGTH'
    assert t2.description[2] == 'CENTER WAVENUMBER'
    assert t2.unit[0] == 'W/(m m um sr)'
    assert t2.unit[1] == 'um'
    assert t2.unit[2] == '1/cm'
    assert t2.fittype[0] == 'D'
    assert t2.fittype[1] == 'D'
    assert t2.fittype[2] == 'D'
    assert t2.reserved_len == 0

    print(t2.summary())