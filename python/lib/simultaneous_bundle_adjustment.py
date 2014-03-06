import numpy as np
from geocal_swig import *
import scipy

class _coo_helper(object):
    '''scipy.sparse.coo_matrix has an akward interface, but is much, much
    faster than lil_matrix. This helper class gives an interface similar to
    lil_matrix, but really just accumulates everthing for producing a
    coo_matrix.'''
    def __init__(self, shape):
        self.row = []
        self.col = []
        self.data = []
        self.shape = shape

    def __setitem__(self, key, value):
        if(value != 0.0):
            self.row.append(key[0])
            self.col.append(key[1])
            self.data.append(value)

    def coo_matrix(self):
        return scipy.sparse.coo_matrix((self.data, (self.row, self.col)),
                                       shape = self.shape)
    
class SimultaneousBundleAdjustment(object):
    '''This class is used to do a simultaneous bundle adjustment. This
    takes a set of tie points along with a set of ImageGroundConnection.
    We then vary the ground location the tie points and the parameters
    of the ImageGroundConnection to minimize the difference between the
    predicted image locations and the actual image locations given by
    the tiepoints.'''
    def __init__(self, igc_coll, tpcol,
                 dem, dem_sigma = 10, gcp_sigma = 10, tp_epsilon = 1):
        '''Constructor. This takes a IgcCollection, and a 
        TiePointCollection. The camera order used in each TiePoint 
        should match the order of IgcCollection.

        The relative weighting given to the Dem surface constraint can
        be passed in as dem_sigma. We scale the surface constraint equations
        by 1/dem_sigma.

        Likewise, the GCP constraint is scaled by 1/gcp_sigma.

        We calculate the jacobians with respect to moving the tiepoint on
        the surface numerically, the epsilon used for the Ecr coordinates
        can be optionally supplied.
        '''
        self.igc_coll = igc_coll
        self.tpcol = tpcol
        self.dem = dem
        self.dem_sigma = dem_sigma
        self.gcp_sigma = gcp_sigma
        self.tp_epsilon = tp_epsilon
        self.niloc = 0
        for tp in self.tpcol: 
            self.niloc += tp.number_image_location
        self.tp_offset = np.zeros(3 * len(self.tpcol))

        # Determine the slices for each of the parameter types. We have
        # the location each tiepoint first, followed by the parameters for
        # igc_coll.
        self.tp_slice = []
        for i in range(len(self.tpcol)):
            self.tp_slice.append(slice(3 * i, 3 * i + 3))

    @property
    def tp_offset_slice(self):
        '''The portion of parameter that gives the tie point offsets'''
        return slice(0, len(self.tp_offset))

    @property
    def igc_coll_param_slice(self):
        '''The portion of parameter that gives the parameters of igc_coll'''
        return slice(len(self.tp_offset), None)

    @property
    def parameter(self):
        '''This is the list of parameters for the SBA. This contains the
        offset for each tie point, along with the parameters for igc_coll.'''
        return np.append(self.tp_offset, self.igc_coll.parameter_subset)

    @parameter.setter
    def parameter(self, value):
        '''This sets the list of parameters'''
        self.tp_offset[:] = value[self.tp_offset_slice]
        self.igc_coll.parameter_subset = value[self.igc_coll_param_slice]

    def index_to_tp_offset(self, tp_index):
        '''Return the tp offset for the given tie point index number.'''
        return self.tp_offset[self.tp_slice[tp_index]]

    def tp_index_to_parameter_index(self, tp_index):
        '''Return the first index in the parameter for the tp offset
        corresponding to the tiepoint tp_index'''
        return 3 * tp_index

    def ground_location(self, tp_index):
        '''Using the current value of the parameters, determine
        the ground location of the given tie point'''
        # Note subtle memory problem here. We initially did not have
        # gl_ecr. This mean Ecr went out of scope as was deleted before
        # gl was used. Easy work around is to store this is a variable
        gl_ecr = Ecr(self.tpcol[tp_index].ground_location)
        gl = gl_ecr.position
        t = self.index_to_tp_offset(tp_index)
        pt = Ecr(gl[0] + t[0], gl[1] + t[1], gl[2] + t[2])
        return pt

    def sba_eq(self, parm):
        '''Return all the constraints'''
        self.parameter = parm
        return np.append(np.append(np.append(self.surface_constraint(), 
                                             self.gcp_constraint()),
                                   self.collinearity_constraint()),
                         self.parameter_constraint())

    def sba_jacobian(self, parm):
        '''Return Jacobian for SBA equation'''
        self.parameter = parm
        res = _coo_helper((len(self.tpcol) + self.tpcol.number_gcp * 3 +
                           self.niloc * 2 + len(self.parameter), 
                           len(self.parameter)))
        self.row_index = 0
        self.__surface_constraint_jacobian(res)
        self.__gcp_constraint_jacobian(res)
        self.__collinearity_constraint_jacobian(res)
        self.__parameter_constraint_jacobian(res)
        return res.coo_matrix()

    def gcp_constraint(self):
        '''Calculate the GCP constraint equations. This is the penalty
        we get from placing a tiepoint somewhere other than the location
        given by the GCP'''
        res = np.empty(3 * self.tpcol.number_gcp)
        i = 0
        for tp_index, tp in enumerate(self.tpcol):
            if(tp.is_gcp):
                res[i:(i+3)] = (self.index_to_tp_offset(tp_index) / 
                                self.gcp_sigma)
                i += 3
        return res

    def __gcp_constraint_jacobian(self, res):
        '''Calculate the Jacobian of the GCP constraint equations. Takes
        matrix to fill in, and row to start at. The row gets updated to
        just past the end of what we fill out'''
        for tp_index, tp in enumerate(self.tpcol):
            if(tp.is_gcp):
                pind = self.tp_index_to_parameter_index(tp_index)
                for j in range(3):
                    res[self.row_index + j, pind + j] = 1.0 / self.gcp_sigma
                self.row_index += 3

    def surface_constraint(self):
        '''Calculate all the surface constraint equations. This is
        the penalty we get from placing a tiepoint somewhere other than
        the surface given by the Dem.'''
        res = np.zeros(len(self.tpcol))
        for i in range(len(self.tpcol)):
            res[i] = self.dem.distance_to_surface(self.ground_location(i))
        res /= self.dem_sigma
        return res
    
    def __surface_constraint_jacobian(self, res):
        '''Calculate the Jacobian of the surface constraint equations. Takes
        matrix to fill in, and row to start at. The row gets updated to
        just past the end of what we fill out'''
        for i in range(len(self.tpcol)):
            gp = self.ground_location(i)
            pind = self.tp_index_to_parameter_index(i)
            x0 = self.dem.distance_to_surface(gp) / self.dem_sigma
            p0 = gp.position
            for j in range(3):
                p = p0.copy()
                p[j] += self.tp_epsilon
                gp.position = p
                x = self.dem.distance_to_surface(gp) / self.dem_sigma
                res[self.row_index + i, pind + j] = (x - x0) / self.tp_epsilon
            gp.position = p0
        self.row_index += len(self.tpcol)
        
    def collinearity_constraint(self):
        '''Calculate the collinearity constraint equations. This is
        difference between the predicted location in each image vs.
        the actual location.'''
        res = np.zeros(self.niloc * 2)
        ind = 0
        for i, tp in enumerate(self.tpcol):
            gp = self.ground_location(i)
            for j, il in enumerate(tp.image_location):
                if(il):
                    ic = self.igc_coll.image_coordinate(j, gp)
                    ictp, lsigma, ssigma = il
                    res[ind] = (ictp.line - ic.line) / lsigma
                    res[ind + 1] = (ictp.sample - ic.sample) / ssigma
                    ind += 2
        return res

    def __collinearity_constraint_jacobian(self, res):
        '''Calculate the Jacobian of the collinearity constraint equations.'''
        ind = self.row_index
        for i, tp in enumerate(self.tpcol):
            gp = self.ground_location(i)
            for j, il in enumerate(tp.image_location):
                if(il):
                    ictp, lsigma, ssigma = il
                    jac = self.igc_coll.image_coordinate_jac_ecr(j, gp)
                    # We have "-" because equation if measured - predicted
                    ts = self.tp_slice[i].start
                    for k in range(3):
                        res[ind, ts + k] = -jac[0,k] / lsigma
                        res[ind + 1, ts + k] = -jac[1,k] / ssigma
                    self.igc_coll.image_coordinate_jac_parm(j, gp, res, ind,
                            self.igc_coll_param_slice.start, -1.0 / lsigma,
                            -1.0 / ssigma)
                    ind += 2
        self.row_index = ind

    def parameter_constraint(self):
        '''Calculate the parameter constraint. This is the penalty for
        moving a particular parameter from its initial value.'''
        # This needs to be scaled, but for now just do this
        return np.array(self.parameter) / 10.0
    
    def __parameter_constraint_jacobian(self, res):
        istart = (len(self.tpcol) + self.niloc * 2)
        for i in range(len(self.parameter)):
            res[istart + i, i] = 1 / 10.0
