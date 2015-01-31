from abc import *
from geocal_swig import *
import numpy as np
import scipy.sparse
import copy


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
    # This implementation works when we have one set of parameter that applies
    # to all the image_ground_connection. This doesn't hold for all 
    # IgcCollection, so derived classes may need to replace this
    self.add_identity_gradient()
    jac_in = self.image_ground_connection(image_index).image_coordinate_jac_parm(ground_point)
    for j in range(jac_in.shape[1]):
        jac[jac_row, jac_col + j] = jac_in[0, j] * line_scale
        jac[jac_row + 1, jac_col + j] = jac_in[1, j] * sample_scale

IgcCollection.image_coordinate_jac_parm = _image_coordinate_jac_parm

def _igc_arr_image_coordinate_jac_parm(self, image_index, ground_point, jac,
                               jac_row, jac_col, line_scale = 1, 
                               sample_scale = 1):
    '''Update Jacobian for image coordinate with respect to change in 
    the parameters of the ground point. 

    We pass in the Jacobian to fill in along with the row and column for
    the start of the submatrix we are filling in. We also optionally pass
    in a scaling for the line and sample entries. This interface may seem
    a bit odd, but this is what we have in the SimultaneousBundleAdjustment
    that uses this call.'''
    # Implemention of IgcArray
    jparm = 0
    for i in range(image_index):
        jparm += len(self.image_ground_connection(image_index).parameter_subset)
    pstart = jparm + jac_col
    jac_in = self.image_ground_connection(image_index).image_coordinate_jac_parm(ground_point)
    for j in range(jac_in.shape[1]):
        jac[jac_row, pstart + j] = jac_in[0, j] * line_scale
        jac[jac_row + 1, pstart + j] = jac_in[1, j] * sample_scale

IgcArray.image_coordinate_jac_parm = _igc_arr_image_coordinate_jac_parm
