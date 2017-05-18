# We have a number of C++ routines that do similar things to Mike Burl's
# matlab code. This set of tests lets us compare between the 2 codes to
# make sure things are mostly consistent.

from test_support import *
import os
from geocal_swig import *
from geocal.burl_camera import *
import numpy.testing as npt

# Optional support for running matlab from python
try:
    from pymatbridge import Matlab
    have_matlab = True
except ImportError:
    have_matlab = False

class MatlabWrapper(object):
    '''Light wrapper on Matlab object that adds checks of status of calls.'''
    def __init__(self, diagnostic=False):
        self.mlab_v = None
        self.burl_path = "/data/smyth/BurlMatlab"
        self.diagnostic=diagnostic
    @property
    def mlab(self):
        if(not have_matlab):
            raise SkipTest
        if(not os.path.exists(self.burl_path)):
            raise SkipTest        
        if(self.mlab_v is None):
            self.mlab_v = Matlab()
            self.mlab_v.start()
            self.mlab_v.run_code("addpath('%s')" % self.burl_path)
        return self.mlab_v
    def run_code(self, v):
        if(self.diagnostic):
            print("Running matlab code \"%s\"" % v)
        res = self.mlab.run_code(v)
        if(not res['success']):
            raise RuntimeError("Matlab code failed: %s" % v)
        if(self.diagnostic):
            print(res['content']['stdout'])
        return res
    def get_variable(self, v):
        return self.mlab.get_variable(v)
    def set_variable(self, v, val):
        res = self.mlab.set_variable(v, val)
        if(not res['success']):
            raise RuntimeError("Matlab setting variable %s to %s failed" % (v,val))
        if(self.diagnostic):
            self.print_variable(v)
    def print_variable(self, v):
        res = self.mlab.run_code(v)
        print(res['content']['stdout'])
        

mlab = MatlabWrapper(diagnostic=False)

class BurlMatlabCamera(object):
    def __init__(self, nu, nv, xi, u0, v0, pld_q_c):
        mlab.set_variable('nu', nu)
        mlab.set_variable('nv', nv)
        mlab.set_variable('xi', xi)
        mlab.set_variable('u0', u0)
        mlab.set_variable('v0', v0)
        mlab.set_variable('pld_q_c', quaternion_to_array(pld_q_c))
        mlab.run_code('cam.nu = nu')
        mlab.run_code('cam.nv = nv')
        mlab.run_code('cam.fu = 1/xi')
        mlab.run_code('cam.fv = 1/xi')
        mlab.run_code('cam.q = 0')
        mlab.run_code('cam.u0 = u0')
        mlab.run_code('cam.v0 = v0')
        mlab.run_code('cam.K = defK(cam.fu, cam.fv, cam.q, cam.u0, cam.v0)')
        mlab.run_code('cam.Kinv = inv(cam.K)')
        # Mike's code combines both the camera and the orbit data conversion
        # to ECI. We set this second change to identity, so we get just
        # the ScLookVector transformation.
        mlab.run_code('cam.ECEF_p_PLD = [0; 0; 0]')
        mlab.run_code('cam.ECEF_q_PLD = [1;0;0;0]')
        mlab.run_code("cam.PLD_q_C = pld_q_c'")
        mlab.run_code("cam.Kappa = eye(3,3)")
        mlab.run_code("cam.KappaInv = eye(3,3)")
        

def ecr_to_tod(gp, t, delta_ut1=0):
    mlab.set_variable('delta_ut1', delta_ut1)
    mlab.run_code("[tt, ut1] = utc_iso_time_string_to_tt_and_ut1('%s',delta_ut1)" % str(t).replace('Z',''))
    mlab.run_code("[j2000_r_tod, ecef_r_tod] = eos_coords(tt, ut1)")
    e = Ecr(gp)
    mlab.set_variable('ecef', [e.position[0],e.position[1],e.position[2]])
    mlab.run_code("tod=(inv(ecef_r_tod)*ecef')'")
    tod = mlab.get_variable('tod')
    return EciTod(tod[0,0], tod[0,1], tod[0, 2])

@matlab
def test_utc_iso_time():
    '''Compare utc_iso_time_string_to_tt_and_ut1 to SPICE calculations'''
    t = Time.parse_time('2016-10-08T11:48:12.3457Z')
    # The ISO time this takes is basically CCSDS without the trailing Z
    mlab.run_code("[tt, ut1] = utc_iso_time_string_to_tt_and_ut1('%s',0)" % str(t).replace('Z',''))
    tt = mlab.get_variable('tt')
    ut1 = mlab.get_variable('ut1')
    # We use a slightly more accurate conversion factor going from TAI to
    # j2000, but we should be pretty close
    assert abs(tt - t.j2000) < 1e-4
    # We don't directly calculate ut1 in geocal, but we'll indirectly test this
    # when we look at eos_coords

@matlab
def test_convert_tod():
    '''Test conversion to EciTod.'''
    # Note, if we load the prediction, do this *before* the high resolution.
    # This way the high resolution is used where available, and falls back
    # to prediction when needed
    bdir = os.environ["SPICEDATA"] + "/pck/"
    SpiceHelper.add_kernel(bdir + "earth_070425_370426_predict.bpc")
    SpiceHelper.add_kernel(bdir + "earth_latest_high_prec.bpc")
    # This future date (at least when we initially did this test) is ~2km off.
    # This is because prediction is going forward from 2007 and just isn't very
    # accurate
    #t = Time.parse_time('2018-10-04T00:00:00Z')
    # Past time is better
    t = Time.parse_time('2016-10-08T11:48:12.3457Z')
    delta_ut1 = -0.2790727222
    gp = Geodetic(10,15)
    # Note rather large distance, due to not including delta_ut1
    assert distance(gp, ecr_to_tod(gp, t).convert_to_cf(t)) < 125
    # Improved with delta_ut1
    assert distance(gp, ecr_to_tod(gp, t, delta_ut1).convert_to_cf(t)) < 7
    
@matlab
def test_camera():
    mcam = BurlMatlabCamera(1024,1024,2e-06,(1024-1)/2.0,(1024-1)/2.0,
                            Quaternion_double(1,0,0,0))
    cam = burl_camera(1024,1024,2e-06,(1024-1)/2.0,(1024-1)/2.0,
                      Quaternion_double(1,0,0,0))
    mlab.print_variable('cam')
    
    


    
