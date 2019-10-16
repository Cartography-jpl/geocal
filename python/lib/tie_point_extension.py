from builtins import str
from builtins import range
from builtins import object
import math
from .raster_image_extension import *
from .safe_matplotlib_import import *
import matplotlib.pyplot as plt
from .misc import makedirs_p
from geocal_swig import IgcMapProjected, CartesianFixedLookVector, \
    LnLookVector, Ecr, ImageCoordinate, distance, TiePoint, TiePointCollection, \
    distance, SimpleDem
import copy
import numpy as np
import re
import os

# Optional support for pandas
try:
    # The default full build of python in afids doesn't have lzma support. We
    # don't actually care, so suppress the warning about this.
    import warnings
    with warnings.catch_warnings():
        warnings.filterwarnings("ignore",category=UserWarning)
        import pandas as pd
    have_pandas = True
except ImportError:
    have_pandas = False

# Note that TiePoint use to be a python only object, and was saved in
# pickle format. It is now C++, and is saved using the boost serialization
# xml format, which supports adding functionality to read older format
# versions. You can use the write_old_mspi_format in the older version of
# the code and read_old_mspi_format in the newer code to convert between the
# old and new formats.


@classmethod
def _tp_read_old_mspi_format(self, filename):
    '''This reads the old MSPI tie-point format. This can be used to
    ingest old test cases, but is probably not of much use other than 
    that. This is a simple ASCII format file, see the test example for
    examples of how this works.'''
    with open(filename, 'r') as f:
        ln = f.readline()
        m = re.search(r'ground_loc(\d+)', ln)
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
            l,s,c11,c12,c21,c22,available, trash = ln.split(" ", 7)
            if(int(available) == 1):
                tp.image_coordinate(i, ImageCoordinate(float(l), float(s)),
                                    math.sqrt(float(c11)),math.sqrt(float(c22)))
    return tp
TiePoint.read_old_mspi_format = _tp_read_old_mspi_format

def _tp__write_old_mspi_format(self, dirname, gcp_sigma = 5):
    '''This writes the old MSPI tie-point format. This can be used to
    compare with the old SBA code. The file name is fixed by the
    tiepoint id, but you specify the dirname to put the file.'''
    filename = "%s/tie_point_%03d.dat" % (dirname, self.id)
    with open(filename, 'w') as f:
        tp_cov = gcp_sigma * gcp_sigma if self.is_gcp else 1e8
        f.write("%0.17e %0.17e %0.17e %0.17e 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 %0.17e 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 %0.17e %d %d : ground_loc%d # %0.6f N %0.6f E\n" % (self.ground_location.position[0], self.ground_location.position[1], self.ground_location.position[2], tp_cov, tp_cov, tp_cov, 1 if self.is_gcp else 0, self.number_image, self.id, self.ground_location.latitude, self.ground_location.longitude))
        for i in range(self.number_image):
            if(self.image_coordinate(i) is None):
                f.write("0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0.00000000000000000e+00 0 : image_loc%d_view%d\n" % (self.id, i))
            else:
                ic = self.image_coordinate(i)
                ln_sigma = self.line_sigma(i)
                smp_sigma = self.sample_sigma(i)
                f.write("%0.17e %0.17e %0.17e 0.00000000000000000e+00 0.00000000000000000e+00 %0.17e 1 : image_loc%d_view%d\n" % (ic.line, ic.sample, ln_sigma * ln_sigma, smp_sigma * smp_sigma, self.id, i))

TiePoint.write_old_mspi_format = _tp__write_old_mspi_format
                    
def _tp_display(self, igc_coll, sz = 500, ref_image = None, number_row = None,
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
        if(self.image_coordinate(i) is None):
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
                pt = igc_proj.coordinate(igc_coll.ground_coordinate(i, self.image_coordinate(i)))
                igc_proj.display(pt, sz)
            elif(surface_image):
                pt = surface_image[i].coordinate(igc_coll.ground_coordinate(i, self.image_coordinate(i)))
                surface_image[i].display(pt, sz)
            else:
                igc_coll.image(i).display(self.image_coordinate(i), sz)
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
    plt.tight_layout()

TiePoint.display = _tp_display

@classmethod
def _tpcol_create_multiple_pass(cls, *args):
    '''This is used to complement IgcMultiplePass. It takes a list of
    TiePointCollections. It assumes the first goes with the first 
    IgcCollection, the second with the second IgcCollection and so on.
    We merge all the tiepoints together, with the addition that we 
    shuffle the image_coordinate to match the stacked image numbering 
    IgcMultiplePass uses. So for example if we have 3 IgcCollection each
    with 9 views then the first set of tiepoints have locations from 
    image 0 to 8, the second set from 9 to 17, and the third 18 to 26.
    '''
    res = TiePointCollection()
    ncam = 0
    for tpcol in args:
        ncam += tpcol[0].number_image
    ioff = 0
    tp_id = 1
    for tpcol in args:
        for tp in tpcol:
            mytp = TiePoint(ncam)
            mytp.id = tp_id
            tp_id += 1
            mytp.is_gcp = tp.is_gcp
            if(tp.ground_location):
                mytp.ground_location = tp.ground_location
            for i in range(tp.number_image):
                ic = tp.image_coordinate(i)
                if(ic):
                    mytp.image_coordinate(i+ioff, ic, tp.line_sigma(i),
                                          tp.sample_sigma(i))
            res.append(mytp)
        ioff += tpcol[0].number_image
    return res

TiePointCollection.create_multiple_pass = _tpcol_create_multiple_pass

def _tpcol_tp_info(self):
    '''Return a pandas DataFrame with all the tiepoint information'''
    ind = [ tp.id for tp in self ]
    is_gcp = [ tp.is_gcp for tp in self ]
    nimgloc = [ tp.number_image_location for tp in self ]
    cols = ["Is GCP", "Number Image Location"]
    d = {"Is GCP": is_gcp, "Number Image Location" : nimgloc }
    for i in range(self[0].number_image):
        d["Line View %d" % (i+1)] = \
            [tp.image_coordinate(i).line if tp.image_coordinate(i) else None 
             for tp in self]
        d["Sample View %d" % (i+1)] = \
            [tp.image_coordinate(i).sample if tp.image_coordinate(i) else None 
             for tp in self]
        cols.append("Line View %d" % (i+1))
        cols.append("Sample View %d" % (i+1))
    return pd.DataFrame(d, index=ind), cols

TiePointCollection.tp_info = _tpcol_tp_info

def _tpcol_tp_res(self, igccol):
    '''Return a pandas DataFrame with all the tiepoint residual information'''
    ind = [ tp.id for tp in self ]
    is_gcp = [ tp.is_gcp for tp in self ]
    nimgloc = [ tp.number_image_location for tp in self ]
    cols = ["Is GCP", "Number Image Location", "Max Residual"]
    d = {"Is GCP": is_gcp, "Number Image Location" : nimgloc }
    max_res = np.zeros(len(self))
    for i in range(self[0].number_image):
        igc = igccol.image_ground_connection(i)
        lres = np.full((len(self),), np.NaN)
        sres = np.full((len(self),), np.NaN)
        for j, tp in enumerate(self):
            iloc = tp.image_coordinate(i)
            if(iloc):
                try:
                    icpred = igc.image_coordinate(tp.ground_location)
                    lres[j] = (iloc.line - icpred.line)
                    sres[j] = (iloc.sample - icpred.sample)
                    max_res[j] = max(abs(lres[j]),abs(sres[j]),max_res[j])
                except RuntimeError as e:
                    if(str(e) != "ImageGroundConnectionFailed"):
                        raise e
        d["Line Res %d" % (i+1)] = lres
        d["Samp Res %d" % (i+1)] = sres
        cols.append("Line Res %d" % (i+1))
        cols.append("Samp Res %d" % (i+1))
    d["Max Residual"] = max_res
    return pd.DataFrame(d, index=ind), cols
        
TiePointCollection.tp_res = _tpcol_tp_res
        
def _tpcol_gcp_diff(self, tpcol_other):
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
    
TiePointCollection.gcp_diff = _tpcol_gcp_diff

def _tpcol_data_frame(self, igccol, image_index):
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
    plt.scatter(df.sample, df.line, c = df.line_residual, cmap = cmap)
    plt.colorbar()
    '''
    if(not have_pandas):
        raise RuntimeError("Need to have pandas installed to use this function.")
    igc = igccol.image_ground_connection(image_index)
    ind = [ tp.id for tp in self ]
    is_gcp = [ tp.is_gcp for tp in self ]
    nimgloc = [ tp.number_image_location for tp in self ]
    data = np.full((len(self), 9), np.NaN)
    d = SimpleDem()
    for i, tp in enumerate(self):
        iloc = tp.image_coordinate(image_index)
        if(iloc):
            try:
                icpred = igc.image_coordinate(tp.ground_location)
                data[i,0] = iloc.line
                data[i,1] = iloc.sample
                data[i,2] = tp.line_sigma(image_index)
                data[i,3] = tp.sample_sigma(image_index)
                data[i, 4] = icpred.line
                data[i, 5] = icpred.sample
                data[i, 6] = distance(d.surface_point(tp.ground_location),
                             d.surface_point(igc.ground_coordinate(iloc)))
            except RuntimeError as e:
                if(str(e) != "ImageGroundConnectionFailed"):
                    raise e
    data[:,7] = data[:, 0] - data[:, 4]
    data[:,8] = data[:, 1] - data[:, 5]
    return pd.DataFrame({ 'line' : data[:,0],
                          'samp' : data[:, 1],
                          'number_image_location' : nimgloc,
                          'line_sigma' : data[:, 2],
                          'samp_sigma' : data[:, 3],
                          'line_pred' : data[:, 4],
                          'samp_pred' : data[:, 5],
                          'ground_2d_distance' : data[:, 6],
                          'line_residual' : data[:, 7],
                          'samp_residual' : data[:, 8],
                          'is_gcp' : is_gcp },
                        index=ind)

TiePointCollection.data_frame = _tpcol_data_frame

@classmethod
def _tpcol_read_old_mspi_format(self, directory):
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

TiePointCollection.read_old_mspi_format = _tpcol_read_old_mspi_format

def _tpcol_write_old_mspi_format(self, dirname, gcp_sigma = 5):
    '''This writes the old MSPI tie-point format. This can be used to
    compare with the old SBA code. The file name is fixed by the
    tiepoint id, but you specify the dirname to put the file. We write
    one file per tiepoint'''
    makedirs_p(dirname)
    for tp in self:
        tp.write_old_mspi_format(dirname, gcp_sigma)

TiePointCollection.write_old_mspi_format = _tpcol_write_old_mspi_format

def _tpcol_extend(self, tparr):
    '''Implement extend for TiePointCollection.'''
    for tp in tparr:
        self.append(tp)
        
TiePointCollection.extend = _tpcol_extend

def _tpcol_data_frame2(self, ref_image = None):
    '''Write the tie-point collection in a pandas table format. This is
    the sort of thing that can be written to a file and read by gnuplot or
    other programs. Missing data is written as NaN.

    Can pass in a reference image to get reference line and sample the
    same as image. This is redundant with the ground location.

    This is more like a data dump, vs the other data_frame function which
    is more looking at the residuals of tie-points. Perhaps at some point
    we can merge these two and just have an option to select the kind of
    data_frame, but for now we just have separate functions.'''
    header = ["ID", "Is_GCP", "Longitude (deg)", "Latitude (deg)",
              "Height (m)"]
    if(ref_image is not None):
        header.extend(["Reference Line", "Reference Sample"])
    for i in range(self[0].number_image):
        header.extend(["Line Image %d" % (i+1),
                       "Sample Image %d" % (i+1),
                       "Line Sigma Image %d" % (i+1),                       
                       "Sample Sigma Image %d" % (i+1)])
    df = pd.DataFrame(data=self.data_array(ref_image),
                      columns=header)
    df = df.astype({'ID': 'int32', 'Is_GCP' : 'int32'})
    return df

TiePointCollection.data_frame2 = _tpcol_data_frame2

__all__ = []
