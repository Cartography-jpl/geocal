from geocal_swig import (nitf_to_quaternion, quaternion_to_nitf)
try:
    from pynitf import TreSENSRB, NitfImageSegment
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False

# ---------------------------------------------------------
# Add functions to sensrb TRE to read and write higher level
# objects
# ---------------------------------------------------------
if(have_pynitf):
    def _orbit_data_sensrb_get(self):
        t = self.find_exactly_one_tre("SENSRB")

    def _orbit_data_sensrb_set(self, od):
        t = self.find_one_tre("SENSRB")
        if(t is None):
            t = TreSENSRB()
            self.tre_list.append(t)

    NitfImageSegment.orbit_data_sensrb = property(_orbit_data_sensrb_get,
                                                  _orbit_data_sensrb_set)
    

if(have_pynitf):
    __all__ = []
else:
    __all__ = []


