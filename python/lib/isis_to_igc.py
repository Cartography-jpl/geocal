from geocal_swig import (GdalRasterImage, SpiceKernelList, SpicePlanetOrbit,
                         SubRasterImage, ConstantSpacingTimeTable,
                         PlanetConstant, PlanetSimpleDem, OrbitListCache,
                         Ipi, IpiImageGroundConnection, Time)
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
    def handle_h(self, h, isis_img, isis_metadata, klist, subset):
        return h.isis_to_igc(isis_img, isis_metadata, klist, subset=subset)

class CtxIsisToIgc:
    def isis_to_igc(self, isis_img, isis_metadata, klist_in,subset=None):
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
        return (True, igc)

IsisToIgcHandleSet.add_default_handle(CtxIsisToIgc())
    
def isis_to_igc(isis_fname, subset=None):
    '''Create an IGC for the given ISIS cube file. This should have
    already had spiceinit run on it (e.g., you called pds_to_isis).

    You can supply a subset if desired, this should be an array of
    [Lstart, Sstart, Number_line, Number_sample].
    '''
    f = GdalRasterImage(isis_fname)
    d = json.loads(f.metadata_list('json:ISIS3')[0])
    klist = read_kernel_from_isis(isis_fname)
    return IsisToIgcHandleSet.default_handle_set().handle(f,d,klist, subset)

__all__ = ["IsisToIgcHandleSet", "isis_to_igc"]    
