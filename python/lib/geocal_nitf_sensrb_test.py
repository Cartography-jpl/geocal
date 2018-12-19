try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (Time, KeplerOrbit)
from geocal.geocal_nitf_sensrb import *
import numpy as np

@require_pynitf
def test_sensrb(isolated_dir):
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
    iseg.orbit_data_sensorb = od
    iseg.orbit_data_sensorb = od
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    # od2 = f2.image_segment[0].orbit_data_sensrb
    print(od)
    #print(od2)
    
