from geocal_swig import (PosCsephb, AttCsattb, OrbitDes)
try:
    from pynitf import (DesCSEPHB, DesCSATTB, create_nitf_des_structure,
                        register_des_class, NitfImageSegment)
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False

# ---------------------------------------------------------
# Override various DESs to use the geocal objects instead
# ---------------------------------------------------------
if(have_pynitf):
    if(DesCSEPHB.__doc__ is not None):
        hlp_csephb = DesCSEPHB.__doc__ + \
'''
This DES is implemented in the PosCsephb object available as pos_csephb.
This should be used to set and read the DES values.
'''
    else:
        hlp_csephb = None

    (DesCSEPHB_geocal, DesCSEPHB_UH_geocal) = create_nitf_des_structure("DesCSEPHB", DesCSEPHB._desc_data, DesCSEPHB._desc_uh, hlp=hlp_csephb, des_implementation_field="pos_csephb", des_implementation_class=PosCsephb)
    DesCSEPHB_geocal.desid = DesCSEPHB.desid
    DesCSEPHB_geocal.desver = DesCSEPHB.desver

    # Register this geocal class ahead of the pynitf DesCSEPHB class, so this
    # gets used to read the data instead of DesCSEPHB

    register_des_class(DesCSEPHB_geocal, priority_order=-1)

    if(DesCSATTB.__doc__ is not None):
        hlp_csattb = DesCSATTB.__doc__ + \
'''
This DES is implemented in the AttCsattb object available as pos_csattb.
This should be used to set and read the DES values.
'''
    else:
        hlp_csattb = None

    (DesCSATTB_geocal, DesCSATTB_UH_geocal) = create_nitf_des_structure("DesCSATTB", DesCSATTB._desc_data, DesCSATTB._desc_uh, hlp=hlp_csattb, des_implementation_field="att_csattb", des_implementation_class=AttCsattb)
    DesCSATTB_geocal.desid = DesCSATTB.desid
    DesCSATTB_geocal.desver = DesCSATTB.desver

    # Register this geocal class ahead of the pynitf DesCSATTB class, so this
    # gets used to read the data instead of DesCSATTB

    register_des_class(DesCSATTB_geocal, priority_order=-1)

    # Have search to find the pos_csephb and att_csattb that goes with a
    # image segment. We may extend this.
    def _pos_csephb(iseg):
        lv = iseg.idlvl
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError("Need to have nitf_file set for image segment level %d" % lv)
        possible = [d for d in f.des_segment if
                    d.subheader.desid == "CSEPHB" and
                    (d.des.user_subheader.numais == "ALL" or
                     lv in d.des.user_subheader.aisdlvl)]
        if(len(possible) == 0):
            return None
        if(len(possible) > 1):
            raise RuntimeError("Found more than one possible pos_csephb for image esegment level %d" % lv)
        return possible[0].des.pos_csephb

    def _att_csattb(iseg):
        lv = iseg.idlvl
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError("Need to have nitf_file set for image segment level %d" % lv)
        possible = [d for d in f.des_segment if
                    d.subheader.desid == "CSATTB" and
                    (d.des.user_subheader.numais == "ALL" or
                     lv in d.des.user_subheader.aisdlvl)]
        if(len(possible) == 0):
            return None
        if(len(possible) > 1):
            raise RuntimeError("Found more than one possible pos_csephb for image esegment level %d" % lv)
        return possible[0].des.att_csattb

    def _orbit_des(iseg):
        if(iseg.pos_csephb is None or iseg.att_csattb is None):
            return None
        return OrbitDes(iseg.pos_csephb, iseg.att_csattb)
    
    NitfImageSegment.pos_csephb = property(_pos_csephb)
    NitfImageSegment.att_csattb = property(_att_csattb)
    NitfImageSegment.orbit_des = property(_orbit_des)
    
if(have_pynitf):
    __all__ = ["DesCSEPHB_geocal","DesCSATTB_geocal",]
else:
    __all__ = []
