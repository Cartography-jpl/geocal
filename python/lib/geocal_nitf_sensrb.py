from geocal_swig import (nitf_to_quaternion, quaternion_to_nitf, Ecr,
                         QuaternionOrbitData, Time, SensrbCamera,
                         Quaternion_double, FrameCoordinate,
                         AircraftOrbitData, quat_rot, deg_to_rad)
try:
    from pynitf import TreSENSRB, NitfImageSegment
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False

# ---------------------------------------------------------
# Add functions to sensrb TRE to read and write higher level
# objects.
#
# Note that we are are currently assuming a framing camera
# with all the data taken at the same time. We can extend
# this if needed.
# ---------------------------------------------------------
if(have_pynitf):
    def _orbit_data_sensrb_get(self):
        t = self.find_exactly_one_tre("SENSRB")
        if(not(t.general_data == "Y" and
               t.geodetic_system == "WGS84" and
               t.geodetic_type == "C" and
               t.length_unit == "SI" and
               t.sensor_velocity_data == "Y" and
               t.attitude_euler_angles == "Y" and
               t.sensor_angle_model == 1)):
            raise RuntimeError("Don't know how to interpret TRE to get OrbitData")
        p = Ecr(t.latitude_or_x + t.sensor_x_offset,
                t.longitude_or_y + t.sensor_y_offset,
                t.altitude_or_z + t.sensor_z_offset)
        v = [t.velocity_north_or_x, t.velocity_east_or_y,
             t.velocity_down_or_z]
        tms = str(t.start_date)
        tm_day_start = Time.parse_time("%s-%s-%sT00:00:00Z" %
                                       (tms[0:4], tms[4:6], tms[6:8]))
        tm = tm_day_start + t.start_time
        if(t.platform_relative == "Y"):
            # We must have angles if platform_relative is "Y"
            return AircraftOrbitData(tm, p, v, t.platform_roll,
                                     t.platform_pitch, t.platform_heading)
        # Otherwise, they may or may not be present. If not present,
        # arbitrarily set the ypr to all zeros, and we catch all the pointing
        # in the camera. Otherwise, take the supplied data.
        if(t.platform_roll is not None and
           t.platform_pitch is not None and
           t.platform_heading is not None):
            return AircraftOrbitData(tm, p, v, t.platform_roll,
                                     t.platform_pitch, t.platform_heading)
        return AircraftOrbitData(tm, p, v, 0, 0, 0)

    NitfImageSegment.orbit_data_sensrb = property(_orbit_data_sensrb_get)

    def _camera_sensrb_get(self):
        t = self.find_exactly_one_tre("SENSRB")
        if(not(t.sensor_array_data == "Y" and
               t.sensor_calibration_data == "Y" and
               t.length_unit == "SI" and
               t.calibration_unit == "px" and
               t.angular_unit == "DEG" and
               t.attitude_euler_angles == "Y" and
               t.sensor_angle_model == 1)):
            raise RuntimeError("Don't know how to interpret TRE to get Camera")
        q = SensrbCamera.sensor_angle_to_quaternion(t.sensor_angle_1,
                                                    t.sensor_angle_2,
                                                    t.sensor_angle_3)
        if(t.platform_relative == "N"):
            # If platform_relative == "N", then the quaternion has
            # the orbit data body_to_local_north buried in it. Take thi
            # out so we get just the camera frame_to_sc part.
            q = self.orbit_data_sensrb.body_to_local_north.conj() * q

        return SensrbCamera(q,
		            t.radial_distort_1, t.radial_distort_2,
                            t.radial_distort_3, t.decent_distort_1,
                            t.decent_distort_2, t.affinity_distort_1,
                            t.affinity_distort_2, t.radial_distort_limit,
		            t.row_detectors, t.column_detectors,
                            t.row_metric / 10.0 / t.row_detectors,
                            t.column_metric / 10.0 / t.column_detectors,
		            t.focal_length / 10.0,
                            FrameCoordinate(t.principal_point_offset_x +
                                            t.row_detectors / 2.0,
                                            t.principal_point_offset_y +
                                            t.column_detectors / 2.0),
                            t.detection,
                            t.calibration_date)

        
    NitfImageSegment.camera_sensrb = property(_camera_sensrb_get)

    def _orbit_data_and_cam_sensrb_set(self, od, cam):
        '''Set orbit data and camera. We need to do this at the
        same time because they are tied together.'''
        t = self.find_one_tre("SENSRB")
        if(t is None):
            t = TreSENSRB()
            self.tre_list.append(t)
        t.general_data = "Y"
        t.geodetic_system = "WGS84"
        t.geodetic_type = "C"
        t.elevation_datum = "HAE"
        t.length_unit = "SI"
        t.angular_unit = "DEG"
        tm = od.time
        t.start_date = str(tm).split("T")[0].replace("-", "")
        tm_day_start = Time.parse_time(str(tm).split("T")[0] + "T00:00:00Z")
        t.start_time = tm - tm_day_start
        t.end_date = t.start_date
        t.end_time = t.start_time
        t.reference_time = 0
        t.reference_row = 1
        t.reference_column = 1
        # Note that SENSRB can only have position as ECR, there is
        # no way to specify this as ECI
        p = od.position_cf.position
        t.latitude_or_x = p[0]
        t.longitude_or_y = p[1]
        t.altitude_or_z = p[2]
        t.sensrb_x_offset = 0
        t.sensrb_y_offset = 0
        t.sensrb_z_offset = 0
        t.sensor_velocity_data = "Y"
        v = od.velocity_cf
        t.velocity_north_or_x = v[0]
        t.velocity_east_or_y = v[1]
        t.velocity_down_or_z = v[2]
        t.attitude_euler_angles = "Y"
        t.sensor_angle_model = 1
        t.attitude_unit_vectors = "N"
        t.attitude_quaternion = "N"
        od2 = AircraftOrbitData(od)
        # The MSP library ignored the platform_relative and always treats
        # this as "N". So we'll write out our data with the "N" to have
        # data that is compatible. But we still write out the platform
        # relative values, which we can combine with the sensor angles to
        # get separation between camera and orbit.
        t.platform_relative = "N"
        # Angle for heading needs to 0 to 360, while we usually use -180 to 180.
        # Convert if necessary
        if(od2.heading >= 0):
            t.platform_heading = od2.heading
        else:
            t.platform_heading = od2.heading + 360
        t.platform_pitch = od2.pitch
        t.platform_roll = od2.roll

        # Now do the camera part
        t.sensor_array_data = "Y"
        t.calibrated = "Y"
        t.sensor_calibration_data = "Y"
        t.length_unit = "SI"
        t.angular_unit = "DEG"
        t.calibration_unit = "px"
        t.generation_count = 0
        t.generation_date = None
        t.generation_time = None
        t.detection = cam.detection_type
        t.row_detectors = cam.number_line(0)
        t.column_detectors = cam.number_sample(0)
        t.sensor_angle_1, t.sensor_angle_2, t.sensor_angle_3 = SensrbCamera.quaternion_to_sensor_angle(od2.body_to_local_north * cam.frame_to_sc)
        # 10.0 is because camera has line pitch in mm, but sensrb
        # records this in cm. "metric" is the entire size of the row/col
        # CCD, so it includes the number of line/samples
        t.row_metric = cam.line_pitch * 10.0 * t.row_detectors
        t.column_metric = cam.sample_pitch * 10.0 * t.column_detectors
        t.focal_length = cam.focal_length * 10.0
        t.row_fov = None
        t.column_fov = None
        t.principal_point_offset_x = cam.principal_point(0).line - cam.number_line(0) / 2.0
        t.principal_point_offset_y = cam.principal_point(0).sample - cam.number_sample(0) / 2.0
        t.radial_distort_1 = cam.k1
        t.radial_distort_2 = cam.k2
        t.radial_distort_3 = cam.k3
        t.decent_distort_1 = cam.p1
        t.decent_distort_2 = cam.p2
        t.affinity_distort_1 = cam.b1
        t.affinity_distort_2 = cam.b2
        t.radial_distort_limit = cam.radial_distort_limit
        t.calibration_date = cam.calibration_date
        t.image_formation_data = "Y"
        t.method = "Single Frame"
        t.mode = "003"
        t.row_count = cam.number_line(0)
        t.column_count = cam.number_sample(0)
        t.row_set = cam.number_line(0)
        t.column_set = cam.number_sample(0)
        t.row_rate = 0
        t.column_rate = 0
        # This is 1 based, I'm pretty sure
        t.first_pixel_row = 1
        t.first_pixel_column = 1
        t.transform_params = 0
        
    NitfImageSegment.orbit_data_and_camera = _orbit_data_and_cam_sensrb_set
    

if(have_pynitf):
    __all__ = []
else:
    __all__ = []


