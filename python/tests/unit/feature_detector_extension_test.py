from multiprocessing import Pool

from geocal import (
    ForstnerFeatureDetector,
    VicarLiteRasterImage,
    have_serialize_supported,
)


def test_feature_detector(stereo_unit_test_data):
    img = VicarLiteRasterImage(str(stereo_unit_test_data / "10MAY21-2.img"))
    fd = ForstnerFeatureDetector()
    if have_serialize_supported():
        p = Pool()
    else:
        p = None
    r = fd.interest_point_grid(img, 10, 10, pool=p)
    assert len(r) == 100
