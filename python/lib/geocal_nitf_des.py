from geocal_swig import (PosCsephb, AttCsattb, OrbitDes, GlasGfmCamera,
                         Quaternion_double, FrameCoordinate)
try:
    from pynitf import (DesCSEPHB, DesCSATTB, create_nitf_des_structure,
                        DesCSSFAB,
                        register_des_class, NitfImageSegment,
                        add_uuid_des_function, NitfFile, NitfDesSegment)
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

    def _pos_csephb_set(iseg, pos_csephb):
        f = iseg.nitf_file
        if(f is None):
            raise RuntimeError("Need to add NitfImageSegment to a NitfFile before we can add a pos_csephb to it")
        d = f.find_des_by_uuid(pos_csephb.id)
        if(d is None):
            d = DesCSEPHB_geocal()
            d.pos_csephb = pos_csephb
            d.generate_uuid_if_needed()
            pos_csephb.id = d.id
            f.des_segment.append(NitfDesSegment(des=d))
        d.add_display_level(iseg.idlvl)
            
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

    def _att_csattb_set(iseg, att_csattb):
        f = iseg.nitf_file
        if(f is None):
            raise RuntimeError("Need to add NitfImageSegment to a NitfFile before we can add a att_csattb to it")
        d = f.find_des_by_uuid(att_csattb.id)
        if(d is None):
            d = DesCSATTB_geocal()
            d.att_csattb = att_csattb
            d.generate_uuid_if_needed()
            att_csattb.id = d.id
            f.des_segment.append(NitfDesSegment(des=d))
        d.add_display_level(iseg.idlvl)

    def _camera_cssfab(d):
        if(d.num_fl_pts != 1):
            raise RuntimeError("We don't currently support multiple focal lengths")
        cam = GlasGfmCamera()
        cam.band_type = d.band_type
        cam.band_wavelength = d.band_wavelength
        cam.band_index = list(d.band_index)
        cam.irepband = list(d.irepband)
        cam.isubcat = list(d.isubcat)
        cam.focal_length_time = nitf_date_second_field_to_geocal_time(d.foc_length_date, d.foc_length_time[0])
        cam.focal_length = d.foc_length[0]
        # We currently can't set position offset. We could add this if
        # this becomes an issue, but the current geocal code doesn't
        # support a position offset in a camera model
        #cam.ppoff = [d.ppoff_x, d.ppoff_y, d.ppoff_z]
        cam.angoff = [d.angoff_x, d.angoff_y, d.angoff_z]
        cam.sample_number_first = d.smpl_num_first
        cam.delta_sample_pair = d.delta_smpl_pair
        fa = np.empty((d.num_fa_pairs, 4))
        for i in range(d.num_fa_pairs):
            fa[i,0] = d.start_falign_x[i]
            fa[i,1] = d.start_falign_y[i]
            fa[i,2] = d.end_falign_x[i]
            fa[i,3] = d.end_falign_y[i]
        cam.field_alignment = fa
        cam.id = ""
        return cam

    def _camera_cssfab_set(d, cam):
        d.sensor_type = cam.sensor_type
        d.band_type = cam.band_type
        d.band_wavelength = cam.band_wavelength
        d.n_bands = len(cam.band_index)
        for i in range(d.n_bands):
            d.band_index[i] = cam.band_index[i]
            d.irepband[i] =  cam.irepband[i]
            d.isubcat[i] = cam.isubcat[i]
        d.fl_interp = 1
        d.num_fl_pts = 1
        d.foc_length_date, d.foc_length_time[0] = geocal_time_to_nitf_date_second_field(cam.focal_length_time)
        d.foc_length[0] = cam.focal_length
        d.ppoff_x = cam.ppoff[0]
        d.ppoff_y = cam.ppoff[1]
        d.ppoff_z = cam.ppoff[2]
        d.angoff_x = cam.angoff[0]
        d.angoff_y = cam.angoff[1]
        d.angoff_z = cam.angoff[2]
        d.smpl_num_first = cam.sample_number_first
        d.delta_smpl_pair = cam.delta_sample_pair
        d.num_fa_pairs = cam.field_alignment.shape[0]
        for i in range(d.num_fa_pairs):
            d.start_falign_x[i] = cam.field_alignment[i, 0]
            d.start_falign_y[i] = cam.field_alignment[i, 1]
            d.end_falign_x[i] = cam.field_alignment[i, 2]
            d.end_falign_y[i] = cam.field_alignment[i, 3]

    def _camera_glas_gfm(iseg):
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
            raise RuntimeError("Found more than one possible camera_glas_gfm for image segment level %d" % lv)
        d = possible[0].des
        return d.camera

    def _camera_glas_gfm_set(iseg, cam):
        f = iseg.nitf_file
        if(f is None):
            raise RuntimeError("Need to add NitfImageSegment to a NitfFile before we can add a camera_glas_gfm to it")
        d = f.find_des_by_uuid(cam.id)
        if(d is None):
            d = DesCSSFAB()
            d.camera = cam
            d.generate_uuid_if_needed()
            cam.id = d.id
            f.des_segment.append(NitfDesSegment(des=d))
        d.add_display_level(iseg.idlvl)
    
    def _orbit_des(iseg):
        if(iseg.pos_csephb is None or iseg.att_csattb is None):
            return None
        return OrbitDes(iseg.pos_csephb, iseg.att_csattb)

    def _orbit_des_set(iseg, orb):
        iseg.pos_csephb = orb.pos_csephb
        iseg.att_csattb = orb.att_csattb
    
    def _find_des_by_uuid(f, id):
        '''Find the DES that has a ID with the given value. This should be a
        unique value, so we should have at most one. Return None if we
        don't find it.
        '''
        
        if(id == ""):
            return None
        for dseg in f.des_segment:
            if(hasattr(dseg.des, "id") and
               dseg.des.id == id):
                return dseg.des
        return None
    
    NitfFile.find_des_by_uuid = _find_des_by_uuid
    NitfImageSegment.pos_csephb = property(_pos_csephb, _pos_csephb_set)
    NitfImageSegment.att_csattb = property(_att_csattb, _att_csattb_set)
    NitfImageSegment.orbit_des = property(_orbit_des, _orbit_des_set)
    NitfImageSegment.camera_glas_gfm = property(_camera_glas_gfm, _camera_glas_gfm_set)
    DesCSSFAB.camera = property(_camera_cssfab, _camera_cssfab_set)
    
if(have_pynitf):
    __all__ = ["DesCSEPHB_geocal","DesCSATTB_geocal",]
else:
    __all__ = []
