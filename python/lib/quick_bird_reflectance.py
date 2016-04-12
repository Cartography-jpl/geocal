from __future__ import print_function
from __future__ import division
from __future__ import absolute_import
from builtins import range
from past.utils import old_div
from .instrument_reflectance import *
import math

class QuickBirdReflectance(InstrumentReflectance):
   '''This class does DN to TOA Reflectance conversion for Quickbird'''
   def __init__(self, multimetafname, panmetafname):
      '''Initialization of class'''
      super(InstrumentReflectance, self).__init__()
      self.pan_year = -999.
      self.pan_month = -999.
      self.pan_day = -999.
      self.pan_hh = -999.
      self.pan_mm = -999.
      self.pan_ssdd = -999.
      self.pan_solarElevation = -999.
      self.pan_solarZenithAngle = -999.
      self.pan_solarZenithAngleInRadians = -999.
      self.pan_solarDist = -999.
      self.esun = [1924.59, 1843.08, 1574.77, 1113.71, 1381.79]
      
      self.absCalFactors = []
      self.effectiveBandwidths = []
      for i in range(5):
         self.absCalFactors.append(-999.)
         self.effectiveBandwidths.append(-999.)
      if multimetafname != None:
         self.readMetaData(multimetafname)
         self.calculateSolarDistance()
      if panmetafname != None:
         self.readMetaData(panmetafname)
         self.calculatePanSolarDistance()

   def pan_band(self):
      return 4;

   def checkInstrumentPreconditions(self, band):
      '''Ensure that everything is ready to do a dn2TOARadiance conversion'''
      if band >= 5 or band < 0:
         raise ValueError("Band should be [0, 4].")
      if self.absCalFactors[band] == -999 or self.effectiveBandwidths[band] == -999:
         raise ValueError("Absolute calibration factor and/or effective band width not set.")

   def dn2TOARadiance_factor(self, band):
      '''Scale factor to convert DN to TOA radiance factor'''
      self.checkInstrumentPreconditions(band)
      return old_div(self.absCalFactors[band],self.effectiveBandwidths[band])

   def readMetaData(self, filename):
      '''Read metadata needed to set up the instrument'''
      metafile = open(filename, 'r')
      isPanMetafile = False

      index = -1
      for line in metafile:
         if line.find("BEGIN_GROUP = BAND_B") != -1:
            index = 0
            continue
         elif line.find("BEGIN_GROUP = BAND_G") != -1:
            index = 1
            continue
         elif line.find("BEGIN_GROUP = BAND_R") != -1:
            index = 2
            continue
         elif line.find("BEGIN_GROUP = BAND_N") != -1:
            index = 3
            continue
         elif line.find("BEGIN_GROUP = BAND_P") != -1:
            isPanMetafile = True
            index = 4
            continue
         elif line.find("END_GROUP = BAND_") != -1:
            index = -1
            continue

         if line.find("absCalFactor = ") != -1:
            self.absCalFactors[index] = float(line.strip().split("=")[1].split(";")[0])
            continue

         if line.find("effectiveBandwidth = ") != -1:
            self.effectiveBandwidths[index] = float(line.strip().split("=")[1].split(";")[0])
            continue

         if line.find("firstLineTime = ") != -1:
            tokens = line.split("=")
            tokens = tokens[1].split("T")
            dateTokens = tokens[0].split("-")
            timeTokens = tokens[1].split(":")
            ssdd = timeTokens[2].split("Z;")[0]
            if isPanMetafile:
               self.pan_year = float(dateTokens[0])
               self.pan_month = float(dateTokens[1])
               self.pan_day = float(dateTokens[2])
               self.pan_hh = float(timeTokens[0])
               self.pan_mm = float(timeTokens[1])
               self.pan_ssdd = float(ssdd)
            else:
               self.year = float(dateTokens[0])
               self.month = float(dateTokens[1])
               self.day = float(dateTokens[2])
               self.hh = float(timeTokens[0])
               self.mm = float(timeTokens[1])
               self.ssdd = float(ssdd)
            continue
         if line.find("meanSunEl = ") != -1:
            solarElevation = line.split("=")[1].split(";")[0]
            if isPanMetafile:
               self.pan_solarElevation = float(solarElevation)
               self.pan_solarZenithAngle = 90. - self.pan_solarElevation
               self.pan_solarZenithAngleInRadians = self.pan_solarZenithAngle*(old_div(math.pi,180.))
            else:
               self.solarElevation = float(solarElevation)
               self.solarZenithAngle = 90. - self.solarElevation
               self.solarZenithAngleInRadians = self.solarZenithAngle*(old_div(math.pi,180.))
            continue

   def calculatePanSolarDistance(self):
      '''Calculate the solar distance. Like calculateSolarDistance, but 
      for the pan band.'''
      if self.pan_year == -999 or self.pan_month == -999 or self.pan_day == -999 or \
             self.pan_hh == -999 or self.pan_mm == -999 or self.pan_ssdd == -999:
         raise ValueError("Metadata for pan band date time is not set")
      ut = self.pan_hh + old_div(self.pan_mm,60.0) + old_div(self.pan_ssdd,3600.0)
      if self.pan_month == 1.0 or self.pan_month == 2.0:
         self.pan_year = self.pan_year - 1;
         self.pan_month = self.pan_month + 12;

      A = int(old_div(self.pan_year,100));
      B = 2 - A + int(old_div(A,4));
      JD = int(365.25*(self.pan_year + 4716)) + int(30.6001*(self.pan_month + 1)) + \
           self.pan_day + old_div(ut,24.0) + B - 1524.5
      D = JD - 2451545.0
      g = 357.529 + 0.98560028*D
      radg = g*(old_div(math.pi,180.))
      self.pan_solarDist = 1.00014 - 0.01671*math.cos(radg) - 0.00014*math.cos(2*radg)

      if self.pan_solarDist < 0.983 or self.pan_solarDist > 1.017:
         raise ValueError("Solar Distance for pan band should be between 0.983 and 1.017")

   def dn2TOAReflectance_factor(self, band):
      '''Scale factor to convert DN pto TOA reflectance. As a convention, 
      we treat band 4 as the pan band.'''
      if band < 4:
         return super(QuickBird, self).dn2TOAReflectance_factor(tile, band)

      if self.pan_solarDist == -999. or self.pan_solarZenithAngleInRadians == -999.:
         raise ValueError("Pan solar distance and/or solar angle not set.")
      
      return old_div((self.dn2TOARadiance_factor(band) *
              math.pow(self.pan_solarDist, 2.)*math.pi),\
              (self.esun[band]*math.cos(self.pan_solarZenithAngleInRadians)))

   def printMetadata(self):
      print("Metadata:")
      print("=========")
      print("datetime: ", self.year, self.month, self.day, self.hh, self.mm, self.ssdd)
      print("solar elevation/zenith: ", self.solarElevation, self.solarZenithAngle)
      print("solar distance: ", self.solarDist)

      print("pan ssdd: ", self.pan_ssdd)
      print("pan solar elevation/zenith: ", self.pan_solarElevation, self.pan_solarZenithAngle)
      print("pan solar distance: ", self.pan_solarDist)

      for i in range(5):
         print("-------")
         print("Band: ", i+1)
         print("abscalfactor: ", self.absCalFactors[i])
         print("effectiveBandwidth: ", self.effectiveBandwidths[i])

