from geocal_swig import (PosCsephb, AttCsattb, OrbitDes,
                         ConstantSpacingTimeTable, SimpleCamera, Ecr,
                         QuaternionCamera, FrameCoordinate,
                         ImageCoordinate, RefractionMsp, NoVelocityAberration,
                         VelocityAberrationExact, Camera,
                         ConstantSpacingFrameletTimeTable,
                         GlasGfmCamera,Time,ScLookVector,
                         SimpleDem, IpiImageGroundConnection, Ipi,
                         ScLookVector,
                         Quaternion_double, OrbitDataImageGroundConnection)
from .geocal_nitf_misc import (nitf_date_second_field_to_geocal_time,
                               geocal_time_to_nitf_date_second_field,
                               geocal_time_to_timestamp,
                               timestamp_to_geocal_time)
from .glas_gfm_covariance import GlasGfmCovariance
import time
import math
import uuid
import io
import textwrap
import scipy
import numpy as np
try:
    import pynitf
    from .geocal_nitf_des import(DesCSEPHB, DesCSATTB)
    have_pynitf = True
except ImportError:
    have_pynitf = False
import numpy as np

class GlasGfm(object):
    '''The GLAS/GFM structure in a NITF file crosses multiple TRE/DESs.
    This class collects all this together so we can treat this as
    "one thing"'''
    def __init__(self, iseg, tre_csexrb):
        self.iseg = iseg
        self.tre_csexrb = tre_csexrb
        self.naif_code = Ecr.EARTH_NAIF_CODE
        if(self.tre_csexrb is not None and self.iseg.nitf_file is not None):
            self.des = self.tre_csexrb.assoc_elem(self.iseg.nitf_file)
        else:
            self.des = []
            
    def create(iseg, igc, refraction_flag=None,
               velocity_aberration_flag=None):
        '''Create a GlasGfm from a igc, and put into the given
        NitfImageSegment. We add DES data to the file f as needed.
        
        For right now, we leave all the optional stuff in TreCSEXRB empty.
        We'll probably want to add a bit to fill this in, I think a fair
        amount can be calculated from the igc.

        We do need a bit of extra metadata. These should just be added
        the ImageGroundConnection as python attributes. Perhaps we can 
        create a special Python/C++ class, but for now we just "know"
        to add these attributes. Look a the unit test for an example.

        It isn't clear exactly how to handle the GlasGfmCovariance. This
        sort of belongs in the igc, but we don't really have the 
        infrastructure in place for that. For now, if 
        igc.glas_gfm_covariance exists then we use that to populate the 
        covariance. Otherwise we create an empty one that can then be
        populated.

        We set the refraction flag based on if the igc has Refraction or
        not. Alternatively, you can pass the refraction_flag to force setting
        this. Similarly for velocity_aberration.
        '''
        res = GlasGfm(iseg, None)
        t = pynitf.TreCSEXRB()
        t.generate_uuid_if_needed()
        t.platform_id = igc.platform_id
        t.payload_id = igc.payload_id
        t.sensor_id = igc.sensor_id
        if(isinstance(igc, IpiImageGroundConnection)):
            orb = igc.ipi.orbit
            cam = igc.ipi.camera
            ttable = igc.ipi.time_table
            if(cam.number_line(0) > 1):
                t.sensor_type = "P"
            else:
                t.sensor_type = "S"
            min_time = ttable.time(ImageCoordinate(0,0))[0]
            max_time = ttable.time(ImageCoordinate(ttable.max_line, 0))[0]
            t.day_first_line_image, t.time_first_line_image = geocal_time_to_nitf_date_second_field(min_time)
            t.time_image_duration = max_time - min_time
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
            t.rolling_shutter_flag = 0
            t.dt[0] = 0
        else:
            raise RuntimeError("Unrecognized ImageGroundConnection type")
        # ground_ref_point
        t.num_lines = igc.number_line
        t.num_samples = igc.number_sample
        if(refraction_flag is not None):
            t.atm_refr_flag = 1 if refraction_flag else 0
        else:
            if(hasattr(igc, "refraction") and
               igc.refraction is not None):
                t.atm_refr_flag = 1
            else:
                t.atm_refr_flag = 0
        if(velocity_aberration_flag is not None):
            t.vel_aber_flag = 1 if velocity_aberration_flag else 0
        else:
            vabb = None
            if(hasattr(igc, "velocity_aberration")):
                vabb = igc.velocity_aberration
            if(vabb is not None and
               isinstance(igc.velocity_aberration, NoVelocityAberration)):
                t.vel_aber_flag = 0
            else:
                t.vel_aber_flag = 1
        res.tre_csexrb = t
        iseg.tre_list.append(res.tre_csexrb)
        f = iseg.nitf_file
        
        # If we have glas_gfm_covariance use that, otherwise create a
        # empty one
        if(hasattr(igc, "glas_gfm_covariance") and
           igc.glas_gfm_covariance is not None):
            cov = igc.glas_gfm_covariance
        else:
            cov = GlasGfmCovariance()
            
        # Get position, attitude, camera and cov DES if already in file,
        # or create.
        
        # Check if we are too close to the maximum user header size, if
        # we are then force a new object to be created.
        for t1 in (orb.pos_csephb, orb.att_csattb, cam, cov):
            t2 = f.find_des_by_uuid(t1.id)
            # We has originally checked t2.user_subheader_size > 9500,
            # but this turns out to be a bit slow. Sufficient just to
            # look at num_assoc_elem, the cuttoff here is about the same
            # as the user_subheader_size. We can switch back if needed
            # for some reason, we are just trying to make sure
            # t2.user_subheader_size fits into 9999, which is the size
            # of the desshl in the nitf DES subheader.
            if(t2 and t2.user_subheader.num_assoc_elem > 240):
                t1.id = ""
        iseg.orbit_des = orb
        iseg.camera_glas_gfm = cam
        iseg.glas_gfm_covariance = cov
        res.des.append(f.find_des_by_uuid(orb.pos_csephb.id))
        res.des.append(f.find_des_by_uuid(orb.att_csattb.id))
        res.des.append(f.find_des_by_uuid(cam.id))
        res.des.append(f.find_des_by_uuid(cov.id))

        # Connect the various pieces together with associations
        for d in res.des:
            d.add_display_level(iseg.idlvl)
            d.add_assoc_elem(res.tre_csexrb)
            res.tre_csexrb.add_assoc_elem(d)
            for d2 in res.des:
                if(d2.id != d.id):
                    d.add_assoc_elem(d2)
        return res

    def igc(self, include_image = False, dem = SimpleDem(),
            naif_code = None, velocity_aberration_exact = False):
        '''Return ImageGroundConnection for GLAS/GFM.
        You can either have the raster_image or raster_image_multi_band
        from the NitfFile included in the igc or not, based on the 
        include_image.
        
        By default, we use the velocity aberration first order model,
        good usually for submeter accuracy. This ignores earth rotation
        and only include the first order term in velocity. You can
        select the exact model if desired, it is more accurate but 
        slower.
        '''
        if(naif_code is not None):
            self.naif_code = naif_code
        orb = self.orbit
        cam = self.camera
        if(self.tre_csexrb.sensor_type in ("S", "P")):
            tt = self.time_table
            ipi = Ipi(orb, cam, 0, tt.min_time, tt.max_time, tt,
                      self.refraction,
                      self.velocity_aberration(velocity_aberration_exact))
            igc = IpiImageGroundConnection(ipi, dem, None,
                                           self.iseg.iid1)
        elif(self.tre_csexrb.sensor_type == "F"):
            if(self.tre_csexrb.time_stamp_loc != 0):
                raise RuntimeError("We don't handle having the time stamps in a MTIMSA TRE")
            t = timestamp_to_geocal_time(self.tre_csexrb.base_timestamp) + self.tre_csexrb.dt_multiplier * 1e-9 * self.tre_csexrb.dt[0]
            igc = OrbitDataImageGroundConnection(orb, t, cam, dem, None,
                     self.iseg.iid1,
                     self.refraction,
                     self.velocity_aberration(velocity_aberration_exact))
        else:
            raise RuntimeError("Unrecognized tre_csexrb sensor type")
        # Add some useful metadata
        igc.platform_id = self.tre_csexrb.platform_id
        igc.payload_id = self.tre_csexrb.payload_id
        igc.sensor_id = self.tre_csexrb.sensor_id
        igc.dyanmic_range = self.tre_csexrb.dynamic_range
        igc.glas_gfm_covariance = self.glas_gfm_covariance
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
    def refraction(self):
        '''Return the Refraction object if we are including refraction, None
        otherwise'''
        if(self.tre_csexrb.atm_refr_flag == 0):
            return None
        return RefractionMsp(self.camera.band_wavelength)
    
    def velocity_aberration(self, velocity_aberration_exact = False):
        '''Return the NoVelocityAberration object if we are not including
        velocity_aberration, VelocityAberrationExact or None otherwise'''
        # TODO Way to include full VelocityAberration?
        if(self.tre_csexrb.vel_aber_flag == 0):
            return NoVelocityAberration()
        if(velocity_aberration_exact):
            return VelocityAberrationExact()
        return None

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
        if self.tre_csexrb.sensor_type == "S":
            return ConstantSpacingTimeTable(tstart, tend, (tend - tstart) / (nline-1))
        elif(self.tre_csexrb.sensor_type == "P"):
            nline_framelet = self.camera.number_line(0)
            nframelet = int(math.floor(nline / nline_framelet))
            return ConstantSpacingFrameletTimeTable(tstart, tend,
                       nline_framelet, (tend - tstart) / (nframelet - 1))
        else:
            raise RuntimeError("Unrecognized tre_csexrb sensor type")
            
    def find_one_des(self, desid):
        res = [d for d in self.des if d.desid == desid]
        if(len(res) == 0):
            return None
        if(len(res) > 1):
            raise RuntimeError("Found more than one possible DES %s in GLAS/GFM" % desid)
        return res[0]

    @property
    def glas_gfm_covariance(self):
        '''Return the GlasGfmCovariance object associated with this GlasGfm'''
        d = self.find_one_des("CSCSDB")
        if(d is None):
            return None
        return GlasGfmCovariance.read_des(d)
    
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
        return OrbitDes(self.pos_csephb, self.att_csattb, self.naif_code)

    def __str__(self):
        res = "GlasGfm:\n"
        res += f"   Platform id:    {self.tre_csexrb.platform_id}\n"
        res += f"   Payload id:     {self.tre_csexrb.payload_id}\n"
        res += f"   Sensor id:      {self.tre_csexrb.sensor_id}\n"
        res += f"   Dynamic range:  {self.tre_csexrb.dynamic_range}\n"
        res += "   Orbit:\n"
        res += textwrap.indent(str(self.orbit), "      ") + "\n"
        res += "   Camera:\n"
        res += textwrap.indent(str(self.camera), "      ") + "\n"
        if(self.tre_csexrb.sensor_type == "F"):
            if(self.tre_csexrb.time_stamp_loc != 0):
                res += "   Timestamp in MTIMSA, which we can't process\n"
            else:
                t = timestamp_to_geocal_time(self.tre_csexrb.base_timestamp) + self.tre_csexrb.dt_multiplier * 1e-9 * self.tre_csexrb.dt[0]
                res += f"   Timestamp: {t}\n"
        else:
            res += "   Time table:\n"
            res += textwrap.indent(str(self.time_table), "      ") + "\n"
        res += "   Covariance:\n"
        res += textwrap.indent(str(self.glas_gfm_covariance), "      ") + "\n"
        return res
        
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

if(have_pynitf):
    pynitf.NitfImageSegment.glas_gfm = property(_glas_gfm)
    pynitf.NitfImageSegment.create_glas_gfm = _create_glas_gfm

# Support functions for GlasGfmCamera

# Really minimal camera, but sufficient to pass to GlasGfmCamera to
# fill in the field angle

class _CameraForGlas(Camera):
    def __init__(self, number_sample, lv_arr, focal_length,
                 sample_number_first=0,
                 delta_sample_pair=1):
        self.nsamp = number_sample
        self.focal_length = focal_length
        self.slv = scipy.interpolate.RegularGridInterpolator(
            (range(sample_number_first, number_sample, delta_sample_pair),),
            np.array([lv_arr[:,0] * (focal_length / lv_arr[:,2]),
                      lv_arr[:,1] * (focal_length / lv_arr[:,2]),
                      [focal_length,]* lv_arr.shape[0]]).transpose(),
            bounds_error = False, fill_value=None)
        super().__init__()
        
    def number_line(self, b):
        return 1

    def number_sample(self, b):
        return self.nsamp

    def sc_look_vector(self, fc, b):
        return ScLookVector(*self.slv([fc.sample])[0])

class _CameraForGfm(Camera):
    def __init__(self, number_line,
                 number_sample, lv_arr, focal_length,
                 line_number_first=0,
                 sample_number_first=0,
                 delta_sample_pair=1,
                 delta_line_pair=1):
        self.nline = number_line
        self.nsamp = number_sample
        self.focal_length = focal_length
        flarr = np.empty((lv_arr.shape[0],lv_arr.shape[1]))
        flarr[:,:] = focal_length
        self.slv = scipy.interpolate.RegularGridInterpolator(
            (range(line_number_first, number_line, delta_line_pair),
             range(sample_number_first, number_sample, delta_sample_pair)),
            np.array([lv_arr[:,:,0] * (focal_length / lv_arr[:,:,2]),
                      lv_arr[:,:,1] * (focal_length / lv_arr[:,:,2]),
                      flarr]).transpose(1,2,0),
            bounds_error = False, fill_value=None)
        super().__init__()
        
    def number_line(self, b):
        return self.nline

    def number_sample(self, b):
        return self.nsamp

    def sc_look_vector(self, fc, b):
        return ScLookVector(*self.slv([fc.line, fc.sample])[0])
    
def _create_glas_from_sc_look_vector(cls, sclv_list, focal_length=1,
                   focal_length_time=Time.parse_time("2020-01-01T00:00:00Z"),
                   sample_number_first=0,
                   delta_sample_pair=1,
                   band_type="N", band_wavelength = 1.45):
    '''Create a GLAS camera model. This gets created from a list of 
    ScLookVector values, which should be evenly spaced with the 
    delta_sample_pair spacing starting as sample_number_first.'''
    gcam = cls(1,sample_number_first + delta_sample_pair * len(sclv_list))
    gcam.focal_length = focal_length
    gcam.focal_length_time = focal_length_time
    gcam.sample_number_first = sample_number_first
    gcam.delta_sample_pair = delta_sample_pair
    gcam.band_type = band_type
    gcam.band_wavelength = band_wavelength

    lv_arr = np.array([[sclv.look_vector[0], sclv.look_vector[1],
                         sclv.look_vector[2]] for sclv in sclv_list])
    cfit = _CameraForGlas(gcam.number_sample(0), lv_arr,
                          gcam.focal_length,
                          sample_number_first=sample_number_first,
                          delta_sample_pair=delta_sample_pair)
    gcam.field_alignment_fit(cfit, delta_sample_pair, 0)
    return gcam

def _create_gfm_from_look_vector(cls, lv_arr, focal_length=1,
                   focal_length_time=Time.parse_time("2020-01-01T00:00:00Z"),
                   line_number_first=0,
                   sample_number_first=0,
                   delta_line_pair=1,
                   delta_sample_pair=1,
                   band_type="N", band_wavelength = 1.45):
    '''Create a GFM camera model. This gets created from a array of 
    look vector values, which should be evenly spaced in line index 
    (first one) and sample index (second one) with the 
    delta_sample_pair spacing starting as sample_number_first. 
    lv_arr should be 3d, with shape (nline points, nsample point, 3)'''
    gcam = cls(line_number_first + delta_line_pair * lv_arr.shape[0],
               sample_number_first + delta_sample_pair * lv_arr.shape[1])
    gcam.focal_length = focal_length
    gcam.focal_length_time = focal_length_time
    gcam.first_line_block = [line_number_first,]
    gcam.first_sample_block = [sample_number_first,]
    gcam.delta_line_block = [delta_line_pair,]
    gcam.delta_sample_block = [delta_sample_pair,]
    gcam.band_type = band_type
    gcam.band_wavelength = band_wavelength

    cfit = _CameraForGfm(gcam.number_line(0),
                         gcam.number_sample(0), lv_arr,
                         gcam.focal_length,
                         line_number_first=line_number_first,
                         sample_number_first=sample_number_first,
                         delta_line_pair=delta_line_pair,
                         delta_sample_pair=delta_sample_pair)
    gcam.field_alignment_block(cfit, delta_line_pair, delta_sample_pair, 0)
    return gcam

def _create_glas_from_field_angle(cls, fa_x, fa_y, **keyword):
    '''Variation of create_glas_from_sc_look_vector where we instead
    take the field angles in degrees.'''
    sclv_list = [ScLookVector.create_from_field_angle(fa_x[i], fa_y[i])
                 for i in range(len(fa_x))]
    return cls.create_glas_from_sc_look_vector(sclv_list, **keyword)
                                     
GlasGfmCamera.create_glas_from_sc_look_vector = classmethod(_create_glas_from_sc_look_vector)
GlasGfmCamera.create_glas_from_field_angle = classmethod(_create_glas_from_field_angle)
GlasGfmCamera.create_gfm_from_look_vector = classmethod(_create_gfm_from_look_vector)

__all__ = ["GlasGfm",]
    
