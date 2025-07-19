# Various miscellaneous functions to help with GeoCal NITF support
import geocal_swig

try:
    import pynitf

    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False

if have_pynitf:

    def nitf_date_second_field_to_geocal_time(date, second=0):
        """There are a number of ways that time is represented in NITF.
        This version takes the date a CCYYMMDD and seconds from that date.
        We convert this to a geocal time. Depending how it comes, date
        might be a string or an int, second can be a float or a string.
        We convert date to str before processing, and second to a float."""
        d = str(date)
        tstring = "%s-%s-%sT00:00:00Z" % (d[0:4], d[4:6], d[6:8])
        return geocal_swig.Time.parse_time(tstring) + float(second)

    def geocal_time_to_nitf_date_second_field(t):
        """Convert geocal time to date and seconds. date is as a
        string CCYYMMDD, time is a float"""
        ts = str(t)
        date = "%s%s%s" % (ts[0:4], ts[5:7], ts[8:10])
        second = t - nitf_date_second_field_to_geocal_time(date)
        return date, second

    def geocal_time_to_timestamp(t):
        """Convert time to base timestamp string of YYYYMMDDhhmmss.nnnnnnnnn"""
        # Note we don't actually keep time accurate to the nearest nanosecond.
        # If this actually matters, we would need to modify our geocal time
        # class (e.g., use a long double, or something like that)
        ts = str(t)
        return "%s%s%s%s%s%s000" % (
            ts[0:4],
            ts[5:7],
            ts[8:10],
            ts[11:13],
            ts[14:16],
            ts[17:26],
        )

    def timestamp_to_geocal_time(t):
        """Reverse of geocal_time_to_timestamp"""
        return geocal_swig.Time.parse_time(
            "%s-%s-%sT%s:%s:%sZ" % (t[0:4], t[4:6], t[6:8], t[8:10], t[10:12], t[12:])
        )


if have_pynitf:
    __all__ = [
        "nitf_date_second_field_to_geocal_time",
        "geocal_time_to_nitf_date_second_field",
        "geocal_time_to_timestamp",
        "timestamp_to_geocal_time",
    ]
else:
    __all__ = []
