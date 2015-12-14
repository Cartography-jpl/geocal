import math
import raster_image_extension
import safe_matplotlib_import
import matplotlib.pyplot as plt
from misc import makedirs_p
from geocal_swig import IgcMapProjected, CartesianFixedLookVector, \
    LnLookVector, Ecr, ImageCoordinate, distance
import copy
import numpy as np
import re
import os

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
    def number_image(self):
        "Number of images covered by the tie point"
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
        res = np.empty((2, self.number_image))
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
        res = np.empty((2, self.number_image))
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
        res = np.empty((2, self.number_image))
        for i in range(self.number_image):
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

    @classmethod
    def read_old_mspi_format(self, filename):
        '''This reads the old MSPI tie-point format. This can be used to
        ingest old test cases, but is probably not of much use other than 
        that. This is a simple ASCII format file, see the test example for
        examples of how this works.'''
        with open(filename, 'r') as f:
            ln = f.readline()
            m = re.search('ground_loc(\d+)', ln)
            id = int(m.group(1))
            x, y, z, c11, c12, c13, c21, c22, c23, c31, c32, c33, \
                is_gcp, numcam, trash = ln.split(" ", 14)
            tp = TiePoint(int(numcam))
            tp.id = id
            tp.ground_location = Ecr(float(x),float(y),float(z))
            tp.is_gcp = (int(is_gcp) == 1)
            # We don't currently do anything with the covariance
            for i in range(tp.number_image):
                ln = f.readline()
                l,s,c11,c12,c21,c22,available, trash = \
                    ln.split(" ", 7)
                if(int(available) == 1):
                    tp.image_location[i] = \
                        ImageCoordinate(float(l), float(s)), \
                        math.sqrt(float(c11)),math.sqrt(float(c22))
        return tp

    def write_old_mspi_format(self, dirname, gcp_sigma = 5):
        '''This writes the old MSPI tie-point format. This can be used to
        compare with the old SBA code. The file name is fixed by the
        tiepoint id, but you specify the dirname to put the file.'''
        filename = "%s/tie_point_%03d.dat" % (dirname, self.id)
        with open(filename, 'w') as f:
            tp_cov = gcp_sigma * gcp_sigma if self.is_gcp else 1e8
            f.write("%0.17e %0.17e %0.17e %0.17e 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 %0.17e 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 %0.17e %d %d : ground_loc%d # %0.6f N %0.6f E\n" % (self.ground_location.position[0], self.ground_location.position[1], self.ground_location.position[2], tp_cov, tp_cov, tp_cov, 1 if self.is_gcp else 0, self.number_image, self.id, self.ground_location.latitude, self.ground_location.longitude))
            for i,v in enumerate(self.image_location):
                if(v is None):
                    f.write("0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0 : image_loc%d_view%d\n" % (self.id, i))
                else:
                    ic, ln_sigma, smp_sigma = v
                    f.write("%0.17e %0.17e %0.17e 0.00000000000000000e+00 0.00000000000000000e+00 %0.17e 1 : image_loc%d_view%d\n" % (ic.line, ic.sample, ln_sigma * ln_sigma, smp_sigma * smp_sigma, self.id, i))

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

        nimg = self.number_image
        if(ref_image is not None):
            nimg = nimg + 1
        if(not number_row):
            number_row = int(math.ceil(math.sqrt(nimg)))
        number_col = int(math.ceil(nimg / float(number_row)))
        plt.clf()
        for i in range(self.number_image):
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
            plt.subplot(number_row, number_col, self.number_image + 1)
            plt.title("Reference Image")
            if(self.is_gcp):
                ic = ref_image.coordinate(self.ground_location)
                ref_image.display(ic, sz)
            else:
                d = np.zeros((sz, sz))
                plt.imshow(d, cmap=plt.cm.gray,vmin=-1,vmax=0,
                           extent = [0,sz,0,sz])

    def __str__(self):
        res =  "TiePoint\n"
        res += "  Id:              %d\n" % self.id
        res += "  Is Gcp:          %s\n" % ("True" if self.is_gcp else "False")
        res += "  Ground location: %s\n" % self.ground_location
        res += "  Image coordinates:\n"
        for iloc in self.image_location:
            if(iloc is None):
                res += "     None\n"
            else:
                res += "     %s, %f, %f\n" % (iloc[0], iloc[1], iloc[2])
        return res


class TiePointCollection(list):
    '''This is just a list of TiePoint, with a few useful functions defined'''
    def __init__(self, inital_array = []):
        self.extend(inital_array)

    @classmethod
    def create_multiple_pass(cls, *args):
        '''This is used to complement IgcMultiplePass. It takes a list of
        TiePointCollections. It assumes the first goes with the first 
        IgcCollection, the second with the second IgcCollection and so on.
        We merge all the tiepoints together, with the addition that we 
        shuffle the image_location to match the stacked image numbering 
        IgcMultiplePass uses. So for example if we have 3 IgcCollection each
        with 9 views then the first set of tiepoints have locations from 
        image 0 to 8, the second set from 9 to 17, and the third 18 to 26.
        '''
        res = TiePointCollection()
        ncam = 0
        for tpcol in args:
            ncam += tpcol[0].number_image
        ioff = 0
        for tpcol in args:
            for tp in tpcol:
                mytp = copy.copy(tp)
                mytp.image_location = [None] * ncam
                mytp.image_location[ioff:(ioff + tp.number_image)] = tp.image_location
                res.append(mytp)
            ioff += tpcol[0].number_image
        return res

    @property
    def number_gcp(self):
        cnt = 0
        for i in self:
            if(i.is_gcp):
                cnt += 1
        return cnt

    
    def gcp_diff(self, tpcol_other):
        '''This returns a pandas DataFrame that shows how much a GCP
        has been moved (e.g., by SBA). We represent this is local north
        coordinates'''
        ind = [ tp.id for tp in self if tp.is_gcp]
        lat = [ ]
        lon = [ ]
        height = [ ] 
        e_diff = [ ]
        n_diff = [ ] 
        u_diff = [ ]
        move_distance = [ ]
        for i, tp in enumerate(self):
            if(tp.is_gcp):
                lat.append(tp.ground_location.latitude)
                lon.append(tp.ground_location.longitude)
                height.append(tp.ground_location.height_reference_surface)
                clv = CartesianFixedLookVector(tp.ground_location,
                                               tpcol_other[i].ground_location)
                llv = LnLookVector(clv, tp.ground_location)
                e_diff.append(llv.look_vector[0])
                n_diff.append(llv.look_vector[1])
                u_diff.append(llv.look_vector[2])
                move_distance.append(distance(tp.ground_location, tpcol_other[i].ground_location))
        return pd.DataFrame({'latitude' : lat,
                             'longitude' : lon,
                             'height' : height,
                             'e_diff' : e_diff,
                             'n_diff' : n_diff,
                             'u_diff' : u_diff,
                             'move_distance': move_distance},
                            index=ind)
        

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
        for i in range(self[0].number_image):
            d["image_%d" % i] = self.data_frame(igccol, i)
        return pd.Panel(d)

    @classmethod
    def read_old_mspi_format(self, directory):
        '''This reads the old MSPI tie-point format. This can be used to
        ingest old test cases, but is probably not of much use other than 
        that. This is a simple ASCII format file, see the test example for
        examples of how this works.
 
        This reads all the files in the given directory.'''
        m = re.compile(r'tie_point_\d+\.dat$')
        lst = [directory + "/" + f for f in os.listdir(directory) if m.match(f)]
        tpcol = TiePointCollection()
        for f in lst:
            tpcol.append(TiePoint.read_old_mspi_format(f))
        return tpcol

    def write_old_mspi_format(self, dirname, gcp_sigma = 5):
        '''This writes the old MSPI tie-point format. This can be used to
        compare with the old SBA code. The file name is fixed by the
        tiepoint id, but you specify the dirname to put the file. We write
        one file per tiepoint'''
        makedirs_p(dirname)
        for tp in self:
            tp.write_old_mspi_format(dirname, gcp_sigma)

    def __str__(self):
        res =  "TiePointCollection\n"
        res += "  Total number tp: %d\n" % len(self)
        res += "  Number GCPs:     %d\n" % self.number_gcp
        return res
        

