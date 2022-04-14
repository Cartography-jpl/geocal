try:
    import pynitf
except ImportError:
    pass

from test_support import *
from .geocal_nitf_des import *
from .glas_gfm import *
from geocal_swig import (IgcMsp, SimpleDem, ImageCoordinate, distance, Geodetic,
                         OrbitDataImageGroundConnection, PosCsephb, AttCsattb,
                         KeplerOrbit, Time, OrbitDes, Ecr, RefractionMsp,
                         quat_rot_x, quat_rot_y, deg_to_rad)
from .sqlite_shelf import write_shelve
import matplotlib.pylab as plt
import matplotlib as mpl
import seaborn as sns
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
import subprocess
import math
import pandas as pd

# This also works with an igc.
def rsm_plot_diff(r, igc, fname=None, min_height = -5000,
                  max_height = -1500):
    '''This provides a quick plot/summary of the differences between the RSM
    we calculate and the underlying igc.

    If a file is supplied, we also write out the cross section plots.'''
    
    dem_nominal = (max_height + min_height) / 2.0
    ln, smp, lncalc, smpcalc, distance_true_vs_calc = r.compare_igc(igc, 1000, 5,
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
    print(glas)
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
    print(f2.image_segment[0].glas_gfm)
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
    assert max_diff1 < 1.5

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
    assert max_diff1 < 1.5

@skip    
def test_create_steep_igc(isolated_dir, igc_gfm):
    '''Test to generate a Steep angle OrbitDataImageGroundConnection suitable
    for a GFM. We use this in C++ unit tests.'''
    # Add a fairly steep angle, so we have a large refraction correction.
    # This is the nominal angle for the MISR DF camera, but this is just
    # "some steep angle" data
    q = (quat_rot_x(0.0) * quat_rot_y(58.0 * deg_to_rad) *
         quat_rot_x(-2.7 * deg_to_rad))
    igc_gfm.camera.frame_to_sc = q
    # Write and read to Nitf file, to get any rounding into place
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
    for i in range(9):
        for j in range(10):
            img[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))
    f.image_segment[0].create_glas_gfm(igc_gfm)
    f.write("gfm_test.ntf")
    f2 = NitfFile("gfm_test.ntf")
    igc_gfm2 = f2.image_segment[0].glas_gfm.igc()
    write_shelve(unit_test_data + "steep_igc_gfm.xml", igc_gfm2)
    
                 
@require_msp    
@require_pynitf
def test_refraction_gfm(isolated_dir, igc_gfm):
    '''Repeat previous test, but with refraction turned on. Check that our
    refraction is working correctly.'''
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
    for i in range(9):
        for j in range(10):
            img[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))
    # Add a fairly steep angle, so we have a large refraction correction.
    # This is the nominal angle for the MISR DF camera, but this is just
    # "some steep angle" data
    q = (quat_rot_x(0.0) * quat_rot_y(58.0 * deg_to_rad) *
         quat_rot_x(-2.7 * deg_to_rad))
    igc_gfm.camera.frame_to_sc = q
    igc_gfm.refraction = RefractionMsp(igc_gfm.camera.band_wavelength)
    # We can put this in, but we don't use it in our igc. We'll need
    # to figure out how this gets used, it isn't any of the things I
    # thought here
    if False:
        igc_gfm.camera.ppoff = [0,0,99]
        od = igc_gfm.orbit_data
        sc_p = Quaternion_double(0,*igc_gfm.camera.ppoff)
        q = od.sc_to_cf
        poff = quaternion_to_array(q * sc_p * q.conj())[1:]
        print(poff)
        od2 = QuaternionOrbitData(od,[poff[0],poff[1],poff[2]],Quaternion_double(1,0,0,0))
        print(igc_gfm.orbit_data)
        print(igc_gfm.orbit_data.position_cf.position)
        igc_gfm.orbit_data = od2
        print(igc_gfm.orbit_data)
        print(igc_gfm.orbit_data.position_cf.position)
    f.image_segment[0].create_glas_gfm(igc_gfm)
    t = f.image_segment[0].glas_gfm.tre_csexrb
    pt = igc_gfm.ground_coordinate(ImageCoordinate(1024, 1024))
    t.ground_ref_point_x = pt.position[0]
    t.ground_ref_point_y = pt.position[1]
    t.ground_ref_point_z = pt.position[2]
    igc_gfm.velocity_aberration = VelocityAberrationExact()
    if False:
        # Compare without aberration correction
        t.vel_aber_flag = 0
        igc_gfm.velocity_aberration = NoVelocityAberration()
    if False:
        # Compare without refraction
        t.atm_refr_flag = 0
        igc_gfm.refraction = None
    f.write("gfm_test.ntf")
    f2 = NitfFile("gfm_test.ntf")
    igc2 = IgcMsp("gfm_test.ntf", SimpleDem(), 0, "GFM", "GFM")
    igc3 = f2.image_segment[0].glas_gfm.igc(velocity_aberration_exact=True)
    write_shelve("igc_gfm.xml", igc_gfm)
    write_shelve("igc3.xml", igc3)
    max_diff1 = -1e8
    max_diff2 = -1e8
    max_diff3 = -1e8
    ic = ImageCoordinate(100, 50)
    print("Single point 1: ", distance(igc2.ground_coordinate(ic), igc_gfm.ground_coordinate(ic)))
    print("Single point 2: ", distance(igc2.ground_coordinate(ic), igc3.ground_coordinate(ic)))
    for i in range(0, igc_gfm.number_line, 20):
        for j in range (0, igc_gfm.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc_gfm.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
            d2 = distance(igc_gfm.ground_coordinate(ic),
                          igc3.ground_coordinate(ic))
            d3 = distance(igc2.ground_coordinate(ic),
                          igc3.ground_coordinate(ic))
            max_diff1 = max(d1, max_diff1)
            max_diff2 = max(d2, max_diff2)
            max_diff3 = max(d3, max_diff3)
    # diff1 is the difference between original glas model, before saving
    # to disk and MSP. This is a combination of MSP differences and 
    # roundoff from saving to the NITF file format
    # (mostly due to camera original angles in angoff being truncated).
    # diff2 is difference with our glas model in memory, and on disk, this
    # is exactly the error from roundoff. diff3 if difference between
    # our code and MSP, and it what is different with our calculations.
    print("max_diff1: ", max_diff1)
    print("max_diff2: ", max_diff2)
    print("max_diff3: ", max_diff3)
    assert max_diff1 < 2.0
    
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
def test_create_from_scratch_glas(isolated_dir, igc_half_meter_pushbroom):
    '''Create a NITF file with GLAS in it. This is completely from scratch,
    we don't use the RIP at all for this.'''
    # "Real" igc
    igc_r = igc_half_meter_pushbroom
    # Now, implement with stuff needed for GLAS
    porb = PosCsephb(igc_r.ipi.orbit,
                     igc_r.ipi.time_table.min_time - 10 * 1e-3,
                     igc_r.ipi.time_table.max_time + 10 * 1e-3,
                     1e-3,
                     PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_5, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(igc_r.ipi.orbit,
                     igc_r.ipi.time_table.min_time - 10 * 1e-3,
                     igc_r.ipi.time_table.max_time + 10 * 1e-3,
                     1e-3,
                     AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb_g = OrbitDes(porb,aorb)
    cam_g = GlasGfmCamera(1, igc_r.ipi.camera.number_sample(0))
    # 1e-3 converts from mm for QuaternionCamera to meter of GlasGfmCamera
    cam_g.focal_length = igc_r.ipi.camera.focal_length * 1e-3
    cam_g.field_alignment_fit(igc_r.ipi.camera,
                              igc_r.ipi.camera.number_sample(0))
    ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
    igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, None)
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

@require_msp    
@require_pynitf
def test_create_staring_glas(isolated_dir, igc_staring):
    '''Create a NITF file with GLAS in it. This is completely from scratch,
    we don't use the RIP at all for this.

    This variation uses a "staring" mode, which gives a strong bow-tie on the
    surface. Want to make sure we handle this correctly, and in particular the
    RSM handles this'''
    # "Real" igc
    igc_r = igc_staring
    igc_r.ipi.velocity_aberration = VelocityAberrationExact()
    tdelta = (igc_r.ipi.time_table.time(ImageCoordinate(1,0))[0] -
              igc_r.ipi.time_table.time(ImageCoordinate(0,0))[0])
    # Now, implement with stuff needed for GLAS
    porb = PosCsephb(igc_r.ipi.orbit,
                     igc_r.ipi.orbit.min_time,
                     igc_r.ipi.orbit.max_time,
                     tdelta,
                     PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_5, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(igc_r.ipi.orbit,
                     igc_r.ipi.orbit.min_time,
                     igc_r.ipi.orbit.max_time,
                     tdelta,
                     AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb_g = OrbitDes(porb,aorb)
    cam_g = GlasGfmCamera(1, igc_r.ipi.camera.number_sample(0))
    # 1e-3 converts from mm for QuaternionCamera to meter of GlasGfmCamera
    cam_g.focal_length = igc_r.ipi.camera.focal_length * 1e-3
    # MSP requires focal length to be from 0 100. Not clear why it would
    # restrict this
    cam_g.field_alignment_fit(igc_r.ipi.camera,
                              igc_r.ipi.camera.number_sample(0))
    ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
    ipi_g.velocity_aberration = VelocityAberrationExact()
    igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, None)
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
    img2 = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=3)
    for i in range(9):
        for j in range(10):
            img2[0, i,j] = i * 10 + j
    f.image_segment.append(pynitf.NitfImageSegment(img))
    f.image_segment.append(pynitf.NitfImageSegment(img2))
    f.image_segment[0].create_glas_gfm(igc_g)
    f.image_segment[1].create_glas_gfm(igc_g)
    # First one without refraction
    f.image_segment[0].glas_gfm.tre_csexrb.atm_refr_flag = 0
    # Second image that does have refraction turned on, so we can compare
    f.image_segment[1].glas_gfm.tre_csexrb.atm_refr_flag = 1
    f.write("glas_test.ntf")
    f2 = NitfFile("glas_test.ntf")
    igc_msp2 = IgcMsp("glas_test.ntf", SimpleDem(), 0, "GLAS", "GLAS")
    igc_msp2_refr = IgcMsp("glas_test.ntf", SimpleDem(), 1, "GLAS", "GLAS")
    with open("f2.txt", "w") as fh:
        print(f2,file=fh)
    igc_g2 = f2.image_segment[0].glas_gfm.igc(velocity_aberration_exact=True)
    igc_g2_refr = f2.image_segment[1].glas_gfm.igc(velocity_aberration_exact=True)

    if False:
        rsm = igc_msp2.generate_rsm("rsm_generate_report.txt")
        return
    
    # Slightly larger tolerance here because there is truncation of floats when
    # we write the out.
    diff = []
    diff2 = []
    diff3 = []
    h = []
    h2 = []
    dist_ref = []
    for i in range(0,2048,100):
        for j in range(0,2048,100):
            gc1 = igc_r.ground_coordinate(ImageCoordinate(i,j))
            gc2 = igc_g2.ground_coordinate(ImageCoordinate(i,j))
            gc3 = igc_msp2.ground_coordinate(ImageCoordinate(i,j))
            gc4 = igc_g2_refr.ground_coordinate(ImageCoordinate(i,j))
            gc5 = igc_msp2_refr.ground_coordinate(ImageCoordinate(i,j))
            assert distance(gc1, gc2) < 0.05
            dist_ref.append(distance(gc2,gc4))
            diff.append(distance(gc3, gc2))
            diff2.append(distance(gc3, gc1))
            diff3.append(distance(gc4, gc5))
            h.append(gc3.height_reference_surface)
            h2.append(gc2.height_reference_surface)
    # Small differences, not clear what this is from. Maybe aberration of light?
    # Refraction is a much bigger effect here with the larger angles
    print("max diff geocal vs. MSP no refraction: ", max(diff))
    print("max diff geocal vs. MSP with refraction: ", max(diff3))
    print("max diff before save: ", max(diff2))
    print("max h: ", max(h))
    print("max h: ", max(h2))
    print("Distance from refraction")
    print(pd.DataFrame(dist_ref).describe())
    
