from geocal_swig import (ImageGroundConnection, GdalRasterImage,
                         PlanetSimpleDem, PlanetConstant, Planetocentric,
                         PlanetFixed, Time, ImageCoordinate)
import json
import os
from .tempdir import run_in_tempdir
from .isis_support import setup_isis
from .pds_label import pds_label
import subprocess
import pandas as pd

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
    
__all__ = ["IsisIgc",]    
