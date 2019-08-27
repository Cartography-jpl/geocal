from geocal_swig import (PosCsephb, AttCsattb, OrbitDes, QuaternionCamera,
                         Quaternion_double, FrameCoordinate)
try:
    from pynitf import (DesCSEPHB, DesCSATTB, create_nitf_des_structure,
                        register_des_class, NitfImageSegment,
                        add_uuid_des_function)
    have_pynitf = True
    from .geocal_nitf_misc import (nitf_date_second_field_to_geocal_time,
                                   geocal_time_to_nitf_date_second_field)
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False
import numpy as np
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
    def _descsephb_handle_diff(self, t):
        '''Handle difference checking between 2 DesCSEPHB_geocal'''
        return (self.interpolation_type == t.interpolation_type and
                self.ephemeris_data_quality == t.ephemeris_data_quality and
                self.ephemeris_source == t.ephemeris_source and
                self.lagrange_order == t.lagrange_order and
                self.is_cf == t.is_cf and
                self.min_time == self.min_time and
                abs(self.time_step - t.time_step) < 1e-6 and
                np.all_class(self.position_data, t.position_data))
    
    DesCSEPHB_geocal.handle_diff = _descsephb_handle_diff

    # Register this geocal class ahead of the pynitf DesCSEPHB class, so this
    # gets used to read the data instead of DesCSEPHB

    add_uuid_des_function(DesCSEPHB_geocal)    
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
    def _descsattb_handle_diff(self, t):
        '''Handle difference checking between 2 DesCSATTB_geocal'''
        d1 = self.att_csattb
        d2 = t.att_csattb
        print(d1)
        print(d2)
        return (d1.interpolation_type == d2.interpolation_type and
                d1.attitude_data_quality == d2.attitude_data_quality and
                d1.attitude_source == d2.attitude_source and
                d1.lagrange_order == d2.lagrange_order and
                d1.is_cf == d2.is_cf and
                d1.min_time == d2.min_time and
                abs(d1.time_step - d2.time_step) < 1e-6 and
                np.allclose(d1.attitude_data, d2.attitude_data))
    
    DesCSATTB_geocal.handle_diff = _descsattb_handle_diff

    # Register this geocal class ahead of the pynitf DesCSATTB class, so this
    # gets used to read the data instead of DesCSATTB

    add_uuid_des_function(DesCSATTB_geocal)    
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

    def _cam_glas_gfm(iseg):
        lv = iseg.idlvl
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError("Need to have nitf_file set for image segment level %d" % lv)
        possible = [d for d in f.des_segment if
                    d.subheader.desid == "CSSFAB" and
                    (d.des.user_subheader.numais == "ALL" or
                     lv in d.des.user_subheader.aisdlvl)]
        if(len(possible) == 0):
            return None
        if(len(possible) > 1):
            raise RuntimeError("Found more than one possible cam_glas_gfm for image segment level %d" % lv)
        # Need to fill this out
        d = possible[0].des
        if(d.num_fl_pts != 1):
            raise RuntimeError("We don't currently support multiple focal lengths")
        cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                               1, 256, 0.00765 / 128, 0.00765 / 128,
                               d.foc_length[0],
                               FrameCoordinate(0, 128),
                               QuaternionCamera.LINE_IS_Y,
                               QuaternionCamera.INCREASE_IS_NEGATIVE,
                               QuaternionCamera.INCREASE_IS_POSITIVE)
        # This is extra metadata that we don't normally have in camera.
        # If we create a GlasGfm camera, we may put this into there.
        cam.band_type = d.band_type
        cam.band_wavelength = d.band_wavelength
        cam.band_index = list(d.band_index)
        cam.irepband = list(d.irepband)
        cam.isubcat = list(d.isubcat)
        cam.foc_length_time = nitf_date_second_field_to_geocal_time(d.foc_length_date, d.foc_length_time[0])
        cam.id = ""
        print(d)
        return cam

    def _cam_glas_gfm_set(iseg, cam):
        pass
    
    
    def _orbit_des(iseg):
        if(iseg.pos_csephb is None or iseg.att_csattb is None):
            return None
        return OrbitDes(iseg.pos_csephb, iseg.att_csattb)
    
    NitfImageSegment.pos_csephb = property(_pos_csephb)
    NitfImageSegment.att_csattb = property(_att_csattb)
    NitfImageSegment.orbit_des = property(_orbit_des)
    NitfImageSegment.cam_glas_gfm = property(_cam_glas_gfm, _cam_glas_gfm)
    
if(have_pynitf):
    __all__ = ["DesCSEPHB_geocal","DesCSATTB_geocal",]
else:
    __all__ = []
