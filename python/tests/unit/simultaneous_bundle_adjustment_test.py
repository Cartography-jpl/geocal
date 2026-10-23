from geocal import (
    VicarLiteDem,
    VicarImageGroundConnection,
    VicarLiteRasterImage,
    IgcArray,
    TiePointCollect,
    SimultaneousBundleAdjustment,
    lm_optimize,
    distance,
    read_shelve,
    write_shelve,
)
from pathlib import Path
import logging
import numpy as np
import pytest
import scipy.optimize
import pickle

# This is really verbose, so we don't in general want to see this
log_python = logging.getLogger("geocal-python")
log_python.setLevel(logging.INFO)
log_optimize = logging.getLogger("geocal-python.lm_optimize")
log_optimize.setLevel(logging.WARNING)

# Module scope fixture can't use the function scope unit_test_data fixtures
_stereo = (
    Path(__file__).parent.parent.parent.parent.absolute() / "unit_test_data/Stereo"
)


@pytest.fixture(scope="module")
def sba_data():
    demin = VicarLiteDem(str(_stereo / "nevada_elv_aoi.img"), True)
    t = [False] * 20
    t[0] = True
    igcs = []
    for n in (1, 2, 3):
        igc = VicarImageGroundConnection(str(_stereo / f"10MAY21-{n}.img"), demin)
        igc.rpc.fit_line_numerator = t
        igc.rpc.fit_sample_numerator = t
        igcs.append(igc)
    gaoi = VicarLiteRasterImage(str(_stereo / "aoi.img")).map_info
    igc_coll = IgcArray(igcs)
    tp_collect = TiePointCollect(igc_coll)
    tpcol, _ = tp_collect.tie_point_grid(10, 10, aoi=gaoi, dem=demin)
    # Arbitrarily pick 2 points and pretend like they are GCPs
    tpcol[0].is_gcp = True
    tpcol[5].is_gcp = True
    sba = SimultaneousBundleAdjustment(igc_coll, tpcol, demin)
    sba_constant_gcp = SimultaneousBundleAdjustment(
        igc_coll, tpcol, demin, hold_gcp_fixed=True
    )
    return {
        "igcs": igcs,
        "sba": sba,
        "sba_constant_gcp": sba_constant_gcp,
        "parm0": sba.parameter,
        "parm0_constant_gcp": sba_constant_gcp.parameter,
    }


@pytest.fixture(scope="function")
def sba(sba_data):
    sba_data["sba"].parameter = sba_data["parm0"]
    sba_data["sba_constant_gcp"].parameter = sba_data["parm0_constant_gcp"]
    return sba_data["sba"]


@pytest.fixture(scope="function")
def sba_constant_gcp(sba, sba_data):
    return sba_data["sba_constant_gcp"]


@pytest.fixture(scope="function")
def igc3(sba, sba_data):
    return sba_data["igcs"][2]


def test_parameter(sba, igc3):
    p = sba.parameter
    assert len(p) == 291
    assert p[-1] == pytest.approx(0.0003422494, abs=1e-6)
    p[-1] = 5
    sba.parameter = p
    assert sba.parameter[-1] == pytest.approx(5)
    assert igc3.rpc.sample_numerator[0] == pytest.approx(5)


def test_parameter_constant_gcp(sba_constant_gcp):
    p = sba_constant_gcp.parameter
    assert len(p) == 291 - 3 * 2


def test_ground_location(sba):
    gp = sba.ground_location(10)
    p = sba.parameter
    p[10 * 3] += 20
    sba.parameter = p
    gp2 = sba.ground_location(10)
    assert distance(gp, gp2) == pytest.approx(20, abs=1e-6)


def test_ground_location_constant_gcp(sba_constant_gcp):
    gp = sba_constant_gcp.ground_location(10)
    p = sba_constant_gcp.parameter
    p[(10 - 2) * 3] += 20
    sba_constant_gcp.parameter = p
    gp2 = sba_constant_gcp.ground_location(10)
    assert distance(gp, gp2) == pytest.approx(20, abs=1e-6)


def test_sba_eq(sba, isolated_dir):
    assert len(sba.surface_constraint()) == 95
    assert len(sba.gcp_constraint()) == 6
    assert len(sba.collinearity_constraint()) == 548
    assert len(sba.parameter_constraint()) == 6
    assert len(sba.sba_eq(sba.parameter)) == 95 + 6 + 548 + 6
    # Can save results if we are diagnosing an issue
    if False:
        write_shelve(
            "simultaneous_bundle_adjustment_test.db:eq_expect",
            sba.sba_eq(sba.parameter),
        )
    # Can compare results if we are diagnosing issue
    if False:
        rexpect = read_shelve("simultaneous_bundle_adjustment_test.db:eq_expect")
        np.testing.assert_almost_equal(sba.sba_eq(sba.parameter), rexpect)


def test_sba_eq_constant_gcp(sba_constant_gcp):
    s = sba_constant_gcp
    assert len(s.surface_constraint()) == 95 - 2
    assert len(s.gcp_constraint()) == 0
    assert len(s.collinearity_constraint()) == 548
    assert len(s.parameter_constraint()) == 6
    assert len(s.sba_eq(s.parameter)) == 95 - 2 + 6 + 548 + 6 - 3 * 2


def test_sba_jacobian(sba):
    t = sba.sba_jacobian(sba.parameter)
    assert t.shape[0] == 95 + 6 + 548 + 6
    assert t.shape[1] == 291


def _solve_lm(s):
    # For this test, want to see lm_optimize diagnostic messages
    log_optimize = logging.getLogger("geocal-python.lm_optimize")
    log_optimize.setLevel(logging.INFO)
    try:
        v = s.sba_eq(s.parameter)
        chisq = np.inner(v, v) / (len(v) - len(s.parameter))
        assert chisq > 50
        print("Chisq", chisq)
        parm = lm_optimize(s.sba_eq, s.parameter, s.sba_jacobian)
        s.parameter = parm
        v = s.sba_eq(s.parameter)
        chisq = np.inner(v, v) / (len(v) - len(s.parameter))
        print("Chisq", chisq)
        assert chisq < 2
    finally:
        log_optimize.setLevel(logging.WARNING)


def test_solve_lm(sba):
    _solve_lm(sba)


def test_solve_lm_constant_gcp(sba_constant_gcp):
    _solve_lm(sba_constant_gcp)


# Don't normally run this. It is much slower then the lm_solve we
# have above
@pytest.mark.skip(
    reason="Much slower than lm_optimize used in other tests, and rewrites a data file. Run manually if needed"
)
def test_solve(sba, sba_data, isolated_dir):
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print("Chisq", chisq)
    assert chisq > 50
    parm, _ = scipy.optimize.leastsq(
        sba.sba_eq, sba.parameter, Dfun=lambda x: sba.sba_jacobian(x).todense()
    )
    sba.parameter = parm
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print("Chisq", chisq)
    assert chisq < 2
    with open("rpc_sba.pkl", "wb") as out:
        for igc in sba_data["igcs"]:
            pickle.dump(igc.rpc, out, pickle.HIGHEST_PROTOCOL)
