import geocal
import glob
import os
import re


# We tested this interactively
# lat/lon, get from here, and get directly from tile making sure we get the
# same answer
class LandsatUpdate(geocal.VicarCartLabMultifileSetup):
    """This is like Landsat7Global, but using the updated files based on GRI
    GCP matching. This might make it into a C++ class at some point, but for
    right now we just populate this ourself.
    """

    def __init__(self):
        dname = "/bigdata/tllogan30/sentinel_GRI/l1c_vicar5par5x5/ortho_b8_corrected"
        # Note actual files have 7201, but this overlaps the next tile with 1 pixel, a
        # pretty standard cartlab convention
        num_pix = 7200
        lat_ref = 40
        lon_ref = -80
        size_tile_deg = 1
        mi_ref = geocal.MapInfo(
            geocal.GeodeticConverter(),
            lon_ref,
            lat_ref + size_tile_deg,
            lon_ref + size_tile_deg,
            lat_ref,
            num_pix,
            num_pix,
        )

        # Get number of pixels for 1 degree in each direction
        x0, y0 = mi_ref.coordinate(geocal.Geodetic(lat_ref, lon_ref))
        x1, y1 = mi_ref.coordinate(geocal.Geodetic(lat_ref + 1, lon_ref + 1))
        num_per_deg_lat = int(round(y1 - y0))
        num_per_deg_lon = int(round(x1 - x0))

        # Get offset relative to our reference tile for each file
        d = []
        for fname in glob.glob(f"{dname}/*.vic"):
            m = re.match(
                r"([ns])(\d+)([ew])(\d+)_gcp_15m_b8.vic", os.path.basename(fname)
            )
            if m:
                lat = int(m[2]) * (1 if m[1] == "n" else -1)
                lon = int(m[4]) * (1 if m[3] == "e" else -1)
                loff = (lat - int(lat_ref)) * num_per_deg_lat
                soff = (lon - int(lon_ref)) * num_per_deg_lon
                d.append([loff, soff, fname])

        # Determine overall bounding box for map, and the add all the files
        # relative to that bounding box.
        lmin = min(v[0] for v in d)
        lmax = max(v[0] for v in d)
        smin = min(v[1] for v in d)
        smax = max(v[1] for v in d)
        mi = mi_ref.subset(
            smin,
            lmin,
            smax - smin + mi_ref.number_x_pixel,
            lmax - lmin + mi_ref.number_y_pixel,
        )
        super().__init__(mi)
        for loff, soff, fname in d:
            self.file_add(
                loff - lmin,
                soff - smin,
                loff - lmin + mi_ref.number_y_pixel,
                soff - smin + mi_ref.number_x_pixel,
                fname,
            )
