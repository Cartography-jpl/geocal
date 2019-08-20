from geocal_swig import (PosCsephb, AttCsattb, OrbitDes)
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
        def __init__(self, tre_csexrb, f):
            self.tre_csexrb = tre_csexrb
            self.des = self.tre_csexrb.assoc_elem(f)
            print(self.des)
            print([d.desid() for d in self.des])

        @property
        def pos_csephb(self):
            '''Return PosCsephb or None. Will trigger an error if we
            have more than one DES that matches.'''
            res = [d.pos_csephb for d in self.des if d.desid() == "CSEPHB"]
            if(len(res) == 0):
                return None
            if(len(res) > 1):
                raise RuntimeError("Found more than one possible pos_csephb for GLAS/GFM")
            return res[0]

        @property
        def att_csattb(self):
            '''Return AttCsattb or None. Will trigger an error if we
            have more than one DES that matches.'''
            res = [d.att_csattb for d in self.des if d.desid() == "CSATTB"]
            if(len(res) == 0):
                return None
            if(len(res) > 1):
                raise RuntimeError("Found more than one possible att_csattb for GLAS/GFM")
            return res[0]

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
        return GlasGfm(t, self.nitf_file)

    # Need to add set also
    pynitf.NitfImageSegment.glas_gfm = property(_glas_gfm)    

if(have_pynitf):
    __all__ = ["GlasGfm",]
else:
    __all__ = []
    
