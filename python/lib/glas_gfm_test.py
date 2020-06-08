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
import matplotlib.pylab as plt
import matplotlib as mpl
import seaborn as sns
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
import subprocess

# This also works with an igc.
def rsm_plot_diff(r, igc, fname=None, min_height = -5000,
                  max_height = -1500):
    '''This provides a quick plot/summary of the differences between the RSM
    we calculate and the underlying igc.

    If a file is supplied, we also write out the cross section plots.'''
    
    dem_nominal = (max_height + min_height) / 2.0
    ln, smp, lncalc, smpcalc = r.compare_igc(igc, 1000, 5,
                                             dem_nominal)
    print(ln)
    print(lncalc)
    print("Max line diff:   ", np.nanmax(abs(ln-lncalc)), " at ",
          np.unravel_index(np.nanargmax(abs(ln-lncalc)), ln.shape))
    print("Max sample diff: ", np.nanmax(abs(smp-smpcalc)), " at ",
          np.unravel_index(np.nanargmax(abs(smp-smpcalc)), smp.shape))
    if(fname is None):
        return
    pdf = PdfPages(fname)
    cmap = mpl.colors.ListedColormap(sns.color_palette("RdBu_r", 256))
    plt.clf()
    plt.ylim(-0.1,0.1)
    for i in range(smp.shape[1]):
        plt.plot(ln[:,i], lncalc[:,i] - ln[:,i], label="Sample %d" % smp[0,i])
    plt.legend()
    plt.title("Line diff")
    plt.xlabel("Line")
    plt.ylabel("Diff")
    pdf.savefig()
    plt.clf()
    plt.ylim(-0.2,0.2)
    for i in range(smp.shape[1]):
        plt.plot(ln[:,i], smpcalc[:,i] - smp[:,i], label="Sample %d" % smp[0,i])
    plt.legend()
    plt.title("Sample diff")
    plt.xlabel("Line")
    plt.ylabel("Diff")
    pdf.savefig()
    plt.clf()
    
    pdf.close()

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
    rsm_plot_diff(igc1, igc2, "diff.pdf")
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
    rsm_plot_diff(igc_gfm, igc2, "diff.pdf")
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
    
@require_pynitf
def test_gfm_nitfdiff(isolated_dir, igc_gfm):
    '''Create two NITF file with GFMs in them it, and make sure we can run nitfdiff on these'''
    f = pynitf.NitfFile()
    f.file_header.fdt = "dt1"
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
    f.write("gfm_test1.ntf")
    f = pynitf.NitfFile()
    f.file_header.fdt = "dt2"
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
    f.write("gfm_test2.ntf")
    subprocess.run("NITF_PLUGIN=geocal nitf_diff gfm_test1.ntf gfm_test2.ntf",
                   shell=True)

@require_msp    
@require_pynitf
def test_create_from_scratch_glas(isolated_dir):
    '''Create a NITF file with GLAS in it. This is completely from scratch,
    we don't use the RIP at all for this.'''
    orb = KeplerOrbit()
    tm = Time.parse_time("2019-02-03T10:00:00Z")
    # Camera that has a roughly 0.5 meter resolution nadir looking, i.e.,
    # about the resolution of WV-2
    cam = QuaternionCamera(Quaternion_double(1,0,0,0), 1, 2048, 20e-9,
                           20-9, 1.6e7, FrameCoordinate(0,1024))
    # Time delta that is roughly 0.5 meter apart
    tdelta = 7.5e-5
    dem = SimpleDem()
    tt = ConstantSpacingTimeTable(tm, tm + tdelta * 2048, tdelta)
    # "Real" igc
    ipi = Ipi(orb, cam, 0, tt.min_time, tt.max_time, tt)
    igc_r = IpiImageGroundConnection(ipi, dem, None)
    # Now, implement with stuff needed for GLAS
    porb = PosCsephb(orb,
                     tt.min_time - 10 * tdelta,
                     tt.max_time + 10 * tdelta,
                     1e-3,
                     PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_5, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(orb,
                     tt.min_time - 10 * tdelta,
                     tt.max_time + 10 * tdelta,
                     1e-3,
                     AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb_g = OrbitDes(porb,aorb)
    cam_g = GlasGfmCamera(1, cam.number_sample(0))
    # 1e-3 converts from mm for QuaternionCamera to meter of GlasGfmCamera
    cam_g.focal_length = cam.focal_length * 1e-3
    cam_g.field_alignment_fit(cam, cam.number_sample(0))
    ipi_g = Ipi(orb_g, cam_g, 0, tt.min_time, tt.max_time, tt)
    igc_g = IpiImageGroundConnection(ipi_g, dem, None)
    igc_g.platform_id = "FAKEPL"
    igc_g.payload_id = "FAKEPY"
    igc_g.sensor_id = "FAKESN"
    for i in range(0,2048,100):
        for j in range(0,2048,100):
            assert distance(igc_r.ground_coordinate(ImageCoordinate(i,j)),
                            igc_g.ground_coordinate(ImageCoordinate(i,j))) < 0.01
    f = pynitf.NitfFile()
    # Should put in 3d data
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
    for i in range(9):
        for j in range(10):
            img[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))
    f.image_segment[0].create_glas_gfm(igc_g)
    f.write("glas_test.ntf")
    f2 = NitfFile("glas_test.ntf")
    with open("f2.txt", "w") as fh:
        print(f2,file=fh)
    igc_g2 = f2.image_segment[0].glas_gfm.igc()
    # Slightly larger tolerance here because there is truncation of floats when
    # we write the out.
    for i in range(0,2048,100):
        for j in range(0,2048,100):
            assert distance(igc_r.ground_coordinate(ImageCoordinate(i,j)),
                            igc_g2.ground_coordinate(ImageCoordinate(i,j))) < 0.05 
