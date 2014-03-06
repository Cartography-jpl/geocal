from abc import *
from geocal_swig import *
from with_parameter import *
import numpy as np
import scipy.sparse

class IgcCollection(WithParameter):
    '''This class describes a collection of ImageGroundConnection. This
    particular class is a abstract base class. This is used to describe
    what needs to be in the interface. Our python code uses duck typing, 
    so as long as you supply the interface given in this class is it not
    necessary to actually inherit from this class (although you can if
    that is useful).

    This is used by the SimultaneousBundleAdjustment class.
    '''

    @abstractproperty
    def number_image(self):
        '''Number of images in this collection.'''
        pass

    @abstractmethod
    def ground_coordinate(self, image_index, ic, dem = None):
        '''Return ground coordinates for the given point'''
        pass
    
    @abstractmethod
    def dem(self, image_index):
        '''Return DEM use for given image index'''
        pass

    @abstractmethod
    def image_coordinate(self, image_index, ground_point):
        '''Return image coordinates for the given ground point'''
        pass
    @abstractmethod
    def image_coordinate_jac_ecr(self, image_index, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the ECR coordinates of the ground point. This can either be a 
        dense matrix or a scip.sparse matrix.'''
        pass
    @abstractmethod
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
        pass
    @abstractmethod
    def image_title(self, image_index):
        '''Title to use when displaying the given image'''
        pass

    @abstractmethod
    def image(self, image_index):
        '''Image corresponding to the given image index.'''
        pass

    @abstractmethod
    def image_ground_connection(self, image_index):
        '''Return ImageGroundConnection for the given image'''

    @abstractmethod
    def subset(self, *indexset):
        '''Return IgcCollection that contains just the selected 
        image indices'''

class ImageGroundConnectionIgc(ImageGroundConnection, WithParameter):
    '''This wraps a IgcCollection with a particular image index into
    a ImageGroundConnection'''
    def __init__(self, igc_collection, image_index):
        ImageGroundConnection.__init__(self, igc_collection.dem(image_index),
                                       igc_collection.image(image_index),
                                       None,
                                       igc_collection.image_title(image_index))
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
        
    def image_coordinate_jac_ecr(self, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the ECR coordinates of the ground point. This can either be a 
        dense matrix or a scip.sparse matrix.'''
        return self.igc_collection.image_coordinate_jac_ecr(self.image_index, 
                                                            ground_point)

    @property
    def parameter(self):
        return []

    @parameter.setter
    def parameter(self, value):
        pass

    @property
    def parameter_subset_mask(self):
        return np.array([True] * len(self.parameter))

    def image_coordinate_jac_parm(self, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the parameters of the ground point. This can either be a 
        dense matrix or a scip.sparse matrix.'''
        return np.array([[]])

class IgcArray(IgcCollection):
    '''This is a simple implementation of IgcCollection where we forward
    everything to an array of ImageGroundConnection. You should fill in
    the attribute "igc" as a array of these values.

    The ImageGroundConnection should have the attributes "image" and
    "title" filled in. This isn't present in the underlying C++ 
    ImageGroundConnection class, but python classes such as 
    GdalImageGroundConnection and VicarImageGroundConnection do have these.'''
    def __init__(self, initial_data = []):
        self.igc = initial_data
        self._parameter_subset_mask = [True] * len(self.parameter)

    def __str__(self):
        res =  "IgcArray\n"
        res += "  Number of images: %d\n" % self.number_image
        res += "  Images:\n"
        for i in range(self.number_image):
            res += "     %s\n" % self.image_title(i)
        res += "  Parameters:\n"
        for i in range(len(self.parameter)):
            res += "     %s: %f\n" % (self.parameter_name[i], self.parameter[i])
        return res

    @property
    def parameter(self):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        res = []
        for v in self.igc:
            res = np.append(res, v.parameter)
        return res

    @parameter.setter
    def parameter(self, value):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        pstart = 0
        for ig in self.igc:
            pl = len(ig.parameter)
            ig.parameter = value[pstart:(pstart + pl)]
            pstart += pl

    @property
    def parameter_subset_mask(self):
        return self._parameter_subset_mask

    @parameter_subset_mask.setter
    def parameter_subset_mask(self, val):
        self._parameter_subset_mask = val

    @property
    def parameter_name(self):
        res = []
        for i, igcv in enumerate(self.igc):
            for pn in igcv.parameter_name:
                res.append(("Igc %d - " % i) + pn)
        return res

    @property
    def number_image(self):
        '''Number of images in this collection.'''
        return len(self.igc)

    def ground_coordinate(self, image_index, ic, dem = None):
        '''Return ground coordinates for the given point'''
        return self.igc[image_index].ground_coordinate(ic, dem)

    def image_coordinate(self, image_index, ground_point):
        '''Return image coordinates for the given ground point'''
        return self.igc[image_index].image_coordinate(ground_point)

    def image_coordinate_jac_ecr(self, image_index, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the ECR coordinates of the ground point. This can either be a 
        dense matrix or a scip.sparse matrix.'''
        return self.igc[image_index].image_coordinate_jac_ecr(ground_point)

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
        nparm = 0
        jparm = 0
        pstart = -1
        pend = -1
        pm = self.parameter_subset_mask
        for i, ig in enumerate(self.igc):
            pl = len(ig.parameter)
            jl = pm[nparm:(nparm + pl)].count(True)
            if(i == image_index):
                pstart = jparm + jac_col
                pmsub = pm[nparm:(nparm + pl)]
            nparm += pl
            jparm += jl
        jac_in = self.igc[image_index].image_coordinate_jac_parm(ground_point)
        col = pstart
        for (i, pmval) in enumerate(pmsub):
            if(pmval):
                jac[jac_row, col] = jac_in[0, i] * line_scale
                jac[jac_row + 1, col] = jac_in[1, i] * sample_scale
                col += 1

    def image_title(self, image_index):
        '''Title to use when displaying the given image'''
        return self.igc[image_index].title

    def image(self, image_index):
        '''Image corresponding to the given image index.'''
        return self.igc[image_index].image

    def dem(self, image_index):
        '''Return DEM use for given image index'''
        return self.igc[image_index].dem

    def image_ground_connection(self, image_index):
        '''Return ImageGroundConnection for the given image'''
        return self.igc[image_index]

    def subset(self, *indexset):
        '''Return IgcCollection that contains just the selected 
        image indices'''
        res = IgcArray()
        for i in indexset:
            res.igc.append(self.image_ground_connection(i))
        return res
