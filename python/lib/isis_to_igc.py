from geocal_swig import (GdalRasterImage, SpiceKernelList, SpicePlanetOrbit,
                         SubRasterImage, ConstantSpacingTimeTable,
                         ConstantSpacingFrameletTimeTable,
                         PlanetConstant, PlanetSimpleDem, OrbitListCache,
                         Ipi, IpiImageGroundConnection, Time,
                         PosCsephb, AttCsattb, OrbitDes, GlasGfmCamera,
                         ScaleImage, NoVelocityAberration, SubCamera,
                         SimpleCamera)
from .isis_support import read_kernel_from_isis
from .priority_handle_set import GeoCalPriorityHandleSet
from .spice_camera import ctx_camera, hrsc_camera, hirise_camera
from .mars_rsm import rsm_hirise, rsm_context
from .sqlite_shelf import write_shelve
from .spice_igc import SpiceIgc
import json

class IsisToIgcHandleSet(GeoCalPriorityHandleSet):
    '''Handle set for pds to isis.

    Note this is a bit of overkill, we have little more than a 
    switch on INSTRUMENT_ID. But since we already have the machinery
    here, go ahead and put this in place. If nothing else, it allows 
    downstream programs to add handles for new instruments.
    '''
    def handle_h(self, h, isis_img, isis_metadata, klist, **keywords):
        return h.isis_to_igc(isis_img, isis_metadata, klist, **keywords)

# TODO Move common stuff into a base class
# We only have two examples here, so it is a bit hard to know exactly how
# this should be done. But already there is a bit of similarity between
# CTX and HiRISE that could be put into a base class. For now, just
# duplicate code - this is only a few 100 lines anyways. But this really
# should have more of a structure.

class CtxIsisToIgc:
    def igc_to_glas(self, igc_r, focal_length):
        '''Convert IGC to a GLAS model.'''
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
        cam_g = GlasGfmCamera(igc_r.ipi.camera, band, delta_sample, "R",
                              0.65, focal_length*1e-3)
        ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                    igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
        igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, igc_r.image)
        igc_g.platform_id = "MRO"
        igc_g.payload_id = "MRO"
        igc_g.sensor_id = "CTX"
        return igc_g
        
    def isis_to_igc(self, isis_img, isis_metadata, klist_in,subset=None,
                    glas_gfm=False, rsm=False, min_height=-5000,
                    max_height=-1500, igc_out_fname=None,
                    match_isis=False, spice_igc=False, **keywords):
        idata = isis_metadata["IsisCube"]["Instrument"]
        if(idata["InstrumentId"] != "CTX"):
            return (False, None)
        # For some reason, spiceinit includes the iak, but leave the
        # ik kernel out. Go ahead and add that, and load the kernels.
        # TODO Might be nice to have a cleaner way to handle this. In
        # particular it would be nice to avoid the hardcoded kernel here.
        t = ["$mro/kernels/ik/mro_ctx_v10.ti",]
        t.extend(klist_in.kernel_list)
        klist = SpiceKernelList(t)
        sline = 0
        nline = isis_img.number_line
        if(subset is not None):
            sline = subset[0]
            nline = subset[2]
            img = SubRasterImage(isis_img, subset[0], subset[1],
                                 subset[2], subset[3])
        else:
            img = isis_img
        # Note IsisIgc uses LT+S. this is actually *wrong*, see the
        # description of that in IsisIgc. We have this code in place
        # so we can duplicate IsisIgc (e.g., make sure everything else
        # agrees)
        if(match_isis):
            orb = SpicePlanetOrbit("MRO", "MRO_CTX", klist,
                                   PlanetConstant.MARS_NAIF_CODE,
                                   "LT+S")
        else:
            orb = SpicePlanetOrbit("MRO", "MRO_CTX", klist,
                                   PlanetConstant.MARS_NAIF_CODE)
        tstart = Time.time_sclk(idata["SpacecraftClockCount"], "MRO")
        # 1e-3 is because LINE_EXPOSURE_DURATION is in milliseconds.
        if(idata["LineExposureDuration"]["unit"] != "MSEC"):
            raise RuntimeError(f"Not sure how to handle LineExposureDuration units of {idata['LineExposureDuration']['unit']}")
        tspace = float(idata["LineExposureDuration"]["value"]) * 1e-3
        if(subset is not None):
            tstart += sline * tspace
        # The data can be averaged on board. I don't think this would be
        # hard to support, the line exposure should get multiplied (since
        # it is a single line time) and the camera modified. But we should
        # really test this out with some test data first though. For now, just
        # notice that the data has on board averaging and trigger an error
        if int(idata['SpatialSumming']) != 1:
            raise RuntimeError("We currently only support a sampling factor of 1")
        tt = ConstantSpacingTimeTable(tstart,
                                      tstart + tspace * (img.number_line-1),
                                      tspace)
        dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
        orb_cache = OrbitListCache(orb, tt)
        start_sample = int(idata["SampleFirstPixel"])
        cam = ctx_camera(start_sample=start_sample, nsamp=img.number_sample)
        if(isinstance(cam, SubCamera)):
            focal_length = cam.full_camera.focal_length
        else:
            focal_length = cam.focal_length
        if(spice_igc):
            return (True, SpiceIgc(orb, cam, tt, img=img))
        ipi = Ipi(orb_cache, cam, 0, tt.min_time, tt.max_time, tt)
        igc = IpiImageGroundConnection(ipi, dem, img)
        # LT+S already corrects for velocity aberration (LT+S is basically
        # aberration plus light time - so more accurate). Turn off the
        # igc calculation if we are matching ISIS
        if(match_isis):
            igc.velocity_aberration = NoVelocityAberration()
        if(igc_out_fname is not None):
            write_shelve(igc_out_fname, igc)
        if(glas_gfm):
            igc = self.igc_to_glas(igc, focal_length)
        if(not rsm):
            return (True, igc)
        else:
            return (True, (igc, rsm_context(igc, min_height=min_height,
                                            max_height=max_height)))

IsisToIgcHandleSet.add_default_handle(CtxIsisToIgc())

class LroWacIsisToIgc:
    def igc_to_glas(self, igc_r, focal_length):
        '''Convert IGC to a GLAS model.'''
        raise RuntimeError("Not implemented")
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
        cam_g = GlasGfmCamera(igc_r.ipi.camera, band, delta_sample, "R",
                              0.65, focal_length*1e-3)
        ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                    igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
        igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, igc_r.image)
        igc_g.platform_id = "MRO"
        igc_g.payload_id = "MRO"
        igc_g.sensor_id = "CTX"
        return igc_g
        
    def isis_to_igc(self, isis_img, isis_metadata, klist,subset=None,
                    glas_gfm=False, rsm=False, min_height=-5000,
                    max_height=-1500, igc_out_fname=None,
                    match_isis=False, spice_igc=False, band=3, **keywords):
        '''This takes the band number. By convention, we have 1 and 2 as
        the UV bands, 3-7 as the visible bands.'''
        idata = isis_metadata["IsisCube"]["Instrument"]
        if(idata["InstrumentId"] != "WAC-VIS"):
            return (False, None)
        klist.load_kernel()
        sline = 0
        nline = isis_img.number_line
        if(subset is not None):
            sline = subset[0]
            nline = subset[2]
            img = SubRasterImage(isis_img, subset[0], subset[1],
                                 subset[2], subset[3])
        else:
            img = isis_img

        # Map band to the filter type
        if(band == 1):
            frame = "LRO_LROCWAC_UV_FILTER_1"
            framelet_size = 16
        elif(band == 2):
            frame = "LRO_LROCWAC_UV_FILTER_2"
            framelet_size = 16
        elif(band == 3):
            frame = "LRO_LROCWAC_VIS_FILTER_1"
            framelet_size = 14
        elif(band == 4):
            frame = "LRO_LROCWAC_VIS_FILTER_2"
            framelet_size = 14
        elif(band == 5):
            frame = "LRO_LROCWAC_VIS_FILTER_3"
            framelet_size = 14
        elif(band == 6):
            frame = "LRO_LROCWAC_VIS_FILTER_4"
            framelet_size = 14
        elif(band == 7):
            frame = "LRO_LROCWAC_VIS_FILTER_5"
            framelet_size = 14
        else:
            raise RuntimeError(f"Unknown band number {band}. Should be 1-7")
            
        # Note IsisIgc uses LT+S. this is actually *wrong*, see the
        # description of that in IsisIgc. We have this code in place
        # so we can duplicate IsisIgc (e.g., make sure everything else
        # agrees)
        if(match_isis):
            orb = SpicePlanetOrbit("MRO", "MRO_CTX", klist,
                                   PlanetConstant.MARS_NAIF_CODE,
                                   "LT+S")
        else:
            orb = SpicePlanetOrbit("LRO", frame, klist,
                                   PlanetConstant.MOON_NAIF_CODE)
        tstart = Time.time_sclk(idata["SpacecraftClockStartCount"], "LRO")
        tend = Time.time_sclk(idata["SpacecraftClockStopCount"], "LRO")
        if(idata["InterframeDelay"]["unit"] != "ms"):
            raise RuntimeError(f"Not sure how to handle InterframeDelay units of {idata['InterframeDelay']['unit']}")
        if(idata["ExposureDuration"]["unit"] != "ms"):
            raise RuntimeError(f"Not sure how to handle ExposureDuration units of {idata['InterframeDelay']['unit']}")
        edur = float(idata["ExposureDuration"]["value"]) * 1e-3
        tspace = float(idata["InterframeDelay"]["value"]) * 1e-3
        # This is what ISIS does. This is probably sensible
        tstart += edur / 2.0
        tt = ConstantSpacingFrameletTimeTable(tstart, tend, framelet_size,
                                              tspace)
        dem = PlanetSimpleDem(PlanetConstant.MOON_NAIF_CODE)
        orb_cache = OrbitListCache(orb, tt)
        # Nominal values, we'll replace with real camera later
        cam = SimpleCamera(0,0,0,6e-3,9.0e-6,9.0e-6,14,704)
        focal_length = 6e-3
        if(spice_igc):
            return (True, SpiceIgc(orb, cam, tt, img=img))
        ipi = Ipi(orb_cache, cam, 0, tt.min_time, tt.max_time, tt)
        igc = IpiImageGroundConnection(ipi, dem, img)
        # LT+S already corrects for velocity aberration (LT+S is basically
        # aberration plus light time - so more accurate). Turn off the
        # igc calculation if we are matching ISIS
        if(match_isis):
            igc.velocity_aberration = NoVelocityAberration()
        if(igc_out_fname is not None):
            write_shelve(igc_out_fname, igc)
        if(glas_gfm):
            igc = self.igc_to_glas(igc, focal_length)
        if(not rsm):
            return (True, igc)
        else:
            raise RuntimeError("We don't support RSM for LRO WAC")

IsisToIgcHandleSet.add_default_handle(LroWacIsisToIgc())

class HiriseIsisToIgc:
    '''Note that the HiRISE data is small floating point numbers. We
    scale to get a reasonable looking image.'''
    def __init__(self, rad_scale=10000):
        self.rad_scale = rad_scale
        
    def igc_to_glas(self, igc_r, focal_length):
        '''Convert IGC to a GLAS model.'''
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
        cam_g = GlasGfmCamera(igc_r.ipi.camera, band, delta_sample, "R", 0.7,
                              focal_length * 1e-3)
        ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                    igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
        igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, igc_r.image)
        igc_g.platform_id = "MRO"
        igc_g.payload_id = "MRO"
        igc_g.sensor_id = "HIRISE"
        return igc_g
        
    def isis_to_igc(self, isis_img, isis_metadata, klist,subset=None,
                    glas_gfm=False, rsm=False, min_height=-5000,
                    max_height=-1500, igc_out_fname=None, match_isis=False,
                    spice_igc = False,
                    **keywords):
        idata = isis_metadata["IsisCube"]["Instrument"]
        if(idata["InstrumentId"] != "HIRISE"):
            return (False, None)
        sline = 0
        nline = isis_img.number_line
        if(subset is not None):
            sline = subset[0]
            nline = subset[2]
            img = SubRasterImage(isis_img, subset[0], subset[1],
                                 subset[2], subset[3])
        else:
            img = isis_img
        if(self.rad_scale is not None):
            img = ScaleImage(img, self.rad_scale)
        klist.load_kernel()
        # Note IsisIgc uses LT+S. this is actually *wrong*, see the
        # description of that in IsisIgc. We have this code in place
        # so we can duplicate IsisIgc (e.g., make sure everything else
        # agrees)
        if(match_isis):
            orb = SpicePlanetOrbit("MRO", "MRO_HIRISE_OPTICAL_AXIS", klist,
                                   PlanetConstant.MARS_NAIF_CODE,
                                   "LT+S")
        else:
            orb = SpicePlanetOrbit("MRO", "MRO_HIRISE_OPTICAL_AXIS", klist,
                                   PlanetConstant.MARS_NAIF_CODE)
        # There are two kinds of spacecraft clocks. The normal
        # resolution is "MRO", the high resolution is for NAIF ID
        # -74999. We have high resolution for HIRISE
        tstart = Time.time_sclk(idata["SpacecraftClockStartCount"], "-74999")
        # Hirise does a TDI, which means it collects a line multiple times
        # and sums it to reduce noise. We have to adjust the start times
        # for this.
        # I got this from the ISIS code, originally comes from
        # https://hirise-pds.lpl.arizona.edu/PDS/DOCUMENT/HIRISE_EDR_SIS.PDF
        unbinned_rate = (74.0 + float(idata["DeltaLineTimerCount"]) / 16) / 1e6
        tstart -= unbinned_rate * (float(idata["Tdi"]) / 2 - 0.5)
        tstart += unbinned_rate * (float(idata["Summing"]) / 2 - 0.5)

        if(idata["LineExposureDuration"]["unit"] != "MICROSECONDS"):
            raise RuntimeError(f"Not sure how to handle LineExposureDuration units of {idata['LineExposureDuration']['unit']}")
        tspace = float(idata["LineExposureDuration"]["value"]) * 1e-6
        if(subset is not None):
            tstart += sline * tspace
        # This is the CCD number. This comes through the "CCD Processing and
        # Memory Modules (CPMM) IDS". Mapping is found in mro_hirise_v12.ti,
        # we don't bother reading this from the SPICE kernels since this is
        # just a fixed mapping.
        ccd_number = [0, 1, 2, 3, 12, 4, 10, 11,
                      5, 13, 6, 7, 8, 9][int(idata["CpmmNumber"])]
        tt = ConstantSpacingTimeTable(tstart,
                                      tstart + tspace * (img.number_line-1),
                                      tspace)
        dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
        orb_cache = OrbitListCache(orb, tt)
        cam = hirise_camera(ccd_number)
        #TODO Handle subsetting in the sample direction for the camera
        if(img.number_sample != cam.number_sample(0)):
            raise RuntimeError(f"The image has {img.number_sample} samples, but the context camera has {cam.number_sample(0)} samples. These need to match")
        if(spice_igc):
            return (True, SpiceIgc(orb, cam, tt, img=img))
        ipi = Ipi(orb_cache, cam, 0, tt.min_time, tt.max_time, tt)
        igc = IpiImageGroundConnection(ipi, dem, img)
        # LT+S already corrects for velocity aberration (LT+S is basically
        # aberration plus light time - so more accurate). Turn off the
        # igc calculation if we are matching ISIS
        if(match_isis):
            igc.velocity_aberration = NoVelocityAberration()
        if(igc_out_fname is not None):
            write_shelve(igc_out_fname, igc)
        if(glas_gfm):
            igc = self.igc_to_glas(igc, cam.focal_length)
        if(not rsm):
            return (True, igc)
        else:
            return (True, (igc, rsm_hirise(igc, min_height=min_height,
                                           max_height=max_height)))

IsisToIgcHandleSet.add_default_handle(HiriseIsisToIgc())

def isis_to_igc(isis_fname, **keywords):
    '''Create an IGC for the given ISIS cube file. This should have
    already had spiceinit run on it (e.g., you called pds_to_isis).

    The specific options for each IGC depends on the type, but some
    of the keywords we have are:

    band - If there are multiple bands, select which one to use. By convention
    this is 1 based.

    subset - You can supply a subset if desired, this should be an array of
    [Lstart, Sstart, Number_line, Number_sample].

    glas_gfm - The default is to return a IGC that includes the native
    Spice orbit/ camera model, etc, but you can request a GLAS/GFM
    model instead.

    rsm, min_height, max_height - You can also request a RSM, in which
    case a pair of igc,rsm gets returned. I'm not 100% sure of this
    interface, but the details of making the RSM depends on the
    specific instrument so for now this is how we do this. For the
    RSM, you can pass in the min and max height to use. This has no
    impact on the IGC.

    igc_out_fname - You can optionally write the full IGC out as a shelve 
    file by supplying igc_out_fname

    match_isis - ISIS incorrectly calculate the light time based on
    the center of the target (e.g., mars center) rather than the
    surface. You can request that we use the same calculation as
    ISIS. This is *wrong*, but it can be useful for testing to make
    the same wrong calculation. This allows us to compare everything
    else (e.g., camera model, timing).

    Spice_igc - If true return a SpiceIgc.
    '''
    f = GdalRasterImage(isis_fname)
    d = json.loads(f.metadata_list('json:ISIS3')[0])
    klist = read_kernel_from_isis(isis_fname)
    return IsisToIgcHandleSet.default_handle_set().handle(f,d,klist,**keywords)

__all__ = ["IsisToIgcHandleSet", "isis_to_igc"]    
