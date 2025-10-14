try:
    import pynitf
except ImportError:
    pass
from fixtures.require_check import require_pynitf


# This requires hard coded data, so don't normally run.
@require_pynitf
def test_nitf_gdal_image(nitf_sample_rip):
    """Try reading SNIP data using GDAL"""
    f = pynitf.NitfFile(nitf_sample_rip)
    print(nitf_sample_rip)
    print(f.image_segment[1].data[:, :, :])
    print(f.image_segment[1].data[:, :, :].max())
    print(f.image_segment[0].raster_image_multi_band)
    print(f.image_segment[1].raster_image)
