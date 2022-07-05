from geocal_swig import (GdalRasterImage, SpiceKernelList, SpicePlanetOrbit,
                         SubRasterImage, ConstantSpacingTimeTable,
                         PlanetConstant, PlanetSimpleDem, OrbitListCache,
                         Ipi, IpiImageGroundConnection, Time,
                         PosCsephb, AttCsattb, OrbitDes, GlasGfmCamera,
                         ScaleImage)
from .isis_support import read_kernel_from_isis
from .priority_handle_set import GeoCalPriorityHandleSet
from .spice_camera import ctx_camera, hrsc_camera, hirise_camera
import json

class IsisToIgcHandleSet(GeoCalPriorityHandleSet):
    '''Handle set for pds to isis.

    Note this is a bit of overkill, we have little more than a 
    switch on INSTRUMENT_ID. But since we already have the machinery
    here, go ahead and put this in place. If nothing else, it allows 
    downstream programs to add handles for new instruments.
    '''
    def handle_h(self, h, isis_img, isis_metadata, klist, subset, glas_gfm):
        return h.isis_to_igc(isis_img, isis_metadata, klist, subset=subset,
                             glas_gfm=glas_gfm)

class CtxIsisToIgc:
    def igc_to_glas(self, igc_r):
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
        cam_g = GlasGfmCamera(igc_r.ipi.camera, band, delta_sample)
        ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                    igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
        igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, igc_r.image)
        igc_g.platform_id = "MRO"
        igc_g.payload_id = "MRO"
        igc_g.sensor_id = "CTX"
        return igc_g
        
    def isis_to_igc(self, isis_img, isis_metadata, klist_in,subset=None,
                    glas_gfm=False):
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
        cam = ctx_camera()
        #TODO Handle subsetting in the sample direction for the camera
        if(img.number_sample != cam.number_sample(0)):
            raise RuntimeError(f"The image has {img.number_sample} samples, but the context camera has {cam.number_sample(0)} samples. These need to match")
        ipi = Ipi(orb_cache, cam, 0, tt.min_time, tt.max_time, tt)
        igc = IpiImageGroundConnection(ipi, dem, img)
        if(glas_gfm):
            igc = self.igc_to_glas(igc)
        return (True, igc)

IsisToIgcHandleSet.add_default_handle(CtxIsisToIgc())

class HiriseIsisToIgc:
    '''Note that the HiRISE data is small floating point numbers. We
    scale to get a reasonable looking image.'''
    def __init__(self, rad_scale=10000):
        self.rad_scale = rad_scale
        
    def igc_to_glas(self, igc_r):
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
        cam_g = GlasGfmCamera(igc_r.ipi.camera, band, delta_sample)
        ipi_g = Ipi(orb_g, cam_g, 0, igc_r.ipi.time_table.min_time,
                    igc_r.ipi.time_table.max_time, igc_r.ipi.time_table)
        igc_g = IpiImageGroundConnection(ipi_g, igc_r.dem, igc_r.image)
        igc_g.platform_id = "MRO"
        igc_g.payload_id = "MRO"
        igc_g.sensor_id = "HIRISE"
        return igc_g
        
    def isis_to_igc(self, isis_img, isis_metadata, klist,subset=None,
                    glas_gfm=False):
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
        orb = SpicePlanetOrbit("MRO", "MRO_HIRISE_OPTICAL_AXIS", klist,
                               PlanetConstant.MARS_NAIF_CODE)
        # There are two kinds of spacecraft clocks. The normal
        # resolution is "MRO", the high resolution is for NAIF ID
        # -74999. We have high resolution for HIRISE
        tstart = Time.time_sclk(idata["SpacecraftClockStartCount"], "-74999")

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
        ipi = Ipi(orb_cache, cam, 0, tt.min_time, tt.max_time, tt)
        igc = IpiImageGroundConnection(ipi, dem, img)
        if(glas_gfm):
            igc = self.igc_to_glas(igc)
        return (True, igc)

IsisToIgcHandleSet.add_default_handle(HiriseIsisToIgc())

def isis_to_igc(isis_fname, subset=None, glas_gfm=False):
    '''Create an IGC for the given ISIS cube file. This should have
    already had spiceinit run on it (e.g., you called pds_to_isis).

    You can supply a subset if desired, this should be an array of
    [Lstart, Sstart, Number_line, Number_sample].

    The default is to return a IGC that includes the native Spice orbit/
    camera model, etc, but you can request a GLAS/GFM model instead.
    '''
    f = GdalRasterImage(isis_fname)
    d = json.loads(f.metadata_list('json:ISIS3')[0])
    klist = read_kernel_from_isis(isis_fname)
    return IsisToIgcHandleSet.default_handle_set().handle(f,d,klist,
                                                          subset, glas_gfm)

__all__ = ["IsisToIgcHandleSet", "isis_to_igc"]    
