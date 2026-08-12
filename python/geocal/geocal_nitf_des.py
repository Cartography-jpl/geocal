from geocal_swig import PosCsephb, AttCsattb, OrbitDes, GlasGfmCamera

try:
    from pynitf import (
        DesCSEPHB,
        DesCSATTB,
        DesCSSFAB,
        NitfImageSegment,
        NitfFile,
        NitfDesSegment,
    )

    have_pynitf = True
    from .geocal_nitf_misc import (
        nitf_date_second_field_to_geocal_time,
        geocal_time_to_nitf_date_second_field,
    )
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False
import numpy as np

# ---------------------------------------------------------
# Override various DESs to use the geocal objects instead
# ---------------------------------------------------------
if have_pynitf:
    DesCSEPHB.des_implementation_field = "pos_csephb"
    DesCSEPHB.des_implementation_class = PosCsephb

    def _descsephb_handle_diff(self, t):
        p1 = self.pos_csephb
        p2 = t.pos_csephb
        return (
            p1.interpolation_type == p2.interpolation_type
            and p1.ephemeris_data_quality == p2.ephemeris_data_quality
            and p1.ephemeris_source == p2.ephemeris_source
            and p1.lagrange_order == p2.lagrange_order
            and p1.is_cf == p2.is_cf
            and p1.min_time == p1.min_time
            and abs(p1.time_step - p2.time_step) < 1e-6
            and np.allclose(p1.position_data, p2.position_data)
        )

    DesCSEPHB.handle_diff = _descsephb_handle_diff
    DesCSATTB.des_implementation_field = "att_csattb"
    DesCSATTB.des_implementation_class = AttCsattb

    def _descsattb_handle_diff(self, t):
        """Handle difference checking between 2 DesCSATTB"""
        d1 = self.att_csattb
        d2 = t.att_csattb
        return (
            d1.interpolation_type == d2.interpolation_type
            and d1.attitude_data_quality == d2.attitude_data_quality
            and d1.attitude_source == d2.attitude_source
            and d1.lagrange_order == d2.lagrange_order
            and d1.is_cf == d2.is_cf
            and d1.min_time == d2.min_time
            and abs(d1.time_step - d2.time_step) < 1e-6
            and np.allclose(d1.attitude_data, d2.attitude_data)
        )

    DesCSATTB.handle_diff = _descsattb_handle_diff

    # Have search to find the pos_csephb and att_csattb that goes with a
    # image segment. We may extend this.
    def _pos_csephb(iseg):
        lv = iseg.idlvl
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError(
                "Need to have nitf_file set for image segment level %d" % lv
            )
        possible = [
            d
            for d in f.des_segment
            if d.subheader.desid == "CSEPHB"
            and (
                d.des.user_subheader.numais == "ALL"
                or lv in d.des.user_subheader.aisdlvl
            )
        ]
        if len(possible) == 0:
            return None
        if len(possible) > 1:
            raise RuntimeError(
                "Found more than one possible pos_csephb for image esegment level %d"
                % lv
            )
        return possible[0].des.pos_csephb

    def _pos_csephb_set(iseg, pos_csephb):
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError(
                "Need to add NitfImageSegment to a NitfFile before we can add a pos_csephb to it"
            )
        d = f.find_des_by_uuid(pos_csephb.id)
        if d is None:
            d = DesCSEPHB()
            d.pos_csephb = pos_csephb
            d.generate_uuid_if_needed()
            pos_csephb.id = d.id
            f.des_segment.append(NitfDesSegment(d))
        d.add_display_level(iseg.idlvl)

    def _att_csattb(iseg):
        lv = iseg.idlvl
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError(
                "Need to have nitf_file set for image segment level %d" % lv
            )
        possible = [
            d
            for d in f.des_segment
            if d.subheader.desid == "CSATTB"
            and (
                d.des.user_subheader.numais == "ALL"
                or lv in d.des.user_subheader.aisdlvl
            )
        ]
        if len(possible) == 0:
            return None
        if len(possible) > 1:
            raise RuntimeError(
                "Found more than one possible pos_csephb for image esegment level %d"
                % lv
            )
        return possible[0].des.att_csattb

    def _att_csattb_set(iseg, att_csattb):
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError(
                "Need to add NitfImageSegment to a NitfFile before we can add a att_csattb to it"
            )
        d = f.find_des_by_uuid(att_csattb.id)
        if d is None:
            d = DesCSATTB()
            d.att_csattb = att_csattb
            d.generate_uuid_if_needed()
            att_csattb.id = d.id
            f.des_segment.append(NitfDesSegment(d))
        d.add_display_level(iseg.idlvl)

    def _camera_cssfab(d):
        if d.num_fl_pts != 1:
            raise RuntimeError("We don't currently support multiple focal lengths")
        cam = GlasGfmCamera()
        cam.band_type = d.band_type
        cam.band_wavelength = d.band_wavelength
        cam.band_index = list(d.band_index)
        cam.irepband = list(d.irepband)
        cam.isubcat = list(d.isubcat)
        cam.focal_length_time = nitf_date_second_field_to_geocal_time(
            d.foc_length_date, d.foc_length_time[0]
        )
        cam.focal_length = d.foc_length[0]
        cam.ppoff = [d.ppoff_x, d.ppoff_y, d.ppoff_z]
        cam.angoff = [d.angoff_x, d.angoff_y, d.angoff_z]
        if d.sensor_type == "S":
            cam.set_number_line(1)
            cam.set_number_sample(int(d.num_fa_pairs * d.delta_smpl_pair))
            cam.sample_number_first = d.smpl_num_first
            cam.delta_sample_pair = d.delta_smpl_pair
            fa = np.empty((d.num_fa_pairs, 4))
            for i in range(d.num_fa_pairs):
                fa[i, 0] = d.start_falign_x[i]
                fa[i, 1] = d.start_falign_y[i]
                fa[i, 2] = d.end_falign_x[i]
                fa[i, 3] = d.end_falign_y[i]
            cam.field_alignment = fa
        elif d.sensor_type == "F":
            cam.field_angle_type = d.field_angle_type
            ndset = d.num_sets_fa_data
            cam.field_angle_interpolation_type = d.fa_interp
            if d.field_angle_type == 0:
                # Currently only work with one focal length
                for i in range(ndset):
                    if d.fl_cal[i] != d.foc_length[0]:
                        raise RuntimeError(
                            "Currently we only work with one focal length, so they all need to be the same."
                        )
                cam.first_line_block = [d.number_fir_line[i] for i in range(ndset)]
                cam.delta_line_block = [d.delta_line[i] for i in range(ndset)]
                cam.first_sample_block = [d.number_fir_samp[i] for i in range(ndset)]
                cam.delta_sample_block = [d.delta_samp[i] for i in range(ndset)]
                maxln = 0
                maxsmp = 0
                for i1 in range(ndset):
                    fa = np.empty(
                        (d.num_fa_blocks_line[i1], d.num_fa_blocks_samp[i1], 2, 2, 2)
                    )
                    maxln = max(
                        cam.first_line_block[i1]
                        + fa.shape[0] * cam.delta_line_block[i1],
                        maxln,
                    )
                    maxsmp = max(
                        cam.first_sample_block[i1]
                        + fa.shape[1] * cam.delta_sample_block[i1],
                        maxsmp,
                    )
                    for i2 in range(fa.shape[0]):
                        for i3 in range(fa.shape[1]):
                            fa[i2, i3, 0, 0, 0] = d.fa_x1[i1, i2, i3]
                            fa[i2, i3, 0, 0, 1] = d.fa_y1[i1, i2, i3]
                            fa[i2, i3, 0, 1, 0] = d.fa_x2[i1, i2, i3]
                            fa[i2, i3, 0, 1, 1] = d.fa_y2[i1, i2, i3]
                            fa[i2, i3, 1, 1, 0] = d.fa_x3[i1, i2, i3]
                            fa[i2, i3, 1, 1, 1] = d.fa_y3[i1, i2, i3]
                            fa[i2, i3, 1, 0, 0] = d.fa_x4[i1, i2, i3]
                            fa[i2, i3, 1, 0, 1] = d.fa_y4[i1, i2, i3]
                    cam.field_alignment_block(i1, fa)
                cam.set_number_line(int(maxln))
                cam.set_number_sample(int(maxsmp))
            else:
                raise RuntimeError("Unknown field angle type %d" % d.field_angle_type)
            if d.telescope_optics_flag != 0:
                # We could perhaps add this if needed in the future
                raise RuntimeError("We don't support telescope optics correction")
        else:
            raise RuntimeError("Unknown sensor type %s" % d.sensor_type)
        cam.id = ""
        return cam

    def _camera_cssfab_set(d, cam):
        d.sensor_type = cam.sensor_type
        d.band_type = cam.band_type
        d.band_wavelength = cam.band_wavelength
        d.n_bands = len(cam.band_index)
        for i in range(d.n_bands):
            d.band_index[i] = cam.band_index[i]
            d.irepband[i] = cam.irepband[i]
            d.isubcat[i] = cam.isubcat[i]
        d.fl_interp = 0
        d.num_fl_pts = 1
        d.foc_length_date, d.foc_length_time[0] = geocal_time_to_nitf_date_second_field(
            cam.focal_length_time
        )
        d.foc_length[0] = cam.focal_length
        d.ppoff_x = cam.ppoff[0]
        d.ppoff_y = cam.ppoff[1]
        d.ppoff_z = cam.ppoff[2]
        d.angoff_x = cam.angoff[0]
        d.angoff_y = cam.angoff[1]
        d.angoff_z = cam.angoff[2]
        if cam.sensor_type == "S":
            d.smpl_num_first = cam.sample_number_first
            d.delta_smpl_pair = cam.delta_sample_pair
            d.num_fa_pairs = cam.field_alignment.shape[0]
            for i in range(d.num_fa_pairs):
                d.start_falign_x[i] = cam.field_alignment[i, 0]
                d.start_falign_y[i] = cam.field_alignment[i, 1]
                d.end_falign_x[i] = cam.field_alignment[i, 2]
                d.end_falign_y[i] = cam.field_alignment[i, 3]
        elif cam.sensor_type == "F":
            d.field_angle_type = cam.field_angle_type
            d.fa_interp = cam.field_angle_interpolation_type
            if cam.field_angle_type == 0:
                d.num_sets_fa_data = cam.first_line_block.shape[0]
                for i1 in range(d.num_sets_fa_data):
                    d.fl_cal[i1] = cam.focal_length
                    d.number_fir_line[i1] = cam.first_line_block[i1]
                    d.delta_line[i1] = cam.delta_line_block[i1]
                    fa = cam.field_alignment_block(i1)
                    d.num_fa_blocks_line[i1] = fa.shape[0]
                    d.number_fir_samp[i1] = cam.first_sample_block[i1]
                    d.delta_samp[i1] = cam.delta_sample_block[i1]
                    d.num_fa_blocks_samp[i1] = fa.shape[1]
                    for i2 in range(fa.shape[0]):
                        for i3 in range(fa.shape[1]):
                            d.fa_x1[i1, i2, i3] = fa[i2, i3, 0, 0, 0]
                            d.fa_y1[i1, i2, i3] = fa[i2, i3, 0, 0, 1]
                            d.fa_x2[i1, i2, i3] = fa[i2, i3, 0, 1, 0]
                            d.fa_y2[i1, i2, i3] = fa[i2, i3, 0, 1, 1]
                            d.fa_x3[i1, i2, i3] = fa[i2, i3, 1, 1, 0]
                            d.fa_y3[i1, i2, i3] = fa[i2, i3, 1, 1, 1]
                            d.fa_x4[i1, i2, i3] = fa[i2, i3, 1, 0, 0]
                            d.fa_y4[i1, i2, i3] = fa[i2, i3, 1, 0, 1]
                pass
            else:
                raise RuntimeError("Unknown field angle type %d" % cam.field_angle_type)
            # We don't support the telescope optics correction. We could
            # perhaps add this in the future
            d.telescope_optics_flag = 0
        else:
            raise RuntimeError("Unknown sensor type %s" % cam.sensor_type)

    def _camera_glas_gfm(iseg):
        lv = iseg.idlvl
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError(
                "Need to have nitf_file set for image segment level %d" % lv
            )
        possible = [
            d
            for d in f.des_segment
            if d.subheader.desid == "CSSFAB"
            and (
                d.des.user_subheader.numais == "ALL"
                or lv in d.des.user_subheader.aisdlvl
            )
        ]
        if len(possible) == 0:
            return None
        if len(possible) > 1:
            raise RuntimeError(
                "Found more than one possible camera_glas_gfm for image segment level %d"
                % lv
            )
        d = possible[0].des
        return d.camera

    def _camera_glas_gfm_set(iseg, cam):
        f = iseg.nitf_file
        if f is None:
            raise RuntimeError(
                "Need to add NitfImageSegment to a NitfFile before we can add a camera_glas_gfm to it"
            )
        d = f.find_des_by_uuid(cam.id)
        if d is None:
            d = DesCSSFAB()
            d.camera = cam
            d.generate_uuid_if_needed()
            cam.id = d.id
            f.des_segment.append(NitfDesSegment(d))
        d.add_display_level(iseg.idlvl)

    def _orbit_des(iseg):
        if iseg.pos_csephb is None or iseg.att_csattb is None:
            return None
        return OrbitDes(iseg.pos_csephb, iseg.att_csattb)

    def _orbit_des_set(iseg, orb):
        iseg.pos_csephb = orb.pos_csephb
        iseg.att_csattb = orb.att_csattb

    def _find_des_by_uuid(f, id):
        """Find the DES that has a ID with the given value. This should be a
        unique value, so we should have at most one. Return None if we
        don't find it.
        """

        if id == "":
            return None
        for dseg in f.des_segment:
            if hasattr(dseg.des, "id") and dseg.des.id == id:
                return dseg.des
        return None

    def _pos_csepha(iseg):
        """Older TRE version of ephemeris. This is used by WV-2, so we
        want to support reading this"""
        t = iseg.find_one_tre("CSEPHA")
        if t is None:
            return None
        return "hi there"

    def _pos_csepha_set(iseg, pos_csepha):
        raise NotImplementedError("Older TRE, we don't currently support writing this.")

    NitfFile.find_des_by_uuid = _find_des_by_uuid
    NitfImageSegment.pos_csephb = property(_pos_csephb, _pos_csephb_set)
    NitfImageSegment.pos_csepha = property(_pos_csepha, _pos_csepha_set)
    NitfImageSegment.att_csattb = property(_att_csattb, _att_csattb_set)
    NitfImageSegment.orbit_des = property(_orbit_des, _orbit_des_set)
    NitfImageSegment.camera_glas_gfm = property(_camera_glas_gfm, _camera_glas_gfm_set)
    DesCSSFAB.camera = property(_camera_cssfab, _camera_cssfab_set)

__all__ = []
