from __future__ import print_function
from __future__ import division
from builtins import range
from past.utils import old_div
from instrument_reflectance import *
import xml.etree.ElementTree as ET
import math

class SpotReflectance(InstrumentReflectance, PanInstrumentReflectance):
    '''This class does DN to TOA Reflectance conversion for Quickbird'''
    def __init__(self, multimetafname, panmetafname):
        '''initiation of class'''
        InstrumentReflectance.__init__(self);
        PanInstrumentReflectance.__init__(self);

        self.esun = [1982.671954, 1826.087443, 1540.494123, 1094.747446, 1706.514896]
        self.gain = []
        self.bias = []
        for i in range(5):
            self.gain.append(-999.)
            self.bias.append(-999.)
        if multimetafname != None:
            self.readMetaData(multimetafname)
            self.calculateSolarDistance()
        if panmetafname != None:
            self.readPanMetaData(panmetafname)
            self.calculatePanSolarDistance()

    def pan_band(self):
        return 4;

    def checkInstrumentPreconditions(self, band):
        '''Ensure that everything is ready to do a dn2TOARadiance conversion'''
        if band >= 5 or band < 0:
            raise ValueError("Band should be [0, 4].")
        if self.scale[band] == -999 or self.bias[band] == -999:
            raise ValueError("Absolute calibration factor and/or effective band width not set.")

    def dn2TOARadiance_factor(self, band):
        '''Scale factor to convert DN to TOA radiance factor'''
        raise RuntimeError("This method should not be called... use dn2TOARadiance directly")

    def dn2TOARadiance(self, tile, band):
        return old_div(tile,self.gain[band]) + self.bias[band];

    def readMetaData(self, filename):
        self.genericMetaDataParser(filename, 'multi');

    def readPanMetaData(self, filename):
        self.genericMetaDataParser(filename, 'pan');

    def printMetadata(self):
        print("Peter, fill this in.")

    def genericMetaDataParser(self, filename, type):
        tree = ET.parse(filename);
        root = tree.getroot();
        for band in root.iter('Band_Radiance'):
            bandtxt = band.find('BAND_ID').text;
            if bandtxt[0] == 'P':
                bandID = 4;
            else:
                bandID = int(bandtxt[1]);
            gain = float(band.find('GAIN').text);
            bias = float(band.find('BIAS').text);
            self.gain[bandID] = gain;
            self.bias[bandID] = bias;

        acqDate = root.find('./Dataset_Sources/Source_Identification/Strip_Source/IMAGING_DATE').text;
        tokens = acqDate.split("-");
        if type == 'multi':
            self.year = float(tokens[0]);
            self.month = float(tokens[1]);
            self.day = float(tokens[2]);
        elif type == 'pan':
            self.pan_year = float(tokens[0]);
            self.pan_month = float(tokens[1]);
            self.pan_day = float(tokens[2]);

        acqTime = root.find('./Dataset_Sources/Source_Identification/Strip_Source/IMAGING_TIME').text;
        tokens = acqTime.split(":");
        if type == 'multi':
            self.hh = float(tokens[0]);
            self.mm = float(tokens[1]);
            self.ssdd = float(tokens[2]);
        elif type == 'pan':
            self.pan_hh = float(tokens[0]);
            self.pan_mm = float(tokens[1]);
            self.pan_ssdd = float(tokens[2]);

        for locGeo in root.iter('Located_Geometric_Values'):
            location = locGeo.find('LOCATION_TYPE').text;
            if location != 'Center':
                continue
            if type == 'multi':
                self.solarElevation = float(locGeo.find('Solar_Incidences').find('SUN_ELEVATION').text);
                self.solarZenithAngle = 90 - self.solarElevation;
                self.solarZenithAngleInRadians = self.solarZenithAngle*(old_div(math.pi,180));
            elif type == 'pan':
                self.pan_solarElevation = float(locGeo.find('Solar_Incidences').find('SUN_ELEVATION').text);
                self.pan_solarZenithAngle = 90 - self.pan_solarElevation;
                self.pan_solarZenithAngleInRadians = self.pan_solarZenithAngle*(old_div(math.pi,180));

