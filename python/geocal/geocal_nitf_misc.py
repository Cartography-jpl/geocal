# Various miscellaneous functions to help with GeoCal NITF support
from __future__ import annotations

import geocal_swig


def nitf_date_second_field_to_geocal_time(
    date: str | int, second: float | str = 0
) -> geocal_swig.Time:
    """There are a number of ways that time is represented in NITF.
    This version takes the date a CCYYMMDD and seconds from that date.
    We convert this to a geocal time. Depending how it comes, date
    might be a string or an int, second can be a float or a string.
    We convert date to str before processing, and second to a float."""
    d = str(date)
    tstring = f"{d[0:4]}-{d[4:6]}-{d[6:8]}T00:00:00Z"
    return geocal_swig.Time.parse_time(tstring) + float(second)


def geocal_time_to_nitf_date_second_field(t: geocal_swig.Time) -> tuple[str, float]:
    """Convert geocal time to date and seconds. date is as a
    string CCYYMMDD, time is a float"""
    ts = str(t)
    date = f"{ts[0:4]}{ts[5:7]}{ts[8:10]}"
    second = t - nitf_date_second_field_to_geocal_time(date)
    return date, second


def geocal_time_to_timestamp(t: geocal_swig.Time) -> str:
    """Convert time to base timestamp string of YYYYMMDDhhmmss.nnnnnnnnn"""
    # Note we don't actually keep time accurate to the nearest nanosecond.
    # If this actually matters, we would need to modify our geocal time
    # class (e.g., use a long double, or something like that)
    ts = str(t)
    return f"{ts[0:4]}{ts[5:7]}{ts[8:10]}{ts[11:13]}{ts[14:16]}{ts[17:26]}000"


def timestamp_to_geocal_time(t: str) -> geocal_swig.Time:
    """Reverse of geocal_time_to_timestamp"""
    return geocal_swig.Time.parse_time(
        f"{t[0:4]}-{t[4:6]}-{t[6:8]}T{t[8:10]}:{t[10:12]}:{t[12:]}Z"
    )


__all__ = [
    "geocal_time_to_nitf_date_second_field",
    "geocal_time_to_timestamp",
    "nitf_date_second_field_to_geocal_time",
    "timestamp_to_geocal_time",
]
