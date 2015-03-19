import math
import raster_image_extension
import safe_matplotlib_import
import matplotlib.pyplot as plt
from geocal_swig import IgcMapProjected

class TiePoint(object):
    '''
    This is a tiepoint. This is little more than a structure.
    This contains the following pieces

    image_location
       This is an array of a fixed number of cameras. A location
       either is \'None\' if we don\'t have an image location for that, or
       the (ImageCoordinate, line_sigma, sample_sigma) if we do.

    ground_location
       The ground location, as a Ecr.

    is_gcp
       A boolean indicating if the ground location is a ground control
       point (so we want to keep the ground_location close to fixed) or not.

   '''
    def __init__(self, ncam):
        self.image_location = [None] * ncam
        self.ground_location = None
        self.is_gcp = False

    @property
    def number_camera(self):
        "Number of cameras covered by the tie point"
        return len(self.image_location)
    
    @property
    def number_image_location(self):
        "Number of image locations we have that have values"
        res = 0
        for v in self.image_location:
            if(v): res += 1
        return res

    def display(self, igc_coll, sz = 500, ref_image = None, number_row = None,
                map_info = None):
        '''This executes plt.imshow for the images that make up this
        tiepoint.  Since we don't store the images in a tiepoint, you
        need to also pass in the IgcCollection that this tiepoint
        comes from (we really only need the RasterImage, but at least
        for now the IgcCollection is more convenient to pass in).

        You can optionally specify the number of rows to use. Otherwise,
        we choose the number of rows and columns to make a square figure

        If you supply a mapinfo, we project the imagery to the surface
        (useful for data that is very distorted from one view to another, or to
        compare to reference image).
        '''

        nimg = self.number_camera
        if(ref_image is not None):
            nimg = nimg + 1
        if(not number_row):
            number_row = int(math.ceil(math.sqrt(nimg)))
        number_col = int(math.ceil(nimg / float(number_row)))
        plt.clf()
        for i in range(self.number_camera):
            if(self.image_location[i] is not None):
                plt.subplot(number_row, number_col, i + 1)
                plt.title(igc_coll.title(i))
                if(map_info):
                    igc_proj = IgcMapProjected(map_info, 
                                               igc_coll.image_ground_connection(i))
                    pt = igc_proj.coordinate(igc_coll.ground_coordinate(i, self.image_location[i][0]))
                    igc_proj.display(pt, sz)
                else:
                    igc_coll.image(i).display(self.image_location[i][0], sz)
        if(ref_image is not None):
            plt.subplot(number_row, number_col, self.number_camera + 1)
            plt.title("Reference Image")
            if(self.is_gcp):
                ic = ref_image.coordinate(self.ground_location)
                ref_image.display(ic, sz)

class TiePointCollection(list):
    '''This is just a list of TiePoint, with a few useful functions defined'''
    def __init__(self, inital_array = []):
        self.extend(inital_array)

    @property
    def number_gcp(self):
        cnt = 0
        for i in self:
            if(i.is_gcp):
                cnt += 1
        return cnt

    def __str__(self):
        res =  "TiePointCollection\n"
        res += "  Total number tp: %d\n" % len(self)
        res += "  Number GCPs:     %d\n" % self.number_gcp
        return res
        

