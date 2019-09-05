# This is a collection of IGC generated for various instruments.

from geocal_swig import *
from .pds_label import pds_label
from .sqlite_shelf import read_shelve
from .spice_camera import ctx_camera, hrsc_camera
import struct

def igc_mro_context(fname, lbl = None, kernel_file = None,
                    kernel_file_post = None, kernel_json = None):
    '''Process for context camera. 

    Because we have often already processed the PDS labels, you can
    optionally pass that if available. If we don't have this, then we
    go ahead and read the labels. You can pass in a specific kernel file to
    read, a kernel JSON file, and/or a post kernel file to read.

    This includes reading the spice kernels that we need.
    '''
    if(lbl is None):
        lbl = pds_label(fname)
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
    return igc

def igc_mex_hrsc(fname, lbl = None, kernel_file = None,
                 kernel_file_post = None, kernel_json = None):
    '''Process for HRSC camera

    Because we have often already processed the PDS labels, you can
    optionally pass that if available. If we don't have this, then we
    go ahead and read the labels. You can pass in a specific kernel file to
    read, a kernel JSON file, and/or a post kernel file to read.

    This includes reading the spice kernels that we need.
    '''
    if(lbl is None):
        lbl = pds_label(fname)
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
    return igc


__all__ = ["igc_mro_context", "igc_mex_hrsc"]
