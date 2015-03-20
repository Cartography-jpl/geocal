from geocal_swig import *
from feature_detector_extension import *
from pprint import *
from multiprocessing import Pool

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"
img = VicarLiteRasterImage(test_data + "10MAY21-2.img")

def test_feature_detector():
    fd = ForstnerFeatureDetector()
    if(have_serialize_supported()):
        p = Pool()
    else:
        p = None
    r = fd.interest_point_grid(img, 10, 10, pool=p)
    assert len(r) == 100
    

