from .nitf_tre import *
from .nitf_tre_sensrb import *
from test_support import *
import io, six

def turn_everything_off(t):
    '''Initialize everything to off, so we can selectively turn things
    on in a unit test.'''
    t.general_data='N'
    t.sensor_array_data='N'
    t.sensor_calibration_data='N'
    t.image_formation_data='N'
    t.attitude_euler_angles='N'
    t.attitude_unit_vectors='N'
    t.attitude_quaternion='N'
    t.sensor_velocity_data='N'
    t.point_set_data=0
    t.time_stamped_data_sets=0
    t.pixel_referenced_data_sets=0
    t.uncertainty_data=0
    t.additional_parameter_data=0

def fill_in_5_and_6(t):
    '''Section 5 and 6 of tre are always required, so have some default
    values to fill in here'''
    # section 5 stuff is always present
    t.reference_time = 0
    t.reference_row=0
    t.reference_column=0
    # section 6 stuff is always present
    t.latitude_or_x = 10
    t.longitude_or_y = 20
    t.altitude_or_z = 100000
    t.sensor_x_offset=0
    t.sensor_y_offset=0
    t.sensor_z_offset=0

def test_tre_sensrb_empty():
    '''Basic test of sensrb where we have everything turned off, and only
    fill in the always on sections'''
    t = TreSENSRB()
    turn_everything_off(t)
    fill_in_5_and_6(t)
    # Section 5 and 6 always need to be filled in
    fill_in_5_and_6(t)
    fh = six.BytesIO()
    t.write_to_file(fh)
    assert fh.getvalue() == b'SENSRB00106NNNN00000000000000000000000000000000000001000000000002000000100000000000000000000000000000NNNN000000000000'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreSENSRB()
    t2.read_from_file(fh2)
    assert t2.reference_time  ==  0
    assert t2.reference_row == 0
    assert t2.reference_column == 0
    assert t2.latitude_or_x  ==  10
    assert t2.longitude_or_y  ==  20
    assert t2.altitude_or_z  ==  100000
    assert t2.sensor_x_offset == 0
    assert t2.sensor_y_offset == 0
    assert t2.sensor_z_offset == 0
    
def test_tre_sensrb_general_data():
    '''Basic test of sensrb, general_data part only'''
    t = TreSENSRB()
    # Turn on general_data, and off everything else
    turn_everything_off(t)
    t.general_data='Y'

    # Set some values
    t.sensor="My sensor"
    t.sensor_uri="blah"
    t.platform="My platform"
    t.platform_uri="blah"
    t.operation_domain="blah"
    t.content_level=1
    t.geodetic_system="WGS84"
    t.geodetic_type="G"
    t.elevation_datum="HAE"
    t.length_unit="SI"
    t.angular_unit="D"
    t.start_date=20170427
    t.start_time=0
    t.end_date=20170427
    t.end_time=1
    t.generation_count=0
    t.generation_date=20170427
    t.generation_time=0
    # Section 5 and 6 always need to be filled in
    fill_in_5_and_6(t)
    fh = six.BytesIO()
    t.write_to_file(fh)
    #print(fh.getvalue())
    assert fh.getvalue() == b'SENSRB00309YMy sensor                blah                            My platform              blah                            blah      1WGS84GHAESID  2017042700000000000000201704270000000000000100201704270000000000NNN00000000000000000000000000000000000001000000000002000000100000000000000000000000000000NNNN000000000000'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreSENSRB()
    t2.read_from_file(fh2)
    assert t2.sensor_array_data == 'N'
    assert t2.sensor_calibration_data == 'N'
    assert t2.image_formation_data == 'N'
    assert t2.attitude_euler_angles == 'N'
    assert t2.attitude_unit_vectors == 'N'
    assert t2.attitude_quaternion == 'N'
    assert t2.sensor_velocity_data == 'N'
    assert t2.point_set_data == 0
    assert t2.time_stamped_data_sets == 0
    assert t2.pixel_referenced_data_sets == 0
    assert t2.uncertainty_data == 0
    assert t2.additional_parameter_data == 0
    assert t2.sensor == "My sensor"
    assert t2.sensor_uri == "blah"
    assert t2.platform == "My platform"
    assert t2.platform_uri == "blah"
    assert t2.operation_domain == "blah"
    assert t2.content_level == 1
    assert t2.geodetic_system == "WGS84"
    assert t2.geodetic_type == "G"
    assert t2.elevation_datum == "HAE"
    assert t2.length_unit == "SI"
    assert t2.angular_unit == "D"
    assert t2.start_date == 20170427
    assert t2.start_time == 0
    assert t2.end_date == 20170427
    assert t2.end_time == 1
    assert t2.generation_count == 0
    assert t2.generation_date == 20170427
    assert t2.generation_time == 0

def test_tre_sensrb_additional_parameter():
    t = TreSENSRB()
    # Turn everything off
    turn_everything_off(t)
    # Fill in stuff always needed
    fill_in_5_and_6(t)
    t.additional_parameter_data = 2
    t.parameter_name[0]="parm 1"
    t.parameter_name[1]="parm 2"
    t.parameter_size[0]=1
    t.parameter_size[1]=2
    t.parameter_count[0] = 1
    t.parameter_count[1] = 3
    t.parameter_value[0,0] = b'0'
    t.parameter_value[1,0] = b'00'
    t.parameter_value[1,1] = b'01'
    t.parameter_value[1,2] = b'02'
    fh = six.BytesIO()
    t.write_to_file(fh)
    #print(fh.getvalue())
    assert fh.getvalue() == b'SENSRB00177NNNN00000000000000000000000000000000000001000000000002000000100000000000000000000000000000NNNN000000000002parm 1                   00100010parm 2                   0020003000102'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreSENSRB()
    t2.read_from_file(fh2)
    assert t2.additional_parameter_data  ==  2
    assert list(t.parameter_name) == ["parm 1", "parm 2"]
    assert list(t.parameter_size) == [1,2]
    assert list(t.parameter_count) == [1,3]
    assert t2.parameter_value[0,0]  ==  b'0'
    assert t2.parameter_value[1,0]  ==  b'00'
    assert t2.parameter_value[1,1]  ==  b'01'
    assert t2.parameter_value[1,2]  ==  b'02'
    
def test_tre_sensrb_sensor_array():
    '''Basic test of sensrb, sensor_array_data part only'''

    t = TreSENSRB()
    # Turn everything off
    turn_everything_off(t)
    # Fill in stuff always needed
    fill_in_5_and_6(t)

    #Fill in sensor array data
    t.sensor_array_data = "Y"
    t.detection = "some detection"
    t.row_detectors = "00001000"
    t.column_detectors = "00001000"
    t.row_metric = "00001000"
    t.column_metric = "00001000"
    t.focal_length = "00009999"
    t.row_fov = "00000130"
    t.column_fov = "00000090"
    t.calibrated = "Y"

    fh = six.BytesIO()
    t.write_to_file(fh)
    #print(fh.getvalue())
    assert fh.getvalue() == b'SENSRB00183NYsome detection      00001000000010000000100000001000000099990000013000000090YNN00000000000000000000000000000000000001000000000002000000100000000000000000000000000000NNNN000000000000'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreSENSRB()
    t2.read_from_file(fh2)
    assert t2.detection == "some detection"
    assert t2.row_detectors == 1000
    assert t2.column_detectors == 1000
    assert t2.row_metric == 1000
    assert t2.column_metric == 1000
    assert t2.focal_length == 9999
    assert t2.row_fov == 130
    assert t2.column_fov == 90
    assert t2.calibrated == "Y"
    
# Tests for other parts
