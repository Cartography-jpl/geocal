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

def _determine_orbit_parm(self, gp, ic, i):
    '''This adjust the orbit parameters so that the given ground point
    return an image coordinate of ic for igc i.'''
    # Vector that we have now
    igc = self.image_ground_connection(i)
    tmpt, fc = igc.time_table.time(ic)
    tlist = [igc.time_table.min_time, tmpt, igc.time_table.max_time]
    for j, tm in enumerate(tlist):
        od = igc.orbit.orbit_data(tm)
        t = od.sc_look_vector(od.cf_look_vector(igc.camera.sc_look_vector(FrameCoordinate(ic.line, ic.sample), igc.band)))
        v1 = np.array([t.direction[0], t.direction[1], t.direction[2]])

        # Vector that we want
        t = od.sc_look_vector(CartesianFixedLookVector(od.position_cf, gp))
        v2 = np.array([t.direction[0], t.direction[1], t.direction[2]])

        # Quaternion that does that
        self.orbit.update_quaterion(i * len(tlist) + j, 
                                    determine_quat_rot(v2, v1))
    
def _determine_orbit_to_match(self, ic, ind):
    '''This adds an orbit model on top of the existing orbit. We adjust the
    orbit model so that all the image ground connections see the same point
    on the ground, which is whatever image_ground_connection(ind) see from
    the passed in image coordinate ic.

    This is entirely used for generating test data, I don't think there is
    any reason you would want to do this with real data.
    '''
    self.orbit = OrbitOffsetCorrection(self.orbit, False, False, False, 
                                       False, False)
    gp = self.ground_coordinate(ind, ic)
    for i in range(self.number_image):
        igc = self.image_ground_connection(i)
        tm, fc = igc.time_table.time(ic)
        self.orbit.insert_time_point(igc.time_table.min_time)
        self.orbit.insert_time_point(tm)
        self.orbit.insert_time_point(igc.time_table.max_time)
    for i in range(self.number_image):
        self._determine_orbit_parm(gp, ic, i)

IgcCollectionRollingShutter._determine_orbit_parm = _determine_orbit_parm
IgcCollectionRollingShutter.determine_orbit_to_match = _determine_orbit_to_match

        
