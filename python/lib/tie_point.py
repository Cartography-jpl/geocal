import math
import raster_image_extension
import safe_matplotlib_import
import matplotlib.pyplot as plt
from geocal_swig import IgcMapProjected
import numpy as np
# Optional support for pandas
try:
    import pandas as pd
    have_pandas = True
except ImportError:
    have_pandas = False

class TiePoint(object):
    '''
    This is a tiepoint. This is little more than a structure.
    This contains the following pieces

    id
       ID number, can be used to identify a tie point.

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
        self.id = 1

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

    @property
    def ic(self):
        '''This repackages the image_location, in a better way for doing
        such things as plotting. We return a 2D array with the first row
        being the image line and the second the image sample. The columns
        are the image indexes. For image locations that are missing, we
        return a np.NaN'''
        res = np.empty((2, self.number_camera))
        for i, iloc in enumerate(self.image_location):
            if(iloc):
                res[0,i] = iloc[0].line
                res[1,i] = iloc[0].sample
            else:
                res[0,i] = np.NaN
                res[1,i] = np.NaN
        return res

    @property
    def ic_sigma(self):
        '''Like ic, but returned line and sample sigma.'''
        res = np.empty((2, self.number_camera))
        for i, iloc in enumerate(self.image_location):
            if(iloc):
                res[0,i] = iloc[1]
                res[1,i] = iloc[2]
            else:
                res[0,i] = np.NaN
                res[1,i] = np.NaN
        return res
        

    def ic_pred(self, igccol):
        '''Like ic, but uses the supplied igccol to predict the image 
        location given our current ground position.'''
        res = np.empty((2, self.number_camera))
        for i in range(self.number_camera):
            try:
                ic = igccol.image_coordinate(i, self.ground_location)
                res[0,i] = ic.line
                res[1,i] = ic.sample
            except RuntimeError as e:
                if(str(e) != "ImageGroundConnectionFailed"):
                    raise e
                res[0,i] = np.NaN
                res[1,i] = np.NaN
        return res
    
    def ic_diff(self, igccol):
        '''Difference between observed and predicted image coordinates'''
        return self.ic - self.ic_pred(igccol)

    def display(self, igc_coll, sz = 500, ref_image = None, number_row = None,
                map_info = None, surface_image = None):
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

        As an alternative to mapinfo, you can supply a list of already 
        projected images. This should have been projected using igc_coll.
        '''

        nimg = self.number_camera
        if(ref_image is not None):
            nimg = nimg + 1
        if(not number_row):
            number_row = int(math.ceil(math.sqrt(nimg)))
        number_col = int(math.ceil(nimg / float(number_row)))
        plt.clf()
        for i in range(self.number_camera):
            if(self.image_location[i] is None):
                plt.subplot(number_row, number_col, i + 1)
                plt.title(igc_coll.title(i))
                d = np.zeros((sz, sz))
                plt.imshow(d, cmap=plt.cm.gray,vmin=-1,vmax=0,
                           extent = [0,sz,0,sz])
            else:
                plt.subplot(number_row, number_col, i + 1)
                plt.title(igc_coll.title(i))
                if(map_info):
                    igc_proj = IgcMapProjected(map_info, 
                                               igc_coll.image_ground_connection(i))
                    pt = igc_proj.coordinate(igc_coll.ground_coordinate(i, self.image_location[i][0]))
                    igc_proj.display(pt, sz)
                elif(surface_image):
                    pt = surface_image[i].coordinate(igc_coll.ground_coordinate(i, self.image_location[i][0]))
                    surface_image[i].display(pt, sz)
                else:
                    igc_coll.image(i).display(self.image_location[i][0], sz)
        if(ref_image is not None):
            plt.subplot(number_row, number_col, self.number_camera + 1)
            plt.title("Reference Image")
            if(self.is_gcp):
                ic = ref_image.coordinate(self.ground_location)
                ref_image.display(ic, sz)
            else:
                d = np.zeros((sz, sz))
                plt.imshow(d, cmap=plt.cm.gray,vmin=-1,vmax=0,
                           extent = [0,sz,0,sz])

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

    def data_frame(self, igccol, image_index):
        '''Return a pandas DataFrame for the given image_index.
        
        Note that in addition to the normal pandas plotting (e.g., 
        something like df.plot(x='sample', y='line_residual', kind='scatter'))
        a useful plot to make is a distribution of the residuals. You
        can set up a nice colormap for this and display the data by:

        import matplotlib as mpl
        import matplotlib.pylab as plt
        import pandas as pd
        import seaborn as sns

        df = tpcol.data_frame(igccol, image_index)
        cmap = mpl.colors.ListedColormap(sns.color_palette("RdBu_r", 256))
        plt.scatter(df.line, df.sample, c = df.line_residual, cmap = cmap)
        plt.colorbar()
        '''
        if(not have_pandas):
            raise RuntimeError("Need to have pandas installed to use this function.")
        igc = igccol.image_ground_connection(image_index)
        ind = [ tp.id for tp in self ]
        is_gcp = [ tp.is_gcp for tp in self ]
        nimgloc = [ tp.number_image_location for tp in self ]
        data = np.full((len(self), 8), np.NaN)
        for i, tp in enumerate(self):
            iloc = tp.image_location[image_index]
            if(iloc):
                try:
                    icpred = igc.image_coordinate(tp.ground_location)
                    data[i,0] = iloc[0].line
                    data[i,1] = iloc[0].sample
                    data[i,2] = iloc[1]
                    data[i,3] = iloc[2]
                    data[i, 4] = icpred.line
                    data[i, 5] = icpred.sample
                except RuntimeError as e:
                    if(str(e) != "ImageGroundConnectionFailed"):
                        raise e
        data[:,6] = data[:, 0] - data[:, 4]
        data[:,7] = data[:, 1] - data[:, 5]
        return pd.DataFrame({ 'line' : data[:,0],
                              'samp' : data[:, 1],
                              'number_image_location' : nimgloc,
                              'line_sigma' : data[:, 2],
                              'samp_sigma' : data[:, 3],
                              'line_pred' : data[:, 4],
                              'samp_pred' : data[:, 5],
                              'line_residual' : data[:, 6],
                              'samp_residual' : data[:, 7],
                              'is_gcp' : is_gcp },
                            index=ind)

    def panel(self, igccol):
        '''Return a pandas Panel for all the image indexes'''
        d = {}
        for i in range(self[0].number_camera):
            d["image_%d" % i] = self.data_frame(igccol, i)
        return pd.Panel(d)

    def __str__(self):
        res =  "TiePointCollection\n"
        res += "  Total number tp: %d\n" % len(self)
        res += "  Number GCPs:     %d\n" % self.number_gcp
        return res
        

