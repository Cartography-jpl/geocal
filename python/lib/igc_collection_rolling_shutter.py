from igc_collection import *
import cPickle

def _new_from_init(cls, version, *args, **kwargs):
    if(cls.pickle_format_version() < version):
        raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args, **kwargs)
    return inst

class IgcCollectionRollingShutter(IgcCollection):
    '''This is a IgcCollection that uses a IgcRollingShutter. 
    The orbit and camera can be anything that we want to use to model the
    geometry and errors.'''
    def __init__(self, igcarr=None, imglist=None, orbit=None, ttlist=None, 
                 cam=None, dem=None) :
        '''This takes a list of the RasterImage, along with the time
        table associated with each RasterImage. We use the supplied camera
        and orbit.'''
        IgcCollection.__init__(self)
        if(igcarr is not None):
            self.igcarr = igcarr
            return
        self.igcarr = []
        for i in range(len(imglist)):
            self.igcarr.append(IgcRollingShutter(orbit, ttlist[i], cam, dem,
                                                 imglist[i]))

    @classmethod
    def pickle_format_version(cls):
        return 1

    def __reduce__(self):
        return _new_from_init, (self.__class__,
                                self.__class__.pickle_format_version(),
                                {"igcarr" : self.igcarr })

    def _v_parameter_mask(self):
        return self.igcarr[0].parameter_mask

    def _v_parameter(self, *args):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        # Awkward interface, but this matches what the C++ needs. If we have
        # no arguments, then we are returning the parameters. Otherwise,
        # we are setting them.
        if(len(args) == 0):
            return self.igcarr[0].parameter
        else:
            self.igcarr[0].parameter = args[0]
            self.igcarr[0].add_identity_gradient()

    def _v_parameter_name(self):
        '''For longer parameter lists, it can be useful to have a description
        of what each parameter is. This function supplies this.'''
        return self.igcarr[0].parameter_name

    @property
    def number_image(self):
        '''Number of images in this collection.'''
        return len(self.igcarr)

    def image_ground_connection(self, image_index):
        '''Return ImageGroundConnection for the given image'''
        return self.igcarr[image_index]

    def subset(self, *indexset):
        '''Return IgcCollection that contains just the selected 
        image indices'''
        imglist = []
        for i in indexset:
            img = self.image(i)
            img.title = self.title(i)
            img.time = self.time[i]
            imglist.append(img)
        res = IgcOffsetCorrection(imglist, self.cam, self.demv, 
                                  self.orbit.uncorrected_orbit,
                                  time_point = self.orbit.time_point,
                                  refraction = self.refraction)
        res.parameter = self.parameter
        return res

    def ground_coordinate(self, image_index, ic, dem = None):
        '''Return ground coordinates for the given point'''
        return self.image_ground_connection(image_index).ground_coordinate(ic, dem)
    
    def dem(self, image_index):
        '''Return DEM use for given image index'''
        return self.image_ground_connection(image_index).dem

    def image_coordinate(self, image_index, ground_point):
        '''Return image coordinates for the given ground point'''
        return self.image_ground_connection(image_index).image_coordinate(ground_point)

    def image_coordinate_jac_cf(self, image_index, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the CartesianFixed coordinates of the ground point. This can either 
        be a dense matrix or a scip.sparse matrix.'''
        return self.image_ground_connection(image_index).image_coordinate_jac_cf(ground_point)

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
        igc = self.image_ground_connection(image_index)
        jac_igc = igc.image_coordinate_jac_parm(ground_point)
        for j in range(jac_igc.shape[1]):
            jac[jac_row, jac_col + j] = jac_igc[0,j] * line_scale
            jac[jac_row+1, jac_col + j] = jac_igc[1,j] * sample_scale
            
    def title(self, image_index):
        '''Title to use when displaying the given image'''
        return self.image_ground_connection(image_index).title

    def image(self, image_index):
        '''Image corresponding to the given image index.'''
        return self.image_ground_connection(image_index).image

