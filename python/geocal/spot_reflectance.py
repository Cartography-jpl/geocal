from __future__ import annotations
from .instrument_reflectance import InstrumentReflectance, PanInstrumentReflectance
import xml.etree.ElementTree as ET
import math
import os
import numpy as np


class SpotReflectance(InstrumentReflectance, PanInstrumentReflectance):
    """This class does DN to TOA Reflectance conversion for Quickbird"""

    def __init__(
        self,
        multimetafname: str | os.PathLike[str],
        panmetafname: str | os.PathLike[str],
    ) -> None:
        """initiation of class"""
        InstrumentReflectance.__init__(self)
        PanInstrumentReflectance.__init__(self)

        self.esun = [1982.671954, 1826.087443, 1540.494123, 1094.747446, 1706.514896]
        self.gain = []
        self.bias = []
        for i in range(5):
            self.gain.append(-999.0)
            self.bias.append(-999.0)
        if multimetafname is not None:
            self.readMetaData(multimetafname)
            self.calculateSolarDistance()
        if panmetafname is not None:
            self.readPanMetaData(panmetafname)
            self.calculatePanSolarDistance()

    def pan_band(self) -> int:
        return 4

    def checkInstrumentPreconditions(self, band: int) -> None:
        """Ensure that everything is ready to do a dn2TOARadiance conversion"""
        if band >= 5 or band < 0:
            raise ValueError("Band should be [0, 4].")
        if self.gain[band] == -999 or self.bias[band] == -999:
            raise ValueError(
                "Absolute calibration factor and/or effective band width not set."
            )

    def dn2TOARadiance_factor(self, band: int) -> float:
        """Scale factor to convert DN to TOA radiance factor"""
        raise RuntimeError(
            "This method should not be called... use dn2TOARadiance directly"
        )

    def dn2TOARadiance(self, tile: np.ndarray, band: int) -> np.ndarray:
        return tile / self.gain[band] + self.bias[band]

    def readMetaData(self, filename: str | os.PathLike[str]) -> None:
        self.genericMetaDataParser(filename, "multi")

    def readPanMetaData(self, filename: str | os.PathLike[str]) -> None:
        self.genericMetaDataParser(filename, "pan")

    def printMetadata(self) -> None:
        print("Peter, fill this in.")

    def _find_text(self, et: ET.Element[str] | None, val: str) -> str:
        if et is None:
            raise RuntimeError("Trouble reading file")
        t1 = et.find(val)
        if t1 is None:
            raise RuntimeError("Trouble reading file")
        t1t = t1.text
        if t1t is None:
            raise RuntimeError("Trouble reading file")
        return t1t

    def genericMetaDataParser(self, filename: str | os.PathLike[str], typ: str) -> None:
        tree = ET.parse(filename)
        root = tree.getroot()
        for band in root.iter("Band_Radiance"):
            bandtxt = self._find_text(band, "BAND_ID")
            if bandtxt[0] == "P":
                bandID = 4
            else:
                bandID = int(bandtxt[1])
            self.gain[bandID] = float(self._find_text(band, "GAIN"))
            self.bias[bandID] = float(self._find_text(band, "BIAS"))

        rf = root.find(
            "./Dataset_Sources/Source_Identification/Strip_Source/IMAGING_DATE"
        )
        if rf is None:
            raise RuntimeError(f"Trouble reading {filename}")
        rft = rf.text
        if rft is None:
            raise RuntimeError(f"Trouble reading {filename}")
        acqDate = rft
        tokens = acqDate.split("-")
        if typ == "multi":
            self.year = float(tokens[0])
            self.month = float(tokens[1])
            self.day = float(tokens[2])
        elif typ == "pan":
            self.pan_year = float(tokens[0])
            self.pan_month = float(tokens[1])
            self.pan_day = float(tokens[2])

        acqTime = self._find_text(
            root, "./Dataset_Sources/Source_Identification/Strip_Source/IMAGING_TIME"
        )
        tokens = acqTime.split(":")
        if typ == "multi":
            self.hh = float(tokens[0])
            self.mm = float(tokens[1])
            self.ssdd = float(tokens[2])
        elif typ == "pan":
            self.pan_hh = float(tokens[0])
            self.pan_mm = float(tokens[1])
            self.pan_ssdd = float(tokens[2])

        for locGeo in root.iter("Located_Geometric_Values"):
            location = self._find_text(locGeo, "LOCATION_TYPE")
            if location != "Center":
                continue
            if typ == "multi":
                self.solarElevation = float(
                    self._find_text(locGeo.find("Solar_Incidences"), "SUN_ELEVATION")
                )
                self.solarZenithAngle = 90 - self.solarElevation
                self.solarZenithAngleInRadians = self.solarZenithAngle * (
                    math.pi / 180.0
                )
            elif typ == "pan":
                self.pan_solarElevation = float(
                    self._find_text(locGeo.find("Solar_Incidences"), "SUN_ELEVATION")
                )
                self.pan_solarZenithAngle = 90 - self.pan_solarElevation
                self.pan_solarZenithAngleInRadians = self.pan_solarZenithAngle * (
                    math.pi / 180.0
                )


__all__ = ["SpotReflectance"]
