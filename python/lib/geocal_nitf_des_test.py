try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (PosCsephb, AttCsattb, OrbitDes, Time, KeplerOrbit,
                         GlasGfmCamera)
from geocal.geocal_nitf_des import *
import numpy as np

pynitf.nitf_des.DEBUG = True

@require_pynitf
def test_poscsephb(isolated_dir):
    '''Create a file, and write out a DesCSEPHB.'''
    f = pynitf.NitfFile()
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    p = PosCsephb(KeplerOrbit(t, t + 100.0), 1.0)
    att = AttCsattb(KeplerOrbit(t, t + 100.0), 1.0)
    des = DesCSEPHB_geocal()
    des.pos_csephb = p
    f.des_segment.append(pynitf.NitfDesSegment(des=des))
    f.write("nitf_des.ntf")
    # Use the raw field class here
    pynitf.unregister_des_class(DesCSEPHB_geocal)
    f2 = pynitf.NitfFile("nitf_des.ntf")
    print(f2)
    # Now put back
    pynitf.register_des_class(DesCSEPHB_geocal, priority_order=1)
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
    orb1 = OrbitDes(p, att)
    orb2 = OrbitDes(p2, att)
    for t in np.linspace(0, 100.0, 200):
        assert distance(orb1.position_cf(orb1.min_time+t),
                        orb2.position_cf(orb2.min_time+t)) < 0.01

@require_pynitf
def test_attcsattb(isolated_dir):
    '''Create a file, and write out a DesCSATTB.'''
    f = pynitf.NitfFile()
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    att = AttCsattb(KeplerOrbit(t, t + 100.0), 1.0)
    des = DesCSATTB_geocal()
    des.att_csattb = att
    f.des_segment.append(pynitf.NitfDesSegment(des=des))
    f.write("nitf_des.ntf")
    # Use the raw field class here
    pynitf.unregister_des_class(DesCSATTB_geocal)
    f2 = pynitf.NitfFile("nitf_des.ntf")
    print(f2)
    # Now put back
    pynitf.register_des_class(DesCSATTB_geocal, priority_order=1)
    f2 = pynitf.NitfFile("nitf_des.ntf")
    print(f2)
    att2 = f2.des_segment[0].des.att_csattb
    assert att.is_cf == att2.is_cf
    assert str(att.min_time) == str(att2.min_time)
    assert str(att.max_time) == str(att2.max_time)
    assert_almost_equal(att.time_step, att2.time_step)
    assert att.interpolation_type == att2.interpolation_type
    assert att.attitude_data_quality == att2.attitude_data_quality
    assert att.attitude_source == att2.attitude_source
    assert att.lagrange_order == att2.lagrange_order
    #orb1 = OrbitDes(p)
    #orb2 = OrbitDes(p2)
    #for t in np.linspace(0, 100.0, 200):
    #    assert distance(orb1.position_cf(orb1.min_time+t),
    #                    orb2.position_cf(orb2.min_time+t)) < 0.01
        
    
@require_pynitf
def test_camera_glass_gfm_read(nitf_sample_rip):
    # Index for the image segment that has GLAS data.
    iseg_index = 1 
    f = pynitf.NitfFile(nitf_sample_rip)
    cam = f.image_segment[iseg_index].camera_glas_gfm
    print(cam)

@require_pynitf
def test_camera_gfm(isolated_dir):
    '''Test of reading a writing a frame camera'''
    tdata = Time.parse_time("2003-01-01T11:11:00Z")
    orb = KeplerOrbit()
    porb = PosCsephb(orb, tdata-10.0,tdata+10.0, 0.5, PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_7, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(orb, tdata-10.0,tdata+10.0, 0.5, AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb = OrbitDes(porb,aorb)
    cam = GlasGfmCamera(2048, 2048)
    cam.focal_length = 123.8e-3
    cam.focal_length_time = orb.min_time
    cam.first_line_block = [0]
    cam.first_sample_block = [0]
    cam.delta_line_block = [2048]
    cam.delta_sample_block = [2048]
    fa = np.empty((1,1,2,2,2))
    fa[0,0,:,0,0] = -1024 * 21e-6
    fa[0,0,:,1,0] = 1024 * 21e-6
    fa[0,0,0,:,1] = 1024 * 21e-6
    fa[0,0,1,:,1] = -1024 * 21e-6
    cam.field_alignment_block(0,fa)
    des = pynitf.DesCSSFAB()
    des.camera = cam
    f = NitfFile()
    f.des_segment.append(pynitf.NitfDesSegment(des=des))
    f.write("nitf_des.ntf")
    f2 = pynitf.NitfFile("nitf_des.ntf")
    print(f2)
    cam = f2.des_segment[0].des.camera
    
    
