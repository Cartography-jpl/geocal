try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (Time, KeplerOrbit, SimpleCamera,
                         FrameCoordinate, SimpleDem, Quaternion_double,
                         SensrbCamera)
from geocal.geocal_nitf_sensrb import *
import numpy as np

@require_pynitf
def test_sensrb_od(isolated_dir):
    '''Create a file, and write out a SENSRB, then make sure we can read it.'''
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(10, 10, np.uint8)
    for i in range(10):
        for j in range(10):
            img[0,i,j] = i + j

    iseg = pynitf.NitfImageSegment(img)
    f.image_segment.append(iseg)
    t = Time.parse_time("1998-06-30T10:51:28.32Z");
    korb = KeplerOrbit(t, t + 100.0);
    od = korb.orbit_data(t + 5)
    iseg.orbit_data_sensrb = od
    iseg.orbit_data_sensrb = od
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    od2 = f2.image_segment[0].orbit_data_sensrb
    print(od)
    print(od2)
    cam = SimpleCamera()
    fc = FrameCoordinate(0,752)
    dem = SimpleDem()
    assert(distance(od.surface_intersect(cam, fc, dem),
                    od2.surface_intersect(cam, fc, dem)) < 1.0)

@require_pynitf
def test_sensrb_cam(isolated_dir):
    '''Create a file, and write out a SENSRB, then make sure we can read it.'''
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(10, 10, np.uint8)
    for i in range(10):
        for j in range(10):
            img[0,i,j] = i + j

    iseg = pynitf.NitfImageSegment(img)
    f.image_segment.append(iseg)
    t = Time.parse_time("1998-06-30T10:51:28.32Z");
    korb = KeplerOrbit(t, t + 100.0);
    od = korb.orbit_data(t + 5)
    iseg.orbit_data_sensrb = od
    cam = SensrbCamera(Quaternion_double(1,0,0,0),
		       1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0,
		       2048, 1024, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(2048/2, 1024/2))
    iseg.camera_sensrb = cam
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    cam2 = f2.image_segment[0].camera_sensrb
    print(cam)
    print(cam2)
    
