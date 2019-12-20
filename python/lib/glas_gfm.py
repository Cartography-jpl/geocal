from geocal_swig import (PosCsephb, AttCsattb, OrbitDes,
                         ConstantSpacingTimeTable, SimpleCamera,
                         QuaternionCamera, FrameCoordinate,
                         SimpleDem, IpiImageGroundConnection, Ipi,
                         Quaternion_double, OrbitDataImageGroundConnection)
from .geocal_nitf_misc import (nitf_date_second_field_to_geocal_time,
                               geocal_time_to_nitf_date_second_field,
                               geocal_time_to_timestamp,
                               timestamp_to_geocal_time)
try:
    import pynitf
    from .geocal_nitf_des import(DesCSEPHB_geocal, DesCSATTB_geocal)
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
                orb = igc.ipi.orbit
                cam = igc.ipi.camera
                ttable = igc.ipi.time_table
                t.day_first_line_image, t.time_first_line_image = geocal_time_to_nitf_date_second_field(ttable.min_time)
                t.time_image_duration = ttable.max_time - ttable.min_time
            elif(isinstance(igc, OrbitDataImageGroundConnection)):
                 t.sensor_type = "F"
                 orb = igc.orbit
                 cam = igc.camera
                 t.time_stamp_loc = 0
                 t.reference_frame_num = 1
                 # Only 1 frame. We may need to modify this
                 t.base_timestamp = geocal_time_to_timestamp(igc.orbit_data.time)
                 t.dt_multiplier = 1
                 t.dt_size = 1
                 t.number_frames = 1
                 t.number_dt = 1
                 t.dt[0] = 0
            else:
                raise RuntimeError("Unrecognized ImageGroundConnection type")
            # ground_ref_point
            t.num_lines = igc.number_line
            t.num_samples = igc.number_sample
            t.atm_refr_flag = 1
            t.vel_aber_flag = 1
            res.tre_csexrb = t
            iseg.tre_list.append(res.tre_csexrb)
            f = iseg.nitf_file

            # Get position, attitude and camera DES if already in file,
            # or create 
            iseg.orbit_des = orb
            iseg.camera_glas_gfm = cam
            res.des.append(f.find_des_by_uuid(orb.pos_csephb.id))
            res.des.append(f.find_des_by_uuid(orb.att_csattb.id))
            res.des.append(f.find_des_by_uuid(cam.id))
            
            # Fake CSCSDB. We copy over one we got from the RIP. This will
            # be replaced with real covariance. I have no idea what most of
            # this is, we are just copying stuff verbatim
            cscsdb = pynitf.DesCSCSDB()
            cscsdb.cov_version_date = "20050407"
            cscsdb.core_sets = 0
            cscsdb.io_cal_ap        = 0
            cscsdb.ts_cal_ap        = 0
            cscsdb.ue_flag          = 0
            # Something wrong with ue for GFM. I have no understanding
            # of this, so we'll need to come back to the covariance stuff
            #cscsdb.ue_flag          = 1
            #cscsdb.line_dimension   = 3
            #cscsdb.sample_dimension = 3
            #cscsdb.urr[0, 0] = 4.0
            #cscsdb.urr[0, 1] = 4.0
            #cscsdb.urr[0, 2] = 4.0
            #cscsdb.urr[1, 0] = 4.0
            #cscsdb.urr[1, 1] = 4.0
            #cscsdb.urr[1, 2] = 4.0
            #cscsdb.urr[2, 0] = 4.0
            #cscsdb.urr[2, 1] = 4.0
            #cscsdb.urr[2, 2] = 4.0
            #cscsdb.urc[0, 0] = 0.0
            #cscsdb.urc[0, 1] = 0.0
            #cscsdb.urc[0, 2] = 0.0
            #cscsdb.urc[1, 0] = 0.0
            #cscsdb.urc[1, 1] = 0.0
            #cscsdb.urc[1, 2] = 0.0
            #cscsdb.urc[2, 0] = 0.0
            #cscsdb.urc[2, 1] = 0.0
            #cscsdb.urc[2, 2] = 0.0
            #cscsdb.ucc[0, 0] = 4.0
            #cscsdb.ucc[0, 1] = 4.0
            #cscsdb.ucc[0, 2] = 4.0
            #cscsdb.ucc[1, 0] = 4.0
            #cscsdb.ucc[1, 1] = 4.0
            #cscsdb.ucc[1, 2] = 4.0
            #cscsdb.ucc[2, 0] = 4.0
            #cscsdb.ucc[2, 1] = 4.0
            #cscsdb.ucc[2, 2] = 4.0
            #cscsdb.line_spdcf       = 9
            #cscsdb.sample_spdcf     = 10
            cscsdb.spdcf_flag       = 0
            cscsdb.direct_covariance_flag = 0
            # Use fixed ID for being able to fake this.
            cscsdb.id = "756a9c88-c536-11e9-b453-782bcb6d0983"
            d = f.find_des_by_uuid(cscsdb.id)
            if(d is None):
                f.des_segment.append(pynitf.NitfDesSegment(des=cscsdb))
                d = cscsdb
            res.des.append(d)

            # Connect the various pieces together with associations
            for d in res.des:
                d.add_display_level(iseg.idlvl)
                d.add_assoc_elem(res.tre_csexrb)
                res.tre_csexrb.add_assoc_elem(d)
                for d2 in res.des:
                    if(d2.id != d.id):
                        d.add_assoc_elem(d2)
            return res

        def igc(self, include_image = False, dem = SimpleDem()):
            '''Return ImageGroundConnection for GLAS/GFM.
            You can either have the raster_image or raster_image_multi_band
            from the NitfFile included in the igc or not, based on the 
            include_image.'''
            orb = self.orbit
            cam = self.camera
            if(cam.sensor_type == "S"):
                tt = self.time_table
                ipi = Ipi(orb, cam, 0, tt.min_time, tt.max_time, tt)
                igc = IpiImageGroundConnection(ipi, dem, None,
                                               self.iseg.iid1)
            elif(cam.sensor_type == "F"):
                if(self.tre_csexrb.time_stamp_loc != 0):
                    raise RuntimeError("We don't handle having the time stamps in a MTIMSA TRE")
                t = timestamp_to_geocal_time(self.tre_csexrb.base_timestamp) + self.tre_csexrb.dt_multiplier * 1e-9 * self.tre_csexrb.dt[0]
                igc = OrbitDataImageGroundConnection(orb, t, cam, dem, None,
                                                     self.iseg.iid1)
            else:
                raise RuntimeError("Unrecognized camera sensor type")
            # Add some useful metadata
            igc.platform_id = self.tre_csexrb.platform_id
            igc.payload_id = self.tre_csexrb.payload_id
            igc.sensor_id = self.tre_csexrb.sensor_id
            igc.dyanmic_range = self.tre_csexrb.dynamic_range
            if(include_image):
                mb = self.iseg.raster_image_multi_band
                if(mb.number_band == 1):
                    igc.image = self.iseg.raster_image
                else:
                    igc.image_multi_band = mb
            return igc

        @property
        def camera(self):
            '''Return the camera'''
            d = self.find_one_des("CSSFAB")
            if(d is None):
                return None
            return d.camera

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
    
