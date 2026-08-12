from __future__ import annotations
from .instrument_reflectance import InstrumentReflectance
import math
import os


class IkonosReflectance(InstrumentReflectance):
    """This class does DN to TOA Reflectance conversion for Ikonos"""

    def __init__(self, metafname: str | os.PathLike[str]) -> None:
        """Initialization of class"""
        super(InstrumentReflectance, self).__init__()
        self.esun = [1930.9, 1854.8, 1556.5, 1156.9, 1375.8]
        self.effectiveBandwidths = [0.0713, 0.0886, 0.0658, 0.0954, 0.403]
        self.absCalFactors = [72.8, 72.7, 94.9, 84.3, 16.1]

        self.readMetaData(metafname)
        self.calculateSolarDistance()

    def pan_band(self) -> int:
        return 4

    def checkInstrumentPreconditions(self, band: int) -> None:
        """Ensure that everything is ready to do a dn2TOARadiance conversion"""
        if band >= 5 or band < 0:
            raise ValueError("Band should be [0, 4].")

    def dn2TOARadiance_factor(self, band: int) -> float:
        """Scale factor to convert DN to TOA radiance factor"""
        self.checkInstrumentPreconditions(band)
        return 1.0 / (self.absCalFactors[band] * self.effectiveBandwidths[band])

    def readMetaData(self, filename: str | os.PathLike[str]) -> None:
        """Read metadata needed to set up the instrument"""
        metafile = open(filename)
        dateSet = False
        angleSet = False

        for line in metafile:
            if angleSet and dateSet:
                break
            if line.find("Acquisition Date/Time:") != -1 and not dateSet:
                tokens = line.split()
                dateTokens = tokens[2].split("-")
                timeTokens = tokens[3].split(":")
                self.year = float(dateTokens[0])
                self.month = float(dateTokens[1])
                self.day = float(dateTokens[2])
                self.hh = float(timeTokens[0])
                self.mm = float(timeTokens[1])
                self.ssdd = 0.0
                dateSet = True
                continue
            if line.find("Sun Angle Elevation:") != -1 and not angleSet:
                tokens = line.split()
                self.solarElevation = float(tokens[3])
                self.solarZenithAngle = 90.0 - self.solarElevation
                self.solarZenithAngleInRadians = self.solarZenithAngle * (
                    math.pi / 180.0
                )
                angleSet = True
                continue

    def printMetadata(self) -> None:
        print("Metadata:")
        print("=========")
        print(
            "datetime: ", self.year, self.month, self.day, self.hh, self.mm, self.ssdd
        )
        print("solar elevation/zenith: ", self.solarElevation, self.solarZenithAngle)
        print("solar distance: ", self.solarDist)

        for i in range(5):
            print("-------")
            print("Band: ", i + 1)
            print("abscalfactor: ", self.absCalFactors[i])
            print("effectiveBandwidth: ", self.effectiveBandwidths[i])


__all__ = ["IkonosReflectance"]
