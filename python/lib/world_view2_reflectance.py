from instrument_reflectance import *
import math

class WorldView2Reflectance(InstrumentReflectance):
   '''This class does DN to TOA Reflectance conversion for WorldView 2'''
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
      self.esun = [1758.2229, 1974.2416, 1856.4104, 1738.4791, \
                   1559.4555, 1342.0695, 1069.7302, 861.2866, 1580.8140]
      
      self.absCalFactors = []
      self.effectiveBandwidths = []
      for i in range(9):
         self.absCalFactors.append(-999.)
         self.effectiveBandwidths.append(-999.)
      if multimetafname != None:
         self.readMetaData(multimetafname)
         self.calculateSolarDistance()
      if panmetafname != None:
         self.readMetaData(panmetafname)
         self.calculatePanSolarDistance()

   def pan_band(self):
      return 8;

   def checkInstrumentPreconditions(self, band):
      '''Ensure that everything is ready to do a dn2TOARadiance conversion'''
      if band >= 9 or band < 0:
         raise ValueError("Band should be [0, 8].")
      if self.absCalFactors[band] == -999 or self.effectiveBandwidths[band] == -999:
         raise ValueError("Absolute calibration factor and/or effective band width not set.")

   def dn2TOARadiance_factor(self, band):
      '''Scale factor to convert DN to TOA radiance factor'''
      self.checkInstrumentPreconditions(band)
      return self.absCalFactors[band]/self.effectiveBandwidths[band]

   def readMetaData(self, filename):
      '''Read metadata needed to set up the instrument'''
      metafile = open(filename, 'r')
      isPanMetafile = False

      index = -1
      for line in metafile:
         tokens = line.split("=")
         if len(tokens) < 2:
            continue
         if tokens[0].strip() == "END_GROUP":
            index = -1
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_C":
            index = 0
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_B":
            index = 1
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_G":
            index = 2
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_Y":
            index = 3
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_R":
            index = 4
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_RE":
            index = 5
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_N":
            index = 6
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_N2":
            index = 7
            continue
         if tokens[0].strip() == "BEGIN_GROUP" and tokens[1].strip() == "BAND_P":
            isPanMetafile = True
            index = 8
            continue
         if tokens[0].strip() == "absCalFactor" and index != -1:
            datatoken = tokens[1].split(";")[0]
            self.absCalFactors[index] = float(datatoken.strip())
            continue
         if tokens[0].strip() == "effectiveBandwidth" and index != -1:
            datatoken = tokens[1].split(";")[0]
            self.effectiveBandwidths[index] = float(datatoken.strip())
            continue
         if tokens[0].strip() == "firstLineTime":
            subtokens = tokens[1].split("T")
            datetokens = subtokens[0].split("-")
            timetokens = subtokens[1].split(":")
            if not isPanMetafile:
               self.year = float(datetokens[0])
               self.month = float(datetokens[1])
               self.day = float(datetokens[2])
               self.hh = float(timetokens[0])
               self.mm = float(timetokens[1])
               datatoken = timetokens[2].split("Z;")[0]
               self.ssdd = float(datatoken.strip())
            else:
               self.pan_year = float(datetokens[0])
               self.pan_month = float(datetokens[1])
               self.pan_day = float(datetokens[2])
               self.pan_hh = float(timetokens[0])
               self.pan_mm = float(timetokens[1])
               datatoken = timetokens[2].split("Z;")[0]
               self.pan_ssdd = float(datatoken.strip())
            continue
         if tokens[0].strip() == "meanSunEl":
            datatoken = tokens[1].split(";")[0]
            if not isPanMetafile:
               self.solarElevation = float(datatoken.strip())
               self.solarZenithAngle = 90. - self.solarElevation
               self.solarZenithAngleInRadians = self.solarZenithAngle*(math.pi/180.)
            else:
               self.pan_solarElevation = float(datatoken.strip())
               self.pan_solarZenithAngle = 90. - self.pan_solarElevation
               self.pan_solarZenithAngleInRadians = self.pan_solarZenithAngle*(math.pi/180.)
            continue

   def calculatePanSolarDistance(self):
      '''Calculate the solar distance. Like calculateSolarDistance, but 
      for the pan band.'''
      if self.pan_year == -999 or self.pan_month == -999 or self.pan_day == -999 or \
             self.pan_hh == -999 or self.pan_mm == -999 or self.pan_ssdd == -999:
         raise ValueError("Metadata for pan band date time is not set")
      ut = self.pan_hh + self.pan_mm/60.0 + self.pan_ssdd/3600.0
      if self.pan_month == 1.0 or self.pan_month == 2.0:
         self.pan_year = self.pan_year - 1;
         self.pan_month = self.pan_month + 12;

      A = int(self.pan_year/100);
      B = 2 - A + int(A/4);
      JD = int(365.25*(self.pan_year + 4716)) + int(30.6001*(self.pan_month + 1)) + \
           self.pan_day + ut/24.0 + B - 1524.5
      D = JD - 2451545.0
      g = 357.529 + 0.98560028*D
      radg = g*(math.pi/180.)
      self.pan_solarDist = 1.00014 - 0.01671*math.cos(radg) - 0.00014*math.cos(2*radg)

      if self.pan_solarDist < 0.983 or self.pan_solarDist > 1.017:
         raise ValueError("Solar Distance for pan band should be between 0.983 and 1.017")

   def dn2TOAReflectance_factor(self, band):
      '''Scale factor to convert DN to TOA reflectance. As a convention, 
      we treat band 8 as the pan band.'''
      if band < 8:
         return super(WorldView2Reflectance, self).dn2TOAReflectance_factor(band)

      if self.pan_solarDist == -999. or self.pan_solarZenithAngleInRadians == -999.:
         raise ValueError("Pan solar distance and/or solar angle not set.")
      
      return (self.dn2TOARadiance_factor(band) *
              math.pow(self.pan_solarDist, 2.)*math.pi)/\
              (self.esun[band]*math.cos(self.pan_solarZenithAngleInRadians))

   def printMetadata(self):
      print "Metadata:"
      print "========="
      print "datetime: ", self.year, self.month, self.day, self.hh, self.mm, self.ssdd
      print "solar elevation/zenith: ", self.solarElevation, self.solarZenithAngle
      print "solar distance: ", self.solarDist

      print "pan ssdd: ", self.pan_ssdd
      print "pan solar elevation/zenith: ", self.pan_solarElevation, self.pan_solarZenithAngle
      print "pan solar distance: ", self.pan_solarDist

      for i in range(9):
         print "-------"
         print "Band: ", i+1
         print "abscalfactor: ", self.absCalFactors[i]
         print "effectiveBandwidth: ", self.effectiveBandwidths[i]

