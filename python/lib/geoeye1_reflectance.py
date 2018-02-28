from __future__ import print_function
from __future__ import division
from __future__ import absolute_import
from builtins import range
from past.utils import old_div
from .instrument_reflectance import *
import math

class Geoeye1Reflectance(InstrumentReflectance):
   '''This class does DN to TOA Reflectance conversion for Geoeye1'''
   def __init__(self, metafname):
      '''Initialization of class'''
      super(InstrumentReflectance, self).__init__()
      self.esun = [196, 185.3, 150.5, 103.9, 161.7]
      self.effectiveBandwidths = [0.0584, 0.0646, 0.0316, 0.1012, 0.3074]
      self.gain = []
      self.offset = []

      for i in range(5):
         self.gain.append(-999.)
         self.offset.append(-999.)
      self.readMetaData(metafname)
      self.calculateSolarDistance()

   def pan_band(self):
      return 4;

   def checkInstrumentPreconditions(self, band):
      '''Ensure that everything is ready to do a dn2TOARadiance conversion'''
      if band >= 5 or band < 0:
         raise ValueError("Band should be [0, 4].")

   def dn2TOARadiance_factor(self, band):
      '''Scale factor to convert DN to TOA radiance factor'''
      self.checkInstrumentPreconditions(band)
      if(self.offset[band] != 0.0):
         raise "Don't currently support nonzero offsets"
      return self.gain[band]

   def readMetaData(self, filename):
      '''Read metadata needed to set up the instrument'''
      metafile = open(filename)
      index = -1

      for line in metafile:
         if line.find("bandNumber = 1;") != -1:
            index = 0
            continue
         elif line.find("bandNumber = 2;") != -1:
            index = 1
            continue
         elif line.find("bandNumber = 3;") != -1:
            index = 2
            continue
         elif line.find("bandNumber = 4;") != -1:
            index = 3
            continue
         elif line.find("bandNumber = 5;") != -1:
            index = 4
            continue
         elif line.find("END_GROUP = bandSpecificInformation;") != -1:
            index = -1
            continue

         if line.find("gain =") != -1:
            self.gain[index] = float(line.split("=")[1].split(";")[0])
            continue
         if line.find("offset = ") != -1:
            self.offset[index] = float(line.split("=")[1].split(";")[0])
            continue

         if line.find("firstLineAcquisitionDateTime = ") != -1:
            tokens = line.split()[2].split("T")
            dateTokens = tokens[0].split("-")
            timeTokens = tokens[1].split(":")
            self.year = float(dateTokens[0])
            self.month = float(dateTokens[1])
            self.day = float(dateTokens[2])
            self.hh = float(timeTokens[0])
            self.mm = float(timeTokens[1])
            self.ssdd = float(timeTokens[2].split("Z")[0])
            continue

         if line.find("firstLineElevationAngle = ") != -1:
            self.solarElevation = float(line.split()[2].split(";")[0])
            self.solarZenithAngle = 90. - self.solarElevation
            self.solarZenithAngleInRadians = self.solarZenithAngle*(old_div(math.pi,180.))
            continue         


   def printMetadata(self):
      print("Metadata:")
      print("=========")
      print("datetime: ", self.year, self.month, self.day, self.hh, self.mm, self.ssdd)
      print("solar elevation/zenith: ", self.solarElevation, self.solarZenithAngle)
      print("solar distance: ", self.solarDist)

      for i in range(5):
         print("-------")
         print("Band: ", i+1)
         print("gain: ", self.gain[i], "offset: ", self.offset[i])
         print("effectiveBandwidth: ", self.effectiveBandwidths[i])


__all__ = ["Geoeye1Reflectance"]         
