from geocal import find_isis_kernel_file, read_kernel_from_isis, pds_to_isis
from fixtures.require_check import require_isis
import os
import pytest
from pathlib import Path

@require_isis
def test_find_isis_kernel(isolated_dir):
    original_cache = os.environ.get("SPICECACHE")
    try:
        os.environ["SPICECACHE"] = str(Path("./spice_cache").absolute())
        f = find_isis_kernel_file("$mro/kernels/spk/mro_psp21_ssd_mro110c.bsp")
        assert f is not None
        f = find_isis_kernel_file("$lro/kernels/spk/fdf29r_2012275_2012306_v01.bsp")
        assert f is not None
        f = find_isis_kernel_file("$lro/kernels/spk/fdf29r_2012275_2012306_v01.bsp")
        assert f is not None
        # Newer kernel only available with S3 buckets, so we can test using
        # rclone instead of rsycnc
        f = find_isis_kernel_file("$lro/kernels/sclk/lro_clkcor_2022263_v00.tsc")
        assert f is not None
    finally:
        if original_cache is None:
            try:
                del os.environ["SPICECACHE"]
            except KeyError:
                pass
        else:
            os.environ["SPICECACHE"] = original_cache


@pytest.mark.long_test
@require_isis
def test_import_ctx(mars_test_data, isolated_dir):
    pds_to_isis(mars_test_data / "P16_007388_2049_XI_24N020W.IMG", "ctx.cub")
    # Test handling the kernels and downloading from the web.
    klist = read_kernel_from_isis("ctx.cub")
    klist.load_kernel()
    # Test passing a file that is already isis file
    pds_to_isis("ctx.cub", "ctx2.cub")
    # Test passing the same file twice
    pds_to_isis("ctx2.cub", "ctx2.cub")


@pytest.mark.long_test
@require_isis
def test_import_hirise(mars_test_data, isolated_dir):
    f1 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_0.img"
    f2 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_1.img"
    pds_to_isis(f1, "hirise.cub", pds_fname2=f2)
    # Test handling the kernels and downloading from the web.
    klist = read_kernel_from_isis("hirise.cub")
    print(klist)
    klist.load_kernel()


@pytest.mark.long_test
@require_isis
def test_import_hrsc(mars_test_data, isolated_dir):
    print(
        "This test currently fails with the spiceinit. Not clear if this is a problem with our code, with ISIS, or just with this particular dataset. We can come back this this"
    )
    raise pytest.SkipTest
    pds_to_isis(mars_test_data / "h1326_0000_nd2.img", "hrsc.cub")
    # Test handling the kernels and downloading from the web.
    klist = read_kernel_from_isis("hrsc.cub")
    klist.load_kernel()


@pytest.mark.long_test
@require_isis
def test_import_lro_wac(isolated_dir):
    f = "/raid28/tllogan/Moon_Luna_Data/WAC/mixed_WAC_NAC_edr_cdr/M1124549036CC.IMG"
    _ = pds_to_isis(f, "wac.cub")
    # Test handling the kernels and downloading from the web.
    klist = read_kernel_from_isis("wac.cub")
    klist.load_kernel()


@pytest.mark.long_test
@require_isis
def test_import_lro_nac_edr(isolated_dir):
    f = "/raid28/tllogan/Moon_Luna_Data/WAC/mixed_WAC_NAC_edr_cdr/M1124549139LE.IMG"
    pds_to_isis(f, "nac.cub")
    # Test handling the kernels and downloading from the web.
    klist = read_kernel_from_isis("nac.cub")
    klist.load_kernel()


# Doesn't currently work. I'm not sure if ISIS actually support this or
# not.
@pytest.mark.skip
@pytest.mark.long_test
@require_isis
def test_import_lro_nac_cdr(isolated_dir):
    f = "/raid28/tllogan/Moon_Luna_Data/WAC/mixed_WAC_NAC_edr_cdr/M1124549139LC.IMG"
    pds_to_isis(f, "nac.cub")
    # Test handling the kernels and downloading from the web.
    klist = read_kernel_from_isis("nac.cub")
    klist.load_kernel()
