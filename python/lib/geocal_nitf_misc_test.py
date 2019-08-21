import geocal_swig
from test_support import *
from geocal.geocal_nitf_misc import *

def test_time_conversion():
    '''Tests going from geocal Time to and from various NITF representations'''
    t = geocal_swig.Time.parse_time("2001-01-02T11:12:03.45678")
    date, second = geocal_time_to_nitf_date_second_field(t)
    assert date == "20010102"
    assert_almost_equal(second, 40323.45678)
    t2 = nitf_date_second_field_to_geocal_time(date, second)
    assert str(t) == str(t2)
    
