from nose.tools import *
from geocal_swig import *
from tie_point_collect import *
from image_ground_connection import *
from simultaneous_bundle_adjustment import *
from lm_optimize import *
import scipy.optimize
import cPickle
import numpy as np
from nose.plugins.skip import Skip, SkipTest
import logging

logging.basicConfig(level = logging.INFO)
test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
t = [False] * 20
t[0] = True
igc1.rpc.fit_line_numerator = t
igc1.rpc.fit_sample_numerator = t
igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
igc2.rpc.fit_line_numerator = t
t2 = [False] * 20
t2[0] = True
t2[3] = True
igc2.rpc.fit_sample_numerator = t2
igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
igc3.rpc.fit_line_numerator = t
t2 = [False] * 20
t2[0] = True
t2[1] = True
igc3.rpc.fit_sample_numerator = t2
gaoi = VicarLiteRasterImage(test_data + "aoi.img").map_info
igc_coll = IgcArray([igc1, igc2, igc3])
igc_coll.parameter_subset_mask = [True, True, True, True, False, True, True, 
                                  False]
tp_collect = TiePointCollect(igc_coll)
tpcol = tp_collect.tie_point_grid(10, 10, aoi = gaoi, dem = demin)
# Arbitrarily pick 2 points and pretend like they are GCPs
tpcol[0].is_gcp = True
tpcol[5].is_gcp = True
sba = SimultaneousBundleAdjustment(igc_coll, tpcol, demin)
parm0 = sba.parameter
print igc_coll.parameter_subset_mask
print igc_coll.parameter_subset
print igc_coll.parameter_subset_name

class TestClass:
    def setUp(self):
        sba.parameter = parm0

    def test_parameter(self):
        p = sba.parameter
        assert len(p) == 291
        assert_almost_equal(p[-1], 0.0003422494)
        p[-1] = 5
        sba.parameter = p
        assert_almost_equal(sba.parameter[-1], 5)
        assert_almost_equal(igc3.rpc.sample_numerator[0], 5)

    def test_ground_location(self):
        gp = sba.ground_location(10)
        p = sba.parameter
        p[10 * 3] += 20
        sba.parameter = p
        gp2 = sba.ground_location(10)
        assert_almost_equal(distance(gp, gp2), 20)

    def test_sba_eq(self):
        assert len(sba.surface_constraint()) == 95
        assert len(sba.gcp_constraint()) == 6
        assert len(sba.collinearity_constraint()) == 548
        assert len(sba.parameter_constraint()) == 291
        assert len(sba.sba_eq(sba.parameter)) == 95 + 6 + 548 + 291

    def test_sba_jacobian(self):
        t = sba.sba_jacobian(sba.parameter)
        assert t.shape[0] == 95 + 6 + 548 + 291
        assert t.shape[1] == 291

    def test_solve_lm(self):
        v = sba.sba_eq(sba.parameter)
        chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
        assert chisq > 50
        print "Chisq", chisq
        parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
        sba.parameter = parm
        v = sba.sba_eq(sba.parameter)
        chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
        print "Chisq", chisq
        assert chisq < 2

    def test_solve(self):
        raise SkipTest
        # Don't normally run this. It is much slower then the lm_solve we
        # have above
        v = sba.sba_eq(sba.parameter)
        chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
        print "Chisq", chisq
        assert chisq > 50
        parm, res = scipy.optimize.leastsq(sba.sba_eq, sba.parameter,
                                           Dfun = lambda x: sba.sba_jacobian(x).todense())
        sba.parameter = parm
        v = sba.sba_eq(sba.parameter)
        chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
        print "Chisq", chisq
        assert chisq < 2
        out = open(test_data + "rpc_sba.pkl", "wb")
        cPickle.dump(igc1.rpc, out, cPickle.HIGHEST_PROTOCOL)
        cPickle.dump(igc2.rpc, out, cPickle.HIGHEST_PROTOCOL)
        cPickle.dump(igc3.rpc, out, cPickle.HIGHEST_PROTOCOL)

        


    



