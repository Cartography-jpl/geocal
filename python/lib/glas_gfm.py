from geocal_swig import (PosCsephb, AttCsattb, OrbitDes,
                         ConstantSpacingTimeTable, SimpleCamera,
                         SimpleDem, IpiImageGroundConnection, Ipi)
from .geocal_nitf_misc import (nitf_date_second_field_to_geocal_time,
                               geocal_time_to_nitf_date_second_field)
try:
    import pynitf
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False
import numpy as np

if(have_pynitf):
    class GlasGfm(object):
        '''The GLAS/GFM structure in a NITF file crosses multiple TRE/DESs.
        This class collects all this together so we can treat this as
        "one thing"'''
        def __init__(self, iseg, tre_csexrb, f):
            self.iseg = iseg
            self.tre_csexrb = tre_csexrb
            self.des = self.tre_csexrb.assoc_elem(f)

        def igc(self, dem = SimpleDem()):
            '''Return ImageGroundConnection for GLAS/GFM'''
            tt = self.time_table
            orb = self.orbit
            cam = self.camera
            ipi = Ipi(orb, cam, 0, tt.min_time, tt.max_time, tt)
            return IpiImageGroundConnection(ipi, dem, None,
                                            self.iseg.iid1)

        @property
        def camera(self):
            '''Return the camera'''
            # Placeholder
            cam_des = self.cssfab
            if(cam_des.num_fl_pts != 1):
                raise RuntimeError("We don't currently support time dependent flocal lengths")
            return SimpleCamera(0,0,0, cam_des.foc_length[0], -0.00765 / 128,
                                -0.00765 / 128, 1,
                                self.tre_csexrb.num_samples)

        @property
        def time_table(self):
            '''Return a TimeTable (for a push broom) or None for a 
            frame camera'''
            if(self.tre_csexrb.day_first_line_image is None):
                return None
            tstart = nitf_date_second_field_to_geocal_time(self.tre_csexrb.day_first_line_image, self.tre_csexrb.time_first_line_image)
            tend = tstart + self.tre_csexrb.time_image_duration
            nline = self.tre_csexrb.num_lines
            # May need a +- 1 in here, we'll want to carefully check this
            return ConstantSpacingTimeTable(tstart, tend, (tend - tstart) / nline)

        def find_one_des(self, desid):
            res = [d for d in self.des if d.desid() == desid]
            if(len(res) == 0):
                return None
            if(len(res) > 1):
                raise RuntimeError("Found more than one possible DES %s in GLAS/GFM" % desid)
            return res[0]

        @property
        def pos_csephb(self):
            '''Return PosCsephb or None. Will trigger an error if we
            have more than one DES that matches.'''
            d = self.find_one_des("CSEPHB")
            if(d is None):
                return None
            return d.pos_csephb

        @property
        def cssfab(self):
            '''Return CSSFAB or None. Will trigger an error if we
            have more than one DES that matches.'''
            return self.find_one_des("CSSFAB")
        
        @property
        def att_csattb(self):
            '''Return AttCsattb or None. Will trigger an error if we
            have more than one DES that matches.'''
            d = self.find_one_des("CSATTB")
            if(d is None):
                return None
            return d.att_csattb

        @property
        def orbit(self):
            '''Return OrbitDes for GLAS/GFM'''
            if(self.pos_csephb is None or self.att_csattb is None):
                return None
            return OrbitDes(self.pos_csephb, self.att_csattb)
        
    def _glas_gfm(self):
        '''Return GlasGfm if we find it, otherwise None'''
        t = self.find_one_tre("CSEXRB")
        if(t is None):
            return None
        if(self.nitf_file is None):
            # Should this be an error? Can only have GlasGfm when we have
            # a file to get the DESs from
            return None
        return GlasGfm(self, t, self.nitf_file)

    # Need to add set also
    pynitf.NitfImageSegment.glas_gfm = property(_glas_gfm)    

if(have_pynitf):
    __all__ = ["GlasGfm",]
else:
    __all__ = []
    
