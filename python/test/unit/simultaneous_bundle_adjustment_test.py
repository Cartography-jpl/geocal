from builtins import object
from .tie_point_collect import *
from .image_ground_connection import *
from .simultaneous_bundle_adjustment import *
from .lm_optimize import *
from .sqlite_shelf import *
from geocal_swig import VicarLiteRasterImage
import scipy.optimize
import pickle
from test_support import *
import logging
import sys
from numpy.testing import assert_almost_equal

console = logging.StreamHandler(stream=sys.stdout)
console.setLevel(logging.WARNING)
logging.getLogger("geocal-python").addHandler(console)

demin = VicarLiteDem(stereo_unit_test_data + "nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img", demin)
t = [False] * 20
t[0] = True
igc1.rpc.fit_line_numerator = t
igc1.rpc.fit_sample_numerator = t
igc2 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-2.img", demin)
igc2.rpc.fit_line_numerator = t
t2 = [False] * 20
t2[0] = True
igc2.rpc.fit_sample_numerator = t2
igc3 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-3.img", demin)
igc3.rpc.fit_line_numerator = t
t2 = [False] * 20
t2[0] = True
igc3.rpc.fit_sample_numerator = t2
gaoi = VicarLiteRasterImage(stereo_unit_test_data + "aoi.img").map_info
igc_coll = IgcArray([igc1, igc2, igc3])
tp_collect = TiePointCollect(igc_coll)
tpcol, diag = tp_collect.tie_point_grid(10, 10, aoi = gaoi, dem = demin)
# Arbitrarily pick 2 points and pretend like they are GCPs
tpcol[0].is_gcp = True
tpcol[5].is_gcp = True
sba = SimultaneousBundleAdjustment(igc_coll, tpcol, demin)
sba_constant_gcp = SimultaneousBundleAdjustment(igc_coll, tpcol, demin,
                                                hold_gcp_fixed = True)
parm0 = sba.parameter
parm0_constant_gcp = sba_constant_gcp.parameter

# This is really verbose, so we don't in general want to see this
log_python = logging.getLogger("geocal-python")
log_python.setLevel(logging.INFO)
log_optimize = logging.getLogger("geocal-python.lm_optimize")
log_optimize.setLevel(logging.WARNING)

class TestClass(object):
    def setUp(self):
        sba.parameter = parm0
        sba_constant_gcp.parameter = parm0_constant_gcp

    def test_parameter(self):
        p = sba.parameter
        assert len(p) == 291
        assert_almost_equal(p[-1], 0.0003422494)
        p[-1] = 5
        sba.parameter = p
        assert_almost_equal(sba.parameter[-1], 5)
        assert_almost_equal(igc3.rpc.sample_numerator[0], 5)

    def test_parameter_constant_gcp(self):
        p = sba_constant_gcp.parameter
        assert len(p) == 291 - 3 * 2

    def test_ground_location(self):
        gp = sba.ground_location(10)
        p = sba.parameter
        p[10 * 3] += 20
        sba.parameter = p
        gp2 = sba.ground_location(10)
        assert_almost_equal(distance(gp, gp2), 20)

    def test_ground_location_constant_gcp(self):
        gp = sba_constant_gcp.ground_location(10)
        p = sba_constant_gcp.parameter
        p[(10 - 2) * 3] += 20
        sba_constant_gcp.parameter = p
        gp2 = sba_constant_gcp.ground_location(10)
        assert_almost_equal(distance(gp, gp2), 20)

    def test_sba_eq(self):
        assert len(sba.surface_constraint()) == 95
        assert len(sba.gcp_constraint()) == 6
        assert len(sba.collinearity_constraint()) == 548
        assert len(sba.parameter_constraint()) == 6
        assert len(sba.sba_eq(sba.parameter)) == 95 + 6 + 548 + 6
        # Can save results if we are diagnosing an issue
        if False:
            write_shelve("simultaneous_bundle_adjustment_test.db:eq_expect", 
                         sba.sba_eq(sba.parameter))
        # Can compare results if we are diagnosing issue
        if False:
            rexpect = read_shelve("simultaneous_bundle_adjustment_test.db:eq_expect")
            assert_almost_equal(sba.sba_eq(sba.parameter), rexpect)

    def test_sba_eq_constant_gcp(self):
        assert len(sba_constant_gcp.surface_constraint()) == 95 - 2
        assert len(sba_constant_gcp.gcp_constraint()) == 0
        assert len(sba_constant_gcp.collinearity_constraint()) == 548
        assert len(sba_constant_gcp.parameter_constraint()) == 6
        assert len(sba_constant_gcp.sba_eq(sba_constant_gcp.parameter)) == 95 - 2 + 6 + 548 + 6 - 3 * 2

    def test_sba_jacobian(self):
        t = sba.sba_jacobian(sba.parameter)
        assert t.shape[0] == 95 + 6 + 548 + 6
        assert t.shape[1] == 291
        # Can save results if we are diagnosing an issue
        if False:
            write_shelve("simultaneous_bundle_adjustment_test.db:jac_expect", 
                         t.todense())
        # Can compare results if we are diagnosing issue
        if False:
            rexpect = read_shelve("simultaneous_bundle_adjustment_test.db:jac_expect")
            assert_almost_equal(t.todense(), rexpect)

    def test_solve_lm(self):
        # For this test, want to see lm_optimize diagnostic messages
        log_optimize = logging.getLogger("geocal-python.lm_optimize")
        log_optimize.setLevel(logging.INFO)
        try:
            v = sba.sba_eq(sba.parameter)
            chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
            assert chisq > 50
            print("Chisq", chisq)
            parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
            sba.parameter = parm
            v = sba.sba_eq(sba.parameter)
            chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
            print("Chisq", chisq)
            assert chisq < 2
        finally:
            log_optimize = logging.getLogger("geocal-python.lm_optimize")
            log_optimize.setLevel(logging.WARNING)

    def test_solve_lm_constant_gcp(self):
        # For this test, want to see lm_optimize diagnostic messages
        log_optimize = logging.getLogger("geocal-python.lm_optimize")
        log_optimize.setLevel(logging.INFO)
        try:
            v = sba_constant_gcp.sba_eq(sba_constant_gcp.parameter)
            chisq = np.inner(v, v) / (len(v) - len(sba_constant_gcp.parameter))
            assert chisq > 50
            print("Chisq", chisq)
            parm = lm_optimize(sba_constant_gcp.sba_eq, 
                               sba_constant_gcp.parameter, 
                               sba_constant_gcp.sba_jacobian)
            sba_constant_gcp.parameter = parm
            v = sba_constant_gcp.sba_eq(sba_constant_gcp.parameter)
            chisq = np.inner(v, v) / (len(v) - len(sba_constant_gcp.parameter))
            print("Chisq", chisq)
            assert chisq < 2
        finally:
            log_optimize = logging.getLogger("geocal-python.lm_optimize")
            log_optimize.setLevel(logging.WARNING)

    # Don't normally run this. It is much slower then the lm_solve we
    # have above
    @skip
    def test_solve(self):
        v = sba.sba_eq(sba.parameter)
        chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
        print("Chisq", chisq)
        assert chisq > 50
        parm, res = scipy.optimize.leastsq(sba.sba_eq, sba.parameter,
                                           Dfun = lambda x: sba.sba_jacobian(x).todense())
        sba.parameter = parm
        v = sba.sba_eq(sba.parameter)
        chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
        print("Chisq", chisq)
        assert chisq < 2
        out = open(stereo_unit_test_data + "rpc_sba.pkl", "wb")
        pickle.dump(igc1.rpc, out, pickle.HIGHEST_PROTOCOL)
        pickle.dump(igc2.rpc, out, pickle.HIGHEST_PROTOCOL)
        pickle.dump(igc3.rpc, out, pickle.HIGHEST_PROTOCOL)

        


    



