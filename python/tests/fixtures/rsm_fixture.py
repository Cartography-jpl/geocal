from geocal import (
    RsmRationalPolynomial,
    GeodeticRadianConverter,
    Rsm,
    LocalRcParameter,
    LocalRcConverter,
    RsmRpPlusGrid,
    RsmGrid,
    RsmMultiSection,
    serialize_write,
    serialize_read_generic,
)
import pytest


@pytest.fixture(scope="function")
def rsm_rational_polynomial(igc_rpc):
    """Create a RsmRationalPolynomial that matches our rpc test fixture"""
    r = RsmRationalPolynomial(3, 3, 3, 3, 3, 3, 3, 3)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(
        igc_rpc,
        GeodeticRadianConverter(),
        hmin,
        hmax,
        0,
        igc_rpc.number_line,
        0,
        igc_rpc.number_sample,
    )
    return r


@pytest.fixture(scope="function")
def rsm_rational_polynomial_lc(igc_rpc):
    """Create a RsmRationalPolynomial that matches our rpc test fixture"""
    r = RsmRationalPolynomial(3, 3, 3, 3, 3, 3, 3, 3)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(
        igc_rpc,
        LocalRcConverter(LocalRcParameter(igc_rpc)),
        hmin,
        hmax,
        0,
        igc_rpc.number_line,
        0,
        igc_rpc.number_sample,
    )
    return r


@pytest.fixture(scope="function")
def rsm_grid(igc_rpc):
    """Create a RsmGrid that matches our rpc test fixture"""
    r = RsmGrid(35, 35, 4)
    r.total_number_row_digit = 8
    r.total_number_col_digit = 8
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(
        igc_rpc,
        GeodeticRadianConverter(),
        hmin,
        hmax,
        0,
        igc_rpc.number_line,
        0,
        igc_rpc.number_sample,
    )
    return r


@pytest.fixture(scope="function")
def rsm_ms_polynomial(igc_rpc):
    rp = RsmRationalPolynomial(3, 3, 3, 3, 3, 3, 3, 3)
    res = RsmMultiSection(igc_rpc.number_line, igc_rpc.number_sample, 3, 2, rp)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    res.fit(
        igc_rpc,
        GeodeticRadianConverter(),
        hmin,
        hmax,
        0,
        igc_rpc.number_line,
        0,
        igc_rpc.number_sample,
    )
    return res


# Note the polynomial here tends to have a few poles, since there are zeros
# in the denominator. Our code works fine with this, but the MSP code fails.
# This is in some sense not a "real" failure, so we use a different polynomial
# for this. The down side is that this is slow (taking a minute or so),
# so we cache the results. If the read here fails, just regenerate the data.
@pytest.fixture(scope="function")
def rsm_ms_polynomial_np(igc_rpc, unit_test_data):
    fname = unit_test_data / "rsm_ms_polynomial_np.xml"
    if False:
        rp = RsmRationalPolynomial(5, 5, 3, 1, 1, 1)
        res = RsmMultiSection(igc_rpc.number_line, igc_rpc.number_sample, 3, 2, rp)
        hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale
        hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
        res.fit(
            igc_rpc,
            GeodeticRadianConverter(),
            hmin,
            hmax,
            0,
            igc_rpc.number_line,
            0,
            igc_rpc.number_sample,
        )
        serialize_write(str(fname), res)
    return serialize_read_generic(str(fname))


@pytest.fixture(scope="function")
def rsm_ms_grid(igc_rpc):
    rg = RsmGrid(10, 10, 2)
    rg.total_number_row_digit = 8
    rg.total_number_col_digit = 8
    res = RsmMultiSection(igc_rpc.number_line, igc_rpc.number_sample, 3, 2, rg)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    res.fit(
        igc_rpc,
        GeodeticRadianConverter(),
        hmin,
        hmax,
        0,
        igc_rpc.number_line,
        0,
        igc_rpc.number_sample,
    )
    return res


@pytest.fixture(scope="function")
def rsm_rp_cgrid(igc_rpc):
    """Create a RsmRationalPolynomial with a correction grid that
    matches our rpc test fixture"""
    rp = RsmRationalPolynomial(2, 2, 2, 1, 1, 1)
    g = RsmGrid(10, 10, 3)
    r = RsmRpPlusGrid(rp, g)
    hmin = igc_rpc.rpc.height_offset - igc_rpc.rpc.height_scale
    hmax = igc_rpc.rpc.height_offset + igc_rpc.rpc.height_scale
    r.fit(
        igc_rpc,
        GeodeticRadianConverter(),
        hmin,
        hmax,
        0,
        igc_rpc.number_line,
        0,
        igc_rpc.number_sample,
    )
    res = Rsm(r, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(igc_rpc, 500, 1500)
    return res


@pytest.fixture(scope="function")
def rsm(rsm_rational_polynomial, igc_rpc):
    res = Rsm(rsm_rational_polynomial, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(igc_rpc, 500, 1500)
    return res


@pytest.fixture(scope="function")
def rsm_lc(rsm_rational_polynomial_lc, igc_rpc):
    res = Rsm(rsm_rational_polynomial_lc, LocalRcConverter(LocalRcParameter(igc_rpc)))
    res.fill_in_ground_domain_vertex(igc_rpc, 500, 1500)
    return res


@pytest.fixture(scope="function")
def rsm_g(rsm_grid, igc_rpc):
    res = Rsm(rsm_grid, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(igc_rpc, 500, 1500)
    res.rsm_id.min_line = rsm_grid.min_line
    res.rsm_id.max_line = rsm_grid.max_line
    res.rsm_id.min_sample = rsm_grid.min_sample
    res.rsm_id.max_sample = rsm_grid.max_sample
    return res


@pytest.fixture(scope="function")
def rsm_ms_rp(rsm_ms_polynomial, igc_rpc):
    res = Rsm(rsm_ms_polynomial, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(igc_rpc, 500, 1500)
    return res


@pytest.fixture(scope="function")
def rsm_ms_rp_np(rsm_ms_polynomial_np, igc_rpc):
    res = Rsm(rsm_ms_polynomial_np, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(igc_rpc, 500, 1500)
    return res


@pytest.fixture(scope="function")
def rsm_ms_g(rsm_ms_grid, igc_rpc):
    res = Rsm(rsm_ms_grid, GeodeticRadianConverter())
    res.fill_in_ground_domain_vertex(igc_rpc, 500, 1500)
    return res
