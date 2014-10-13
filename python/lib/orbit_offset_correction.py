from geocal_swig import *
import math
import bisect
import scipy.sparse as sp
import numpy as np

class OrbitOffsetCorrection(Orbit):
    '''This class gives an orbit that tries to correct errors in another
    underlying orbit. This uses a simple error model which captures a
    common set of orbit errors.

    This adds a fixed offset to the inertial position of the orbit, modeling 
    a static error in the ephemeris.

    A time dependent correction is added to the spacecraft to Cartesian 
    inertial system. This correction supplies a yaw, pitch, and roll 
    correction at fixed time values. For times in between we interpolate the 
    quaternion correction.

    The velocity is left unchanged.

    The underlying orbit should return a QuaternionOrbitData orbit data, 
    since this is currently the only type supported.'''
    def __init__(self, uncorrected_orbit, time_point = [], 
                 initial_parameter = None, outside_is_error = True):
        '''Initialize orbit offset corrections. We pass in the 
        uncorrected orbit, which should return a QuaternionOrbitData 
        as orbit data. Optionally the time points to supply attitude 
        correction along with the initial parameter can be supplied. The
        default is no corrections. You can modify this after this is 
        created by adding to self.time_points and setting self.parameter.

        Note that self.time_points should be sorted.

        You can either treat times outside of the self.time_points range as
        having a 0 correction for attitude, or as an out of range error. This 
        is controlled by the optional outside_is_error field'''
        Orbit.__init__(self, uncorrected_orbit.min_time,
                       uncorrected_orbit.max_time)
        self.outside_is_error = outside_is_error
        self.uncorrected_orbit = uncorrected_orbit
        self.__parameter = np.array([0,0,0], np.double)
        self.time_point = time_point
        if(initial_parameter is not None):
            self.parameter = initial_parameter
        else:
            self.parameter = np.zeros(self.parameter.shape, np.double)
        self.parameter_mask = [True] * len(self.parameter)
        self.fit_position = True
        self.fit_yaw = True
        self.fit_pitch = True
        self.fit_roll = True

    def __getstate__(self):
        return { "uncorrected_orbit" : self.uncorrected_orbit,
                 "parameter" : self.__parameter,
                 "time_point" : self.__time_point,
                 "outside_is_error" : self.outside_is_error,
                 "fit_position" : self.fit_position,
                 "fit_yaw" : self.fit_yaw,
                 "fit_pitch" : self.fit_pitch,
                 "fit_roll" : self.fit_roll,
                 }

    def __setstate__(self, dict):
        self.__init__(dict["uncorrected_orbit"], 
                      time_point = dict["time_point"],
                      initial_parameter = dict["parameter"],
                      outside_is_error = dict["outside_is_error"])
        self.fit_position = dict["fit_position"]
        self.fit_yaw = dict["fit_yaw"]
        self.fit_pitch = dict["fit_pitch"]
        self.fit_roll = dict["fit_roll"]

    def _v_parameter(self, *args):
        '''Parameters used in correction. This is the first the offset in
        position, followed by the yaw, pitch, roll correction at 
        time_point[0]; yaw, pitch roll at time_point[1], etc.'''
        # Awkward interface, but this matches what the C++ needs. If we have
        # no arguments, then we are returning the parameters. Otherwise,
        # we are setting them.
        if(len(args) == 0):
            return self.__parameter
        else:
            value = args[0]
            if(len(value) != 3 + 3 * len(self.time_point)):
                raise ValueError("Parameter is the wrong length. It was length %d, but should have been %d" % (len(value), 3 + 3 * len(self.time_point)))
            self.__parameter = np.array(value, np.double)

    def _v_parameter_name(self):
        res = ["Position X Offset", "Position Y Offset", "Position Z Offset"]
        for tm in self.time_point:
            res.append("Yaw correction time " + str(tm))
            res.append("Pitch correction time " + str(tm))
            res.append("Roll correction time " + str(tm))
        return res

    @property
    def time_point(self):
        '''Time points that we supply attitude corrections at. This is a
        strictly monotonic ascending time series'''
        return self.__time_point

    def insert_time_point(self, t):
        '''Insert a new time point. The initial value for this correction
        is 0.'''
        if(len(self.__time_point) == 0):
            self.time_point = [t]
        elif(t > self.__time_point[-1]):
            self.__time_point.append(t)
            self.__parameter = np.append(self.__parameter, [0,0,0])
        else:
            inew = bisect.bisect_left(self.__time_point, t)
            self.__time_point.insert(inew, t)
            for i in range(3 + 3 * inew, 3 + 3 * inew + 3):
                self.__parameter = np.insert(self.__parameter, i, 0)

    @time_point.setter
    def time_point(self, value):
        for i in range(1, len(value)):
            if(value[i - 1] >= value[i]):
                raise "The list of times passed to time_point must be sorted"
        self.__time_point = value
        self.__parameter.resize(3 + 3 * len(self.__time_point))

    def orbit_data(self, t):
        '''Return orbit data for given time'''
        od = self.uncorrected_orbit.orbit_data(t)
        pcorr = od.position_ci.position.copy()
        for i in range(3):
            pcorr[i] += self.__parameter[i]
        return QuaternionOrbitData(od.time, od.position_ci.create(pcorr),
                                   od.velocity_ci, 
                                   od.sc_to_ci * self.quaternion_correction(t))
    
    def image_coordinate(self, t, gc, cam, b = 0):
        '''Return image coordinate for given time.'''
        od = self.orbit_data(t)
        fc = od.frame_coordinate(gc, cam, b)
        return ImageCoordinate(fc.line, fc.sample)

    def att_parameters_affect(self, t):
        '''This return a range indicating which parameter affect things
        at the given time. This allows IgcOffsetCorrection to create
        a sparse jacobian by only looking at a subset of parameters'''
        # bisect will find smallest i such that self.__time_point[i] >= t
        tind = bisect.bisect_right(self.__time_point, t)
        # These are the minimum and maximum parameter indexes that affect
        # the image coordinate at all. Outside of this range the parameters
        # have no effect.
        pindmin = 3 + 3 * (tind - 1)
        if(t == self.__time_point[tind - 1]):
            pindmax = min(3 + 3 * tind, self.__parameter.shape[0])
        else:
            pindmax = min(3 + 3 * (tind + 1), self.__parameter.shape[0])
        return range(pindmin, pindmax)

    def quaternion_correction(self, t):
        '''Return the quaternion correction for the given time t.'''
        # Special handling for no quaternion corrections
        if(len(self.__time_point) == 0):
            return Quaternion_double(1, 0, 0, 0)
        # Special handling for t = largest value
        if(t == self.__time_point[-1]):
            return self.__quat_i(len(self.__time_point) - 1)
        # bisect will find smallest i such that self.__time_point[i] >= t
        i = bisect.bisect_right(self.__time_point, t)
        if(i < 1 or i >= len(self.__time_point)):
            if(self.outside_is_error):
                raise ValueError
            else:
                return Quaternion_double(1,0,0,0)
        return self.interpolate(self.__quat_i(i - 1), self.__quat_i(i),
                                t - self.__time_point[i - 1],
                                self.__time_point[i] - self.__time_point[i - 1])

    def __quat_i(self, i):
        '''Return quaternion correction for time point i. Right now we
        calculate this each time, we can look at caching this if 
        performance becomes an issue'''
        return self.__quat_ypr(self.__parameter[3 + 3 * i],
                               self.__parameter[3 + 3 * i + 1],
                               self.__parameter[3 + 3 * i + 2])

    def __quat_ypr(self, yaw, pitch, roll):
        '''This creates a quaternion for the given yaw, pitch, and roll.
        Right now, yaw pitch and roll should be in arcseconds (we may
        change this)'''
        yaw_rad = math.radians(yaw / (60.0 * 60.0))
        pitch_rad = math.radians(pitch / (60.0 * 60.0))
        roll_rad = math.radians(roll / (60.0 * 60.0))
        return quat_rot("xyz", pitch_rad, roll_rad, yaw_rad)
