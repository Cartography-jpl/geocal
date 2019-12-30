try:
    import pynitf
except ImportError:
    pass

from test_support import *
from geocal.geocal_nitf_des import *
from geocal.glas_gfm import *
from geocal_swig import (IgcMsp, SimpleDem, ImageCoordinate, distance, Geodetic,
                         OrbitDataImageGroundConnection, PosCsephb, AttCsattb,
                         KeplerOrbit, Time, OrbitDes, Ecr)

@require_msp    
@require_pynitf
def test_rip_glas(nitf_sample_rip):
    '''Test reading GLAS data from the NITF RIP sample'''
    # Index for the image segment that has GLAS data.
    iseg_index = 1 
    igc1 = IgcMsp(nitf_sample_rip, SimpleDem(), iseg_index, "GLAS", "GLAS")
    f = pynitf.NitfFile(nitf_sample_rip)
    glas = f.image_segment[iseg_index].glas_gfm
    ref_pt = Ecr(glas.tre_csexrb.ground_ref_point_x,
                 glas.tre_csexrb.ground_ref_point_y,
                 glas.tre_csexrb.ground_ref_point_z)
    print("Reference Point:", Geodetic(ref_pt))
    print(igc1.image_coordinate(ref_pt))
    print("Refraction flag: %d" % glas.tre_csexrb.atm_refr_flag)
    print("Velocity flag: %d" % glas.tre_csexrb.vel_aber_flag)
    igc2 = glas.igc()
    igc3 = IgcMsp(nitf_sample_rip, SimpleDem(), iseg_index, "RSM", "RSM")
    print("Resolution %f m" % igc1.resolution_meter())
    print("Resolution %f m" % igc2.resolution_meter())
    print("Resolution %f m" % igc3.resolution_meter())
    max_diff1 = -1e8
    max_diff2 = -1e8
    for i in range(0, igc2.number_line, 20):
        for j in range (0, igc2.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc1.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            d2 = distance(igc3.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            max_diff1 = max(d1, max_diff1)
            max_diff2 = max(d2, max_diff2)
    print(max_diff1)
    print(max_diff2)
    # Difference here is because the RIP has velocity and refraction turned
    # off. Our calculation can't easily turn off velocity aberration. We have
    # other tests that agree submeter when we have files with velocity
    # aberration.
    assert max_diff1 < 20.0

@require_msp    
@require_pynitf
def test_glas_with_image(nitf_sample_rip):
    '''Test reading GLAS data from the NITF RIP sample, and also reading
    the image data.'''
    # Index for the image segment that has GLAS data.
    iseg_index = 1 
    f = pynitf.NitfFile(nitf_sample_rip)
    glas = f.image_segment[iseg_index].glas_gfm
    igc = glas.igc(include_image=True)
    assert igc.image_multi_band.number_band == 172
    assert igc.image_multi_band.raster_image(0).number_line == 3352
    assert igc.image_multi_band.raster_image(0).number_sample == 256

    
@require_msp    
@require_pynitf
def test_create_glas(nitf_sample_rip):
    '''Create a NITF file with GLAS in it, and make sure MSP can read this'''
    f = pynitf.NitfFile()
    # Create two images, so we can make sure the DESs aren't duplicated
    # w/o reason.
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
    for i in range(9):
        for j in range(10):
            img[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))
    img2 = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=3)
    for i in range(9):
        for j in range(10):
            img2[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img2))

    fin = pynitf.NitfFile(nitf_sample_rip)
    with open("fin.txt", "w") as fh:
        print(fin,file=fh)
    iseg_index = 1
    igc1 = fin.image_segment[iseg_index].glas_gfm.igc()
    f.image_segment[0].create_glas_gfm(igc1)
    f.image_segment[1].create_glas_gfm(igc1)
    print(f)
    f.write("glas_test.ntf")
    f2 = NitfFile("glas_test.ntf")
    with open("f2.txt", "w") as fh:
        print(f2,file=fh)
    igc2 = IgcMsp("glas_test.ntf", SimpleDem(), 0, "GLAS", "GLAS")
    igc3 = f2.image_segment[0].glas_gfm.igc()
    print("Resolution %f m" % igc1.resolution_meter())
    print("Resolution %f m" % igc2.resolution_meter())
    max_diff1 = -1e8
    max_diff2 = -1e8
    for i in range(0, igc1.number_line, 20):
        for j in range (0, igc1.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc1.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            d2 = distance(igc1.ground_coordinate(ic),
                          igc3.ground_coordinate(ic))
            max_diff1 = max(d1, max_diff1)
            max_diff2 = max(d2, max_diff2)
    print(max_diff1)
    print(max_diff2)
    assert max_diff1 < 1.0

@require_msp    
@require_pynitf
def test_create_rot_glas(nitf_sample_rip):
    '''Create a NITF file with GLAS in it, and make sure MSP can read this.
       This tests the rotation of the camera'''
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
    for i in range(9):
        for j in range(10):
            img[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

    fin = pynitf.NitfFile(nitf_sample_rip)
    print(nitf_sample_rip)
    with open("fin.txt", "w") as fh:
        print(fin,file=fh)
    iseg_index = 1
    igc1 = fin.image_segment[iseg_index].glas_gfm.igc()
    igc1.ipi.camera.angoff = 0.2, 0.5, 1.0
    f.image_segment[0].create_glas_gfm(igc1)
    # Turn off refraction in MSP calculation. Normally we want this on,
    # but we turn it off so we can get better agreement with our Igc.
    # We'll probably want to investigate this and include refraction in
    # our calculation, but punt on this for now.
    f.image_segment[0].glas_gfm.tre_csexrb.atm_refr_flag = 0
    f.write("glas_test.ntf")
    f2 = NitfFile("glas_test.ntf")
    with open("f2.txt", "w") as fh:
        print(f2,file=fh)
    igc2 = IgcMsp("glas_test.ntf", SimpleDem(), 0, "GLAS", "GLAS")
    igc3 = f2.image_segment[0].glas_gfm.igc()
    max_diff1 = -1e8
    max_diff2 = -1e8
    for i in range(0, igc1.number_line, 20):
        for j in range (0, igc1.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc1.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            d2 = distance(igc1.ground_coordinate(ic),
                          igc3.ground_coordinate(ic))
            max_diff1 = max(d1, max_diff1)
            max_diff2 = max(d2, max_diff2)
    print(max_diff1)
    print(max_diff2)
    assert max_diff1 < 1.0

@require_msp    
@require_pynitf
def test_create_multiple_fa_glas(nitf_sample_rip):
    '''Create a NITF file with GLAS in it, and make sure MSP can read this.
       This tests having more than one pair in the field_alignment'''
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
    for i in range(9):
        for j in range(10):
            img[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

    fin = pynitf.NitfFile(nitf_sample_rip)
    print(nitf_sample_rip)
    with open("fin.txt", "w") as fh:
        print(fin,file=fh)
    iseg_index = 1
    igc1 = fin.image_segment[iseg_index].glas_gfm.igc()
    igc1.ipi.camera.delta_sample_pair = 128
    # This is a weird camera, where pixel 128 suddenly jumps 2 pixel
    # in line direction. but this gives us a good clear test, it
    # is very obvious if we are using the wrong part of the field_alignment
    # in our calculation
    igc1.ipi.camera.field_alignment = np.array([[-0.00765, 0, 0, 0],
                         [0, 0.00765 / 128 * 2, 0.00765, 0.00765 / 128 * 2]])
    f.image_segment[0].create_glas_gfm(igc1)
    # Turn off refraction in MSP calculation. Normally we want this on,
    # but we turn it off so we can get better agreement with our Igc.
    # We'll probably want to investigate this and include refraction in
    # our calculation, but punt on this for now.
    f.image_segment[0].glas_gfm.tre_csexrb.atm_refr_flag = 0
    f.write("glas_test.ntf")
    f2 = NitfFile("glas_test.ntf")
    with open("f2.txt", "w") as fh:
        print(f2,file=fh)
    igc2 = IgcMsp("glas_test.ntf", SimpleDem(), 0, "GLAS", "GLAS")
    igc3 = f2.image_segment[0].glas_gfm.igc()
    max_diff1 = -1e8
    max_diff2 = -1e8
    for i in range(0, igc1.number_line, 20):
        for j in range (0, igc1.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc1.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            d2 = distance(igc1.ground_coordinate(ic),
                          igc3.ground_coordinate(ic))
            max_diff1 = max(d1, max_diff1)
            max_diff2 = max(d2, max_diff2)
    print(max_diff1)
    print(max_diff2)
    assert max_diff1 < 1.0
    
    

@require_msp    
@require_pynitf
def test_create_gfm(isolated_dir, igc_gfm):
    '''Create a NITF file with GFM in it, and make sure MSP can read this'''
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
    for i in range(9):
        for j in range(10):
            img[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

    f.image_segment[0].create_glas_gfm(igc_gfm)
    t = f.image_segment[0].glas_gfm.tre_csexrb
    pt = igc_gfm.ground_coordinate(ImageCoordinate(1024, 1024))
    t.ground_ref_point_x = pt.position[0]
    t.ground_ref_point_y = pt.position[1]
    t.ground_ref_point_z = pt.position[2]
    t.atm_refr_flag = 0
    #t.vel_aber_flag = 0
    print(f)
    f.write("gfm_test.ntf")
    f2 = NitfFile("gfm_test.ntf")
    with open("f2.txt", "w") as fh:
        print(f2,file=fh)
    igc2 = IgcMsp("gfm_test.ntf", SimpleDem(), 0, "GFM", "GFM")
    igc3 = f2.image_segment[0].glas_gfm.igc()

    print("Resolution %f m" % igc_gfm.resolution_meter())
    print("Resolution %f m" % igc_gfm.resolution_meter())
    max_diff1 = -1e8
    max_diff2 = -1e8
    for i in range(0, igc_gfm.number_line, 20):
        for j in range (0, igc_gfm.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc_gfm.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            d2 = distance(igc_gfm.ground_coordinate(ic),
                          igc3.ground_coordinate(ic))
            print(igc_gfm.image_coordinate(igc_gfm.ground_coordinate(ic)))
            max_diff1 = max(d1, max_diff1)
            max_diff2 = max(d2, max_diff2)
    print(max_diff1)
    print(max_diff2)
    assert max_diff1 < 1.0

@long_test
@require_msp    
@require_pynitf
def test_create_many_gfm(isolated_dir, igc_gfm):
    '''Create a large number of images segments, to test handling of DES 
    overflow'''

    f = pynitf.NitfFile()

    pt = igc_gfm.ground_coordinate(ImageCoordinate(1024, 1024))
    for i in range(300):
        print("Doing %d" % i)
        img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2+i)
        for i1 in range(9):
            for i2 in range(10):
                img[0, i1,i2] = i1 * 10 + i2
        f.image_segment.append(pynitf.NitfImageSegment(img))

        f.image_segment[i].create_glas_gfm(igc_gfm)
        t = f.image_segment[i].glas_gfm.tre_csexrb
        t.ground_ref_point_x = pt.position[0]
        t.ground_ref_point_y = pt.position[1]
        t.ground_ref_point_z = pt.position[2]
        
    f.write("gfm_large_number.ntf")
    f2 = NitfFile("gfm_large_number.ntf")
    with open("f2.txt", "w") as fh:
        print(f2,file=fh)
    for iseg in f2.image_segment:
        igc2 = iseg.glas_gfm.igc()
        ic = ImageCoordinate(10, 20)
        assert distance(igc_gfm.ground_coordinate(ic),
                        igc2.ground_coordinate(ic)) < 1e-2
    
