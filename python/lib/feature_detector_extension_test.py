from geocal import *
from feature_detector_extension import *
from pprint import *
from multiprocessing import Pool

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"
img = VicarLiteRasterImage(test_data + "10MAY21-2.img")

def test_feature_detector():
    fd = ForstnerFeatureDetector()
    p = Pool()
    r = fd.interest_point_grid(img, 10, 10, pool=p)
    assert len(r) == 100
    

