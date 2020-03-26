# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _igc_msp.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_igc_msp')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_igc_msp')
    _igc_msp = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_msp', [dirname(__file__)])
        except ImportError:
            import _igc_msp
            return _igc_msp
        try:
            _mod = imp.load_module('_igc_msp', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _igc_msp = swig_import_helper()
    del swig_import_helper
else:
    import _igc_msp
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_method(set):
    def set_attr(self, name, value):
        if (name == "thisown"):
            return self.this.own(value)
        if hasattr(self, name) or (name == "this"):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except __builtin__.Exception:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _igc_msp.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_igc_msp.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_igc_msp.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_igc_msp.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_igc_msp.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_igc_msp.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_igc_msp.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_igc_msp.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_igc_msp.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_igc_msp.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_igc_msp.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_igc_msp.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_igc_msp.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_igc_msp.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_igc_msp.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_igc_msp.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_igc_msp.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _igc_msp.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _igc_msp.SHARED_PTR_DISOWN

import os

def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

def _new_from_serialization(data):
    return geocal_swig.serialize_read_binary(data)

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_read_binary(data)
    finally:
      os.chdir(curdir)


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

def have_msp_supported():
    """

    bool GeoCal::have_msp_supported()
    Return true if we were built with MSP support, false otherwise. 
    """
    return _igc_msp.have_msp_supported()
class IgcMsp(geocal_swig.image_ground_connection.ImageGroundConnectionCopy):
    """

    This is an ImageGroundConnection that uses the MSP (Mensuration
    Service Program) library to implement an ImageGroundConnection.

    This uses a set of plugins for the CSM (Community Sensor Model,
    seehttps://github.com/sminster/csm) to support various sensor models.
    In particular, this supports various NITF formats such as RPC, RSM,
    SENSRB, GLAS/GFM.

    Note this goes from the CSM library to our ImageGroundConnection
    interface. We may well write a plugin to go the other way at some
    point, so software that uses the CSM library can use GeoCal
    ImageGroundConnection objects.

    C++ includes: igc_msp.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        IgcMsp::IgcMsp(const std::string &Fname, const boost::shared_ptr< Dem >
        &D=boost::shared_ptr< Dem >(new SimpleDem()), int Image_index=0)
        Constructor.

        Note to use the MSP library, you should make sure the proper
        environment variables are set. This happens in the normal GeoCal
        setup, but the variables are MSP_DATA_DIR, MSPCCS_DATA, and
        CSM_PLUGIN_DIR. Note the directory should end in "/", just as a
        convention - so /foo/bar/plugins/. 
        """
        _igc_msp.IgcMsp_swiginit(self, _igc_msp.new_IgcMsp(*args))

    def msp_print_plugin_list():
        """

        void IgcMsp::msp_print_plugin_list()
        Print a list of all plugins. 
        """
        return _igc_msp.IgcMsp_msp_print_plugin_list()

    msp_print_plugin_list = staticmethod(msp_print_plugin_list)

    def msp_register_plugin(Plugin_name):
        """

        void IgcMsp::msp_register_plugin(const std::string &Plugin_name)
        Register the given plugin.

        Note that we already register all the plugins at CSM_PLUGIN_DIR, so
        you don't usually need to use this function. 
        """
        return _igc_msp.IgcMsp_msp_register_plugin(Plugin_name)

    msp_register_plugin = staticmethod(msp_register_plugin)

    def msp_plugin_list():
        """

        std::vector< std::string > IgcMsp::msp_plugin_list()
        Return list of registered plugins. 
        """
        return _igc_msp.IgcMsp_msp_plugin_list()

    msp_plugin_list = staticmethod(msp_plugin_list)

    def msp_model_list(Plugin):
        """

        std::vector< std::string > IgcMsp::msp_model_list(const std::string &Plugin)
        For a given plugin, return the list of models it supports.

        Some plugins may support more than one sensor model. 
        """
        return _igc_msp.IgcMsp_msp_model_list(Plugin)

    msp_model_list = staticmethod(msp_model_list)

    def image_ids(Fname):
        """

        std::vector< std::string > IgcMsp::image_ids(const std::string &Fname)
        Return the list of image IDS for the given NITF file.

        If the file has only one 
        """
        return _igc_msp.IgcMsp_image_ids(Fname)

    image_ids = staticmethod(image_ids)

    def sensor_velocity(self, Ic):
        """

        blitz::Array< double, 1 > IgcMsp::sensor_velocity(const ImageCoordinate &Ic) const

        """
        return _igc_msp.IgcMsp_sensor_velocity(self, Ic)


    def generate_rsm_tre(self, *args):
        """

        std::string IgcMsp::generate_rsm_tre(const std::string &Report="", const std::string &Rsm_config="")
        const
        Use the MSP RSM generation code to create a TRE string (it doesn't
        directly create a NITF file).

        You can optionally supply a report file to write to and/or specify the
        RSM configuration file to use. The default is
        "$MSP_DATA_DIR/rsm/database_settings.strat".

        Note the TRE string can be converted to an actual RSM. We add this
        functionality in the python interface, it isn't all that easy to do in
        C++ and we really only need this at the python level anyways. 
        """
        return _igc_msp.IgcMsp_generate_rsm_tre(self, *args)


    def _v_family(self):
        """

        std::string IgcMsp::family() const

        """
        return _igc_msp.IgcMsp__v_family(self)


    @property
    def family(self):
        return self._v_family()


    def _v_version(self):
        """

        std::string IgcMsp::version() const

        """
        return _igc_msp.IgcMsp__v_version(self)


    @property
    def version(self):
        return self._v_version()


    def _v_model_name(self):
        """

        std::string IgcMsp::model_name() const

        """
        return _igc_msp.IgcMsp__v_model_name(self)


    @property
    def model_name(self):
        return self._v_model_name()


    def _v_file_name(self):
        """

        std::string IgcMsp::file_name() const

        """
        return _igc_msp.IgcMsp__v_file_name(self)


    @property
    def file_name(self):
        return self._v_file_name()


    def _v_image_index(self):
        """

        int IgcMsp::image_index() const

        """
        return _igc_msp.IgcMsp__v_image_index(self)


    @property
    def image_index(self):
        return self._v_image_index()


    def _v_pedigree(self):
        """

        std::string IgcMsp::pedigree() const

        """
        return _igc_msp.IgcMsp__v_pedigree(self)


    @property
    def pedigree(self):
        return self._v_pedigree()


    def _v_image_identifer(self):
        """

        std::string IgcMsp::image_identifer() const

        """
        return _igc_msp.IgcMsp__v_image_identifer(self)


    @property
    def image_identifer(self):
        return self._v_image_identifer()


    def _v_sensor_identifer(self):
        """

        std::string IgcMsp::sensor_identifer() const

        """
        return _igc_msp.IgcMsp__v_sensor_identifer(self)


    @property
    def sensor_identifer(self):
        return self._v_sensor_identifer()


    def _v_platform_identifer(self):
        """

        std::string IgcMsp::platform_identifer() const

        """
        return _igc_msp.IgcMsp__v_platform_identifer(self)


    @property
    def platform_identifer(self):
        return self._v_platform_identifer()


    def _v_collection_identifer(self):
        """

        std::string IgcMsp::collection_identifer() const

        """
        return _igc_msp.IgcMsp__v_collection_identifer(self)


    @property
    def collection_identifer(self):
        return self._v_collection_identifer()


    def _v_trajectory_identifer(self):
        """

        std::string IgcMsp::trajectory_identifer() const

        """
        return _igc_msp.IgcMsp__v_trajectory_identifer(self)


    @property
    def trajectory_identifer(self):
        return self._v_trajectory_identifer()


    def _v_sensor_type(self):
        """

        std::string IgcMsp::sensor_type() const

        """
        return _igc_msp.IgcMsp__v_sensor_type(self)


    @property
    def sensor_type(self):
        return self._v_sensor_type()


    def _v_sensor_mode(self):
        """

        std::string IgcMsp::sensor_mode() const

        """
        return _igc_msp.IgcMsp__v_sensor_mode(self)


    @property
    def sensor_mode(self):
        return self._v_sensor_mode()


    def _v_reference_date_time(self):
        """

        std::string IgcMsp::reference_date_time() const

        """
        return _igc_msp.IgcMsp__v_reference_date_time(self)


    @property
    def reference_date_time(self):
        return self._v_reference_date_time()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _igc_msp.delete_IgcMsp
IgcMsp.sensor_velocity = new_instancemethod(_igc_msp.IgcMsp_sensor_velocity, None, IgcMsp)
IgcMsp.generate_rsm_tre = new_instancemethod(_igc_msp.IgcMsp_generate_rsm_tre, None, IgcMsp)
IgcMsp._v_family = new_instancemethod(_igc_msp.IgcMsp__v_family, None, IgcMsp)
IgcMsp._v_version = new_instancemethod(_igc_msp.IgcMsp__v_version, None, IgcMsp)
IgcMsp._v_model_name = new_instancemethod(_igc_msp.IgcMsp__v_model_name, None, IgcMsp)
IgcMsp._v_file_name = new_instancemethod(_igc_msp.IgcMsp__v_file_name, None, IgcMsp)
IgcMsp._v_image_index = new_instancemethod(_igc_msp.IgcMsp__v_image_index, None, IgcMsp)
IgcMsp._v_pedigree = new_instancemethod(_igc_msp.IgcMsp__v_pedigree, None, IgcMsp)
IgcMsp._v_image_identifer = new_instancemethod(_igc_msp.IgcMsp__v_image_identifer, None, IgcMsp)
IgcMsp._v_sensor_identifer = new_instancemethod(_igc_msp.IgcMsp__v_sensor_identifer, None, IgcMsp)
IgcMsp._v_platform_identifer = new_instancemethod(_igc_msp.IgcMsp__v_platform_identifer, None, IgcMsp)
IgcMsp._v_collection_identifer = new_instancemethod(_igc_msp.IgcMsp__v_collection_identifer, None, IgcMsp)
IgcMsp._v_trajectory_identifer = new_instancemethod(_igc_msp.IgcMsp__v_trajectory_identifer, None, IgcMsp)
IgcMsp._v_sensor_type = new_instancemethod(_igc_msp.IgcMsp__v_sensor_type, None, IgcMsp)
IgcMsp._v_sensor_mode = new_instancemethod(_igc_msp.IgcMsp__v_sensor_mode, None, IgcMsp)
IgcMsp._v_reference_date_time = new_instancemethod(_igc_msp.IgcMsp__v_reference_date_time, None, IgcMsp)
IgcMsp_swigregister = _igc_msp.IgcMsp_swigregister
IgcMsp_swigregister(IgcMsp)

def IgcMsp_msp_print_plugin_list():
    """

    void IgcMsp::msp_print_plugin_list()
    Print a list of all plugins. 
    """
    return _igc_msp.IgcMsp_msp_print_plugin_list()

def IgcMsp_msp_register_plugin(Plugin_name):
    """

    void IgcMsp::msp_register_plugin(const std::string &Plugin_name)
    Register the given plugin.

    Note that we already register all the plugins at CSM_PLUGIN_DIR, so
    you don't usually need to use this function. 
    """
    return _igc_msp.IgcMsp_msp_register_plugin(Plugin_name)

def IgcMsp_msp_plugin_list():
    """

    std::vector< std::string > IgcMsp::msp_plugin_list()
    Return list of registered plugins. 
    """
    return _igc_msp.IgcMsp_msp_plugin_list()

def IgcMsp_msp_model_list(Plugin):
    """

    std::vector< std::string > IgcMsp::msp_model_list(const std::string &Plugin)
    For a given plugin, return the list of models it supports.

    Some plugins may support more than one sensor model. 
    """
    return _igc_msp.IgcMsp_msp_model_list(Plugin)

def IgcMsp_image_ids(Fname):
    """

    std::vector< std::string > IgcMsp::image_ids(const std::string &Fname)
    Return the list of image IDS for the given NITF file.

    If the file has only one 
    """
    return _igc_msp.IgcMsp_image_ids(Fname)


__all__ = ["IgcMsp","have_msp_supported"]



