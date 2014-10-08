# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _ipi_image_ground_connection.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ipi_image_ground_connection', [dirname(__file__)])
        except ImportError:
            import _ipi_image_ground_connection
            return _ipi_image_ground_connection
        if fp is not None:
            try:
                _mod = imp.load_module('_ipi_image_ground_connection', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ipi_image_ground_connection = swig_import_helper()
    del swig_import_helper
else:
    import _ipi_image_ground_connection
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


SHARED_PTR_DISOWN = _ipi_image_ground_connection.SHARED_PTR_DISOWN
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
import geocal_swig.geocal_exception
import geocal_swig.observer
import geocal_swig.with_parameter
class IpiImageGroundConnection(geocal_swig.image_ground_connection.ImageGroundConnection):
    """
    This is a ImageGroundConnection where the connection is made by a Ipi.

    C++ includes: ipi_image_ground_connection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::IpiImageGroundConnection::IpiImageGroundConnection(const boost::shared_ptr< Ipi > &I, const boost::shared_ptr< Dem > &D,
        const boost::shared_ptr< RasterImage > &Img, const std::string
        &Title="Image", double Resolution=30, double Max_height=9000)

        """
        _ipi_image_ground_connection.IpiImageGroundConnection_swiginit(self,_ipi_image_ground_connection.new_IpiImageGroundConnection(*args))
    def cf_look_vector(self, *args):
        """
        virtual void GeoCal::IpiImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
        boost::shared_ptr< CartesianFixed > &P) const

        """
        return _ipi_image_ground_connection.IpiImageGroundConnection_cf_look_vector(self, *args)

    def _v_ipi(self):
        """
        const boost::shared_ptr<Ipi>& GeoCal::IpiImageGroundConnection::ipi_ptr() const
        IPI that we are using. 
        """
        return _ipi_image_ground_connection.IpiImageGroundConnection__v_ipi(self)

    @property
    def ipi(self):
        return self._v_ipi()

    def _v_resolution(self):
        """
        double GeoCal::IpiImageGroundConnection::resolution() const
        Resolution we step through Dem at, in meters. 
        """
        return _ipi_image_ground_connection.IpiImageGroundConnection__v_resolution(self)

    @property
    def resolution(self):
        return self._v_resolution()

    def _v_maximum_height(self):
        """
        double GeoCal::IpiImageGroundConnection::maximum_height() const
        Maximum height we look through Dem at. 
        """
        return _ipi_image_ground_connection.IpiImageGroundConnection__v_maximum_height(self)

    @property
    def maximum_height(self):
        return self._v_maximum_height()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.ipi,self.dem,self.image,self.title,self.resolution,self.maximum_height)

    __swig_destroy__ = _ipi_image_ground_connection.delete_IpiImageGroundConnection
IpiImageGroundConnection.cf_look_vector = new_instancemethod(_ipi_image_ground_connection.IpiImageGroundConnection_cf_look_vector,None,IpiImageGroundConnection)
IpiImageGroundConnection._v_ipi = new_instancemethod(_ipi_image_ground_connection.IpiImageGroundConnection__v_ipi,None,IpiImageGroundConnection)
IpiImageGroundConnection._v_resolution = new_instancemethod(_ipi_image_ground_connection.IpiImageGroundConnection__v_resolution,None,IpiImageGroundConnection)
IpiImageGroundConnection._v_maximum_height = new_instancemethod(_ipi_image_ground_connection.IpiImageGroundConnection__v_maximum_height,None,IpiImageGroundConnection)
IpiImageGroundConnection_swigregister = _ipi_image_ground_connection.IpiImageGroundConnection_swigregister
IpiImageGroundConnection_swigregister(IpiImageGroundConnection)



