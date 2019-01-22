from geocal_swig import (nitf_to_quaternion, quaternion_to_nitf, Ecr,
                         QuaternionOrbitData, Time)
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
    

if(have_pynitf):
    __all__ = []
else:
    __all__ = []


