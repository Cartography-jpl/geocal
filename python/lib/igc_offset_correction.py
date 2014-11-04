from igc_collection import *
import cPickle

def _new_from_init(cls, version, *args):
    if(cls.pickle_format_version() < version):
        raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

class IgcOffsetCorrection(IgcCollection):
    '''This is a IgcCollection that uses a OrbitDataImageGroundConnection. 
    The orbit and camera can be anything that we want to use to model the
    geometry and errors.'''
    def __init__(self, imglist, cam, dem, orbit, 
                 refraction = None,
                 image_time = None, image_title = None):
        '''This takes a list of the RasterImage, along with the times
        associated with each RasterImage. We use the supplied camera
        and uncorrected orbit.

        You can either supply an existing Refraction object, or we will 
        create one with an estimate of the index of refraction at the 
        surface. 

        The RasterImage should have the additional attributes "time"
        and "title" added to it, or alternatively you can supply the
        time and title as a separate list.
        
        The orbit is an OrbitOffsetCorrection.'''
        IgcCollection.__init__(self)
        self.imglist = imglist
        self.cam = cam
        self.demv = dem
        if(image_time is None):
            self.time = map(lambda x : x.time, self.imglist)
        else:
            self.time = image_time
        if(image_title is None):
            self.title = map(lambda x : x.title, self.imglist)
        else:
            self.title = image_title
        self.orbit = orbit
        if(refraction):
            self.refraction = refraction
        else:
            t = self.time[0]
            t2 = OrbitDataImageGroundConnection(self.orbit.orbit_data(t),
                                                self.cam, self.demv,
                                                self.image(0), 
                                                self.image_title(0))
            gc = t2.ground_coordinate(ImageCoordinate(cam.number_line(0) / 2.0,
                                                    cam.number_sample(0) / 2.0))
            self.refraction = Refraction(gc.height_reference_surface,
                                         gc.latitude)
        self._igc_cache = [None] * self.number_image

    @classmethod
    def pickle_format_version(cls):
        return 1

    def __reduce__(self):
        return _new_from_init, (self.__class__,
                                self.__class__.pickle_format_version(),
                                self.imglist, 
                                self.cam,
                                self.demv,
                                self.orbit,
                                self.refraction,
                                self.time,
                                self.title)

    def _v_parameter_mask(self):
        return np.append(self.orbit.parameter_mask, self.cam.parameter_mask)

    def _v_parameter(self, *args):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        # Awkward interface, but this matches what the C++ needs. If we have
        # no arguments, then we are returning the parameters. Otherwise,
        # we are setting them.
        if(len(args) == 0):
            return np.append(self.orbit.parameter, self.cam.parameter)
        else:
            value = args[0]
            # Set parameters, and also include the Jacobian for the parameters.
            # This is just the identity matrix
            olen = len(self.orbit.parameter)
            omask = self.orbit.parameter_mask
            olen_nvar = np.count_nonzero(omask)
            clen = len(self.cam.parameter)
            cmask = self.cam.parameter_mask
            clen_nvar = np.count_nonzero(cmask)
            ojac = np.zeros((olen, olen_nvar + clen_nvar))
            cjac = np.zeros((clen, olen_nvar + clen_nvar))
            ind = 0
            for i in range(olen):
                if(omask[i]):
                    ojac[i,ind] = 1
                    ind += 1
            for i in range(clen):
                if(cmask[i]):
                    cjac[i,ind] = 1
                    ind += 1
            self.orbit.parameter_with_derivative = \
                ArrayAd_double_1(value[0:olen], ojac)
            self.cam.parameter_with_derivative = \
                ArrayAd_double_1(value[olen:(olen+clen)], cjac)

    def _v_parameter_name(self):
        '''For longer parameter lists, it can be useful to have a description
        of what each parameter is. This function supplies this.'''
        t = self.orbit.parameter_name
        t.extend(self.cam.parameter_name)
        return t

    @property
    def number_image(self):
        '''Number of images in this collection.'''
        return len(self.imglist)

    def image_ground_connection(self, image_index):
        '''Return ImageGroundConnection for the given image'''
        if(self._igc_cache[image_index] is None):
            t = self.time[image_index]
            res = OrbitDataImageGroundConnection(self.orbit, t,
                                                 self.cam, self.demv, 
                                                 self.image(image_index),
                                                 self.image_title(image_index),
                                                 self.refraction)
            self._igc_cache[image_index] = res
        return self._igc_cache[image_index]

    def subset(self, *indexset):
        '''Return IgcCollection that contains just the selected 
        image indices'''
        imglist = []
        for i in indexset:
            img = self.image(i)
            img.title = self.image_title(i)
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
        return self.demv

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
        jac[jac_row, jac_col:(jac_col + jac_igc.shape[1])] = \
            jac_igc[0,:] * line_scale
        jac[jac_row+1, jac_col:(jac_col + jac_igc.shape[1])] = \
            jac_igc[1,:] * sample_scale
            
    def image_title(self, image_index):
        '''Title to use when displaying the given image'''
        return self.title[image_index]

    def image(self, image_index):
        '''Image corresponding to the given image index.'''
        return self.imglist[image_index]

