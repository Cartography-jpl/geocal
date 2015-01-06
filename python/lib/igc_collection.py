from abc import *
from geocal_swig import *
import numpy as np
import scipy.sparse
import copy


def _new_from_init(cls, version, *args):
    if(cls.pickle_format_version() < version):
        raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

# Add image_coordinate_jac to IgcCollection. We can't define this in C++
# because we want to use the python sparse matrix which isn't available in C++
def _image_coordinate_jac_parm(self, image_index, ground_point, jac,
                               jac_row, jac_col, line_scale = 1, 
                               sample_scale = 1):
    '''Update Jacobian for image coordinate with respect to change in 
    the parameters of the ground point. 

    We pass in the Jacobian to fill in along with the row and column for
    the start of the submatrix we are filling in. We also optionally pass
    in a scaling for the line and sample entries. This interface may seem
    a bit odd, but this is what we have in the SimultaneousBundleAdjustment
    that uses this call.'''
    raise NotImplementedError("Subclass should implement this method")

IgcCollection.image_coordinate_jac_parm = _image_coordinate_jac_parm

class ImageGroundConnectionIgc(ImageGroundConnection, WithParameter):
    '''This wraps a IgcCollection with a particular image index into
    a ImageGroundConnection'''
    def __init__(self, igc_collection, image_index):
        ImageGroundConnection.__init__(self, igc_collection.dem(image_index),
                                       igc_collection.image(image_index),
                                       None,
                                       igc_collection.title(image_index))
        self.igc_collection = igc_collection
        self.image_index = image_index

    def ground_coordinate_dem(self, ic, dem):
        '''Return ground coordinates for the given point'''
        return self.igc_collection.ground_coordinate(self.image_index, ic, 
                                                     dem = dem)

    def image_coordinate(self, ground_point):
        '''Return image coordinates for the given ground point'''
        return self.igc_collection.image_coordinate(self.image_index, 
                                                    ground_point)
        
    def image_coordinate_jac_cf(self, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the CartesianFixed coordinates of the ground point. This can either 
        be a dense matrix or a scip.sparse matrix.'''
        return self.igc_collection.image_coordinate_jac_cf(self.image_index, 
                                                            ground_point)

    def image_coordinate_jac_parm(self, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the parameters of the ground point. This can either be a 
        dense matrix or a scip.sparse matrix.'''
        return np.array([[]])

class IgcArray(IgcCollection):
    '''This is a simple implementation of IgcCollection where we forward
    everything to an array of ImageGroundConnection. You should fill in
    the attribute "igc" as a array of these values.

    igc can also contain zero or more IgcCollection. We treat this like 
    a stack of ImageGroundConnection. So if igccol1 contains 2 images, igc2
    is a ImageGroundConnection, and igccol3 contains 3 images then the first 
    image is the first image of igccol1, the second is the second image of 
    igccol1, the third is igc2, and the fourth is the first image of iggcol3.

    The ImageGroundConnection should have the attributes "image" and
    "title" filled in. This isn't present in the underlying C++ 
    ImageGroundConnection class, but python classes such as 
    GdalImageGroundConnection and VicarImageGroundConnection do have these.'''
    def __init__(self, initial_data = []):
        self.igc = copy.copy(initial_data)
        IgcCollection.__init__(self)

    @classmethod
    def pickle_format_version(cls):
        return 1

    def __reduce__(self):
        return _new_from_init, (self.__class__,
                                self.__class__.pickle_format_version(),
                                self.igc)

    def __str__(self):
        res =  "IgcArray\n"
        res += "  Number of images: %d\n" % self.number_image
        res += "  Images:\n"
        for i in range(self.number_image):
            res += "     %s\n" % self.title(i)
        res += "  Parameters:\n"
        for i in range(len(self.parameter_subset)):
            res += "     %s: %f\n" % (self.parameter_name_subset[i], 
                                      self.parameter_subset[i])
        return res

    def _v_parameter_mask(self):
            res = []
            for v in self.igc:
                res = np.append(res, v.parameter_mask)
            return res
        
    def _v_parameter(self, *args):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        # Awkward interface, but this matches what the C++ needs. If we have
        # no arguments, then we are returning the parameters. Otherwise,
        # we are setting them.
        if(len(args) == 0):
            res = []
            for v in self.igc:
                res = np.append(res, v.parameter)
            return res
        else:
            value = args[0]
            pstart = 0
            for ig in self.igc:
                pl = len(ig.parameter)
                ig.parameter = value[pstart:(pstart + pl)]
                pstart += pl

    def _v_parameter_name(self):
        res = []
        for i, igcv in enumerate(self.igc):
            for pn in igcv.parameter_name:
                res.append(("Igc %d - " % i) + pn)
        return res

    @property
    def number_image(self):
        '''Number of images in this collection.'''
        res = 0
        for ig in self.igc:
            if(isinstance(ig, ImageGroundConnection)):
                res += 1
            else:
                res += ig.number_image
        return res

    def _igc_or_coll(self, image_index):
        '''Return ImageGroundConnection or IgcCollection for the given 
        image_index. Also return the index into the IgcCollection, or -1'''
        i = 0
        if(image_index < 0):
            raise IndexError("image_index out of range")
        for ig in self.igc:
            if(isinstance(ig, ImageGroundConnection)):
                if(i == image_index):
                    return ig, -1
                i += 1
            else:
                if(image_index < i + ig.number_image):
                    return ig, image_index - i
                i += ig.number_image
        raise IndexError("image_index out of range")

    def _igc_or_coll_call(self, image_index, func_name, *args):
        '''Call the given function with the given arguments for
        the given image_index.'''
        ig, i = self._igc_or_coll(image_index)
        if(i < 0):
            return getattr(ig, func_name)(*args)
        return getattr(ig, func_name)(i, *args)

    def ground_coordinate(self, image_index, ic, dem = None):
        '''Return ground coordinates for the given point'''
        return self._igc_or_coll_call(image_index, "ground_coordinate", 
                                      ic, dem)

    def image_coordinate(self, image_index, ground_point):
        '''Return image coordinates for the given ground point'''
        return self._igc_or_coll_call(image_index, "image_coordinate", 
                                      ground_point)

    def image_coordinate_jac_cf(self, image_index, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the CartesianFixed coordinates of the ground point. This can either 
        be a dense matrix or a scip.sparse matrix.'''
        return self._igc_or_coll_call(image_index, "image_coordinate_jac_cf", 
                                      ground_point)

    def image_coordinate_jac_parm(self, image_index, ground_point, jac,
                                  jac_row, jac_col, line_scale = 1, 
                                  sample_scale = 1):
        '''Update Jacobian for image coordinate with respect to change in 
        the parameters of the ground point. 

        We pass in the Jacobian to fill in along with the row and column for
        the start of the submatrix we are filling in. We also optionally pass
        in a scaling for the line and sample entries. This interface may seem
        a bit odd, but this is what we have in the SimultaneousBundleAdjustment
        that uses this call.'''
        jparm = 0
        i = 0
        for ig in self.igc:
            pl = len(ig.parameter_subset)
            if(isinstance(ig, ImageGroundConnection)):
                i += 1
            else:
                i += ig.number_image
            if(image_index < i):
                pstart = jparm + jac_col
                break
            jparm += pl
        ig, i = self._igc_or_coll(image_index)
        if(i < 0):
            jac_in = \
                self.igc[image_index].image_coordinate_jac_parm(ground_point)
            for j in range(jac_in.shape[1]):
                jac[jac_row, pstart + j] = jac_in[0, j] * line_scale
                jac[jac_row + 1, pstart + j] = jac_in[1, j] * sample_scale
        else:
            ig.image_coordinate_jac_parm(i, ground_point, jac, jac_row, pstart,
                                         line_scale, sample_scale)

    def title(self, image_index):
        '''Title to use when displaying the given image'''
        ig, i = self._igc_or_coll(image_index)
        if(i < 0):
            return ig.title
        return ig.title(i)

    def image(self, image_index):
        '''Image corresponding to the given image index.'''
        ig, i = self._igc_or_coll(image_index)
        if(i < 0):
            return ig.image
        return ig.image(i)

    def dem(self, image_index):
        '''Return DEM use for given image index'''
        ig, i = self._igc_or_coll(image_index)
        if(i < 0):
            return ig.dem
        return ig.dem(i)

    def image_ground_connection(self, image_index):
        '''Return ImageGroundConnection for the given image'''
        ig, i = self._igc_or_coll(image_index)
        if(i < 0):
            return ig
        return ig.image_ground_connection(i)

    def subset(self, *indexset):
        '''Return IgcCollection that contains just the selected 
        image indices'''
        res = IgcArray()
        for i in indexset:
            res.igc.append(self.image_ground_connection(i))
        return res
