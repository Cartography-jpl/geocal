try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (IgcMsp, SimpleDem, ImageCoordinate, distance)
import os

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.image_subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0,i,j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

@require_msp    
@require_pynitf
def test_snip_example(isolated_dir):
    fname = "/home/smyth/Local/SNIP NITF Example/07APR2005_Hyperion_331405N0442002E_SWIR172_001_L1R.ntf"
    # Ok if data isn't available. We can only run test if we have the
    # test data.
    if(not os.path.exists(fname)):
        raise SkipTest
    igc1 = IgcMsp(fname, SimpleDem(), 1, "GLAS", "GLAS")
    igc2 = IgcMsp(fname, SimpleDem(), 1, "RSM", "RSM")
    # Not sure why, but default IgcMsp is not what we would expect. Neither
    # GLAS nor RSM is selected
    igc3 = IgcMsp(fname, SimpleDem(), 1)
    print(igc1)
    print(igc2)
    print(igc3)
    pt1 = igc1.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt2 = igc2.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt3 = igc3.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    print(distance(pt1, pt2))
    print(distance(pt1, pt3))
    
@require_msp    
@require_pynitf
def test_snip_create(isolated_dir):
    '''Basic creation test. Right now, we just copy things'''
    pynitf.register_des_class(pynitf.NitfDesCopy, priority_order=999)
    fname = "/home/smyth/Local/SNIP NITF Example/07APR2005_Hyperion_331405N0442002E_SWIR172_001_L1R.ntf"
    # Ok if data isn't available. We can only run test if we have the
    # test data.
    if(not os.path.exists(fname)):
        raise SkipTest
    f = NitfFile(fname)
    tcsexrb = f.image_segment[1].find_exactly_one_tre("CSEXRB")
#    f2 = NitfFile(f)
#    create_image_seg(f2)
#    f2.tre_list.append(f.find_exactly_one_tre("CSDIDA"))
#    f2.tre_list.append(f.find_exactly_one_tre("MATESA"))
#    f2.image_segment[0].tre_list.append(tcsexrb)
#    csattb = [d for d in f.des_segment if d.subheader.desid == "CSATTB"][0]
#    f2.des_segment.append(csattb)
#    csephb = [d for d in f.des_segment if d.subheader.desid == "CSEPHB"][0]
#    f2.des_segment.append(csephb)
#    cssfab = [d for d in f.des_segment if d.subheader.desid == "CSSFAB"][0]
#    f2.des_segment.append(cssfab)
#    cscsdb = [d for d in f.des_segment if d.subheader.desid == "CSCSDB"][0]
#    print(csattb.des.user_subheader)
#    print(csephb.des.user_subheader)
#    print(cssfab.des.user_subheader)
#    print(cscsdb.des.user_subheader)
#    f2.des_segment.append(cscsdb)
    f.write("test.ntf")
    igc1 = IgcMsp(fname, SimpleDem(), 1, "GLAS", "GLAS")
#    igc2 = IgcMsp("test.ntf", SimpleDem(), 0, "GLAS", "GLAS")
    igc2 = IgcMsp("test.ntf", SimpleDem(), 1, "GLAS", "GLAS")
    pt1 = igc1.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt2 = igc2.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    print(distance(pt1, pt2))
    pynitf.unregister_des_class(pynitf.NitfDesCopy)
    
