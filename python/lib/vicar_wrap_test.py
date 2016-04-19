from __future__ import division
from past.utils import old_div
from geocal_swig import *
from geocal.vicar_wrap import *
from nose.tools import *
import math
from nose.plugins.skip import Skip, SkipTest
import os

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"
try:
    # Depending on the options used when building, this class might not
    # be available. If not, just skip the tests
    EciTodBurl
    have_carto = True
except NameError:
    have_carto = False

if(have_carto):
    EciTodBurl.set_delta_ut1(0.1128609)

def test_scinterp():
    '''This was pulled from tstscinterp.pdf. This isn't actually a very
    good test, since we don't actually interpolate anything. But we want
    to include this test since it is what the underlying pdf file uses.'''
    if(not have_carto or not VicarFile.vicar_available()):
        raise SkipTest
    pos = EciTodBurl(3722048.0,4126137.9,3955545.4)
    tpos = Time.time_acs(31556737.5)
    att = Quaternion_double(0.06938316945407,0.20331591888905,
                            0.82545247058087, -0.52199313380038)
    tatt = tpos
    tinterp = tpos
    res = scinterp(tinterp, pos, tpos, att, tatt, pos, tpos, att, tatt)
    assert abs(res.time - tinterp) < 1e-6
    assert distance(res.position_cf, pos.convert_to_cf(tinterp)) < 0.01
    assert_almost_equal(res.sc_to_ci.R_component_1, att.R_component_1, 
                        6)
    assert_almost_equal(res.sc_to_ci.R_component_2, att.R_component_2, 
                        6)
    assert_almost_equal(res.sc_to_ci.R_component_3, att.R_component_3, 
                        6)
    assert_almost_equal(res.sc_to_ci.R_component_4, att.R_component_4, 
                        6)

def test_sc2rpc():
    '''Test data from devsc2rpc'''
    if(not have_carto or not VicarFile.vicar_available()):
        raise SkipTest
    od = QuaternionOrbitData(Time.time_acs(215077459.471879),
                             EciTodBurl(3435100.496, 945571.538, -6053387.573),
                             [0,0,0],
                             Quaternion_double(0.946366, 0.0, -0.323096813, 
                                               0.0))
    leapsecond_file = test_data + "leapsecond.dat"
    res = sc2rpc(od, EciTodBurl.get_delta_ut1(), leapsecond_file, 
                 ImageCoordinate(1,1),
                 0.0, Quaternion_double(1,0,0,0), 2500000, 2500000)
    expected = Geodetic(-60.3268510235, 47.235039309, 0)
    assert distance(res, expected) < 0.1

def test_compare_sc2rpc():
    '''Calculate using my code, and see how close we are to sc2rpc'''
    if(not have_carto or not VicarFile.vicar_available()):
        raise SkipTest
    od = QuaternionOrbitData(Time.time_acs(215077459.471879),
                             EciTodBurl(3435100.496, 945571.538, -6053387.573),
                             [0,0,0],
                             Quaternion_double(0.946366, 0.0, -0.323096813, 
                                               0.0))
    leapsecond_file = test_data + "leapsecond.dat"
    sc_to_cam = Quaternion_double(0.98,0,0.2,0)
    sc2calc = lambda ic : sc2rpc(od, EciTodBurl.get_delta_ut1(), 
                                 leapsecond_file, ic,
                                 0.0, sc_to_cam, 2500000, 2500000)
    cam = QuaternionCamera(sc_to_cam, 3376, 3649, old_div(1.0, 2500000), 
                           old_div(1.0, 2500000), 1.0, FrameCoordinate(1688.0,1824.5),
                           QuaternionCamera.LINE_IS_Y)
    ic = ImageCoordinate(1, 1)
    gc_sc2rpc = sc2calc(ic)
    gc_cam = od.surface_intersect(cam, FrameCoordinate(ic.line, ic.sample),
                          SimpleDem(gc_sc2rpc.height_reference_surface))
    assert distance(gc_sc2rpc, gc_cam) < 1.0


