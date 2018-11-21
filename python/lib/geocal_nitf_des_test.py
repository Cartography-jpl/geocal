try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (PosCsephb, OrbitDes, Time, KeplerOrbit)
from geocal.geocal_nitf_des import *
import numpy as np

@require_pynitf
def test_poscsephb(isolated_dir):
    '''Create a file, and write out a DesCSEPHB.'''
    f = pynitf.NitfFile()
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    p = PosCsephb(KeplerOrbit(t, t + 100.0), 1.0)
    des = DesCSEPHB_geocal()
    des.pos_csephb = p
    f.des_segment.append(pynitf.NitfDesSegment(des=des))
    f.write("nitf_des.ntf")
    # Use the raw field class here
    pynitf.unregister_des_class(DesCSEPHB_geocal)
    f2 = pynitf.NitfFile("nitf_des.ntf")
    print(f2)
    # Now put back
    pynitf.register_des_class(DesCSEPHB_geocal, priority_order=-1)
    f2 = pynitf.NitfFile("nitf_des.ntf")
    print(f2)
    p2 = f2.des_segment[0].des.pos_csephb
    assert p.is_cf == p2.is_cf
    assert str(p.min_time) == str(p2.min_time)
    assert str(p.max_time) == str(p2.max_time)
    assert_almost_equal(p.time_step, p2.time_step)
    assert p.interpolation_type == p2.interpolation_type
    assert p.ephemeris_data_quality == p2.ephemeris_data_quality
    assert p.ephemeris_source == p2.ephemeris_source
    assert p.lagrange_order == p2.lagrange_order
    orb1 = OrbitDes(p)
    orb2 = OrbitDes(p2)
    for t in np.linspace(0, 100.0, 200):
        assert distance(orb1.position_cf(orb1.min_time+t),
                        orb2.position_cf(orb2.min_time+t)) < 0.01

    
