from builtins import object
import math


#################################################################################
class InstrumentReflectance(object):
    """This is the base class for doing a DN to TOA Reflectance conversion.
    We derive from this class and supply instrument specific information."""

    def __init__(self):
        """Initialization of class"""
        self.year = -999.0
        self.month = -999.0
        self.day = -999.0
        self.hh = -999.0
        self.mm = -999.0
        self.ssdd = -999.0
        self.solarElevation = -999.0
        self.solarZenithAngle = -999.0
        self.solarZenithAngleInRadians = -999.0
        self.solarDist = -999.0

    def pan_band(self):
        """If we have a panchromatic band, this returns the band number. Otherwise, it returns None"""
        return None

    def checkInstrumentPreconditions(self, band):
        """Ensure that everything is ready to do a dn2TOARadiance conversion"""
        raise NotImplementedError("Subclasses should implement this method.")

    def readMetaData(self, filename):
        """Read metadata needed to set up the instrument"""
        raise NotImplementedError("Subclasses should implement this method.")

    def dn2TOARadiance_factor(self, band):
        """Scale factor to convert DN to TOA radiance factor"""
        raise NotImplementedError("Subclasses should implement this method.")

    def dn2TOAReflectance_factor(self, band):
        """Scale factor to convert DN to TOA reflectance factor"""
        if self.solarDist == -999.0 or self.solarZenithAngleInRadians == -999.0:
            raise ValueError("Solar Distance and/or solar angle not set.")

        return (
            self.dn2TOARadiance_factor(band) * math.pow(self.solarDist, 2.0) * math.pi
        ) / (self.esun[band] * math.cos(self.solarZenithAngleInRadians))

    def dn2TOARadiance(self, tile, band):
        """Convert from DN passed in as tile to TOA radiance"""
        return tile * self.dn2TOARadiance_factor(band)

    def dn2TOAReflectance(self, tile, band):
        """Convert DN passe in as tile to TOA reflectance"""
        return tile * self.dn2TOAReflectance_factor(band)

    def calculateSolarDistance(self):
        """Calculate the solar distance."""
        if (
            self.year == -999
            or self.month == -999
            or self.day == -999
            or self.hh == -999
            or self.mm == -999
            or self.ssdd == -999
        ):
            raise ValueError("Metadata for date time is not set")
        ut = self.hh + self.mm / 60.0 + self.ssdd / 3600.0
        if self.month == 1.0 or self.month == 2.0:
            self.year = self.year - 1
            self.month = self.month + 12

        A = int(self.year / 100)
        B = 2 - A + int(A / 4)
        JD = (
            int(365.25 * (self.year + 4716))
            + int(30.6001 * (self.month + 1))
            + self.day
            + ut / 24.0
            + B
            - 1524.5
        )
        D = JD - 2451545.0
        g = 357.529 + 0.98560028 * D
        radg = g * (math.pi / 180.0)
        self.solarDist = (
            1.00014 - 0.01671 * math.cos(radg) - 0.00014 * math.cos(2 * radg)
        )

        if self.solarDist < 0.983 or self.solarDist > 1.017:
            raise ValueError("Solar Distance should be between 0.983 and 1.017")


#################################################################################
class PanInstrumentReflectance(object):
    def __init__(self):
        self.pan_year = -999.0
        self.pan_month = -999.0
        self.pan_day = -999.0
        self.pan_hh = -999.0
        self.pan_mm = -999.0
        self.pan_ssdd = -999.0
        self.pan_solarElevation = -999.0
        self.pan_solarZenithAngle = -999.0
        self.pan_solarZenithAngleInRadians = -999.0
        self.pan_solarDist = -999.0

    def readPanMetaData(self, filename):
        """Read pan metadata needed to set up the instrument"""
        raise NotImplementedError("Subclasses should implement this method.")

    def calculatePanSolarDistance(self):
        """Calculate the solar distance."""
        if (
            self.pan_year == -999
            or self.pan_month == -999
            or self.pan_day == -999
            or self.pan_hh == -999
            or self.pan_mm == -999
            or self.pan_ssdd == -999
        ):
            raise ValueError("Metadata for date time is not set")
        ut = self.pan_hh + self.pan_mm / 60.0 + self.pan_ssdd / 3600.0
        if self.pan_month == 1.0 or self.pan_month == 2.0:
            self.pan_year = self.pan_year - 1
            self.pan_month = self.pan_month + 12

        A = int(self.pan_year / 100)
        B = 2 - A + int(A / 4)
        JD = (
            int(365.25 * (self.pan_year + 4716))
            + int(30.6001 * (self.pan_month + 1))
            + self.pan_day
            + ut / 24.0
            + B
            - 1524.5
        )
        D = JD - 2451545.0
        g = 357.529 + 0.98560028 * D
        radg = g * (math.pi / 180.0)
        self.pan_solarDist = (
            1.00014 - 0.01671 * math.cos(radg) - 0.00014 * math.cos(2 * radg)
        )

        if self.pan_solarDist < 0.983 or self.pan_solarDist > 1.017:
            raise ValueError("Solar Distance should be between 0.983 and 1.017")


__all__ = ["InstrumentReflectance", "PanInstrumentReflectance"]
