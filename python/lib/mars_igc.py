# This is a collection of IGC generated for various instruments.

from geocal_swig import *
from .pds_label import pds_label
from .sqlite_shelf import read_shelve
from .spice_camera import ctx_camera, hrsc_camera, hirise_camera
import struct

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
                    subset=None):
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
    # We have masked pixels in the L1 data, which we want to chop out
    left_masked = 38
    right_masked = 18
    img = SubRasterImage(img, 0, left_masked, img.number_line, 5000)
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
    tt = ConstantSpacingTimeTable(tstart, tstart + tspace * (img.number_line-1),
                                  tspace)
    dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
    orb_cache = OrbitListCache(orb, tt)
    ipi = Ipi(orb_cache, ctx_camera(), 0, tt.min_time, tt.max_time, tt)
    igc = IpiImageGroundConnection(ipi, dem, img)
    if(subset is not None):
        igc = geocal.OffsetImageGroundConnection(igc, subset[0], subset[1],
                                          subset[2], subset[3])
    return igc

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
        igc = geocal.OffsetImageGroundConnection(igc, subset[0], subset[1],
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
    if(subset is not None):
        igc = geocal.OffsetImageGroundConnection(igc, subset[0], subset[1],
                                          subset[2], subset[3])
    return igc


__all__ = ["igc_mro_context", "igc_mex_hrsc", "igc_mro_hirise"]
