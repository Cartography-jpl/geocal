from geocal_swig import (PosCsephb, AttCsattb, OrbitDes,
                         ConstantSpacingTimeTable, SimpleCamera,
                         QuaternionCamera, FrameCoordinate,
                         SimpleDem, IpiImageGroundConnection, Ipi,
                         Quaternion_double)
from .geocal_nitf_misc import (nitf_date_second_field_to_geocal_time,
                               geocal_time_to_nitf_date_second_field)
from .geocal_nitf_des import(DesCSEPHB_geocal, DesCSATTB_geocal)
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
        def __init__(self, iseg, tre_csexrb):
            self.iseg = iseg
            self.tre_csexrb = tre_csexrb
            if(self.tre_csexrb is not None and self.iseg.nitf_file is not None):
                self.des = self.tre_csexrb.assoc_elem(self.iseg.nitf_file)
            else:
                self.des = []

        def _find_des(f, id):
            if(id == ""):
                return None
            for dseg in f.des_segment:
                if(hasattr(dseg.des, "id") and
                   dseg.des.id == id):
                    return dseg.des
            return None
            

        def create(iseg, igc):
            '''Create a GlasGfm from a igc, and put into the given
            NitfImageSegment. We add DES data to the file f as needed.

            For right now, we leave all the optional stuff in TreCSEXRB empty.
            We'll probably want to add a bit to fill this in, I think a fair
            amount can be calculated from the igc.

            We do need a bit of extra metadata. These should just be added
            the ImageGroundConnection as python attributes. Perhaps we can 
            create a special Python/C++ class, but for now we just "know"
            to add these attributes. Look a the unit test for an example.
            '''
            res = GlasGfm(iseg, None)
            t = pynitf.TreCSEXRB()
            t.generate_uuid_if_needed()
            t.platform_id = igc.platform_id
            t.payload_id = igc.payload_id
            t.sensor_id = igc.sensor_id
            if(isinstance(igc, IpiImageGroundConnection)):
                t.sensor_type = "S"
            else:
                raise RuntimeError("Unrecognized ImageGroundConnection type")
            # ground_ref_point
            ttable = igc.ipi.time_table
            t.day_first_line_image, t.time_first_line_image = geocal_time_to_nitf_date_second_field(ttable.min_time)
            t.time_image_duration = ttable.max_time - ttable.min_time
            t.num_lines = igc.number_line
            t.num_samples = igc.number_sample
            t.atm_refr_flag = 1
            t.vel_aber_flag = 1
            res.tre_csexrb = t
            iseg.tre_list.append(res.tre_csexrb)
            f = iseg.nitf_file

            # Briefly, fake camera stuff just so we can check everything.
            # We'll fix this shortly.
            d1 = igc.cam_des
            d = GlasGfm._find_des(f, d1.id)
            if(d is None):
                d = d1
                f.des_segment.append(pynitf.NitfDesSegment(des=d))
            res.des.append(d)

            # Get position and attitude DES if already in file, or create
            pos_csephb = igc.ipi.orbit.pos_csephb
            att_csattb = igc.ipi.orbit.att_csattb
            d = GlasGfm._find_des(f, pos_csephb.id)
            if(d is None):
                d = DesCSEPHB_geocal()
                d.pos_csephb = pos_csephb
                d.generate_uuid_if_needed()
                pos_csephb.id = d.id
                f.des_segment.append(pynitf.NitfDesSegment(des=d))
            res.des.append(d)
            d = GlasGfm._find_des(f, att_csattb.id)
            if(d is None):
                d = DesCSATTB_geocal()
                d.att_csattb = att_csattb
                d.generate_uuid_if_needed()
                att_csattb.id = d.id
                f.des_segment.append(pynitf.NitfDesSegment(des=d))
            res.des.append(d)
            
            # Fake CSCSDB. We copy over one we got from the RIP. This will
            # be replaced with real covariance. I have no idea what most of
            # this is, we are just copying stuff verbatim
            cscsdb = pynitf.DesCSCSDB()
            cscsdb.cov_version_date = "20050407"
            cscsdb.core_sets = 1
            cscsdb.ref_frame_position[0] = 3
            cscsdb.ref_frame_attitude[0] = 2
            cscsdb.num_groups[0] = 4
            cscsdb.corr_ref_date[0,0] = "20050407"
            cscsdb.corr_ref_date[0,1] = "20050407"
            cscsdb.corr_ref_date[0,2] = "20050407"
            cscsdb.corr_ref_date[0,3] = "20050407"
            cscsdb.corr_ref_time[0,0] = 72409.889
            cscsdb.corr_ref_time[0,1] = 72409.889
            cscsdb.corr_ref_time[0,2] = 72409.889
            cscsdb.corr_ref_time[0,3] = 72409.889
            cscsdb.num_adj_parm[0, 0] = 3
            cscsdb.num_adj_parm[0, 1] = 2
            cscsdb.num_adj_parm[0, 2] = 1
            cscsdb.num_adj_parm[0, 3] = 1
            cscsdb.adj_parm_id[0,0,0] = 1
            cscsdb.adj_parm_id[0,0,1] = 2
            cscsdb.adj_parm_id[0,0,2] = 3
            cscsdb.adj_parm_id[0,1,0] = 4
            cscsdb.adj_parm_id[0,1,1] = 5
            cscsdb.adj_parm_id[0,2,0] = 6
            cscsdb.adj_parm_id[0,3,0] = 7
            cscsdb.basic_sub_alloc[0, 0] = 1
            cscsdb.basic_sub_alloc[0, 1] = 1
            cscsdb.basic_sub_alloc[0, 2] = 1
            cscsdb.basic_sub_alloc[0, 3] = 1
            cscsdb.errcov_c1[0, 0, 0] = 2500.0
            cscsdb.errcov_c1[0, 0, 1] = 0.0
            cscsdb.errcov_c1[0, 0, 2] = 0.0
            cscsdb.errcov_c1[0, 0, 3] = 2500.0
            cscsdb.errcov_c1[0, 0, 4] = 0.0
            cscsdb.errcov_c1[0, 0, 5] = 2500.0
            cscsdb.errcov_c1[0, 1, 0] = 2.0164e-08
            cscsdb.errcov_c1[0, 1, 1] = 0.0
            cscsdb.errcov_c1[0, 1, 2] = 2.0164e-08
            cscsdb.errcov_c1[0, 2, 0] = 2.0164e-08
            cscsdb.errcov_c1[0, 3, 0] = 0.000196
            cscsdb.basic_pf_flag[0, 0] = 0
            cscsdb.basic_pf_flag[0, 1] = 0
            cscsdb.basic_pf_flag[0, 2] = 0
            cscsdb.basic_pf_flag[0, 3] = 0
            cscsdb.basic_pl_flag[0, 0] = 0
            cscsdb.basic_pl_flag[0, 1] = 0
            cscsdb.basic_pl_flag[0, 2] = 0
            cscsdb.basic_pl_flag[0, 3] = 0
            cscsdb.basic_sr_flag[0, 0] = 1
            cscsdb.basic_sr_flag[0, 1] = 1
            cscsdb.basic_sr_flag[0, 2] = 1
            cscsdb.basic_sr_flag[0, 3] = 1
            cscsdb.basic_sr_spdcf[0, 0] = 1
            cscsdb.basic_sr_spdcf[0, 1] = 3
            cscsdb.basic_sr_spdcf[0, 2] = 5
            cscsdb.basic_sr_spdcf[0, 3] = 7
            cscsdb.post_sub_alloc[0, 0] = 0
            cscsdb.post_sub_alloc[0, 1] = 1
            cscsdb.post_sub_alloc[0, 2] = 0
            cscsdb.post_sub_alloc[0, 3] = 0
            cscsdb.post_start_date[0, 1] = "20050407"
            cscsdb.post_start_time[0, 1] = 26649.889
            cscsdb.post_dt[0, 1]       = 1.875
            cscsdb.num_posts[0, 1]     = 8
            cscsdb.common_posts_cov[0, 1] = 1
            cscsdb.errcov_c2[0, 1, 0] = 7.225e-09
            cscsdb.errcov_c2[0, 1, 1] = 0.0
            cscsdb.errcov_c2[0, 1, 2] = 7.225e-09
            cscsdb.post_interp[0, 1]   = 1
            cscsdb.post_pf_flag[0, 1]  = 0
            cscsdb.post_pl_flag[0, 1]  = 0
            cscsdb.post_sr_flag[0, 1]  = 1
            cscsdb.post_sr_spdcf[0, 1] = 4
            cscsdb.post_corr[0, 1]     = 0
            cscsdb.io_cal_ap        = 0
            cscsdb.ts_cal_ap        = 0
            cscsdb.ue_flag          = 1
            cscsdb.line_dimension   = 3
            cscsdb.sample_dimension = 3
            cscsdb.urr[0, 0] = 4.0
            cscsdb.urr[0, 1] = 4.0
            cscsdb.urr[0, 2] = 4.0
            cscsdb.urr[1, 0] = 4.0
            cscsdb.urr[1, 1] = 4.0
            cscsdb.urr[1, 2] = 4.0
            cscsdb.urr[2, 0] = 4.0
            cscsdb.urr[2, 1] = 4.0
            cscsdb.urr[2, 2] = 4.0
            cscsdb.urc[0, 0] = 0.0
            cscsdb.urc[0, 1] = 0.0
            cscsdb.urc[0, 2] = 0.0
            cscsdb.urc[1, 0] = 0.0
            cscsdb.urc[1, 1] = 0.0
            cscsdb.urc[1, 2] = 0.0
            cscsdb.urc[2, 0] = 0.0
            cscsdb.urc[2, 1] = 0.0
            cscsdb.urc[2, 2] = 0.0
            cscsdb.ucc[0, 0] = 4.0
            cscsdb.ucc[0, 1] = 4.0
            cscsdb.ucc[0, 2] = 4.0
            cscsdb.ucc[1, 0] = 4.0
            cscsdb.ucc[1, 1] = 4.0
            cscsdb.ucc[1, 2] = 4.0
            cscsdb.ucc[2, 0] = 4.0
            cscsdb.ucc[2, 1] = 4.0
            cscsdb.ucc[2, 2] = 4.0
            cscsdb.line_spdcf       = 9
            cscsdb.sample_spdcf     = 10
            cscsdb.spdcf_flag       = 1
            cscsdb.data_to_copy = b'07010101.0001.0000000.00000000.000000+1.00000000000000E+03020101.0001.0000000.00000000.000000+5.00000000000000E+02030101.0001.0000000.00000000.000000+5.00000000000000E+02040101.0001.0000000.00000000.000000+5.00000000000000E+02050101.0001.0000000.00000000.000000+5.00000000000000E+02060101.0001.0000000.00000000.000000+5.00000000000000E+02070101.0001.0000000.00000000.000000+1.00000000000000E+020000000000'
            # Use fixed ID for being able to fake this.
            cscsdb.id = "756a9c88-c536-11e9-b453-782bcb6d0983"
            d = GlasGfm._find_des(f, cscsdb.id)
            if(d is None):
                f.des_segment.append(pynitf.NitfDesSegment(des=cscsdb))
                d = cscsdb
            res.des.append(d)

            # Connect the various pieces together with associations
            print(res.des)
            for d in res.des:
                d.add_display_level(iseg.idlvl)
                print(d)
                d.add_assoc_elem(res.tre_csexrb)
                res.tre_csexrb.add_assoc_elem(d)
                for d2 in res.des:
                    if(d2.id != d.id):
                        d.add_assoc_elem(d2)
            return res

        def igc(self, dem = SimpleDem()):
            '''Return ImageGroundConnection for GLAS/GFM'''
            tt = self.time_table
            orb = self.orbit
            cam = self.camera
            ipi = Ipi(orb, cam, 0, tt.min_time, tt.max_time, tt)
            igc = IpiImageGroundConnection(ipi, dem, None,
                                           self.iseg.iid1)
            # Add some useful metadata
            igc.platform_id = self.tre_csexrb.platform_id
            igc.payload_id = self.tre_csexrb.payload_id
            igc.sensor_id = self.tre_csexrb.sensor_id
            igc.dyanmic_range = self.tre_csexrb.dynamic_range
            return igc

        @property
        def camera(self):
            '''Return the camera'''
            # Placeholder
            cam_des = self.cssfab
            if(cam_des.num_fl_pts != 1):
                raise RuntimeError("We don't currently support time dependent flocal lengths")

            if False:
                return SimpleCamera(0,0,0, cam_des.foc_length[0], -0.00765 / 128,
                                    -0.00765 / 128, 1,
                                    self.tre_csexrb.num_samples)
            return QuaternionCamera(Quaternion_double(1,0,0,0),
                                    1, 256, 0.00765 / 128, 0.00765 / 128,
                                    cam_des.foc_length[0],
                                    FrameCoordinate(0, 128),
                                    QuaternionCamera.LINE_IS_Y,
                                    QuaternionCamera.INCREASE_IS_NEGATIVE,
                                    QuaternionCamera.INCREASE_IS_POSITIVE)

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
            return ConstantSpacingTimeTable(tstart, tend, (tend - tstart) / (nline-1))

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
        return GlasGfm(self, t)

    def _create_glas_gfm(self, igc):
        return GlasGfm.create(self, igc)

    pynitf.NitfImageSegment.glas_gfm = property(_glas_gfm)
    pynitf.NitfImageSegment.create_glas_gfm = _create_glas_gfm

if(have_pynitf):
    __all__ = ["GlasGfm",]
else:
    __all__ = []
    
