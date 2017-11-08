from .nitf_file import *
from test_support import *

def create_image_seg(f):
    img = NitfImageFromNumpy(nrow=10, ncol=10)
    for i in range(10):
        for j in range(10):
            img.data[i,j] = i + j
    f.image_segment.append(NitfImageSegment(img))

def test_rsm_rp(isolated_dir, rsm):
    '''Create a file, and write out a RSM. This RSM has just a single 
    rational polynomial in it'''    
    f = NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    f2 = NitfFile("nitf_rsm.ntf")
    print(f2)

def test_rsm_grid(isolated_dir, rsm_g):
    '''Create a file, and write out a RSM. This RSM has just a single 
    grid in it'''    
    f = NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_g
    f.write("nitf_rsm.ntf")
    f2 = NitfFile("nitf_rsm.ntf")
    print(f2)

def test_rsm_ms_rp(isolated_dir, rsm_ms_rp):
    '''Create a file, and write out a RSM. This has a multi section
    rational polynomial in it'''    
    f = NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_ms_rp
    f.write("nitf_rsm.ntf")
    f2 = NitfFile("nitf_rsm.ntf")
    print(f2)

def test_rsm_ms_rp(isolated_dir, rsm_ms_g):
    '''Create a file, and write out a RSM. This has a multi section
    grid in it'''    
    f = NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_ms_g
    f.write("nitf_rsm.ntf")
    f2 = NitfFile("nitf_rsm.ntf")
    print(f2)
    
