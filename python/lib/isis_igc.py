from geocal_swig import (ImageGroundConnection, GdalRasterImage,
                         PlanetSimpleDem, PlanetConstant, Planetocentric,
                         PlanetFixed, Time, ImageCoordinate, GlasGfmCamera,
                         CartesianFixedLookVector)
import json
import os
from .tempdir import run_in_tempdir
from .isis_support import setup_isis
from .pds_label import pds_label
import subprocess
import pandas as pd
import csv
import numpy as np

class IsisIgc(ImageGroundConnection):
    '''Note that this is a IGC that is directly implemented by ISIS.
    This is a very inefficient class, we route everything through the command
    line program campt. The purpose of this is to do comparisons for IGCs we
    create with what ISIS calculates.

    See the routine isis_to_igc, which creates a ImageGroundConnection based
    on ISIS data. The intent is that those IGC will give a much more
    efficient calculation then going through campt, but this allows us
    to compare the 2.

    Note that ISIS uses LT+S for the position to account for light transfer
    time and velocity aberration. This is different than our IGCs, so this
    will disagree. But the final locations on the ground should end up
    being the same.

    IMPORTANT - Note that the use of LT+S is *actually* wrong. The light
    time calculated by the spice routine spkezr_c is to the center of the body
    (e.g., to mars center). This is very different than the light time
    to the surface (see sincpt_c), which for mars data is much shorter.
    This leads to a multiple pixel error in ISIS.

    There is a somewhat related ticket 
    https://github.com/USGS-Astrogeology/ISIS3/issues/1356. This has been
    opened for almost 4 years, so I don't think this will be resolved 
    any time soon.
    '''
    def __init__(self, isis_fname):
        self.fname = os.path.abspath(isis_fname)
        self.f = GdalRasterImage(self.fname)
        self.d = json.loads(self.f.metadata_list('json:ISIS3')[0])
        if(self.d['IsisCube']['Instrument']['TargetName'] == "Moon"):
            self._naif_code = PlanetConstant.MOON_NAIF_CODE
        elif(self.d['IsisCube']['Instrument']['TargetName'] == "Mars"):
            self._naif_code = PlanetConstant.MARS_NAIF_CODE
        dem = PlanetSimpleDem(self._naif_code)
        super().__init__(dem, self.f, None, "ISIS IGC")

    @run_in_tempdir()
    def ground_coordinate_dem(self, ic, d):
        # We ignore the DEM. Could perhaps add some kind of check
        # for this, but we are only using this for comparison with ISIS
        # so not worth trying to get that sorted out.
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/campt",
                        f"from={self.fname}", "to=campt.csv",
                        "type=IMAGE", f"line={str(ic.line + 0.5)}",
                        f"sample={str(ic.sample + 0.5)}",
                        "format=flat"],
                       check=True, stdout=subprocess.DEVNULL)
        t = pd.read_csv("campt.csv")
        return PlanetFixed(t["BodyFixedCoordinateX"][0] * 1000,
                           t["BodyFixedCoordinateY"][0] * 1000,
                           t["BodyFixedCoordinateZ"][0] * 1000,
                           self._naif_code)

    @run_in_tempdir()
    def cf_look_vector_pos(self, ic):
        '''Note that ISIS use LT+S, which means that this position is
        the *apparent* position as seen from the ground, including
        aberration. This will be different than what we have in our
        IGC.'''
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/campt",
                        f"from={self.fname}", "to=campt.csv",
                        "type=IMAGE", f"line={str(ic.line + 0.5)}",
                        f"sample={str(ic.sample + 0.5)}",
                        "format=flat"],
                       check=True, stdout=subprocess.DEVNULL)
        t = pd.read_csv("campt.csv")
        return PlanetFixed(t["SpacecraftPositionX"][0] * 1000,
                           t["SpacecraftPositionY"][0] * 1000,
                           t["SpacecraftPositionZ"][0] * 1000,
                           self._naif_code)

    @run_in_tempdir()
    def cf_look_vector_lv(self, ic):
        '''Note that ISIS use LT+S, which means that its look vector
        (correctly) does not include a velocity aberration correction. So
        In general this won't agree with our IGC, but the points on the
        ground should end up in the same place.'''
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/campt",
                        f"from={self.fname}", "to=campt.csv",
                        "type=IMAGE", f"line={str(ic.line + 0.5)}",
                        f"sample={str(ic.sample + 0.5)}",
                        "format=flat"],
                       check=True, stdout=subprocess.DEVNULL)
        t = pd.read_csv("campt.csv")
        return CartesianFixedLookVector(t["LookDirectionBodyFixedX"][0],
                                        t["LookDirectionBodyFixedY"][0],
                                        t["LookDirectionBodyFixedZ"][0])
    
    @run_in_tempdir()
    def image_coordinate(self, gc):
        # We ignore the gc height here. Could perhaps add some kind of check
        # for this, but we are only using this for comparison with ISIS
        # so not worth trying to get that sorted out.
        setup_isis()
        lat = gc.latitude
        lon = gc.longitude
        if(lon < 0):
            lon += 360
        subprocess.run([f"{os.environ['ISISROOT']}/bin/campt",
                        f"from={self.fname}", "to=campt.csv",
                        "type=GROUND", f"latitude={str(lat)}",
                        f"longitude={str(lon)}",
                        "format=flat"],
                       check=True, stdout=subprocess.DEVNULL)
        t = pd.read_csv("campt.csv")
        return ImageCoordinate(t["Line"][0]-0.5, t["Sample"][0]-0.5)

    @run_in_tempdir()
    def pixel_time(self, ic):
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/campt",
                        f"from={self.fname}", "to=campt.csv",
                        "type=IMAGE", f"line={str(ic.line + 0.5)}",
                        f"sample={str(ic.sample + 0.5)}",
                        "format=flat"],
                       check=True, stdout=subprocess.DEVNULL)
        t = pd.read_csv("campt.csv")
        return Time.parse_time(t["UTC"][0])

    @run_in_tempdir()
    def glas_cam_model(self, focal_length):
        '''Brute force a GLAS camera model by calculating for every
        point in a full line.'''
        with open("points.csv", "w") as f:
            writer = csv.writer(f)
            for i in range(self.number_sample):
                writer.writerow([i+0.5,0])
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/campt",
                        f"from={self.fname}", "to=campt.csv",
                        "coordtype=IMAGE", "coordlist=points.csv",
                        "format=flat"],
                       check=True, stdout=subprocess.DEVNULL)
        t = pd.read_csv("campt.csv")
        x = t["LookDirectionCameraX"][:]
        y = t["LookDirectionCameraY"][:]
        z = t["LookDirectionCameraZ"][:]
        gcam = GlasGfmCamera(1,self.number_sample)
        # Glas focal length is in meters, while
        # input is in millimeters
        gcam.focal_length = focal_length * 1e-3
        fa = np.empty((self.number_sample,4))
        for smp in range(self.number_sample - 1):
            fa[smp,0] = x[smp] / (z[smp] / gcam.focal_length)
            fa[smp,1] = y[smp] / (z[smp] / gcam.focal_length)
            fa[smp,2] = x[smp+1] / (z[smp+1] / gcam.focal_length)
            fa[smp,3] = y[smp+1] / (z[smp+1] / gcam.focal_length)
            # Extrapolated for the last entry in the field angle table    
        smp = self.number_sample - 1
        fa[smp,0] = x[smp] / (z[smp] / gcam.focal_length)
        fa[smp,1] = y[smp] / (z[smp] / gcam.focal_length)
        fa[smp,2] = fa[smp,0] + (fa[smp-1,2]-fa[smp-1,0])
        fa[smp,3] = fa[smp,1] + (fa[smp-1,3]-fa[smp-1,1])
        gcam.sample_number_first = 0
        gcam.delta_sample_pair = 1
        gcam.field_alignment = fa
        return gcam

    @run_in_tempdir()
    def gfm_cam_model(self, focal_length, number_line):
        '''Brute force a GFM camera model by calculating for every
        point in given number of lines.'''
        with open("points.csv", "w") as f:
            writer = csv.writer(f)
            for j in range(number_line):
                for i in range(self.number_sample):
                    writer.writerow([i+0.5,j+0.5])
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/campt",
                        f"from={self.fname}", "to=campt.csv",
                        "coordtype=IMAGE", "coordlist=points.csv",
                        "format=flat"],
                       check=True, stdout=subprocess.DEVNULL)
        t = pd.read_csv("campt.csv")
        x = np.array(t["LookDirectionCameraX"][:]).reshape((self.number_sample, number_line)).transpose()
        y = np.array(t["LookDirectionCameraY"][:]).reshape((self.number_sample, number_line)).transpose()
        z = np.array(t["LookDirectionCameraZ"][:]).reshape((self.number_sample, number_line)).transpose()
        gcam = GlasGfmCamera(number_line,self.number_sample)
        # Glas focal length is in meters, while
        # input is in millimeters
        gcam.focal_length = focal_length * 1e-3
        fa = np.empty((number_line, self.number_sample, 2, 2, 2))
        for ln in range(number_line - 1):
            for smp in range(self.number_sample):
                fa[ln,smp,0,0,0] = x[ln,smp] / (z[ln,smp] / gcam.focal_length)
                fa[ln,smp,0,0,1] = y[ln,smp] / (z[ln,smp] / gcam.focal_length)
                fa[ln,smp,1,0,0] = x[ln+1,smp] / (z[ln+1,smp] / gcam.focal_length)
                fa[ln,smp,1,0,1] = y[ln+1,smp] / (z[ln+1,smp] / gcam.focal_length)
                # Extrapolate last sample
                if(smp < self.number_sample - 1):
                    fa[ln,smp,0,1,0] = x[ln,smp+1] / (z[ln,smp+1] / gcam.focal_length)
                    fa[ln,smp,0,1,1] = y[ln,smp+1] / (z[ln,smp+1] / gcam.focal_length)
                    fa[ln,smp,1,1,0] = x[ln+1,smp+1] / (z[ln+1,smp+1] / gcam.focal_length)
                    fa[ln,smp,1,1,1] = y[ln+1,smp+1] / (z[ln+1,smp+1] / gcam.focal_length)
                else:
                    fa[ln,smp,0,1,0] = fa[ln,smp,0,0,0]+(fa[ln,smp-1,0,1,0]-
                                                         fa[ln,smp-1,0,0,0])
                    fa[ln,smp,0,1,1] = fa[ln,smp,0,0,1]+(fa[ln,smp-1,0,1,1]-
                                                         fa[ln,smp-1,0,0,1])
                    fa[ln,smp,1,1,0] = fa[ln,smp,1,0,0]+(fa[ln,smp-1,1,1,0]-
                                                         fa[ln,smp-1,1,0,0])
                    fa[ln,smp,1,1,1] = fa[ln,smp,1,0,1]+(fa[ln,smp-1,1,1,1]-
                                                         fa[ln,smp-1,1,0,1])
        # Extrapolate last line
        ln = number_line - 1
        for smp in range(self.number_sample):
            for i in range(2):
                for j in range(2):
                    fa[ln,smp,0,i,j] = fa[ln-1,smp,1,i,j]
                    fa[ln,smp,1,i,j] = fa[ln,smp,0,i,j]+(fa[ln-1,smp,1,i,j]-
                                                         fa[ln-1,smp,0,i,j])
        gcam.first_line_block = np.array([0,])
        gcam.first_sample_block = np.array([0,])
        gcam.delta_line_block = np.array([1,])
        gcam.delta_sample_block = np.array([1,])
        gcam.field_alignment_block(0,fa)
        return gcam
    
__all__ = ["IsisIgc",]    
