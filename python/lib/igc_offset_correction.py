from igc_collection import *
from orbit_offset_correction import *
import cPickle

class IgcOffsetCorrection(IgcCollection):
    '''This is a IgcCollection that uses a OrbitOffsetCorrection as the
    underlying error model.'''
    def __init__(self, imglist, cam, dem, uncorr_orb, 
                 time_point = [], refraction = None,
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
        
        The orbit is an OrbitOffsetCorrection. You can either pass the
        time points to use in the attitude correction to this constructor,
        or you can just edit self.orbit however you like.'''
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
        self.orbit = OrbitOffsetCorrection(uncorr_orb, time_point)
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
        self._jac_cache = [None] * self.number_image
        self.fit_refraction = False
        self.fit_scale = False

    @property
    def fit_position(self):
        return self.orbit.fit_position

    @fit_position.setter
    def fit_position(self, val):
        self.orbit.fit_position = val

    @property
    def fit_yaw(self):
        return self.orbit.fit_yaw

    @fit_yaw.setter
    def fit_yaw(self, val):
        self.orbit.fit_yaw = val

    @property
    def fit_pitch(self):
        return self.orbit.fit_pitch

    @fit_pitch.setter
    def fit_pitch(self, val):
        self.orbit.fit_pitch = val

    @property
    def fit_roll(self):
        return self.orbit.fit_roll

    @fit_roll.setter
    def fit_roll(self, val):
        self.orbit.fit_roll = val

    def __getstate__(self):
        # Don't include cache in pickling
        res = self.__dict__.copy()
        del res["_igc_cache"]
        del res["_jac_cache"]
        return res
    
    def __setstate__(self, dict):
        # Add empty cache when we unpickle
        self.__dict__ = dict
        self._igc_cache = [None] * self.number_image
        self._jac_cache = [None] * self.number_image

    @property
    def parameter(self):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        par = [self.cam.line_scale, self.cam.sample_scale, 
               (self.refraction.index_refraction_surface - 1.00027) * 1e5]
        return np.append(self.orbit.parameter, par) 

    @parameter.setter
    def parameter(self, value):
        '''Value of parameters controlling mapping to and from image 
        coordinates'''
        self._igc_cache = [None] * self.number_image
        self.orbit.parameter = value[0:-3]
        scale_par = [value[-3], value[-2]]
        ref_par = value[-1]
        self.cam.line_scale = scale_par[0]
        self.cam.sample_scale = scale_par[1]
        self.refraction.index_refraction_surface = 1.00027 + 1e-5 * ref_par

    @property
    def parameter_name(self):
        '''For longer parameter lists, it can be useful to have a description
        of what each parameter is. This function supplies this.'''
        t = self.orbit.parameter_name
        t.append("Camera line scale")
        t.append("Camera sample scale")
        t.append("Refraction factor")
        return t
    
    @property
    def parameter_subset_mask(self):
        '''This returns a list of flags indicating which parameters should
        be included in the parameter_subset values.'''
        mask = [self.fit_scale, self.fit_scale,
                self.fit_refraction]
        return np.append(self.orbit.parameter_subset_mask, mask)

    @property
    def number_image(self):
        '''Number of images in this collection.'''
        return len(self.imglist)

    def image_ground_connection(self, image_index):
        '''Return ImageGroundConnection for the given image'''
        if(self._igc_cache[image_index] is None):
            t = self.time[image_index]
            res = OrbitDataImageGroundConnection(self.orbit.orbit_data(t), 
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

    def image_coordinate_jac_ecr(self, image_index, ground_point):
        '''Return Jacobian for image coordinate with respect to change in 
        the ECR coordinates of the ground point. This can either be a 
        dense matrix or a scip.sparse matrix.'''
        cache = self.__jacobian_cache(image_index)
        return cache["igc0"].image_coordinate_jac_ecr(ground_point)

    def __jacobian_cache(self, image_index):
        '''Cache stuff we use for calculating the jacobian.'''
        if(self._jac_cache[image_index] is None):
            att_eps = 10
            pos_eps = 1 
            tm = self.time[image_index]
            orb = self.orbit
            cache = {}
            cache["igc0"] = OrbitDataImageGroundConnection(orb.orbit_data(tm),
                                         self.cam, self.demv,
                                         self.image(image_index),
                                         self.image_title(image_index), 
                                         self.refraction)
            p0 = orb.parameter.copy()
            cache["igc"] = []
            for i in range(3):
                j = self.parameter_index_to_subset_index(i)
                if(j is not None):
                    orb.parameter = p0.copy()
                    orb.parameter[i] += pos_eps
                    cache["igc"].append((j,OrbitDataImageGroundConnection
                                         (orb.orbit_data(tm), self.cam, 
                                          self.demv,
                                          self.image(image_index),
                                          self.image_title(image_index), 
                                          self.refraction),
                                         pos_eps))
            for i in orb.att_parameters_affect(tm):
                j = self.parameter_index_to_subset_index(i)
                if(j is not None):
                    orb.parameter = p0.copy()
                    orb.parameter[i] += att_eps
                    cache["igc"].append((j,OrbitDataImageGroundConnection
                                         (orb.orbit_data(tm), self.cam, 
                                          self.demv,
                                          self.image(image_index),
                                          self.image_title(image_index),
                                          self.refraction),
                                         att_eps))
            orb.parameter = p0
            line_scale_index = len(p0) + 0
            sample_scale_index = len(p0) + 1
            ref_index = len(p0) + 2
            j = self.parameter_index_to_subset_index(line_scale_index)
            if(j is not None):
                scale_eps = 0.01
                # Duplicate camera through pickling
                cam = cPickle.loads(cPickle.dumps(self.cam))
                cam.line_scale = cam.line_scale + scale_eps
                cache["igc"].append((j,OrbitDataImageGroundConnection
                                     (orb.orbit_data(tm), cam, 
                                      self.demv,
                                      self.image(image_index),
                                      self.image_title(image_index),
                                      self.refraction),
                                     scale_eps))
            j = self.parameter_index_to_subset_index(sample_scale_index)
            if(j is not None):
                scale_eps = 0.01
                # Duplicate camera through pickling
                cam = cPickle.loads(cPickle.dumps(self.cam))
                cam.sample_scale = cam.sample_scale + scale_eps
                cache["igc"].append((j,OrbitDataImageGroundConnection
                                     (orb.orbit_data(tm), cam, 
                                      self.demv,
                                      self.image(image_index),
                                      self.image_title(image_index),
                                      self.refraction),
                                     scale_eps))
            j = self.parameter_index_to_subset_index(ref_index)
            if(j is not None):
                ref_eps = 0.1
                ref = Refraction(self.refraction.altitude,
                                 self.refraction.latitude,
                                 self.refraction.index_refraction_surface +
                                 ref_eps * 1e-5)
                cache["igc"].append((j,OrbitDataImageGroundConnection
                                     (orb.orbit_data(tm), self.cam, 
                                      self.demv,
                                      self.image(image_index),
                                      self.image_title(image_index),
                                      ref),
                                     ref_eps))
            self._jac_cache[image_index] = cache
        return self._jac_cache[image_index]

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
        cache = self.__jacobian_cache(image_index)
        ic0 = cache["igc0"].image_coordinate(ground_point)
        for i,igc,eps in cache["igc"]:
            ic = igc.image_coordinate(ground_point)
            jac[jac_row, jac_col + i] = \
                (ic.line - ic0.line) / eps * line_scale
            jac[jac_row + 1, jac_col + i] = \
                (ic.sample - ic0.sample) / eps * sample_scale

    def image_title(self, image_index):
        '''Title to use when displaying the given image'''
        return self.title[image_index]

    def image(self, image_index):
        '''Image corresponding to the given image index.'''
        return self.imglist[image_index]

