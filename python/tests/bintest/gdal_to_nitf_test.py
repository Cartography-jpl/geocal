import geocal
import subprocess
import numpy.testing as npt
from fixtures.require_check import require_pynitf, require_vicar_gdalplugin


@require_pynitf
@require_vicar_gdalplugin
def test_gdal_to_nitf(isolated_dir, stereo_unit_test_data):
    """Create a simple NITF from gdal_to_nitf, and check that everything is
    ok"""
    subprocess.check_call(
        [
            "gdal_to_nitf",
            "-q",
            str(stereo_unit_test_data / "10MAY21-1.img"),
            "gdal_to_nitf.ntf",
        ]
    )

    # Check that NITF was created correctly
    f1 = geocal.GdalRasterImage("gdal_to_nitf.ntf")
    f2 = geocal.GdalRasterImage(str(stereo_unit_test_data / "10MAY21-1.img"))
    rpc = f1.rpc
    rpc2 = f2.rpc
    npt.assert_almost_equal(rpc.error_bias, rpc2.error_bias, 5)
    npt.assert_almost_equal(rpc.error_random, rpc2.error_random, 5)
    npt.assert_almost_equal(rpc.height_offset, rpc2.height_offset, 3)
    npt.assert_almost_equal(rpc.height_scale, rpc2.height_scale, 3)
    npt.assert_almost_equal(rpc.latitude_offset, rpc2.latitude_offset, 3)
    npt.assert_almost_equal(rpc.latitude_scale, rpc2.latitude_scale, 3)
    npt.assert_almost_equal(rpc.longitude_offset, rpc2.longitude_offset, 3)
    npt.assert_almost_equal(rpc.longitude_scale, rpc2.longitude_scale, 3)
    npt.assert_almost_equal(rpc.line_offset, rpc2.line_offset, 3)
    npt.assert_almost_equal(rpc.line_scale, rpc2.line_scale, 3)
    npt.assert_almost_equal(rpc.sample_offset, rpc2.sample_offset, 3)
    npt.assert_almost_equal(rpc.sample_scale, rpc2.sample_scale, 3)
    for i in range(20):
        npt.assert_almost_equal(rpc.line_numerator[i], rpc2.line_numerator[i], 6)
        npt.assert_almost_equal(rpc.line_denominator[i], rpc2.line_denominator[i], 6)
        npt.assert_almost_equal(rpc.sample_numerator[i], rpc2.sample_numerator[i], 6)
        npt.assert_almost_equal(
            rpc.sample_denominator[i], rpc2.sample_denominator[i], 6
        )

    # Check that IGEOLO was created correctly. We manually inspected
    # the corners here with gdalinfo, but we don't have any easy
    # way to test this, so just make sure IGEOLO is the same
    assert (
        f1["NITF_IGEOLO"]
        == "364644N1160720W364645N1160642W364611N1160641W364609N1160720W"
    )


@require_pynitf
@require_vicar_gdalplugin
def test_gdal_to_nitf_a(isolated_dir, stereo_unit_test_data):
    """Create a simple NITF from gdal_to_nitf, and check that everything is
    ok"""
    # This is the same data as previous test, except we have changed the
    # RPC to a "A" type. Note that this isn't a real RPC, just some play
    # data we can use to check everything out.
    subprocess.check_call(
        [
            "gdal_to_nitf",
            "-q",
            str(stereo_unit_test_data / "10MAY21-1_A.img"),
            "gdal_to_nitf.ntf",
        ]
    )

    # Check that NITF was created correctly
    f1 = geocal.GdalRasterImage("gdal_to_nitf.ntf")
    f2 = geocal.GdalRasterImage(str(stereo_unit_test_data / "10MAY21-1_A.img"))
    # GDAL NITF always returns the RPC as type B. Go ahead and explicitly
    # convert both, so can directly compare
    rpc = f1.rpc.rpc_type_b()
    rpc2 = f2.rpc.rpc_type_b()
    print(rpc)
    print(rpc2)
    npt.assert_almost_equal(rpc.error_bias, rpc2.error_bias, 5)
    npt.assert_almost_equal(rpc.error_random, rpc2.error_random, 5)
    npt.assert_almost_equal(rpc.height_offset, rpc2.height_offset, 3)
    npt.assert_almost_equal(rpc.height_scale, rpc2.height_scale, 3)
    npt.assert_almost_equal(rpc.latitude_offset, rpc2.latitude_offset, 3)
    npt.assert_almost_equal(rpc.latitude_scale, rpc2.latitude_scale, 3)
    npt.assert_almost_equal(rpc.longitude_offset, rpc2.longitude_offset, 3)
    npt.assert_almost_equal(rpc.longitude_scale, rpc2.longitude_scale, 3)
    npt.assert_almost_equal(rpc.line_offset, rpc2.line_offset, 3)
    npt.assert_almost_equal(rpc.line_scale, rpc2.line_scale, 3)
    npt.assert_almost_equal(rpc.sample_offset, rpc2.sample_offset, 3)
    npt.assert_almost_equal(rpc.sample_scale, rpc2.sample_scale, 3)
    for i in range(20):
        npt.assert_almost_equal(rpc.line_numerator[i], rpc2.line_numerator[i], 6)
        npt.assert_almost_equal(rpc.line_denominator[i], rpc2.line_denominator[i], 6)
        npt.assert_almost_equal(rpc.sample_numerator[i], rpc2.sample_numerator[i], 6)
        npt.assert_almost_equal(
            rpc.sample_denominator[i], rpc2.sample_denominator[i], 6
        )

    # Check that IGEOLO was created correctly. We manually inspected
    # the corners here with gdalinfo, but we don't have any easy
    # way to test this, so just make sure IGEOLO is the same
    assert (
        f1["NITF_IGEOLO"]
        == "364644N1160720W364645N1160642W364611N1160641W364609N1160720W"
    )


@require_pynitf
@require_vicar_gdalplugin
def test_gda1_to_nitf_no_rpc(isolated_dir, unit_test_data):
    """Create a simple NITF from gda1_to_nitf, and check that everything is
    ok. This input data does not have an RPC, so we can check that is handled
    correct1y."""
    subprocess.check_call(
        [
            "gdal_to_nitf",
            "-q",
            str(unit_test_data / "test_pixel_is_point.img"),
            "gdal_to_nitf.ntf",
        ]
    )
