from builtins import str
from builtins import range
from builtins import object
import numpy as np
from geocal_swig import *
import scipy


class _coo_helper(object):
    """scipy.sparse.coo_matrix has an akward interface, but is much, much
    faster than lil_matrix. This helper class gives an interface similar to
    lil_matrix, but really just accumulates everthing for producing a
    coo_matrix."""

    def __init__(self, shape):
        self.row = []
        self.col = []
        self.data = []
        self.shape = shape

    def __setitem__(self, key, value):
        if type(key[0]) is slice:
            r0 = list(range(*key[0].indices(self.shape[0])))
            if type(key[1]) is slice:
                r1 = list(range(*key[1].indices(self.shape[1])))
                for i1, i2 in enumerate(r0):
                    for j1, j2 in enumerate(r1):
                        if value[i1, j1] != 0.0:
                            self.row.append(i2)
                            self.col.append(j2)
                            self.data.append(value[i1, j1])
            else:
                for i1, i2 in enumerate(r0):
                    if value[i1] != 0.0:
                        self.row.append(i2)
                        self.col.append(key[1])
                        self.data.append(value[i1])
        else:
            if type(key[1]) is slice:
                r1 = list(range(*key[1].indices(self.shape[1])))
                for j1, j2 in enumerate(r1):
                    if value[j1] != 0.0:
                        self.row.append(key[0])
                        self.col.append(j2)
                        self.data.append(value[j1])
            else:
                if value != 0.0:
                    self.row.append(key[0])
                    self.col.append(key[1])
                    self.data.append(value)

    def coo_matrix(self):
        return scipy.sparse.coo_matrix(
            (self.data, (self.row, self.col)), shape=self.shape
        )


class SimultaneousBundleAdjustment(object):
    """This class is used to do a simultaneous bundle adjustment. This
    takes a set of tie points along with a set of ImageGroundConnection.
    We then vary the ground location the tie points and the parameters
    of the ImageGroundConnection to minimize the difference between the
    predicted image locations and the actual image locations given by
    the tiepoints.

    Note that you often want to use the automatically calculated Jacobians
    (done using the chain rule). However, there are cases where you do
    *not* want to do this.

    We initially ran into this with the IpiImageGroundConnection
    can have points where the Jacobian is very noisy, so small changes in
    a parameter can cause a relatively large change in the line/sample (e.g.,
    a minor tweak causes the line/sample to jump by 0.1 or 0.2 pixels). These
    changes are small in relation to anything that really matters (e..g, 1/3
    pixel), but can cause very large spikes in the Jacobian. For these cases,
    you may want to use a finite difference approximation, where the step
    size is something like what you expect meaningful parameter change to be.
    This has the effect of replacing the real Jacobian with the Jacobian of
    smoothed version of the Ipi equations w/o these sudden jumps. Not sure
    when you might want to do this, perhaps a good test if you aren't sure
    is to compare the real Jacobain with the finite difference one and if they
    are close to each other you can use the real Jacobian. I've never seen
    an issue with a frame camera, just with the Ipi for a push broom (and in
    that case for a aircraft which has much larger jumps in attitude).

    Note that the IpiImageGroundConnection problem was solved in a
    different manner (adding collinearity constraint that uses FrameCoor at
    a fixed time), so the finite difference is not needed for this particular
    ImageGroundConnection. But we've left the finite difference in place if
    this is useful in other circumstances.

    To support the finite difference, you can supply the step size to use the
    collinearity ECR jacobian, and/or a separate array for the step size to
    use in the collinearity parameter jacobian. The other jacobians (GCP,
    surface, parameter) are all simple enough that there is never any reason
    to resort to finite differences for these.
    """

    def __init__(
        self,
        igc_coll,
        tpcol,
        dem,
        dem_sigma=10,
        gcp_sigma=10,
        tp_sigma=None,
        tp_epsilon=1,
        p0=None,
        psigma=None,
        hold_gcp_fixed=False,
        ecr_fd_step_size=None,
        parameter_fd_step_size=None,
    ):
        """Constructor. This takes a IgcCollection, and a
        TiePointCollection. The camera order used in each TiePoint
        should match the order of IgcCollection.

        The relative weighting given to the Dem surface constraint can
        be passed in as dem_sigma. We scale the surface constraint equations
        by 1/dem_sigma.

        Likewise, the GCP constraint is scaled by 1/gcp_sigma.

        By default, the location of the tiepoints (which aren't GCPs) is
        not constrained at all. But it can be useful in some case to loosely
        constrain this (e.g., have a sigma that is 10 * the GCP sigma) to
        prevent the points from being moved too far. If you supply a
        tp_sigma, then we add a GCP constraint equation for points that
        aren't marked as GCPs that is this looser value.

        You can optionally hold GCP completely fixed, not allowing them to be
        moved at all. This can be useful in some cases where the SBA is
        moving the GCPs too far to account for other errors, particularly
        when the GCP locations are more certain than other sources of errors.

        We calculate the jacobians with respect to moving the tiepoint on
        the surface numerically, the tp_epsilon used for the CartesianFixed
        coordinates can be optionally supplied.

        The parameters are constrained to p0, with a scaling of psigma. If
        these aren't specified the initial value of the igc_coll parameter
        is used, with a scaling of 10. You can change this after the
        constructor by changing self.p0 and/or self.psigma.

        You can optionally supply the step size to use for a finite differene
        jacobian of the collinearity ecr and/or the collinearity parameter
        jacobian. See the class description for why you might want to use
        finite difference instead of the true jacobian calculation in some
        cases.
        """
        self.igc_coll = igc_coll
        self.tpcol = tpcol
        self.dem = dem
        self.dem_sigma = dem_sigma
        self.gcp_sigma = gcp_sigma
        self.hold_gcp_fixed = hold_gcp_fixed
        self.tp_epsilon = tp_epsilon
        self.tp_sigma = tp_sigma
        self.ecr_fd_step_size = ecr_fd_step_size
        self.parameter_fd_step_size = parameter_fd_step_size
        if self.ecr_fd_step_size is not None or self.parameter_fd_step_size is not None:
            raise RuntimeError("Don't currently support finite difference jacobians")
        if p0 is None:
            self.p0 = self.igc_coll.parameter_subset.copy()
        else:
            self.p0 = p0
        if psigma is None:
            self.psigma = np.zeros(self.p0.shape)
            self.psigma[:] = 10
        else:
            self.psigma = psigma
        self.niloc = 0
        for tp in self.tpcol:
            self.niloc += tp.number_image_location
        if self.hold_gcp_fixed:
            self.tp_offset = np.zeros(3 * (len(self.tpcol) - self.tpcol.number_gcp))
        else:
            self.tp_offset = np.zeros(3 * len(self.tpcol))

        # Determine the slices for each of the parameter types. We have
        # the location each tiepoint first, followed by the parameters for
        # igc_coll.
        self.tp_slice = []
        j = 0
        for i, tp in enumerate(self.tpcol):
            if not self.hold_gcp_fixed or not tp.is_gcp:
                self.tp_slice.append(slice(j, j + 3))
                j += 3
            else:
                self.tp_slice.append(None)

    @property
    def tp_offset_slice(self):
        """The portion of parameter that gives the tie point offsets"""
        return slice(0, len(self.tp_offset))

    @property
    def len_surface_constraint(self):
        res = len(self.tpcol)
        if self.hold_gcp_fixed:
            res -= self.tpcol.number_gcp
        return res

    @property
    def surface_constraint_slice(self):
        """The portion of the sba equations that gives the surface constraint"""
        return slice(0, self.len_surface_constraint)

    @property
    def len_gcp_constraint(self):
        if self.tp_sigma is not None:
            res = len(self.tpcol) * 3
        else:
            res = self.tpcol.number_gcp * 3
        if self.hold_gcp_fixed:
            res -= self.tpcol.number_gcp * 3
        return res

    @property
    def gcp_constraint_slice(self):
        """The portion of sba equation that gives the GCP constraint"""
        return slice(
            self.len_surface_constraint,
            self.len_surface_constraint + len_gcp_constraint,
        )

    @property
    def len_collinearity_constraint(self):
        return self.niloc * 2

    @property
    def collinearity_constraint_slice(self):
        """The portion of the sba equations that gives the collinearity
        constraint"""
        return slice(
            self.len_surface_constraint + self.len_gcp_constraint,
            self.len_surface_constraint
            + self.len_gcp_constraint
            + self.len_collinearity_constraint,
        )

    @property
    def parameter_constraint_slice(self):
        """The portion of the sba equations that gives the parameter
        constraint"""
        return slice(
            self.len_surface_constraint
            + self.len_gcp_constraint
            + self.len_collinearity_constraint,
            None,
        )

    @property
    def igc_coll_param_slice(self):
        """The portion of parameter that gives the parameters of igc_coll"""
        return slice(len(self.tp_offset), None)

    @property
    def parameter(self):
        """This is the list of parameters for the SBA. This contains the
        offset for each tie point, along with the parameters for igc_coll."""
        return np.append(self.tp_offset, self.igc_coll.parameter_subset)

    @parameter.setter
    def parameter(self, value):
        """This sets the list of parameters"""
        self.tp_offset[:] = value[self.tp_offset_slice]
        self.igc_coll.parameter_subset = value[self.igc_coll_param_slice]

    def index_to_tp_offset(self, tp_index):
        """Return the tp offset for the given tie point index number."""
        t = self.tp_slice[tp_index]
        if t is None:
            return None
        return self.tp_offset[t]

    def tp_index_to_parameter_index(self, tp_index):
        """Return the first index in the parameter for the tp offset
        corresponding to the tiepoint tp_index"""
        res = 0
        for i in range(tp_index):
            if not self.hold_gcp_fixed or not self.tpcol[i].is_gcp:
                res += 3
        return res

    def ground_location(self, tp_index):
        """Using the current value of the parameters, determine
        the ground location of the given tie point"""
        # Note subtle memory problem here. We initially did not have
        # gl_cf. This mean CartesianFixed went out of scope as was
        # deleted before gl was used. Easy work around is to store
        # this is a variable
        gp = self.tpcol[tp_index].ground_location
        gl = gp.position
        t = self.index_to_tp_offset(tp_index)
        if t is not None:
            pt = gp.create([gl[0] + t[0], gl[1] + t[1], gl[2] + t[2]])
        else:
            pt = gp
        return pt

    def sba_eq(self, parm):
        """Return all the constraints"""
        self.parameter = parm
        return np.append(
            np.append(
                np.append(self.surface_constraint(), self.gcp_constraint()),
                self.collinearity_constraint(),
            ),
            self.parameter_constraint(),
        )

    def sba_jacobian(self, parm):
        """Return Jacobian for SBA equation"""
        self.parameter = parm
        len_igc_parameter = self.igc_coll.parameter_subset.shape[0]
        len_parameter = len(self.parameter)
        res = _coo_helper(
            (
                self.len_surface_constraint
                + self.len_gcp_constraint
                + self.len_collinearity_constraint
                + len_igc_parameter,
                len_parameter,
            )
        )
        self.row_index = 0
        self.__surface_constraint_jacobian(res)
        self.__gcp_constraint_jacobian(res)
        self.__collinearity_constraint_jacobian(res)
        self.__parameter_constraint_jacobian(res)
        return res.coo_matrix()

    def gcp_constraint(self):
        """Calculate the GCP constraint equations. This is the penalty
        we get from placing a tiepoint somewhere other than the location
        given by the GCP"""
        res = np.empty(self.len_gcp_constraint)
        i = 0
        for tp_index, tp in enumerate(self.tpcol):
            if tp.is_gcp and not self.hold_gcp_fixed:
                res[i : (i + 3)] = self.index_to_tp_offset(tp_index) / self.gcp_sigma
                i += 3
            elif tp.is_gcp and self.hold_gcp_fixed:
                pass
            elif self.tp_sigma is not None:
                res[i : (i + 3)] = self.index_to_tp_offset(tp_index) / self.tp_sigma
                i += 3
        return res

    def __gcp_constraint_jacobian(self, res):
        """Calculate the Jacobian of the GCP constraint equations. Takes
        matrix to fill in, and row to start at. The row gets updated to
        just past the end of what we fill out"""
        for tp_index, tp in enumerate(self.tpcol):
            if tp.is_gcp and not self.hold_gcp_fixed:
                pind = self.tp_index_to_parameter_index(tp_index)
                for j in range(3):
                    res[self.row_index + j, pind + j] = 1.0 / self.gcp_sigma
                self.row_index += 3
            elif tp.is_gcp and self.hold_gcp_fixed:
                pass
            elif self.tp_sigma is not None:
                pind = self.tp_index_to_parameter_index(tp_index)
                for j in range(3):
                    res[self.row_index + j, pind + j] = 1.0 / self.tp_sigma
                self.row_index += 3

    def surface_constraint(self):
        """Calculate all the surface constraint equations. This is
        the penalty we get from placing a tiepoint somewhere other than
        the surface given by the Dem."""
        res = np.zeros(self.len_surface_constraint)
        j = 0
        for i, tp in enumerate(self.tpcol):
            if not self.hold_gcp_fixed or not tp.is_gcp:
                res[j] = self.dem.distance_to_surface(self.ground_location(i))
                j += 1
        res /= self.dem_sigma
        return res

    def __surface_constraint_jacobian(self, res):
        """Calculate the Jacobian of the surface constraint equations. Takes
        matrix to fill in, and row to start at. The row gets updated to
        just past the end of what we fill out"""
        for i, tp in enumerate(self.tpcol):
            if not self.hold_gcp_fixed or not tp.is_gcp:
                gp = self.ground_location(i)
                pind = self.tp_index_to_parameter_index(i)
                x0 = self.dem.distance_to_surface(gp) / self.dem_sigma
                p0 = gp.position
                for j in range(3):
                    p = p0.copy()
                    p[j] += self.tp_epsilon
                    gp.position = p
                    x = self.dem.distance_to_surface(gp) / self.dem_sigma
                    res[self.row_index, pind + j] = (x - x0) / self.tp_epsilon
                gp.position = p0
                self.row_index += 1

    def collinearity_constraint(self):
        """Calculate the collinearity constraint equations. This is
        difference between the predicted location in each image vs.
        the actual location."""
        res = np.zeros(self.niloc * 2)
        ind = 0
        for i, tp in enumerate(self.tpcol):
            gp = self.ground_location(i)
            for j in range(tp.number_image):
                if tp.image_coordinate(j):
                    try:
                        ictp = tp.image_coordinate(j)
                        lsigma = tp.line_sigma(j)
                        ssigma = tp.sample_sigma(j)
                        weight = [1.0 / lsigma, 1.0 / ssigma]
                        cres = self.igc_coll.collinearity_residual(j, gp, ictp)
                        rs = slice(ind, ind + 2)
                        res[rs] = cres * weight
                    except RuntimeError as e:
                        if str(e) != "ImageGroundConnectionFailed":
                            raise e
                        res[ind] = 0
                        res[ind + 1] = 0
                    ind += 2
        return res

    def __collinearity_constraint_jacobian(self, res):
        """Calculate the Jacobian of the collinearity constraint equations."""
        self.igc_coll.add_identity_gradient()
        ind = self.row_index
        for i, tp in enumerate(self.tpcol):
            gp = self.ground_location(i)
            for j in range(tp.number_image):
                if tp.image_coordinate(j):
                    ictp = tp.image_coordinate(j)
                    lsigma = tp.line_sigma(j)
                    ssigma = tp.sample_sigma(j)
                    weight = [[1.0 / lsigma], [1.0 / ssigma]]
                    try:
                        jac = self.igc_coll.collinearity_residual_jacobian(j, gp, ictp)
                        rs = slice(ind, ind + 2)
                        ts = self.tp_slice[i]
                        if ts is not None:
                            ts2 = slice(-3, None)
                            res[rs, ts] = jac[:, ts2] * weight
                        ps = self.igc_coll_param_slice
                        ps2 = slice(None, -3)
                        res[rs, ps] = jac[:, ps2] * weight
                    except RuntimeError as e:
                        if str(e) != "ImageGroundConnectionFailed":
                            raise e
                        pass
                    ind += 2
        self.row_index = ind

    def parameter_constraint(self):
        """Calculate the parameter constraint. This is the penalty for
        moving a particular parameter from its initial value."""
        # This needs to be scaled, but for now just do this
        return (self.igc_coll.parameter_subset - self.p0) / self.psigma

    def __parameter_constraint_jacobian(self, res):
        istart = self.parameter_constraint_slice.start
        jstart = self.igc_coll_param_slice.start
        for i in range(self.igc_coll.parameter_subset.shape[0]):
            res[istart + i, jstart + i] = 1.0 / self.psigma[i]


__all__ = ["SimultaneousBundleAdjustment"]
