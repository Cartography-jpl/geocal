# This is a collection of IGC generated for various instruments.

from geocal_swig import *
from .pds_label import pds_label
from .sqlite_shelf import read_shelve
from .spice_camera import ctx_camera, hrsc_camera, hirise_camera
import struct
import subprocess
import os

# Note, when adding a new instrument there are a few good sanity things to
# check:
# 1. Check check_rc_assumption, this verifies that we have line, sample, and
#    look directions nearly orthogonal. If you have the camera rotated wrong,
#    you can catch that because the dot products won't be close to 0.
# 2. Compare spice calculation of boresight with the full Igc calculation. This
#    makes sure you have things lined up right.
# 3. It is also worth comparing data projected by our code vs. data that
#    has gone through ISIS. We won't line up exactly, but it should be pretty
#    close. This helps find gross errors in our models, etc.


def igc_mro_context(fname, lbl = None, kernel_file = None,
                    kernel_file_post = None, kernel_json = None,
                    subset=None, left_mask=38, right_mask=18):
    '''Process for context camera. 

    Because we have often already processed the PDS labels, you can
    optionally pass that if available. If we don't have this, then we
    go ahead and read the labels. You can pass in a specific kernel file to
    read, a kernel JSON file, and/or a post kernel file to read.

    This includes reading the spice kernels that we need.

    You can supply a subset if desired, this should be an array of
    [Lstart, Sstart, Number_line, Number_sample].
    '''
    if(lbl is None):
        lbl = pds_label(fname)
    if(lbl['INSTRUMENT_NAME'] != 'CONTEXT CAMERA'):
        raise RuntimeError("igc_mro_context called for file %s which is actually INSTRUMENT_NAME %s" % (fname, lbl['INSTRUMENT_NAME']))
    tstart = Time.parse_time(lbl["START_TIME"])
    bdir = os.environ["MARS_KERNEL"] + "/mro_kernel/"
    if(kernel_file):
        kfile = kernel_file
    else:
        kfile = bdir + "mro.ker"
    if(kernel_file_post):
        kpfile = kernel_file_post
    else:
        kpfile = None
    if(kernel_json):
        kjson = kernel_json
    else:
        kjson = bdir + "kernel.json"
    kdat = read_shelve(kjson)
    # We need better logic here, but for now just set up the kernels we
    # handed
    klist = [kfile,
             kdat["ck_kernel"].kernel(tstart),
             kdat["spk_kernel"].kernel(tstart)]
    if(kpfile):
        klist.append(kpfile)
    orb = SpicePlanetOrbit("MRO", "MRO_CTX", klist,
                           PlanetConstant.MARS_NAIF_CODE)
    img = GdalRasterImage(fname)
    sline = 0
    nline = img.number_line
    if(subset is not None):
        sline = subset[0]
        nline = subset[2]
    img = SubRasterImage(img, sline, left_mask, nline,
                         img.number_sample-(left_mask+right_mask))
    if(lbl["SAMPLE_BIT_MODE_ID"] == "SQROOT"):
        img = ContextSqrtDecodeImage(img)
    # The START_TIME is the commanded start time, the actual start time
    # might be different. According to the CTX SIS (available at
    # https://pdsimage2.wr.usgs.gov/archive/mro-m-ctx-2-edr-l0-v1.0/mrox_0452/document/
    #), we should use the SPACECRAFT_CLOCK_START_COUNT instead. Should be
    # pretty close to START_TIME. Note that we use START_TIME first because
    # we need to read the spice kernels before being able to convert SCLK to
    # time.
    tstart = Time.time_sclk(lbl["SPACECRAFT_CLOCK_START_COUNT"], "MRO")
    # 1e-3 is because LINE_EXPOSURE_DURATION is in milliseconds.
    tspace = float(lbl["LINE_EXPOSURE_DURATION"].split(" ")[0]) * 1e-3
    if(subset is not None):
        tstart += sline * tspace
    # The data can be averaged on board. I don't think this would be
    # hard to support, the line exposure should get multiplied (since
    # it is a single line time) and the camera modified. But we should
    # really test this out with some test data first though. For now, just
    # notice that the data has on board averaging and trigger an error
    if int(lbl["SAMPLING_FACTOR"]) != 1:
        raise RuntimeError("We currently only support a sampling factor of 1")
    tt = ConstantSpacingTimeTable(tstart, tstart + tspace * (img.number_line-1),
                                  tspace)
    dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
    orb_cache = OrbitListCache(orb, tt)
    cam = ctx_camera()
    if(img.number_sample != cam.number_sample(0)):
        raise RuntimeError(f"The image has {img.number_sample} samples, but the context camera has {cam.number_sample(0)} samples. These need to match")
    ipi = Ipi(orb_cache, cam, 0, tt.min_time, tt.max_time, tt)
    igc = IpiImageGroundConnection(ipi, dem, img)
    return igc

def igc_mro_context_to_glas(igc_r):
    '''Create a igc that can be used with GLAS from the results of 
    igc_mro_context'''
    tspace = igc_r.ipi.time_table.time_space
    porb = PosCsephb(igc_r.ipi.orbit.orbit_underlying,
                     igc_r.ipi.time_table.min_time - 10 * tspace,
                     igc_r.ipi.time_table.max_time + 10 * tspace,
                     tspace,
                     PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_5, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(igc_r.ipi.orbit.orbit_underlying,
                     igc_r.ipi.time_table.min_time - 10 * tspace,
                     igc_r.ipi.time_table.max_time + 10 * tspace,
                     tspace,
                     AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb_g = OrbitDes(porb,aorb, PlanetConstant.MARS_NAIF_CODE)
    band = 0
    delta_sample = 100
    cam_g = GlasGfmCamera(igc_r.ipi.camera, band, delta_sample)
    ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
    igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, None)
    igc_g.platform_id = "MRO"
    igc_g.payload_id = "MRO"
    igc_g.sensor_id = "CTX"
    return igc_g

def igc_mex_hrsc(fname, lbl = None, kernel_file = None,
                 kernel_file_post = None, kernel_json = None,
                 subset = None):
    '''Process for HRSC camera

    Because we have often already processed the PDS labels, you can
    optionally pass that if available. If we don't have this, then we
    go ahead and read the labels. You can pass in a specific kernel file to
    read, a kernel JSON file, and/or a post kernel file to read.

    This includes reading the spice kernels that we need.

    You can supply a subset if desired, this should be an array of
    [Lstart, Sstart, Number_line, Number_sample].
    '''
    if(lbl is None):
        lbl = pds_label(fname)
    if(lbl['INSTRUMENT_NAME'] != 'HIGH RESOLUTION STEREO CAMERA'):
        raise RuntimeError("igc_mex_hrsc called for file %s which is actually INSTRUMENT_NAME %s" % (fname, lbl['INSTRUMENT_NAME']))
    tstart = Time.parse_time(lbl["START_TIME"])
    tend = Time.parse_time(lbl["STOP_TIME"])
    if(lbl["DETECTOR_ID"] != "MEX_HRSC_NADIR"):
        raise RuntimeError("Right now we only work with MEX_HRSC_NADIR")
    bdir = os.environ["MARS_KERNEL"] + "/mex_kernel/"
    if(kernel_file):
        kfile = kernel_file
    else:
        kfile = bdir + "mex.ker"
    if(kernel_file_post):
        kpfile = kernel_file_post
    else:
        kpfile = bdir + "mex_reconstituted.ker"
    if(kernel_json):
        kjson = kernel_json
    else:
        kjson = bdir + "kernel.json"
    kdat = read_shelve(kjson)
    # We need better logic here, but for now just set up the kernels we
    # handed
    klist = [kfile,
             kdat["ck_kernel"].kernel(tstart),
             kdat["spk_kernel"].kernel(tstart)]
    if(kpfile):
        klist.append(kpfile)
    orb = SpicePlanetOrbit("MEX", "MEX_HRSC_HEAD", klist,
                           PlanetConstant.MARS_NAIF_CODE)
    img = GdalRasterImage(fname)
    # The PDS image has a byte label in the front of each line. GDAL
    # doesn't read this (it just skips it), but we can read it directly.
    # The byte label is described in HRSC_LABEL_HEADER.PDF
    # (at ftp://psa.esac.esa.int/pub/mirror/MARS-EXPRESS/HRSC/MEX-M-HRSC-3-RDR-V3.0/DOCUMENT),
    # but basically the first 8 bytes are just a double that gives the J2000
    # time for that line (and we don't care about anything else in the binary
    # label). We read through the data to create our TimeTable.
    record_bytes = int(lbl['RECORD_BYTES'])
    # This is 1 based, convert to 0 based with -1
    istart = int(lbl['^IMAGE']) - 1
    tlist = Vector_Time()
    with open(fname, "rb") as f:
        for i in range(istart, istart + img.number_line):
            f.seek(i * record_bytes, 0)
            tlist.push_back(Time.time_et(struct.unpack("d", f.read(8))[0]))
    tt = MeasuredTimeTable(tlist)
    dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
    orb_cache = OrbitListCache(orb, tt)
    ipi = Ipi(orb_cache, hrsc_camera(), 0, tt.min_time, tt.max_time, tt)
    igc = IpiImageGroundConnection(ipi, dem, img)
    if(subset is not None):
        igc = OffsetImageGroundConnection(igc, subset[0], subset[1],
                                          subset[2], subset[3])
    return igc

def igc_mro_hirise(fname, lbl = None, kernel_file = None,
                   kernel_file_post = None, kernel_json = None,
                   subset = None, rad_scale=10000):
    '''Process for HIRISE camera. 

    Right now, we are using ISIS as input. We will try to move this
    to PDS, but there are a number of preprocessing steps that we 
    leveraging off of for now. The data should go through:

    1. hi2isis from=blah_0.img to=blah_0.cub
    2. spiceinit from=blah_0.cub
    3. hical from=blah_0.cub to=blah_0.cal.cub
    4. Also do "_1"
    5. histitch from1=blah_0 from2=blah_1 to=blah
    6. Normalize if desired cubenorm from=blah to=blah.norm.cub

    Because we have often already processed the PDS labels, you can
    optionally pass that if available. If we don't have this, then we
    go ahead and read the labels. You can pass in a specific kernel file to
    read, a kernel JSON file, and/or a post kernel file to read.

    This includes reading the spice kernels that we need.

    You can supply a subset if desired, this should be an array of
    [Lstart, Sstart, Number_line, Number_sample].
    '''
    if(lbl is None):
        lbl = pds_label(fname)
    if(lbl['InstrumentId'] != 'HIRISE'):
        raise RuntimeError("igc_mro_hirise called for file %s which is actually INSTRUMENT_NAME %s" % (fname, lbl['InstrumentId']))
    tstart = Time.parse_time(lbl["StartTime"])
    bdir = os.environ["MARS_KERNEL"] + "/mro_kernel/"
    if(kernel_file):
        kfile = kernel_file
    else:
        kfile = bdir + "mro.ker"
    if(kernel_file_post):
        kpfile = kernel_file_post
    else:
        kpfile = None
    if(kernel_json):
        kjson = kernel_json
    else:
        kjson = bdir + "kernel.json"
    kdat = read_shelve(kjson)
    # We need better logic here, but for now just set up the kernels we
    # handed
    klist = [kfile,
             kdat["ck_kernel"].kernel(tstart),
             kdat["spk_kernel"].kernel(tstart)]
    if(kpfile):
        klist.append(kpfile)
    orb = SpicePlanetOrbit("MRO", "MRO_HIRISE_OPTICAL_AXIS", klist,
                           PlanetConstant.MARS_NAIF_CODE)
    img = ScaleImage(GdalRasterImage(fname), rad_scale)
    sline = 0
    nline = img.number_line
    if(subset is not None):
        sline = subset[0]
        nline = subset[2]
        img = SubRasterImage(img, sline, 0, nline, img.number_sample)
    # There are two kinds of spacecraft clocks. The normal resolution is "MRO",
    # the high resolution is for NAIF ID -74999. We have high resolution for
    # HIRISE
    tstart = Time.time_sclk(lbl["SpacecraftClockStartCount"], "-74999")
    # Hirise does a TDI, which means it collects a line multiple times
    # and sums it to reduce noise. We have to adjust the start times for this.
    # Not sure of the source of this equation, I got this from the ISIS code
    unbinned_rate = (74.0 + float(lbl["DeltaLineTimerCount"]) / 16) / 1e6
    tspace = unbinned_rate * float(lbl["Summing"])
    tstart -= unbinned_rate * (float(lbl["Tdi"]) / 2 - 0.5)
    tstart += unbinned_rate * (float(lbl["Summing"]) / 2 - 0.5)
    tstart += tspace * sline
    # Effective CCD Line number for the given TDI mode
    ccd_line = -64.0 + float(lbl["Tdi"]) / 2
    # Channel number is right and left, they get combined when we
    # put the channels together. Somewhat confusing name, this is *not*
    # the CCD number (e.g., red0 through red9). The offset is for the
    # right vs left. We might be off by 1, I got this from ISIS and they
    # use a different convention. We'll want to verify this at some point.
    if(int(lbl["ChannelNumber"]) == 0):
        ccd_start_sample = 1025
    # This is the CCD number. This comes through the "CCD Processing and
    # Memory Modules (CPMM) IDS". Mapping is found in mro_hirise_v12.ti,
    # we don't bother reading this from the SPICE kernels since this is
    # just a fixed mapping.
    ccd_number = [0, 1, 2, 3, 12, 4, 10, 11,
                  5, 13, 6, 7, 8, 9][int(lbl["CpmmNumber"])]
    cam = hirise_camera(ccd_number)
    # Should be able to look at HiriseCamera.cpp in ISIS to figure out
    # camera
    tt = ConstantSpacingTimeTable(tstart, tstart + tspace * (img.number_line-1),
                                  tspace)
    dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
    orb_cache = OrbitListCache(orb, tt)
    ipi = Ipi(orb_cache, cam, 0, tt.min_time, tt.max_time, tt)
    igc = IpiImageGroundConnection(ipi, dem, img)
    return igc

def igc_mro_hirise_to_glas(igc_r):
    '''Create a igc that can be used with GLAS from the results of 
    igc_mro_hirise'''
    tspace = igc_r.ipi.time_table.time_space
    porb = PosCsephb(igc_r.ipi.orbit.orbit_underlying,
                     igc_r.ipi.time_table.min_time - 10 * tspace,
                     igc_r.ipi.time_table.max_time + 10 * tspace,
                     tspace,
                     PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_5, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(igc_r.ipi.orbit.orbit_underlying,
                     igc_r.ipi.time_table.min_time - 10 * tspace,
                     igc_r.ipi.time_table.max_time + 10 * tspace,
                     tspace,
                     AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb_g = OrbitDes(porb,aorb, PlanetConstant.MARS_NAIF_CODE)
    band = 0
    delta_sample = 2048 / 16
    cam_g = GlasGfmCamera(igc_r.ipi.camera, band, delta_sample)
    ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
    igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, None)
    igc_g.platform_id = "MRO"
    igc_g.payload_id = "MRO"
    igc_g.sensor_id = "HiRISE"
    return igc_g

# Not sure if this actually fits here or not, but not an obvious other place
# for this
def vicar_update_glas(fin, fout, orbit_updated):
    '''Copy an existing VICAR file with a GLAS igc, and update the orbit
    used in the file (e.g., after doing a OrbitOffsetCorrection or something
    like that)'''
    try:
        os.remove(fout)
    except FileNotFoundError:
        pass
    subprocess.run(["cp", fin, fout], check=True)
    f = VicarRasterImage(fout, 1, VicarFile.UPDATE)
    igc = f.igc_glas_gfm
    porb_original = igc.ipi.orbit.pos_csephb
    porb = PosCsephb(orbit_updated,
                     porb_original.min_time,
                     porb_original.max_time,
                     porb_original.time_step,
                     PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_5, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb_original = igc.ipi.orbit.att_csattb
    aorb = AttCsattb(orbit_updated,
                     aorb_original.min_time,
                     aorb_original.max_time,
                     aorb_original.time_step,
                     AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb_g = OrbitDes(porb, aorb, igc.ipi.orbit.naif_code)
    igc.ipi.orbit = orb_g
    f.set_igc_glas_gfm(igc)

    
    
    
    


__all__ = ["igc_mro_context", "igc_mro_context_to_glas", "igc_mex_hrsc",
           "igc_mro_hirise", "igc_mro_hirise_to_glas", "vicar_update_glas"]
