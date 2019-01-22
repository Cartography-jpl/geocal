from geocal_swig import (nitf_to_quaternion, quaternion_to_nitf, Ecr,
                         QuaternionOrbitData, Time, SensrbCamera,
                         Quaternion_double, FrameCoordinate)
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
# Note we often have the orbit data and camera attitudes
# separate. SENSRB doesn't actually support this, instead
# the "orbit data" should include both the platform attitude
# and the orientation of the camera relative to the platform.
#
# Note
# ---------------------------------------------------------
if(have_pynitf):
    def _orbit_data_sensrb_get(self):
        t = self.find_exactly_one_tre("SENSRB")
        if(not(t.general_data == "Y" and
               t.geodetic_system == "WGS84" and
               t.geodetic_type == "C" and
               t.length_unit == "SI" and
               t.sensor_velocity_data == "Y" and
               t.attitude_quaternion == "Y")):
            raise RuntimeError("Don't know how to interpret TRE to get OrbitData")
        p = Ecr(t.latitude_or_x + t.sensor_x_offset,
                t.longitude_or_y + t.sensor_y_offset,
                t.altitude_or_z + t.sensor_z_offset)
        v = [t.velocity_north_or_x, t.velocity_east_or_y,
             t.velocity_down_or_z]
        q = nitf_to_quaternion([t.attitude_q1, t.attitude_q2, t.attitude_q3,
                                t.attitude_q4])
        tms = str(t.start_date)
        tm_day_start = Time.parse_time("%s-%s-%sT00:00:00Z" %
                                       (tms[0:4], tms[4:6], tms[6:8]))
        tm = tm_day_start + t.start_time
        return QuaternionOrbitData(tm, p, v, q)

    def _orbit_data_sensrb_set(self, od):
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
        t.attitude_euler_angles = "N"
        t.attitude_unit_vectors = "N"
        t.attitude_quaternion = "Y"
        att = quaternion_to_nitf(od.sc_to_cf)
        t.attitude_q1 = att[0]
        t.attitude_q2 = att[1]
        t.attitude_q3 = att[2]
        t.attitude_q4 = att[3]

    NitfImageSegment.orbit_data_sensrb = property(_orbit_data_sensrb_get,
                                                  _orbit_data_sensrb_set)

    def _camera_sensrb_get(self):
        t = self.find_exactly_one_tre("SENSRB")
        if(not(t.sensor_array_data == "Y" and
               t.sensor_calibration_data == "Y" and
               t.length_unit == "SI" and
               t.calibration_unit == "px" and
               t.angular_unit == "DEG")):
            raise RuntimeError("Don't know how to interpret TRE to get Camera")
        return SensrbCamera(Quaternion_double(1,0,0,0),
		            t.radial_distort_1, t.radial_distort_2,
                            t.radial_distort_3, t.decent_distort_1,
                            t.decent_distort_2, t.affinity_distort_1,
                            t.affinity_distort_2, 
		            t.row_detectors, t.column_detectors,
                            t.row_metric / 10.0, t.column_metric / 10.0,
		            t.focal_length / 10.0,
                            FrameCoordinate(t.principal_point_offset_x +
                                            t.row_detectors / 2.0,
                                            t.principal_point_offset_y +
                                            t.column_detectors / 2.0))

    def _camera_sensrb_set(self, cam):
        t = self.find_one_tre("SENSRB")
        if(t is None):
            t = TreSENSRB()
            self.tre_list.append(t)
        t.sensor_array_data = "Y"
        t.calibrated = "Y"
        t.sensor_calibration_data = "Y"
        t.length_unit = "SI"
        t.angular_unit = "DEG"
        t.calibration_unit = "px"
        t.detection = cam.detection_type
        t.row_detectors = cam.number_line(0)
        t.column_detectors = cam.number_sample(0)
        # 10.0 is because camera has line pitch in mm, but sensrb
        # records this in cm.
        t.row_metric = cam.line_pitch * 10.0
        t.column_metric = cam.sample_pitch * 10.0
        t.focal_length = cam.focal_length * 10.0
        # Need to fill this in
        t.row_fov = -1
        t.column_fov = -1
        t.principal_point_offset_x = cam.principal_point(0).line - cam.number_line(0) / 2.0
        t.principal_point_offset_y = cam.principal_point(0).sample - cam.number_sample(0) / 2.0
        t.radial_distort_1 = cam.k1
        t.radial_distort_2 = cam.k2
        t.radial_distort_3 = cam.k3
        t.decent_distort_1 = cam.p1
        t.decent_distort_2 = cam.p2
        t.affinity_distort_1 = cam.b1
        t.affinity_distort_2 = cam.b2
        # Need to fill this in
        t.radial_distort_limit = -1
        t.calibration_date = cam.calibration_date
        # Need to add handling of quaterion
        
    NitfImageSegment.camera_sensrb = property(_camera_sensrb_get,
                                              _camera_sensrb_set)
    

if(have_pynitf):
    __all__ = []
else:
    __all__ = []


