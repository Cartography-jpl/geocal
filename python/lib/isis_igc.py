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
    to compare the 2.'''
    def __init__(self, isis_fname):
        self.fname = os.path.abspath(isis_fname)
        self.f = GdalRasterImage(self.fname)
        self.d = json.loads(self.f.metadata_list('json:ISIS3')[0])
        # TODO Handling for other planets
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
    def glas_cam_model(self, focal_length=0.352927):
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
        gcam.focal_length = focal_length
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
    
__all__ = ["IsisIgc",]    
