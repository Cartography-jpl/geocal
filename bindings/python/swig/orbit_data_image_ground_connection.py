# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _orbit_data_image_ground_connection.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_data_image_ground_connection', [dirname(__file__)])
        except ImportError:
            import _orbit_data_image_ground_connection
            return _orbit_data_image_ground_connection
        if fp is not None:
            try:
                _mod = imp.load_module('_orbit_data_image_ground_connection', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _orbit_data_image_ground_connection = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_data_image_ground_connection
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except:
    weakref_proxy = lambda x: x


SHARED_PTR_DISOWN = _orbit_data_image_ground_connection.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

def _new_vector(cls, version, lst):
    '''Create a vector from a list.'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    for i in lst:
       inst.append(i)
    return inst

def _new_from_set(cls, version, *args):
    '''For use with pickle, covers common case where we use a set function 
    to assign the value'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    inst.set(*args)
    return inst

import geocal_swig.image_ground_connection
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
import geocal_swig.observer
class OrbitDataImageGroundConnection(geocal_swig.image_ground_connection.ImageGroundConnection):
    """
    This is a ImageGroundConnection where the connection is made by
    OrbitData and a Camera.

    C++ includes: orbit_data_image_ground_connection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::OrbitDataImageGroundConnection::OrbitDataImageGroundConnection(Orbit &Orb, const Time &Tm, const boost::shared_ptr< Camera > &Cam,
        const boost::shared_ptr< Dem > &D, const boost::shared_ptr<
        RasterImage > &Img, const std::string Title="", const
        boost::shared_ptr< Refraction > &Ref=boost::shared_ptr< Refraction
        >(), double Resolution=30, int Band=0, double Max_height=9000)
        Constructor that takes an Orbit and a time.

        We populate this using the OrbitData from the orbit for that time.
        Moreover, we make this class an Observer of the underlying orbit. When
        the orbit notifies us of changes, we regenerate the orbit data. This
        means that this class will remain in sync with changes in the
        underlying orbit. 
        """
        _orbit_data_image_ground_connection.OrbitDataImageGroundConnection_swiginit(self,_orbit_data_image_ground_connection.new_OrbitDataImageGroundConnection(*args))
    def cf_look_vector(self, *args):
        """
        virtual void GeoCal::OrbitDataImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
        boost::shared_ptr< CartesianFixed > &P) const

        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection_cf_look_vector(self, *args)

    def image_coordinate_with_derivative(self, *args):
        """
        virtual ImageCoordinateWithDerivative GeoCal::OrbitDataImageGroundConnection::image_coordinate_with_derivative(const GroundCoordinate &Gc) const
        This is image_coordinate, but include the derivative of this with
        respect to the parameters of the Camera and OrbitData.

        Not sure if we want this to be a general ImageGroundConnection
        function, but for now we have this defined just for this class. 
        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection_image_coordinate_with_derivative(self, *args)

    def _v_orbit_data(self, *args):
        """
        void GeoCal::OrbitDataImageGroundConnection::orbit_data(const boost::shared_ptr< OrbitData > &Od)
        Set orbit data that we are using. 
        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_orbit_data(self, *args)

    @property
    def orbit_data(self):
        return self._v_orbit_data()

    @orbit_data.setter
    def orbit_data(self, value):
      self._v_orbit_data(value)

    def _v_camera(self, *args):
        """
        void GeoCal::OrbitDataImageGroundConnection::camera(const boost::shared_ptr< Camera > &C)
        Set Camera that we are using. 
        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_camera(self, *args)

    @property
    def camera(self):
        return self._v_camera()

    @camera.setter
    def camera(self, value):
      self._v_camera(value)

    def _v_resolution(self, *args):
        """
        void GeoCal::OrbitDataImageGroundConnection::resolution(double R)
        Set resolution in meters that we examine Dem out.

        This affects how long ground_coordinate takes to figure out. It should
        be about the resolution of the Dem 
        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_resolution(self, *args)

    @property
    def resolution(self):
        return self._v_resolution()

    @resolution.setter
    def resolution(self, value):
      self._v_resolution(value)

    def _v_refraction(self, *args):
        """
        void GeoCal::OrbitDataImageGroundConnection::refraction(const boost::shared_ptr< Refraction > &Ref)
        Set refraction object we are using.

        May be null if we aren't including refraction. 
        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_refraction(self, *args)

    @property
    def refraction(self):
        return self._v_refraction()

    @refraction.setter
    def refraction(self, value):
      self._v_refraction(value)

    def _v_band(self, *args):
        """
        void GeoCal::OrbitDataImageGroundConnection::band(int B)
        Set camera band we are using. 
        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_band(self, *args)

    @property
    def band(self):
        return self._v_band()

    @band.setter
    def band(self, value):
      self._v_band(value)

    def _v_max_height(self, *args):
        """
        void GeoCal::OrbitDataImageGroundConnection::max_height(double Max_h)
        Set Maximum height that we expect to see in the Dem. 
        """
        return _orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_max_height(self, *args)

    @property
    def max_height(self):
        return self._v_max_height()

    @max_height.setter
    def max_height(self, value):
      self._v_max_height(value)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.orbit_data,self.camera,self.dem,self.image,self.title,self.refraction,self.resolution,self.band,self.max_height)

    __swig_destroy__ = _orbit_data_image_ground_connection.delete_OrbitDataImageGroundConnection
OrbitDataImageGroundConnection.cf_look_vector = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection_cf_look_vector,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection.image_coordinate_with_derivative = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection_image_coordinate_with_derivative,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_image = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_image,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_title = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_title,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_orbit_data = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_orbit_data,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_camera = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_camera,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_resolution = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_resolution,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_refraction = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_refraction,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_band = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_band,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection._v_max_height = new_instancemethod(_orbit_data_image_ground_connection.OrbitDataImageGroundConnection__v_max_height,None,OrbitDataImageGroundConnection)
OrbitDataImageGroundConnection_swigregister = _orbit_data_image_ground_connection.OrbitDataImageGroundConnection_swigregister
OrbitDataImageGroundConnection_swigregister(OrbitDataImageGroundConnection)



